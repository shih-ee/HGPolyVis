#include "ERData.h"
#include <set>
#include <unordered_set>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Eigenvalues>
#include "ElementUtils.h"
#include <algorithm>
#include "LogConsole.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QInputDialog>
#include <adept_source.h>
#include <adept.h>
#include <type_traits>
#include "string.h"
#include <algorithm>
#include <array>
#include <algorithm>
#include <stack>
#include "FileIO.h"
#include <ogdf/planarity/BoyerMyrvold.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/energybased/FMMMLayout.h>


ERData::ERData()
{
	maxX = maxY = minX = minY = 0.0;
	midX = 0, midY = 0;
}

ERData::ERData(const ERData & src)
{

    massCenter = src.massCenter;

    principleDir1 = src.principleDir1;
    principleDir2 = src.principleDir2;

    principleDir1Project_max = src.principleDir1Project_max;
    principleDir1Project_min = src.principleDir1Project_min;
    principleDir2Project_max = src.principleDir2Project_max;
    principleDir2Project_min = src.principleDir2Project_min;

    for (int i = 0; i < src.m_entityList.size(); i++) {
        Entity *newe = new Entity(*src.m_entityList[i]);
		addEntity(newe);
    }

    for (int i = 0; i < src.m_relationshipList.size(); i++) {
        Relationship *newr = new Relationship(*src.m_relationshipList[i]);
		addRelationship(newr);
    }

    //copy the connection
    for (int i = 0; i < m_relationshipList.size(); i++) {
        Relationship *re = m_relationshipList[i];
        Relationship *src_re = src.m_relationshipList[i];
        vector<Entity*> enList;
        vector<Relationship*> reList;

        for (int j = 0; j < src_re->getIncidentEntities().size(); j++) {
            int index = src_re->getIncidentEntity(j)->getIndex();
            Entity *en = m_entityList[index];
            enList.push_back(en);
        }

        for (int j = 0; j < src_re->getAdjacentRelationships().size(); j++) {
            int reindex = src_re->getAdjacentRelationship(j)->getIndex();
            Relationship *rre = m_relationshipList[reindex];
            reList.push_back(rre);
        }

        re->copyToAdjacentRelationshipList(reList);
        re->copyToIncidentEntityList(enList);
    }

    for (int i = 0; i < m_entityList.size(); i++) {
        Entity *en = m_entityList[i];
        Entity *src_en = src.m_entityList[i];
        vector<Entity*> enList;
        vector<Relationship*> reList;

        for (int j = 0; j < src_en->getAdjacentEntities().size(); j++) {
            int index = src_en->getAdjacentEntity(j)->getIndex();
            Entity *t_en = m_entityList[index];
            enList.push_back(t_en);
        }

        for (int j = 0; j < src_en->getIncidentRelationships().size(); j++) {
            int reindex = src_en->getIncidentRelationship(j)->getIndex();
            Relationship *re = m_relationshipList[reindex];
            reList.push_back(re);
        }

        en->copyToIncidentRelationshipList(reList);
        en->copyToAdjacentEntityList(enList);
    }

	initializeMonogonOrientations();
}

ERData::~ERData()
{
	for (int i = 0; i < m_relationshipList.size(); i++) {
		delete(m_relationshipList[i]);
		m_relationshipList[i] = NULL;
	}
	m_relationshipList.clear();

	for (int i = 0; i < m_entityList.size(); i++) {
		delete(m_entityList[i]);
		m_entityList[i] = NULL;
	}
	m_relationshipList.clear();

	int nverts = m_entityArraySize;
	int nedges = m_relationshipArraySize;

	if (m_entityIncidence) delete[] m_entityIncidence;
	if (m_entityAdjacency) delete[] m_entityAdjacency;
	if (m_entityAdjacencyFactor) delete[] m_entityAdjacencyFactor;
	if (m_entityBetweennessCentrality) delete[] m_entityBetweennessCentrality;
	if (m_entityEccentricity) delete[] m_entityEccentricity;
	if (m_entityClosenessCentrality) delete[] m_entityClosenessCentrality;
	if (m_entityEffectiveResistance) delete[] m_entityEffectiveResistance;

	if (m_relationshipIncidence) delete[] m_relationshipIncidence;
	if (m_relationshipAdjacency) delete[] m_relationshipAdjacency;
	if (m_relationshipAdjacencyFactor) delete[] m_relationshipAdjacencyFactor;
	if (m_relationshipBetweennessCentrality) delete[] m_relationshipBetweennessCentrality;
	if (m_relationshipEccentricity) delete[] m_relationshipEccentricity;
	if (m_relationshipClosenessCentrality) delete[] m_relationshipClosenessCentrality;
	if (m_relationshipEffectiveResistance) delete[] m_relationshipEffectiveResistance;

	if (m_incidenceMatrix)
	{
		for (int i = 0; i < nedges; i++)
			delete[] m_incidenceMatrix[i];
		delete[] m_incidenceMatrix;
	}
	if (m_entityAdjacencyMatrix)
	{
		for (int i = 0; i < nverts; i++)
			delete[] m_entityAdjacencyMatrix[i];
		delete[] m_entityAdjacencyMatrix;
	}
	if (m_relationshipAdjacencyMatrix)
	{
		for (int i = 0; i < nedges; i++)
			delete[] m_relationshipAdjacencyMatrix[i];
		delete[] m_relationshipAdjacencyMatrix;
	}

	if (m_entitiesOnCycles) delete[] m_entitiesOnCycles;
	if (m_relationshipsOnCycles) delete[] m_relationshipsOnCycles;

	if (m_KonigGraphLayout) delete m_KonigGraphLayout;
	if (m_KonigGraph) delete m_KonigGraph;
	//if (m_TriGraphLayout) delete m_TriGraphLayout;
	//if (m_TriGraph) delete m_TriGraph;
}

int ERData::getMaxDegree(ELEMENTTYPE elementType) const{
	if(elementType == ELEMENTTYPE::ENTITY)
		return maxEntityDegree;

	if (elementType == ELEMENTTYPE::RELATIONSHIP)
		return maxRelationshipDegree;

}

int ERData::getMinDegree(ELEMENTTYPE elementType) const{
	if (elementType == ELEMENTTYPE::ENTITY)
		return minEntityDegree;

	if (elementType == ELEMENTTYPE::RELATIONSHIP)
		return minRelationshipDegree;
}

void ERData::assignIndices() {

	for (int i = 0; i < m_entityList.size(); i++) {
		if (m_entityList[i]->getIndex() != i) {
			LogConsole::writeConsole("En %d is changed to %d", m_entityList[i]->getIndex(), i);
			m_entityList[i]->setIndex(i);
		}
	}

	for (int i = 0; i < m_relationshipList.size(); i++) {
		if (m_relationshipList[i]->getIndex() != i) {
			LogConsole::writeConsole("Re %d is changed to %d", m_relationshipList[i]->getIndex(), i);
			m_relationshipList[i]->setIndex(i);
		}
	}
}

void ERData::updateEntityIndices(int indexstart)
{
    for (int i = indexstart; i < m_entityList.size(); i++) {
		int cur_id = m_entityList[i]->getIndex();
		if (cur_id != i) {
			m_entityList[i]->setIndex(i);
			LogConsole::writeRedConsole("change entity index %d to %d", cur_id, i);
		}
    }
}

void ERData::updateRelationshipIndices(int indexstart)
{
    for (int i = indexstart; i < m_relationshipList.size(); i++) {
		int cur_id = m_relationshipList[i]->getIndex();
		if (cur_id != i) {
			m_relationshipList[i]->setIndex(i);
			LogConsole::writeRedConsole("change relationship index %d to %d", cur_id, i);
		}
    }
}

void ERData::updateEntityLabelVisibilities(vector<Entity*> vis_en_list)
{
	if (vis_en_list.empty()) {
		for (auto en : m_entityList) {
			en->setLabelVisibility(true);
		}
	}
	else {
		for (auto en : m_entityList) {
			en->setLabelVisibility(false);
		}

		for (auto en : vis_en_list) {
			en->setLabelVisibility(true);
		}
	}
}

void ERData::updateRelationshipsLabelVisibilities(vector<RE*> vis_en_list)
{
	if (vis_en_list.empty()) {
		for (auto en : m_relationshipList) {
			en->setLabelVisibility(true);
		}
	}
	else {
		for (auto en : m_relationshipList) {
			en->setLabelVisibility(false);
		}

		for (auto en : vis_en_list) {
			en->setLabelVisibility(true);
		}
	}
}

void ERData::assignEntityLabelsWithIndices()
{
	for (int i = 0; i < m_entityList.size(); i++) {
		m_entityList[i]->setIndex(i);
		m_entityList[i]->setLabelWithIndex();
	}
}

void ERData::assignRelationshipLabelsWithIndices()
{
	for (int i = 0; i < m_relationshipList.size(); i++) {
		m_relationshipList[i]->setIndex(i);
		m_relationshipList[i]->setLabelWithIndex();
	}
}

/*move to the last of the list*/
void ERData::bringRelationshipBackward(Relationship * re)
{
	int index = isInsideList(re, m_relationshipList);

	if(index == -1){
		LogConsole::writeConsole("no such relationship in list");
		return;
	}

	if (index == m_relationshipList.size() - 1) {
		LogConsole::writeConsole("This relationship is the last one");
		return;
	}

	int overlappingIndex = m_relationshipList.size() - 1;

	if (overlappingIndex <= index)
	{
		LogConsole::showError("wrong found relationship");
		return;
	}

	iter_swap(m_relationshipList.begin()+index, m_relationshipList.begin() + overlappingIndex);
	LogConsole::writeConsole("Re %d %d swapped", index, overlappingIndex);
	m_relationshipList[index]->setIndex(index);
	m_relationshipList[overlappingIndex]->setIndex(overlappingIndex);
}

void ERData::sendRelationshipsToBack(REList relationships)
{
	int swap_offset = 1;
	for (Relationship* re : relationships)
	{
		int index = isInsideList(re, m_relationshipList);
		int swapIndex = m_relationshipList.size() - swap_offset;
		if (index == -1)
		{
			LogConsole::writeConsole("no such relationship in list");
			continue;
		}
		if (index == swapIndex)
		{
			LogConsole::writeConsole("Relationship already in place");
			continue;
		}

		iter_swap(m_relationshipList.begin() + index, m_relationshipList.begin() + swapIndex);
		LogConsole::writeConsole("Re %d %d swapped", index, swapIndex);
		m_relationshipList[index]->setIndex(index);
		m_relationshipList[swapIndex]->setIndex(swapIndex);
		swap_offset++;
	}
}

/*move to the first of the list*/
void ERData::bringRelationshipForward(Relationship * re)
{
	int index = isInsideList(re, m_relationshipList);

	if (index == -1) {
		LogConsole::writeConsole("no such relationship in list");
		return;
	}

	if (index == 0) {
		LogConsole::writeConsole("This relationship is the first one");
		return;
	}

	int overlappingIndex = 0;

	if (overlappingIndex >= index)
	{
		LogConsole::showError("wrong overlap relationship found");
		return;
	}

	iter_swap(m_relationshipList.begin() + index, m_relationshipList.begin() + overlappingIndex);
	
	LogConsole::writeConsole("Re %d %d swapped", index, overlappingIndex);
	m_relationshipList[index]->setIndex(index);
	m_relationshipList[overlappingIndex]->setIndex(overlappingIndex);

}

void ERData::sendRelationshipsToFront(REList relationships)
{
	int swap_offset = 0;
	for (Relationship* re : relationships)
	{
		int index = isInsideList(re, m_relationshipList);
		int swapIndex = swap_offset;
		if (index == -1)
		{
			LogConsole::writeConsole("no such relationship in list");
			continue;
		}
		if (index == swapIndex)
		{
			LogConsole::writeConsole("Relationship already in place");
			continue;
		}

		iter_swap(m_relationshipList.begin() + index, m_relationshipList.begin() + swapIndex);
		LogConsole::writeConsole("Re %d %d swapped", index, swapIndex);
		m_relationshipList[index]->setIndex(index);
		m_relationshipList[swapIndex]->setIndex(swapIndex);
		swap_offset++;
	}
}

int randomIndex(int size) {

	float r1 = (float)rand() / (float)RAND_MAX;
	int index1 = r1*size;

	return index1;
}

bool isInsideList(int index, vector<int> indexlist) {
	for (int i = 0; i < indexlist.size(); i++) {
		if (index == indexlist[i]){
			return true;
		}
	}
	return false;
}

bool ERData::isInsideRelationshipList(vector<Entity*> entityList) {

	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *re = m_relationshipList[i];
		bool res = re->isSameToRelationship(entityList);

		if (res)
			return true;
	}

	return false;
}

void ERData::checkData()
{
    set<int> indexset;

    for (int i = 0; i < m_entityList.size(); i++) {
        int index = m_entityList[i]->getIndex();
        if (index != i)
            LogConsole::showError("entity %d index does not equal to its order in data", i);

        bool inside = indexset.insert(index).second;
        if(!inside)
            LogConsole::showError("entity %d %d have same index", i);
        
    }

    indexset.clear();
    for (int i = 0; i < m_relationshipList.size(); i++) {
        int index = m_relationshipList[i]->getIndex();
        if (index != i)
            LogConsole::showError("polygon %d index does not equal to its order in data", i);

        bool inside = indexset.insert(index).second;
        if (!inside)
            LogConsole::showError("entity %d %d have same index", i);
    }
}

bool ERData::checkData(const ERData &d2)
{
    if (m_entityList.size() != d2.m_entityList.size())
    {
        LogConsole::showError("wrong copy");
        return false;
    }

    if (m_relationshipList.size() != d2.m_relationshipList.size())
    {
        LogConsole::showError("wrong copy");
        return false;
    }

    for (int i = 0; i < m_entityList.size(); i++) {
        Entity *en = m_entityList[i];
        Entity *compareToEn = d2.m_entityList[i];

        if (en->getIndex() != compareToEn->getIndex()) {
            LogConsole::showError("wrong copy");
            return false;
        }

        if(en->getAdjacentEntities().size() != compareToEn->getAdjacentEntities().size())
        {
            LogConsole::showError("wrong copy");
            return false;
        }

        for (int j = 0; j < en->getAdjacentEntities().size(); j++) {
            Entity *e1 = en->getAdjacentEntity(j);
            Entity *e2 = compareToEn->getAdjacentEntity(j);

            if(e1->getIndex() != e2->getIndex())
            {
                LogConsole::showError("wrong copy");
                return false;
            }
        }

        if (en->getIncidentRelationships().size() != compareToEn->getIncidentRelationships().size())
        {
            LogConsole::showError("wrong copy");
            return false;
        }

        for (int j = 0; j < en->getIncidentRelationships().size(); j++) {
            Relationship *e1 = en->getIncidentRelationship(j);
            Relationship *e2 = compareToEn->getIncidentRelationship(j);

            if (e1->getIndex() != e2->getIndex())
            {
                LogConsole::showError("wrong copy");
                return false;
            }
        }
    }

    for (int i = 0; i < m_relationshipList.size(); i++) {
        Relationship *re = m_relationshipList[i];
        Relationship *compareToRe = d2.m_relationshipList[i];

        if (re->getIndex() != compareToRe->getIndex()) {
            LogConsole::showError("wrong copy");
            return false;
        }

        if (re->getIncidentEntities().size() != compareToRe->getIncidentEntities().size())
        {
            LogConsole::showError("wrong copy");
            return false;
        }

        for (int j = 0; j < re->getIncidentEntities().size(); j++) {
            Entity *e1 = re->getIncidentEntity(j);
            Entity *e2 = compareToRe->getIncidentEntity(j);

            if (e1->getIndex() != e2->getIndex())
            {
                LogConsole::showError("wrong copy");
                return false;
            }
        }

        if (re->getAdjacentRelationships().size() != compareToRe->getAdjacentRelationships().size())
        {
            LogConsole::showError("wrong copy");
            return false;
        }

        for (int j = 0; j < re->getAdjacentRelationships().size(); j++) {
            Relationship *e1 = re->getAdjacentRelationship(j);
            Relationship *e2 = compareToRe->getAdjacentRelationship(j);

            if (e1->getIndex() != e2->getIndex())
            {
                LogConsole::showError("wrong copy");
                return false;
            }
        }
    }
}

void ERData::generateRandomRelationship(int card, int num) {

	//generate quads
	for (int i = 0; i < num; i++) {

		vector<Entity*> entitylist;
		vector<int> indexlist;


		for (int n = 0; n < card; n++) {

			int index = randomIndex(m_entityList.size());
			bool repeat = isInsideList(index,indexlist);
			while (repeat) {
				index = randomIndex(m_entityList.size());
				repeat = isInsideList(index, indexlist);
			}

			indexlist.push_back(index);

		}

		for (int n = 0; n < indexlist.size(); n++) {
			Entity *entity = m_entityList[indexlist[n]];
			entitylist.push_back(entity);
		}

		bool existed = isInsideRelationshipList(entitylist);
		if(!existed){
			Relationship *tri = new Relationship(entitylist);
			addRelationship(tri);

			tri->updateRelatedMonogons();
            tri->update2D();
		}
	}
}

//remove polygons with degree 0
void ERData::removeDagglingRelationships() {

	int i = 0;
	int count = 0;

	while (i < m_relationshipList.size()) {

		Relationship *poly = m_relationshipList[i];

		if (poly->getDegree() == 0) {
			deleteRelationship(poly);
			i--;
			count++;
		}
		i++;
	}
	LogConsole::writeConsole("removed relationships:%d", count);
}

void ERData::removeDagglingEntities() {

	int i = 0;
	int count = 0;

	while (i < m_entityList.size()) {

		Entity *entity = m_entityList[i];

		if (entity->getDegree() == 0) {
			deleteEntity(entity);
			i--;
			count++;
		}
		i++;
	}
	LogConsole::writeConsole("removed entities:%d", count);
}

void ERData::removeMonogons()
{
	vector<Relationship*> relist = m_relationshipList;

	int count = 0;
	for (auto &re : relist) {

		/*only remove monogon*/
		if (re->getCardinality() == 1) {
			
			count++;
			deleteRelationship(re);
		}
	}

	LogConsole::writeConsole("removed %d monogons", count);
}

void ERData::removeEntitiesWithoutDualElements()
{
	vector<Entity*> enlist = m_entityList;

	int count = 0;
	for (auto &en : enlist) {

		auto dualEle = en->getDualRelationship();

		if (dualEle == NULL) {
			removeEntityFromIncidents(en);
			deleteEntity(en);
			count++;
		}
	}

	LogConsole::writeConsole("removed %d entities", count);
}

void ERData::convertToLinear(int mergeThreshold)
{
	/*
	Greedy algorithm to convert the ER data into linear hypergraph
	if a polygon is sharing multiple vertices with another polygon, then merge them
	*/

	for (int i = 0; i < m_relationshipList.size(); i++) {
		RE* re = m_relationshipList[i];
		vector<RE*> toMerge;
		toMerge.push_back(re);
		for (int j = 0; j < re->getAdjacentRelationships().size(); j++) {
			RE *adj = re->getAdjacentRelationship(j);
			int sharedVertsNum = getSharedEntities(re, adj).size();

			int cardi = re->getCardinality();
			int cardj = adj->getCardinality();

			if (sharedVertsNum > mergeThreshold || cardi == sharedVertsNum || cardj == sharedVertsNum)
				toMerge.push_back(adj);
		}
		if (toMerge.size() >= 2) {
			mergeToRelationship(vector<Entity*>(), toMerge);
			i = 0;
		}
	}
}

void ERData::convertToSimple()
{
	vector<RE*> sortedREList = m_relationshipList;
	std::sort(sortedREList.begin(), sortedREList.end(), [](const Relationship *i, const Relationship *j) {
		return (i->getCardinality()<j->getCardinality());
	});


	int count = 0;

	for (int i = 0; i < sortedREList.size(); i++) {
		
		RE* re = sortedREList[i];

		/*check monogons*/
		if (re->getCardinality() == 1) {
			int degree = re->getDegree();
			if (degree > 0) {
				deleteRelationship(re);
				count++;
				i--;
			}
			continue;
		}

		for (int j = 0; j < re->getAdjacentRelationships().size(); j++) {
			RE *adj = re->getAdjacentRelationship(j);
			int sharedVertsNum = getSharedEntities(re, adj).size();

			if (sharedVertsNum == re->getCardinality())
			{
				deleteRelationship(re);
				count++;
				i--;
				break;
			}
		}
	}

	LogConsole::writeConsole("%d relationships are removed", count);

}

vector<int> ERData::convertMonogonsToDigons()
{
	vector<int> monogon_indices;
	for (Relationship* mon : getMonogons())
	{
		monogon_indices.push_back(mon->getIndex());
	}
	
	for (int index : monogon_indices)
	{
		Relationship* mon = getRelationship(index);
		Vec3 loc = mon->getIncidentEntity(0)->getLocation();
		double angle = mon->getMonogonRotation();
		double length = mon->getMonogonLength();
		Vec3 dir = 2.0 * length * Vec3(cos(angle), sin(angle), 0.0);
		Entity* newEn = new Entity(loc + dir);
		addEntity(newEn);
		addEntityToRelationship(newEn, getRelationship(index));
	}

	return monogon_indices;
}

void ERData::convertDigonsToMonogons(vector<int> indices)
{
	for (int index : indices)
	{
		if (getRelationship(index)->getCardinality() == 2)
		{
			Entity* en = getRelationship(index)->getIncidentEntity(1);
			deleteEntity(en, false);
			getRelationship(index)->update2D();
		}
	}
}

/*Energy Calculation*/
double ERData::calculateGeometryInfo()
{
	double sum = 0;


    LogConsole::writeConsole("-------GEOMETRY INFO--------");

	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *r = m_relationshipList[i];

        int num = r->getCardinality();
        LogConsole::writeConsole("R%d card(%d)", i, num);

		r->showGeometryInfo();
		double ev = r->calcRegularityEnergy_edgeVariance();
        LogConsole::writeConsole("R%d edge variance: %.16f", i, ev);

        double av = r->calcRegularityEnergy_angleVariance();
        LogConsole::writeConsole("R%d angle variance: %.16f", i, av);

		double area = r->area();
        LogConsole::writeConsole("R%d area :%.16f", i, area);

        double area2 = r->area2();
        LogConsole::writeConsole("R%d area2:%.16f", i, area2);

        double perimeter = r->perimeter();
        LogConsole::writeConsole("R%d perimeter: %.16f", i, perimeter);

		double C = 4.0 * (double)num*tan(M_PI / (double)num);
		LogConsole::writeConsole("R%d C: %f", i, C);

		double aspectRatio = perimeter*perimeter - C*area;
		LogConsole::writeConsole("R%d P^2 - C*area: %f", i, aspectRatio);


		double exp_ccr = gmainwindow->expectedLength *0.5 / sin(M_PI / num);
		LogConsole::writeConsole("expected ccr: %f", exp_ccr);

		double cur_ccr = r->calcCircumscribedCircleRadius();
		LogConsole::writeConsole("current ccr: %f", cur_ccr);

		Vec3 center = r->getCenter(0.0);
		LogConsole::writeConsole("R%d center: %f, %f, %f", i, center.x, center.y, center.z);

        
        double theta = (Element::m_DigonLensAngle) / 180.0*M_PI;
        double expectedAreaRatio = ((double)num*sin(theta / 2.0)*sin(theta / 2.0)) / (tan(M_PI / num)*(theta - sin(theta)));
        double ratio = area / expectedAreaRatio;

        LogConsole::writeConsole("expected ratio: %.16f", expectedAreaRatio);
        LogConsole::writeConsole("area/ratio: %.16f", ratio);
        LogConsole::writeConsole("current AR: %.16f", area/(perimeter*perimeter));
        LogConsole::writeConsole("");
	}

    LogConsole::writeConsole("-------END--------");

	return sum;
}

double ERData::calculateUniformAreaEnergy(double expectedArea)
{
	double sum = 0;

	if (expectedArea == 0) {
		LogConsole::showError("cannot set expected area as 0");
		return sum;
	}

	double sumArea = 0;

	int count = 0;
	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *r = m_relationshipList[i];

		if (r->getCardinality() < 3)
			continue;

		count++;
		double area = r->area();
		sumArea += area;
		r->calcUniformAreaEnergy(expectedArea);
	}

	double averageArea = sumArea / count;
	LogConsole::writeConsole("average area is %f", averageArea);

	return sum;
}

vector<int> ERData::getTwosVertsToBeFixed()
{
	vector<int> vindex;
	bool verts_found = false;
	
	//try find the digon
	if (!verts_found) {
		for (int i = 0; i < m_relationshipList.size(); i++) {
			
			if (m_relationshipList[i]->getCardinality() != 2)
				continue;

			int index1 = m_relationshipList[i]->getIncidentEntity(0)->getIndex();
			vindex.push_back(index1);
			int index2 = m_relationshipList[i]->getIncidentEntity(1)->getIndex();
			vindex.push_back(index2);

			verts_found = true;
			break;
		}
	}

	//try find a triangle's first two edges
	if (!verts_found) {
		for (int i = 0; i < m_relationshipList.size(); i++) {

			if (m_relationshipList[i]->getCardinality() != 3)
				continue;

			int index1 = m_relationshipList[i]->getIncidentEntity(0)->getIndex();
			vindex.push_back(index1);
			int index2 = m_relationshipList[i]->getIncidentEntity(1)->getIndex();
			vindex.push_back(index2);

			verts_found = true;
			break;
		}
	}

	//try find a poly with two consecutive degree 1 verts
	if (!verts_found) {
		for (int i = 0; i < m_relationshipList.size(); i++) {
			RE* r = m_relationshipList[i];
			
			vector<EN*> degreeOneVerts = r->getEntitiesByDegree(1);
			if (degreeOneVerts.size() < 2)
				continue;

			int insidePolyIndex1 = r->getEntityInsidePolyIndex(degreeOneVerts[0]);
			int insidePolyIndex2 = r->getEntityInsidePolyIndex(degreeOneVerts[1]);
			if (abs(insidePolyIndex1 - insidePolyIndex2) != 1)
				continue;

			int index1 = degreeOneVerts[0]->getIndex();
			int index2 = degreeOneVerts[1]->getIndex();

			vindex.push_back(index1);
			vindex.push_back(index2);

			verts_found = true;
			break;
		}
	}


	//find first poly's first two nodes
	if (!verts_found) {
		for (int i = 0; i < m_relationshipList.size(); i++) {

			if (m_relationshipList[i]->getCardinality() == 2)
				LogConsole::showError("error when finding fixed verts");

			if (m_relationshipList[i]->getCardinality() < 2)
				continue;

			int index1 = m_relationshipList[i]->getIncidentEntity(0)->getIndex();
			vindex.push_back(index1);
			int index2 = m_relationshipList[i]->getIncidentEntity(1)->getIndex();
			vindex.push_back(index2);

			verts_found = true;
			break;
		}
	}

	return vindex;
}

vector<int> ERData::getSortedFixedCompIndices() const
{
	vector<int>fixedVertCompTemp = fixedVertComp;
	std::sort(fixedVertCompTemp.begin(), fixedVertCompTemp.end(), [](int x, int y) { return x < y; });
	return fixedVertCompTemp;
}

bool ERData::isVertFree(int vertIndex)
{
	int x_listindex = vertex_index_x_to_list_index(vertIndex, fixedVertComp);
	int y_listindex = vertex_index_y_to_list_index(vertIndex, fixedVertComp);

	if (x_listindex == -1 || y_listindex == -1)
		return false;
	else
		return true;
}

int ERData::getFreeVertCompNum() const
{
	int fixedCompNum = fixedVertComp.size();
	int N = getEntityNum() * 2 - fixedCompNum;
	return N;
}

vector<int> ERData::getSortedFixedMonoIndices() const
{
	/*vector<int> fixedMonoCompTemp = fixedMonoComp;
	std::sort(fixedMonoCompTemp.begin(), fixedMonoCompTemp.end());
	return fixedMonoCompTemp;*/
	return fixedMonoComp;
}

bool ERData::isMonogonFixed(int monoIndex)
{
	int nv = getFreeVertCompNum();
	int listindex = monogon_index_to_list_index(monoIndex, nv, fixedMonoComp);

	if (listindex == -1)
		return false;
	else
		return true;
}

int ERData::getFreeMonoCompNum() const
{
	int fixedCompNum = fixedMonoComp.size();
	int M = getMonogonNum() - fixedCompNum;
	return M;
}


void ERData::getRelationshipColorVariableRange(ELEMENT_COLORSCHEME colorScheme)
{
	maxColorVariable = -DBL_MAX;
	minColorVariable = DBL_MAX;

	switch (colorScheme)
	{
	case ELEMENT_COLORSCHEME::COLOR_INDEX:
		maxColorVariable = m_relationshipList.size();
		minColorVariable = 0;
		break;
	case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
		maxColorVariable = getMaxIncidence();
		minColorVariable = getMinIncidence();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
		maxColorVariable = getMaxAdjacency();
		minColorVariable = getMinAdjacency();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
		maxColorVariable = getMaxAdjacencyFactor();
		minColorVariable = getMinAdjacencyFactor();
		break;
	case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
		maxColorVariable = getMaxBetweennessCentrality();
		minColorVariable = getMinBetweennessCentrality();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
		maxColorVariable = getMaxEccentricity();
		minColorVariable = getMinEccentricity();
		break;
	case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
		maxColorVariable = getMaxClosenessCentrality();
		minColorVariable = getMinClosenessCentrality();
		break;
	default:
		break;
	}
}

void ERData::assignRelationshipColors(ELEMENT_COLORSCHEME colorScheme, ColorGradient colorGradient)
{
	getRelationshipColorVariableRange(colorScheme);

	for (Relationship* re : m_relationshipList)
	{
		float value = 0;
		switch (colorScheme)
		{
		case ELEMENT_COLORSCHEME::COLOR_INDEX:
			value = re->getIndex();
			break;
		case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
			value = getRelationshipIncidence(re);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
			value = getRelationshipAdjacency(re);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
			value = getRelationshipAdjacencyFactor(re);
			break;
		case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
			value = getRelationshipBetweennessCentrality(re);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
			value = getRelationshipEccentricity(re);
			break;
		case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
			value = getRelationshipClosenessCentrality(re);
			break;
		default:
			break;
		}

		if (abs(maxColorVariable - minColorVariable) > LOW_EPI)
		{
			value = float(value - maxColorVariable) / float(minColorVariable - maxColorVariable);

			float r, g, b;
			colorGradient.getColorAtValue(value, r, g, b);
			re->setColor(Vec4(r, g, b, 0.5));
		}
	}
}

void ERData::assignRelationshipColors(ELEMENT_COLORSCHEME colorScheme)
{
	for (Relationship* re : m_relationshipList)
	{
		Vec4 color;
		switch (colorScheme)
		{
		case ELEMENT_COLORSCHEME::COLOR_INDEX:
			color = Element::getDiscreteColor(re->getIndex());
			break;
		case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
			color = Element::getDiscreteColor(getRelationshipIncidence(re));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
			color = Element::getDiscreteColor(getRelationshipAdjacency(re));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
			color = Element::getDiscreteColor(getRelationshipAdjacencyFactor(re));
			break;
		case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
			color = Element::getDiscreteColor(getRelationshipBetweennessCentrality(re));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
			color = Element::getDiscreteColor(getRelationshipEccentricity(re));
			break;
		case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
			color = Element::getDiscreteColor(getRelationshipClosenessCentrality(re));
			break;
		default:
			color = Element::getDiscreteColor(0);
			break;
		}

		re->setColor(color);
	}
}

void ERData::assignRelationshipColors()
{
	for (auto &re : m_relationshipList) {
		Vec4 color = Element::getDiscreteColor(re->getCardinality());
		re->setColor(color);
	}
}

void ERData::assignRelationshipColors(vector<Vec4> colors)
{
	for (auto &re : m_relationshipList) {

		Vec4 color;

		int card = re->getCardinality();
		if (card < colors.size())
			color = colors[card];
		else
			color = colors.back();
		
		re->setColor(color);
	}
}

void ERData::assignRelationshipColorsBySetIntersections()
{
	updateRelationshipsBoundingBox();
	updateRelationshipIndices();

	int pcount = m_relationshipList.size();
	if (pcount == 0)
		return;
	
	int *colorIndices = new int[pcount];
	for (int i = 0; i < pcount; i++) {
		colorIndices[i] = -1;
	}

	int decidedColor = 0;

	while (decidedColor < pcount) {
		//queue for flooding polys
		queue<RE*> adj_poly_queue;

		/*find first poly wihtout decided color*/
		int first_p_no_color = 0;
		for (int i = 0; i < pcount; i++) {
			if (colorIndices[i] == -1)
			{
				first_p_no_color = i;
				break;
			}
		}
		adj_poly_queue.push(m_relationshipList[first_p_no_color]);

		while (!adj_poly_queue.empty()) {
			RE* cp = adj_poly_queue.front();
			int cp_index = cp->getIndex();
			adj_poly_queue.pop();

			/*get all adj polys and update the queue*/
			set<int>adj_colors;
			int colorTimes[COLORCOUNT] = {0};

			/*get overlapping polys and adj polys*/
			vector<RE*> overlap_polys;
			for (int i = 0; i < pcount; i++) {
				RE* re = m_relationshipList[i];
				if (re == cp)
					continue;

				if (checkIfTwoRelationAdjacent(re, cp)) {
					overlap_polys.push_back(re);
					continue;
				}

				if (isBoundingBoxIntersect(re, cp)) {
					overlap_polys.push_back(re);
					continue;
				}
			}

			for (int j = 0; j < overlap_polys.size(); j++) {
				RE* adjp = overlap_polys[j];
				int adjp_index = adjp->getIndex();

				if(colorIndices[adjp_index] == -1)
					adj_poly_queue.push(adjp);
				else
				{
					adj_colors.insert(colorIndices[adjp_index]);
					int colorIndex = colorIndices[adjp_index];
					colorTimes[colorIndex]++;
				}
			}

			if(overlap_polys.size() >= COLORCOUNT - 1)
			{
				/*if there are more than COLORCOUNT - 1 adj polys
				then use the color used least*/
				int minColorTimes = colorTimes[0];
				int minColorIndex = 0;
				for (int i = 0; i < COLORCOUNT; i++) {
					if (colorTimes[i] < minColorTimes) {
						minColorTimes = colorTimes[i];
						minColorIndex = i;
					}
				}
				decidedColor++;
				colorIndices[cp_index] = minColorIndex;
				assert(minColorIndex < COLORCOUNT);
				Vec4 color = Element::polyColor[minColorIndex] / 255.0;
				cp->setColor(color);
			}
			else {
				/*Use the color not used*/
				for (int i = 0; i < COLORCOUNT; i++) {
					if (adj_colors.find(i) == adj_colors.end()) {
						decidedColor++;
						colorIndices[cp_index] = i;
						assert(i < COLORCOUNT);
						Vec4 color = Element::polyColor[i] / 255.0;
						cp->setColor(color);
						break;
					}
				}
			}
		}
	}
	delete[] colorIndices;
}

void ERData::variateRelationshipColors(ColorVariationType type, double scale)
{
	srand(129745u);
	updateRelationshipsBoundingBox();
	vector<RE*> sortedREList = m_relationshipList;
	std::sort(sortedREList.begin(), sortedREList.end(), [](const Relationship *i, const Relationship *j) {
		return (i->getDegree()>j->getDegree());
	});

	auto isSimilarColor = [](Vec4 color1, Vec4 color2, double scale) {

		Vec4 diff = color2 - color1;
		double len = length(diff);

		if (len < 0.5*scale)
			return true;

		return false;
	};

	auto variateColor = [](Vec4 color1, double scale, ColorVariationType type) {

		QColor oldcolor = QColor(color1.R*255, color1.G * 255, color1.B * 255, color1.Alpha * 255);
		int hue, sat, value, alpha;
		oldcolor.getHsv(&hue, &sat, &value, &alpha);
		double randscale = scale*2.0*(double(rand()) / RAND_MAX-0.5);

		if (type == ColorVariationType_HUE) {
			int oldhue = hue;
			hue = oldhue + randscale*360.0;
			hue = hue % 360;
			if (hue < 0)
				hue += 360;
		}
		else if(type == ColorVariationType_ALPHA){
			
			/*only decrease the transpancy*/
			alpha = alpha + randscale *255;
			if (alpha > 255)
				alpha = 255;
			if (alpha < 0)
				alpha = 0;
		}

		QColor newColor = QColor::fromHsv(hue, sat, value, alpha);
		Vec4 color_var(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF());
		return color_var;
	};

	while (true) {

		int count = 0;

		for (int i = 0; i < sortedREList.size(); i++) {

			auto re1 = sortedREList[i];

			for (int j = i + 1; j < sortedREList.size(); j++) {

				auto re2 = sortedREList[j];

				Vec4 color = re1->getColor();
				Vec4 adjcolor = re2->getColor();

				if (!checkIfTwoRelationAdjacent(re1, re2) && !isBoundingBoxIntersect(re1, re2)) {
					continue;
				}

				if (isSimilarColor(color, adjcolor, scale))
				{
					color = variateColor(color, scale, type);
					re1->setColor(color);
					count++;
				}
			}
		}

		LogConsole::writeConsole("%d colors changed", count);

		if (count == 0)
			break;
	}

}

void ERData::assignRelationshipColorsBySubstring(vector<std::string> subs)
{
	for (Relationship* re : m_relationshipList)
	{
		std::string label = re->getLabel();
		Vec4 color = Vec4(1.0, 1.0, 1.0, 0.5);
		for (int i = 0; i < subs.size(); i++)
		{
			std::string sub = subs.at(i);
			if (label.find(sub) != std::string::npos)
				color = Element::getDiscreteColor(i+1);
		}
		re->setColor(color);
	}
}


void ERData::assignEntitiesColorsDual()
{
	for (auto &re : m_entityList) {
		Vec4 color = Element::getDiscreteColor(re->getDegree());
		color.Alpha = 1.0;
		re->setColor(color);
	}
}

void ERData::assignEntitiesColors(ELEMENT_COLORSCHEME colorScheme, ColorGradient colorGradient)
{
	double max = -DBL_MAX;
	double min = DBL_MAX;

	switch (colorScheme)
	{
	case ELEMENT_COLORSCHEME::COLOR_INDEX:
		max = m_entityList.size();
		min = 0;
		break;
	case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
		max = getMaxIncidence();
		min = getMinIncidence();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
		max = getMaxAdjacency();
		min = getMinAdjacency();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
		max = getMaxAdjacencyFactor();
		min = getMinAdjacencyFactor();
		break;
	case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
		max = getMaxBetweennessCentrality();
		min = getMinBetweennessCentrality();
		break;
	case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
		max = getMaxEccentricity();
		min = getMinEccentricity();
		break;
	case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
		max = getMaxClosenessCentrality();
		min = getMinClosenessCentrality();
		break;
	default:
		break;
	}

	for (Entity* en : m_entityList)
	{
		float value = 0;
		switch (colorScheme)
		{
		case ELEMENT_COLORSCHEME::COLOR_INDEX:
			value = en->getIndex();
			break;
		case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
			value = getEntityIncidence(en);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
			value = getEntityAdjacency(en);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
			value = getEntityAdjacencyFactor(en);
			break;
		case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
			value = getEntityBetweennessCentrality(en);
			break;
		case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
			value = getEntityEccentricity(en);
			break;
		case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
			value = getEntityClosenessCentrality(en);
			break;
		default:
			break;
		}

		if (abs(max - min) > LOW_EPI)
		{
			value = float(value - max) / float(min - max);

			float r, g, b;
			colorGradient.getColorAtValue(value, r, g, b);
			en->setColor(Vec4(r, g, b, 1.0));
		}
	}
}

void ERData::assignEntitiesColors(ELEMENT_COLORSCHEME colorScheme)
{
	for (Entity* en : m_entityList)
	{
		Vec4 color;
		switch (colorScheme)
		{
		case ELEMENT_COLORSCHEME::COLOR_INDEX:
			color = Element::getDiscreteColor(en->getIndex());
			break;
		case ELEMENT_COLORSCHEME::COLOR_INCIDENCE:
			color = Element::getDiscreteColor(getEntityIncidence(en));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJACENCY:
			color = Element::getDiscreteColor(getEntityAdjacency(en));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ADJFACTOR:
			color = Element::getDiscreteColor(getEntityAdjacencyFactor(en));
			break;
		case ELEMENT_COLORSCHEME::COLOR_BETWEENNESS:
			color = Element::getDiscreteColor(getEntityBetweennessCentrality(en));
			break;
		case ELEMENT_COLORSCHEME::COLOR_ECCENTRICITY:
			color = Element::getDiscreteColor(getEntityEccentricity(en));
			break;
		case ELEMENT_COLORSCHEME::COLOR_CLOSENESS:
			color = Element::getDiscreteColor(getEntityClosenessCentrality(en));
			break;
		default:
			break;
		}

		color.Alpha = 1.0;
		en->setColor(color);
	}
}

void ERData::assignEntitiesColors(Vec4 color)
{
	for (auto &en : m_entityList) {
		en->setColor(color);
	}
}

void ERData::syncColorsWithDualElements(bool withAlpha)
{
	for (const auto &re : m_relationshipList) {
		Vec4 color = re->getColor();
		color *= Vec4(0.8, 0.8, 0.8, 1.0);
		auto dual = re->getDualEntity();
		if (dual) {
			if (!withAlpha)
				color.Alpha = 1.0;
			dual->setColor(color);
		}
	}

	//for (const auto &en : m_entityList) {
	//	Vec4 color = en->getColor();
	//	auto dual = en->getDualRelationship();
	//	if (dual) {
	//		if (!withAlpha)
	//			color.Alpha = 0.5;
	//		dual->setColor(color);
	//	}
	//}
}

void ERData::assignEntitiesColorsBySubstring(vector<std::string> subs)
{
	for (Entity* en : m_entityList)
	{
		std::string label = en->getLabel();
		Vec4 color = Vec4(0.8, 0.8, 0.8, 1.0);
		for (int i = 0; i < subs.size(); i++)
		{
			std::string sub = subs.at(i);
			if (label.find(sub) != std::string::npos)
				color = Element::getDiscreteColor(i+1);
		}
		color.Alpha = 1.0;
		en->setColor(color);
	}
}


void ERData::designLayout_Grid(double sidelength)
{
	vector<EN*> unfixedverts = m_entityList;
	
	{
		int count = unfixedverts.size();

		double h = sidelength;
		double w = sidelength;
		double steph, stepw;

		int n = ceil(sqrt(count))/2;

		if (n == 1) {
			steph = h;
			stepw = w;
		}
		else {

			steph = h / (n - 1);
			stepw = w / (n - 1);
		}

		for (int i = 0; i < count; i++) {
			int x = i%n;
			int y = i / n;
			unfixedverts[i]->setLocation(x*stepw - w/2.0, y*steph - h/2.0, 0);
		}
	}

	update2D();
}

void ERData::designLayout_GridCircle(double radius, double fixedDisp)
{
	double centerX = 0;
	double centerY = 0;

	set<int> vert_indices;
	for (int i = 0; i < fixedVertComp.size(); i++) {
		int fvc = fixedVertComp[i];
		int vIndex = fvc / 2;
		vert_indices.insert(vIndex);
	}

	vector<int> indices(vert_indices.begin(), vert_indices.end());
	if (vert_indices.size() != 0) {
		getEntity(indices[0])->setLocation(-fixedDisp, 0);
		getEntity(indices[1])->setLocation(fixedDisp, 0);
	}

	if (fixedDisp != radius || vert_indices.size() == 0) {
		//do not leave space for fixed verts 
		double offsetangle = 2.0* M_PI / (m_entityList.size() - vert_indices.size());

		int count = 0;
		for (unsigned int j = 0; j < m_entityList.size(); j++)
		{
			if (find(indices.begin(), indices.end(), j) != indices.end())
				continue;

			Entity* en = m_entityList[j];
			double xPos = centerX + radius*cos(offsetangle*count);
			double yPos = centerY + radius*sin(offsetangle*count);
			en->setLocation(Vec3(xPos, yPos, 0));
			count++;
		}
	}
	else {
		//Leave space for fixed verts
		int fixedVertsNum = indices.size();
		int toDistributeVertNum = (m_entityList.size() - fixedVertsNum);
		double offsetangle = 2.0* M_PI / toDistributeVertNum;

		int count = 0;
		for (unsigned int j = 0; j < m_entityList.size(); j++)
		{
			if (find(indices.begin(), indices.end(), j) != indices.end())
				continue;

			Entity* en = m_entityList[j];
			double xPos = centerX + radius*cos(offsetangle*(count + 0.2));
			double yPos = centerY + radius*sin(offsetangle*(count + 0.2));//0.5 offsetangle to avoid overlap with fixed verts
			en->setLocation(Vec3(xPos, yPos, 0));
			count++;
		}
	}

	update2D();
}

void ERData::designLayout_RectGrid(int dimension, bool col_wise, float start_x, float start_y, float col_sep, float row_sep, ENList enlist)
{
	if (enlist.empty())
		return;

	for (int i = 0; i < enlist.size(); i++)
	{
		int dim1 = i % dimension;
		int dim2 = i / dimension;
		if (col_wise)
			enlist[i]->setLocation(dim1 * col_sep + start_x, dim2 * row_sep + start_y, 0);
		else
			enlist[i]->setLocation(dim2 * col_sep + start_x, dim1 * row_sep + start_y, 0);
	}
	update2D();
}

void ERData::designLayout_CircGrid(float center_x, float center_y, float radius, float start, ENList enlist, ELEMENT_STATISTIC sort, bool ascending)
{
	if (enlist.empty())
		return;

	if (sort != ELEMENT_STATISTIC::ELEM_NONE)
		std::sort(enlist.begin(), enlist.end(), EntityCompare(sort, ascending));

	float gap = 2 * M_PI / (float)enlist.size();
	for (int i = 0; i < enlist.size(); i++)
	{
		float a = start + i * gap;
		enlist[i]->setLocation(Vec3(cos(a) * radius + center_x, sin(a) * radius + center_y, 0));
	}
	update2D();
}

void ERData::designLayout_linear(double spacex)
{
	int count = m_entityList.size();

	for (int i = 0; i < count; i++) {

		Vec3 start = Vec3(0, 0, 0);
		Vec3 loc = start + Vec3(spacex, 0, 0)*i;
		m_entityList[i]->setLocation(loc);
	}

	update2D();
}

void ERData::designLayout_GridEisenstein()
{
	int count = m_entityList.size();

	double h = 1.9;
	double w = 1.9;
	double steph, stepw;

	int n = ceil(sqrt(count));

	if (n == 1) {
		steph = h;
		stepw = w;
	}
	else {
		steph = h / (n - 1);
		stepw = w / (n - 1);
	}

	Vec3 re = Vec3(stepw, 0, 0);
	Vec3 im = Vec3(stepw*0.5, stepw*0.5*sqrt(3), 0);

	for (int i = 0; i < count; i++) {
		int x = i % n;
		int y = i / n;

		Vec3 start = Vec3(-0.9, -0.9, 0);
		Vec3 loc = start + x*re + y*im;
		m_entityList[i]->setLocation(loc);
	}

	update2D();
}

void ERData::designLayout_RandomShuffle(unsigned rseed)
{
	std::srand(rseed);
	std::vector<Vec3> locations;
	for (int i = 0; i < m_entityList.size(); i++) {
		if (!m_entityList[i]->flag_Optimizable)
			continue;
		locations.push_back(m_entityList[i]->getLocation());
	}

	std::random_shuffle(locations.begin(), locations.end());

	int count = 0;
	for (int i = 0; i < m_entityList.size(); i++) {
		if (!m_entityList[i]->flag_Optimizable)
			continue;
		m_entityList[i]->setLocation(locations[count]);
		count++;
	}

    update2D();
}

void ERData::designLayout_Random(double radius)
{
	//srand(100000);

	for (int i = 0; i < m_entityList.size(); i++) {

		float x = (float)rand() / (float)RAND_MAX;
		float y = (float)rand() / (float)RAND_MAX;

		x = x*2.0*radius - radius;
		y = y*2.0*radius - radius;

		m_entityList[i]->setLocation(Vec3(x, y, 0));
	}

    update2D();
}

void ERData::designLayout_RandomRectangular(double min_x, double min_y, double width, double height, ENList enlist)
{
	if (enlist.empty())
		return;

	for (Entity* en : enlist)
	{
		double x = (double)rand() / (double)RAND_MAX;
		double y = (double)rand() / (double)RAND_MAX;

		x = x * width + min_x;
		y = y * height + min_y;

		en->setLocation(Vec3(x, y, 0));
	}
	update2D();
}

void ERData::designLayout_RandomCircular(double x, double y, double rad_min, double rad_max, ENList enlist)
{
	if (enlist.empty())
		return;

	double diff = rad_max - rad_min;
	for (Entity* en : enlist)
	{
		double r = (double)rand() / (double)RAND_MAX;
		double a = (double)rand() / (double)RAND_MAX;

		r = r * diff + rad_min;
		a = a * 2 * M_PI;

		en->setLocation(Vec3(cos(a) * r + x, sin(a) * r + y, 0));
	}
	update2D();
}

void ERData::designLayout_Random3D()
{
	for (int i = 0; i < m_entityList.size(); i++) {

		float x = (float)rand() / (float)RAND_MAX;
		float y = (float)rand() / (float)RAND_MAX;
		float z = (float)rand() / (float)RAND_MAX;

		x = x*2.0 - 1.0;
		y = y*2.0 - 1.0;
		z = z*2.0 - 1.0;

		m_entityList[i]->setLocation(Vec3(x, y, z));
	}

    update2D();
}

void ERData::designLayout_colocated(Vec3 vec)
{
	for (int i = 0; i < m_entityList.size(); i++) {
		m_entityList[i]->setLocation(vec);
	}

    update2D();
}

double ERData::updateStep(double step, double curr_energy, double last_energy, int &progress)
{

	double t = 0.9;

	if (curr_energy < last_energy) {
		progress++;

		if (progress >= 5) {
			progress = 0;
			step = step / t;
		}
	}
	else {
		progress = 0;
		step = step * t;
	}

	return step;
}

Vec3 ERData::getAttractiveForce(Entity * en, /*vector<Entity*> enList,*/ double edge_length)
{
	Vec3 force = Vec3(0.0);

	for (int i = 0; i < en->getAdjacentEntities().size(); i++) 
	{
		Vec3 dir = en->getAdjacentEntity(i)->getLocation() - en->getLocation();

		double f = length(dir)*length(dir) / edge_length;
		normalize(dir);
		force += f*dir;
	}

	return force;
}

Vec3 ERData::getRepulsiveForce(Entity * en, vector<Entity*> enList, double edge_length, double relative_coef)
{
	Vec3 force = Vec3(0.0);

	for (int i = 0; i < enList.size(); i++) {

		if (en == enList[i])
			continue;

		Vec3 dir = enList[i]->getLocation() - en->getLocation();

		double f;
		if (length(dir) <= 0.00000001)
			f = -relative_coef * edge_length * edge_length / 0.00000001;
		else {
			double w = length(dir);
			f = -relative_coef * edge_length * edge_length / w;
		}

		normalize(dir);
		force += f*dir;
	}

	return force;
}

Vec3 ERData::getRepulsiveForce2(Entity * en, vector<Entity*> enList, double edge_length, double relative_coef, double div_pow)
{
	Vec3 force = Vec3(0.0);

	for (int i = 0; i < enList.size(); i++) {

		if (en == enList[i])
			continue;

		Vec3 dir = enList[i]->getLocation() - en->getLocation();

		double f;
		if (length(dir) <= 0.00000001)
			f = -relative_coef*edge_length*edge_length / 0.00000001;
		else {
			double w = length(dir);
			f = -relative_coef * edge_length * edge_length / pow(w, div_pow);
		}

		normalize(dir);
		force += f*dir;
	}

	return force;
}

void ERData::designLayout_ForceDirected(vector<Entity*> enList, double step, double edge_length, double relative_coef, double div_pow, int times, double tol, bool showAnimation)
{
	double energy, last_energy;

	srand(100000);

	energy = INFITE;
	bool converged = false;
	int count = 0;
	int progress = 0;
	while (!converged && count <= times) 
	{
		QCoreApplication::processEvents();

		if (gmainwindow->flag_stopOptimization)
			break;

		last_energy = energy;
		energy = 0;
		//calculate energy for all vertices
		for (Entity* en : m_entityList) 
		{
			if (!isEntityInsideEntityList(en, enList) || !en->flag_Optimizable)
				continue;

			Vec3 force(0.0);
			//calculate attractive forces
			force += getAttractiveForce(en, edge_length);
			//calculate repulsive forces
			force += getRepulsiveForce2(en, m_entityList, edge_length, relative_coef, div_pow);

			//update location of entity
			if (length(force) < 10e-13) 
			{
				float x = double(rand()) / double(RAND_MAX)*M_PI;
				float y = double(rand()) / double(RAND_MAX)*M_PI;

				float dx = (x - 0.5)*0.1;
				float dy = (y - 0.5)*0.1;

                en->translate(dx, dy);
			}
			else
				en->translate(step * force / length(force), STARRIZATIONR_FLAG::NOTSTARRIZE, UPDATESCAFFOLD_FLAG::NOTUPDATESCAFFOLD);

			//update energy
			energy += length(force)*length(force);
		}

		step = updateStep(step, energy, last_energy, progress);

		if(showAnimation)
			update2D();
		
		if (step < 1e-5 * step)
			step = 1e-5 * step;

		if (abs(energy - last_energy) < edge_length * tol) {
			LogConsole::writeConsole("FD Converged: Energy does not decrease");
			converged = true;
		}

		if (energy <= 1e-4) {
			LogConsole::writeConsole("FD Converged: Energy less than 1e-4");
			converged = true;
		}

		if (showAnimation)
		{
			update2D();
			gmainwindow->refreshOpenGLWindow();
		}

		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		count++;
	}
}

void ERData::constructRelationshipInfos()
{
	reInfos.clear();
	reInfos.resize(m_relationshipList.size());

	int count = 0;
	for (int i = 0; i < m_relationshipList.size(); i++) {
		reInfos[i].card = m_relationshipList[i]->getCardinality();

		if (reInfos[i].card == 1) {
			reInfos[i].monogonIndex = count;
			count++;
		}

		reInfos[i].sameReList.resize(m_relationshipList.size());
		for (int j = 0; j < m_relationshipList.size(); j++) {

			bool issame = isTwoResSame(i, j);
			reInfos[i].sameReList[j] = issame;
		}
	}
}

int ERData::getNumberOfAjacentMonogon(int vindex) const
{
	Entity *v1 = m_entityList[vindex];
	int sum = 0;
	for (int i = 0; i < v1->getIncidentRelationships().size(); i++) {

		Relationship *p1 = v1->getIncidentRelationship(i);

		if (p1->getCardinality() == 1) {
			sum++;
		}
	}

	return sum;
}

void ERData::designLayout_FDMaxent_FixedVerts(vector<Entity*> enList, vector<Entity*> fixedEnList, double step, double edge_length, double relative_coef, double div_pow, int times, double tol, bool showAnimation)
{
	double energy, last_energy;

	srand(100000);

	energy = INFITE;
	bool converged = false;
	int count = 0;
	int progress = 0;
	while (!converged && count <= times) {

		QCoreApplication::processEvents();

		if (gmainwindow->flag_stopOptimization)
			break;

		last_energy = energy;
		energy = 0;
		//calculate energy for all vertices
		for (int i = 0; i < enList.size(); i++) {

			int fixed = isInsideList<Entity>(enList[i], fixedEnList);
			if (fixed != -1)
				continue;

			Vec3 force = Vec3(0.0);

			//calculate attractive forces
			force += getAttractiveForce(enList[i], edge_length);
			//calculate repulsive forces
			force += getRepulsiveForce2(enList[i], enList, edge_length, relative_coef, div_pow);

			//update location of entity
			if (length(force) < 10e-13) {
				float x = double(rand()) / double(RAND_MAX)*M_PI;
				float y = double(rand()) / double(RAND_MAX)*M_PI;

				float dx = (x - 0.5)*0.1;
				float dy = (y - 0.5)*0.1;

				enList[i]->translate(dx, dy);
			}
			else
				enList[i]->translate(step*force / length(force), STARRIZATIONR_FLAG::NOTSTARRIZE, UPDATESCAFFOLD_FLAG::NOTUPDATESCAFFOLD);

			//update energy
			energy += length(force)*length(force);
		}

		step = updateStep(step, energy, last_energy, progress);

		if (step < 1e-5*step)
			step = 1e-5*step;

		if (abs(energy - last_energy) < edge_length * tol) {
			LogConsole::writeConsole("FD Converged: Energy does not decrease");
			converged = true;
		}

		if (energy <= 1e-4) {
			LogConsole::writeConsole("FD Converged: Energy less than 1e-4");
			converged = true;
		}

		if (showAnimation)
		{
			update2D();
			gmainwindow->refreshOpenGLWindow();
		}


		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		count++;
	}

	update2D();
}

bool ERData::checkEnclosing(RE * r1, RE * r2)
{
	//if one poly is sub of another poly, then they must overlap
	int sharedVertsNum = getSharedEntities(r1, r2).size();
	if (sharedVertsNum == r1->getCardinality() || sharedVertsNum == r2->getCardinality()) {
		return true;
	}

	return false;
}

//add edges to each polygon
bool ERData::convertToGraphData() {

	int newEdges = 0;

	vector<Relationship*> currentRElist = m_relationshipList;

	for (int i = 0; i < currentRElist.size(); i++) {
		
		Relationship *r = currentRElist[i];

		if (r->getCardinality() <= 2)
			continue;

		for (int j = 0; j < r->getIncidentEntities().size(); j++) {
			
			Entity *e1 = r->getIncidentEntity(j);
			
			for (int k = j + 1; k < r->getIncidentEntities().size(); k++) {
				
				Entity *e2 = r->getIncidentEntity(k);
				
				vector<Entity*> list;
				list.push_back(e1);
				list.push_back(e2);
				int res = isInsideRelationshipList(list);
				if (!res) {
					Relationship *newr = new Relationship(list);
                    newr->update2D();
					addRelationship(newr);
					newEdges++;
				}
			}
		}

		deleteRelationship(r);
	}

	initializeMonogonOrientations();

	LogConsole::writeConsole("binary relationships:%d", newEdges);
	return true;
}

bool ERData::convertToBipartiteGraphData()
{
	int renum = m_relationshipList.size();

	for (int i = 0; i < renum; i++)
	{
		Relationship* re = m_relationshipList[i];
		Entity* new_en = new Entity(re->getCenter(re->getMonogonLength()));
		new_en->setLabel(re->getLabel());
		new_en->setColor(re->getColor());
		addEntity(new_en);

		for (Entity* en : re->getIncidentEntities())
		{
			Relationship* new_re = new Relationship(new_en, en);
			addRelationship(new_re);
		}
		
		deleteRelationship(re);
	}

	return true;
}

bool ERData::getEntityLocationsFromGraphData(ERData * dst) {

	if(dst->m_entityList.size() != this->m_entityList.size()){
		LogConsole::showError("The number of entities in graph version has been changed, please regenerate the graph.");
		return false;
	}

	for (int i = 0; i < dst->m_entityList.size(); i++) {
		m_entityList[i]->setLocation(dst->m_entityList[i]->getLocation());
	}

	return true;
}

//find cliques
bool ERData::convertToERData(ERData * &dst) {

	vector<Entity*> R;
	vector<Entity*> X;
	vector<Entity*> P = m_entityList;
	vector<vector<Entity*>> cliques;
	BronKerbosh(R, P, X, cliques);

	for (int i = 0; i < cliques.size(); i++) {
		Relationship *r = new Relationship(cliques[i]);
        r->update2D();
		addRelationship(r);
	}

	return true;
}

int ERData::findCliques(vector<Entity*> entityList)
{
    set<Entity*> R;
    set<Entity*> X;
    set<Entity*> P;
    vector<vector<Entity*>> cliques;

    for (int i = 0; i<entityList.size(); i++)
        P.insert(entityList[i]);

    BronKerbosh(R, P, X, cliques);

    for (int i = 0; i < cliques.size(); i++) {
        Relationship *r = new Relationship(cliques[i]);
        r->update2D();
		addRelationship(r);
    }

	return cliques.size();
}

bool ERData::findClique(vector<Entity*> enlist)
{

	bool res = isEntitiesClique(enlist);

	if (res) {
		LogConsole::writeConsole("this is a clique");
	}

	return false;
}

void ERData::BronKerbosh(vector<Entity*>& R, vector<Entity*>& P, vector<Entity*>& X, vector<vector<Entity*>>& cliques)
{

	if (P.size() == 0 && X.size() == 0) {
		if (R.size() > 2) {
			cliques.push_back(R);
			LogConsole::writeConsole("Found one clique");
		}
	}

	for (int index = 0; index < P.size(); index++) {

		Entity *v = P[index];

		//R union v
		vector<Entity*> RUnionV = R;
		int k = isInsideList<Entity>(v, RUnionV);
		if (k == -1)
			RUnionV.push_back(v);

		//P intersect N(v)
		vector<Entity*> PintersectV;
		for (int i = 0; i < v->getAdjacentEntities().size(); i++) {
			int inside = isInsideList<Entity>(v->getAdjacentEntity(i), P);
			if (inside != -1)
				PintersectV.push_back(v->getAdjacentEntity(i));
		}

		//X intersect N(v)
		vector<Entity*> XintersectV;
		for (int i = 0; i < v->getAdjacentEntities().size(); i++) {
			int inside = isInsideList<Entity>(v->getAdjacentEntity(i), X);
			if (inside != -1)
				XintersectV.push_back(v->getAdjacentEntity(i));
		}

		BronKerbosh(RUnionV, PintersectV, XintersectV, cliques);

		//P = P exclude v
		int insidep = isInsideList<Entity>(v, P);
		if (insidep != -1) {
			P.erase(P.begin() + insidep);
			index--;
		}

		//X = X union v
		int insidex = isInsideList<Entity>(v, X);
		if (insidex == -1)
			X.push_back(v);
	}
}

void ERData::BronKerbosh(set<Entity*> R, set<Entity*> P, set<Entity*> X, vector<vector<Entity*>>& cliques)
{
    if (P.size() == 0 && X.size() == 0) {
        if (R.size() > 2) {
            vector<Entity*> oneClique;
            for (std::set<Entity*>::iterator it = R.begin(); it != R.end(); ++it)
                oneClique.push_back(*it);
            cliques.push_back(oneClique);
            LogConsole::writeConsole("Found one clique");
        }
    }

    while(P.size() >0){

        Entity *v = *P.begin();//P[index];

        //R union v
        set<Entity*> RUnionV = R;
        RUnionV.insert(v);

        //P intersect N(v)
        set<Entity*> PintersectV;
        for (int i = 0; i < v->getAdjacentEntities().size(); i++) {
            Entity *vi = v->getAdjacentEntity(i);
            set<Entity*>::iterator viInP;
            viInP = P.find(vi);
            if(viInP!= P.end())
                PintersectV.insert(vi);
        }

        set<Entity*> XintersectV;
        for (int i = 0; i < v->getAdjacentEntities().size(); i++) {
            Entity *vi = v->getAdjacentEntity(i);
            set<Entity*>::iterator viInX;
            viInX = X.find(vi);
            if (viInX != X.end())
                XintersectV.insert(vi);
        }

        BronKerbosh(RUnionV, PintersectV, XintersectV, cliques);

        //P = P exclude v
        P.erase(v);

        //X = X union v
        X.insert(v);

    }
}

/*convert the ERdata to dual ERdata*/
bool ERData::writeDualERData(vector<Entity*> enlist, vector<Relationship*> relist, const char * dst)
{
	FILE *this_file = fopen(dst, "w");

	if (this_file)
	{
		fprintf(this_file, "Nary Relationship Data\n");

		int dualEntityCount = relist.size();
		int dualRelationshipCount = enlist.size();

		fprintf(this_file, "%d %d\n", dualEntityCount, dualRelationshipCount);

		/*entities*/
		for (int i = 0; i < dualEntityCount; i++) {
			Relationship *r = relist[i];

			//id	label	x y z
			fprintf(this_file, "%d %s %f %f %f \n", i, r->getLabel().c_str(), 0., 0., 0.);
		}

		//relationships
		int i = 0;

		for (auto re : enlist) {
			//check if the all the entities of relationship is inisde the enlist
			bool valid = true;
			vector<int> vertexindexlist;
			for (auto incidentre : re->getIncidentRelationships())
			{
				int res = isInsideList<Relationship>(incidentre, relist);
				if (res == -1) {
					valid = false;
					break;
				}
				vertexindexlist.push_back(res);
			}

			if (!valid)
				continue;

			//id	label	caridnality		entitiesIds
			fprintf(this_file, "%d %s %d", i, re->getLabel().c_str(), vertexindexlist.size());

			for (auto n : vertexindexlist) {
				fprintf(this_file, " %d", n);
			}

			fprintf(this_file, "\n");
			i++;
		}

		fclose(this_file);

		return true;
	}
	else
		return false;
}

vector<Entity*> ERData::getEntitiesDegreeNotOne()
{
	vector<Entity*> res;

	for (auto en : m_entityList) {
		if (en->getDegree() != 1)
			res.push_back(en);
	}

	return res;
}

vector<Relationship*> ERData::getMonogons()
{
	vector<Relationship*> res;

	for (auto re : m_relationshipList) {
		if (re->getCardinality() == 1)
		{
			res.push_back(re);
		}
	}

	return res;
}

int ERData::getMonogonNum() const
{
	int count = 0;
	for (Relationship* re : m_relationshipList)
	{
		if (re->getCardinality() == 1)
			count++;
	}
	return count;
}

/*return re is the ith monogon among all monogons*/
int ERData::getMonogonIndex(Relationship * re) const
{
	int count = 0;
	for (int i = 0; i < m_relationshipList.size(); i++) {
		if (m_relationshipList[i]->getCardinality() != 1) {
			continue;
		}

		if (re == m_relationshipList[i])
			return count;

		count++;
	}

	return -1;
}

int ERData::getMonogonIndex(const Relationship* re) const
{
	int count = 0;
	for (int i = 0; i < m_relationshipList.size(); i++) {
		if (m_relationshipList[i]->getCardinality() != 1) 
		{
			continue;
		}

		if (re->getIndex() == i)
			return count;

		count++;
	}

	return -1;
}

ERData* ERData::convertToDualERData()
{
	
	ERData * dualERData = new ERData();

	int dualEntityCount = m_relationshipList.size();
	int dualRelationshipCount = m_entityList.size();

	srand(100000);
	double xRange = maxX - minX;

	/*entities*/
	for (int i = 0; i < dualEntityCount; i++) {
		Relationship *r = m_relationshipList[i];

		Entity *entity = new Entity();
		entity->setLabel(r->getLabel());
		entity->setOpidx(r->getOpidx());
		
		Vec4 recolor = r->getColor();
		recolor *= Vec4(0.8, 0.8, 0.8, 1.0);
		recolor.Alpha = 1.0;
		entity->setColor(recolor);
		dualERData->addEntity(entity);

		//Vec3 center = calculateCenter(r->getIncidentEntities());
		Vec3 center = r->getCenter(r->getMonogonLength());

		if(r->hasMultipleRelationship())
		{
			double xjitter = xRange*0.001*(double)rand() / RAND_MAX;
			center.x += xjitter;
		}

		entity->setLocation(center);

		r->setDualElement(entity);
		entity->setDualElement(r);
	}

	for (auto re : m_entityList) {

		vector<Entity*> entitylist;
		for (auto incidentre : re->getIncidentRelationships())
		{
			Entity *entemp = dualERData->getEntity(incidentre->getIndex());
			entitylist.push_back(entemp);
		}

		Relationship *renew = new Relationship(entitylist);
		renew->setLabel(re->getLabel());
		renew->setOpidx(re->getOpidx());

		Vec4 recolor = re->getColor();
		recolor.Alpha = 0.5;

		renew->setColor(recolor);
		dualERData->addRelationship(renew);

		re->setDualElement(renew);
		renew->setDualElement(re);
        
	}

	m_dualERData = dualERData;
	dualERData->m_dualERData = this;

	return dualERData;
}

ERData* ERData::getDualERData()
{
	return m_dualERData;
}

void ERData::syncDualLayout()
{
	if (!m_dualERData)
		return;

	for (Relationship* re : m_relationshipList)
	{
		Vec3 loc = re->getCenter(re->getMonogonLength());
		re->getDualEntity()->setLocation(loc);
	}

	m_dualERData->update2D();
}

ERData* ERData::getBipartiteGraph()
{
	int numens = m_entityList.size();
	int numres = m_relationshipList.size();

	ERData* kgraph = new ERData();
	for (int i = 0; i < numens; i++)
		kgraph->addEntity(new Entity());
	for (int i = 0; i < numres; i++)
		kgraph->addEntity(new Entity());

	for (Relationship* re : m_relationshipList)
	{
		int re_idx = re->getIndex() + numens;
		for (Entity* en : re->getIncidentEntities())
		{
			int en_idx = en->getIndex();
			kgraph->addRelationship(new Relationship(kgraph->getEntity(en_idx), kgraph->getEntity(re_idx)));
		}
	}
	return kgraph;
}

ERData* ERData::getSubHypergraph(vector<int>* enlist)
{
	// TODO
	return this;
}

ERData* ERData::getPartialHypergraph(vector<int>* relist)
{
	ERData* partial = new ERData();
	for (int idx : *relist)
	{
		Relationship* re = m_relationshipList[idx];
		vector<Entity*> new_entities;
		for (Entity* en : re->getIncidentEntities())
		{
			Entity* new_en = new Entity(en->getLocation());
			partial->addEntity(new_en);
			new_entities.push_back(new_en);
			new_en->setOpidx(en->getIndex());
		}
		partial->addRelationship(new_entities);
	}
	
	return partial;
}

void ERData::createEdgesScaffolds()
{
	//create scaffold which is straight lines
	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *r = m_relationshipList[i];
		if (r->getIncidentEntities().size() != 2)
		{
			LogConsole::showError("This is not a bipartite data");
			return;
		}

		vector<Scaffold> *scaffold = r->getScaffoldList();
		scaffold->clear();

		Scaffold bundleEdge;
		bundleEdge.m_Curved = true;
		bundleEdge.m_CurvedLocation.push_back(r->getIncidentEntity(0)->getLocation());
		bundleEdge.m_CurvedLocation.push_back(r->getIncidentEntity(1)->getLocation());

		scaffold->push_back(bundleEdge);
	}
}

bool ERData::writeERSubsets(const char * dst)
{
	vector<vector<Entity*>> entitylistOfGraphs = FindGraphs(m_entityList);
	vector<vector<Relationship*>> relationshiplistOfGraphs;


	for (int i = 0; i < entitylistOfGraphs.size(); i++)
	{
		vector<Entity*> &CurrentGraph = entitylistOfGraphs[i];
		vector<Relationship*> currentRelationshiplistOfGraph = FindAllIncidentRelationships(CurrentGraph);
		relationshiplistOfGraphs.push_back(currentRelationshiplistOfGraph);
	}

	if (entitylistOfGraphs.size() != relationshiplistOfGraphs.size()) {
		LogConsole::showError("finding separated subdataset fails");
		return false;
	}

	for (int i = 0; i < entitylistOfGraphs.size(); i++) {
		char str[250];
		sprintf(str, "/sub_%d.er", i);
		char filename[250];
		strcpy(filename, dst);
		strcat(filename, str);
		string comment = "subset " + to_string(i);
		FileIO::writeERFile(filename, entitylistOfGraphs[i], relationshiplistOfGraphs[i], comment);
	}

	return true;
}

bool SearchNotComplete(vector<Entity*> entityList)
{
	bool toReturn = false;

	for (int i = 0; i < entityList.size(); i++)
	{
		Entity* CurrentVertex = entityList[i];
		for (int j = 0; j < CurrentVertex->getAdjacentEntities().size(); j++)
		{
			if (!CurrentVertex->getAdjacentEntity(j)->m_IsSearched)
			{
				toReturn = true;
				break;
			}
		}
		if (toReturn)
		{
			break;
		}
	}
	return toReturn;
}

void ERData::scaleEntities(Vec3 scale, Vec3 m_center, ENList* enlist)
{
    if (scale == 1.0)
        return;

	if (!enlist)
		enlist = &m_entityList;

    for (Entity* en : *enlist) 
	{
        Vec3 loc = en->getLocation();
		Vec3 dir = loc - m_center;
		dir *= scale;

        Vec3 newloc = m_center + dir;
        en->setLocation(newloc);
    }

    update2D();
}

void ERData::rotateEntities(double angle, Vec3 m_center, ENList* enlist)
{
	if (angle == 0.0)
		return;

	if (!enlist)
		enlist = &m_entityList;

    for (Entity* en : *enlist) 
	{
        Vec3 entity1 = en->getLocation();
        Vec3 dir1 = entity1 - m_center;
        dir1 = rotate(dir1, angle);
        entity1 = m_center + dir1;
        en->setLocation(Vec3(entity1.a, entity1.b, 0));
    }

    update2D();
}

void ERData::rotateMonogons(double angle, REList* relist)
{
	if (!relist)
		relist = &m_relationshipList;

	for (Relationship* re : *relist) 
	{
		if (!re)
			continue;
		if (re->getCardinality() == 1)
			re->rotatePolygon(angle, re->getCenter(0.0));
	}

	update2D();
}

void ERData::moveEntities(double dx, double dy, ENList* enlist)
{
	if (!enlist)
		enlist = &m_entityList;

    for (Entity* en : *enlist) 
	{
        Vec3 loc = en->getLocation();
        Vec3 newloc = Vec3(loc.x+dx, loc.y + dy, 0);
        en->setLocation(newloc);
    }

	update2D();
}

void ERData::reflectEntities(double angle, Vec3 center, ENList* enlist)
{
	Vec3 axis(cos(angle), sin(angle), 0.0);

	for (Entity* en : *enlist)
	{
		Vec3 loc = en->getLocation();
		Vec3 dir = center - loc;
		Vec3 newloc = center + dir - (2.0 * dot(dir, axis) * axis);
		en->setLocation(newloc);
	}

	update2D();
}

void ERData::reflectLayout(Vec3 v1, Vec3 v2, ENList* enlist)
{
	Vec3 n = v1 - v2;

	for (Entity* en : *enlist) 
	{
		Vec3 v = en->getLocation();

		Vec3 d = v2 - v;
		normalize(n);

		Vec3 newloc = v2 + d - 2.0*dot(d, n)*n;

		en->setLocation(newloc);
	}

	update2D();
}

vector<vector<Entity*>> ERData::FindGraphs(vector<Entity*>& entityList)
{
	vector<vector<Entity*>> toReturnList;
	// Reset Search Flag first
	for (int i = 0; i < entityList.size(); i++)
	{
		Entity* CurrentVertex = entityList[i];
		CurrentVertex->m_IsSearched = false;
	}

	for (int i = 0; i < entityList.size(); i++)
	{

		vector<Entity*> ToAddInGraphList;
		Entity* CurrentVertex = entityList[i];

		if (!CurrentVertex->m_IsSearched)
		{
			CurrentVertex->m_IsSearched = true;
			ToAddInGraphList.push_back(CurrentVertex);
		}
		else
		{
			continue;
		}

		while (SearchNotComplete(ToAddInGraphList))
		{
			for (int j = 0; j < ToAddInGraphList.size(); j++)
			{
				Entity* CurrentSearchVertex = ToAddInGraphList[j];
				for (int k = 0; k < CurrentSearchVertex->getAdjacentEntities().size(); k++)
				{
					Entity* CurrentAdjacentSearchVertex = CurrentSearchVertex->getAdjacentEntity(k);
					if (!CurrentAdjacentSearchVertex->m_IsSearched)
					{
						CurrentAdjacentSearchVertex->m_IsSearched = true;
						ToAddInGraphList.push_back(CurrentAdjacentSearchVertex);
					}
				}
			}
		}

		toReturnList.push_back(ToAddInGraphList);
	}

	return toReturnList;
}

vector<Entity*> ERData::FindAllLeaves(vector<Entity*>& entityList)
{
	vector<Entity*> toReturnList;
	for (int i = 0; i < entityList.size(); i++)
	{
		Entity* CurrentVertex = entityList[i];
		if (CurrentVertex->getAdjacentEntities().size() <= 1)
		{
			toReturnList.push_back(CurrentVertex);
		}

	}
	return toReturnList;
}

vector<Entity*> ERData::FindAllNonLeaves(vector<Entity*>& entityList)
{
	vector<Entity*> toReturnList;

	for (int i = 0; i < entityList.size(); i++)
	{
		Entity* CurrentVertex = entityList[i];
		if (CurrentVertex->getAdjacentEntities().size() > 1)
		{
			toReturnList.push_back(CurrentVertex);
		}

	}
	return toReturnList;
}

vector<Relationship*> ERData::FindAllIncidentRelationships(vector<Entity*> enlist)
{
	vector<Relationship*> relist;

	for (int i = 0; i < enlist.size(); i++) {
		Entity *en = enlist[i];

		for (int j = 0; j< en->getIncidentRelationships().size(); j++) {
			Relationship * re = en->getIncidentRelationship(j);

			int inside = isInsideList<Relationship>(re, relist);
			if (inside == -1) {
				relist.push_back(re);
			}
		}
	}

	return relist;
}

void ERData::designLayout_GridSeparatedTree(int col, bool sort)
{	
    // 3 Columns
	vector<vector<Entity*>> listOfGraphs = FindGraphs(m_entityList);
	int numberOfGraph = listOfGraphs.size();
	int ColumnCount = int(sqrt(listOfGraphs.size()));

	if(col != -1)
		ColumnCount = col;


	int RowCount = numberOfGraph / ColumnCount;

	double width = 20;

	double stepw = width / ColumnCount;
	double steph = stepw;

	double radius = 0.45*width / ColumnCount;

	if (sort) {
		std::sort(listOfGraphs.begin(), listOfGraphs.end(), [](vector<Entity*> a, vector<Entity*> b) {
			return a.size() < b.size();
		});
	}

	for (int i = 0; i < listOfGraphs.size(); i++)
	{

		vector<Entity*> &CurrentGraph = listOfGraphs[i];

		vector<Entity*> leafVertices = FindAllLeaves(CurrentGraph);

		vector<Entity*> internalVertices = FindAllNonLeaves(CurrentGraph);

		leafVertices = CurrentGraph;

		double angle = 2 * M_PI / (leafVertices.size());

		int xGraph = i % ColumnCount;
		int yGraph = i / ColumnCount;

		double centerX = -0.9 + stepw*xGraph;
		double centerY = -0.9 - steph*yGraph;

		for (int j = 0; j < leafVertices.size(); j++)
		{
			Entity* CurrentLeafVertex = leafVertices[j];
			double xPos = centerX + radius*cos(angle*j);
			double yPos = centerY + radius*sin(angle*j);
			CurrentLeafVertex->setLocation(Vec3(xPos, yPos, 0));
		}
	}

	update2D();
}

void ERData::setEntitiesSize(double size)
{
	for (int i = 0; i < getEntityNum(); i++) {
		getEntity(i)->m_Size = size;
	}
}

void ERData::setMonogonsSize(double size)
{
	for (auto re : m_relationshipList) {
		if (re->getCardinality() == 1)
		{
			re->setMonogonSize(size);
		}
	}
}

void ERData::setMonogonLength(double size)
{
	for (auto re : m_relationshipList) {
		if (re->getCardinality() == 1)
		{
			re->setMonogonLength(size);
		}
	}
}

void ERData::updateMonogons2D()
{
	for (auto &re : m_relationshipList) {
		if (re->getCardinality() == 1) {
			re->update2D(STARRIZATIONR_FLAG::NOTSTARRIZE, UPDATESCAFFOLD_FLAG::UPDATESCAFFOLD);
		}
	}
}

void ERData::resetMonogonAngles(vector<Entity*> enlist)
{
	if (enlist.size() == 0)
		enlist = m_entityList;

	for (auto en : enlist) {

		for (auto adjRe : en->getIncidentRelationships()) {

			if (adjRe->getCardinality() == 1)
			{
				adjRe->initializeMonogonOrientation();
			}
		}
	}
}

void ERData::distributeMonogonAngles(vector<Entity*> enlist)
{
	if (enlist.size() == 0)
		enlist = m_entityList;

	for (auto en : enlist)
	{
		bool can_distribute = en->distributeMonogons();
		if (!can_distribute)
		{
			for (auto adjRe : en->getIncidentRelationships()) {

				if (adjRe->getCardinality() == 1)
				{
					adjRe->initializeMonogonOrientation();
				}
			}
		}
	}
}

void ERData::calculateRelationshipsConvexHull()
{
	for (auto &re : m_relationshipList) {
		re->calcConvexHull();
	}
}

void ERData::update2D()
{
    starrizeRelationships();
    create2DScaffolds();
}

void ERData::initializeMonogonOrientations(vector<RE*> mlist)
{
	if (mlist.size() == 0)
		mlist = m_relationshipList;

	for (int i = 0; i < mlist.size(); i++) {

		if (mlist[i]->getCardinality() == 1)
			mlist[i]->initializeMonogonOrientation();
	}
}

void ERData::updateRelatedMonogons(Entity * entarget)
{
	entarget->updateRelatedMonogons();
}

void ERData::updateRelatedMonogons(Relationship * rtarget)
{
	rtarget->updateRelatedMonogons();
}

void ERData::create2DScaffolds(int ReCard)
{
    for (int i = 0; i < m_relationshipList.size(); i++) {

		if (ReCard != -1 && m_relationshipList[i]->getCardinality() != ReCard)
			continue;

        m_relationshipList[i]->create2DScaffolds();
    }
}

void ERData::calculateEntityCard() {

     maxEntityCardinality = INT_MIN;
     minEntityCardinality = INT_MAX;

	 if (m_entityList.empty()) {
		 maxEntityCardinality = 0;
		 minEntityCardinality = 0;
	 }

	for (int i = 0; i < m_entityList.size(); i++) {
		Entity *entity = m_entityList[i];

		int card = entity->getCardinality();

		if (card < minEntityCardinality)
			minEntityCardinality = card;
		if (card >maxEntityCardinality)
			maxEntityCardinality = card;
	}
}

void ERData::calculateRelationshipCard() {

     maxRelationshipCardinality = INT_MIN;
     minRelationshipCardinality = INT_MAX;

	 if (m_relationshipList.empty()) {
		 maxRelationshipCardinality = 0;
		 minRelationshipCardinality = 0;
	 }

	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *re = m_relationshipList[i];

		int card = re->getCardinality();

		if (card < minRelationshipCardinality)
			minRelationshipCardinality = card;
		if (card >maxRelationshipCardinality)
			maxRelationshipCardinality = card;
	}
	//LogConsole::writeConsole("Relationships cardinalities are calculated");
}

int ERData::getEntityNum() const
{
	return m_entityList.size();
}

int ERData::getRelationshipNum() const
{
	return m_relationshipList.size();
}

double ERData::getEntityX(int vi) const
{
	return m_entityList[vi]->getLocation().x;
}

double ERData::getEntityY(int vi) const
{
	return m_entityList[vi]->getLocation().y;
}

double ERData::getMonogonRotation(int mi) const
{
	int monogon_count = 0;
	for (Relationship* re : m_relationshipList)
	{
		if (re->getCardinality() == 1)
		{
			if (mi == monogon_count)
				return re->getMonogonRotation();
			else
				monogon_count++;
		}
	}
	return 0.0;
}

double ERData::getEntityRadius(int index) const
{
	return m_entityList[index]->m_Size;
}

int ERData::getRelationshipCard(int index) const
{
	return m_relationshipList[index]->getCardinality();
}

int ERData::getRelationshipDegree(int index) const
{
	return m_relationshipList[index]->getDegree();
}

bool ERData::isRelationshipOptimizable(int index) const
{
	return m_relationshipList[index]->flag_Optimizable;
}

int ERData::getEntityDegree(int index) const
{
	return m_entityList[index]->getDegree();
}

bool ERData::isTwoResSame(int indexi, int indexj) const
{
	return isTwoRelationshipSame(m_relationshipList[indexi], m_relationshipList[indexj]);
}

bool ERData::isTwoResBBIntersect(int indexi, int indexj) const
{
	return isBoundingBoxIntersect(m_relationshipList[indexi], m_relationshipList[indexj]);
}

double ERData::getAverageEdgeLenth() const
{
	double sumEdgeLen = 0;
	int sumEdgeCount = 0;
	for (int i = 0; i < getRelationshipNum(); i++) {
		int card = m_relationshipList[i]->getCardinality();

		if (card == 1)
			continue;

		double p = m_relationshipList[i]->perimeter();

		sumEdgeLen += p;
		sumEdgeCount += card;
	}

	double averageL = sumEdgeLen / sumEdgeCount;
	return averageL;
}

vector<int> ERData::getRelationshipVertIndices(int ri) const
{
	return m_relationshipList[ri]->getEntityIndices();
}

int ERData::getSharedEntityNum(int re_i, int re_j) const
{
	int count = 0;

	RE* a = m_relationshipList[re_i];
	RE* b = m_relationshipList[re_j];

	for (int i = 0; i < a->getCardinality(); i++) {
		Entity *e = a->getIncidentEntity(i);
		int inside = isInsideList<Entity>(e, b->getIncidentEntities());
		if (inside != -1)
			count++;
	}

	return count;
}

vector<int> ERData::getSharedEntityIndices(int re_i, int re_j) const
{
	vector<int> indices;

	RE* a = m_relationshipList[re_i];
	RE* b = m_relationshipList[re_j];

	for (int i = 0; i < a->getCardinality(); i++) {
		Entity *e = a->getIncidentEntity(i);
		int inside = isInsideList<Entity>(e, b->getIncidentEntities());
		if (inside != -1)
			indices.push_back(e->getIndex());
	}

	return indices;

}

bool ERData::entityHasEnoughAngularSpace(Entity * e, double buffer) const
{
	double angleSum = 0;
	for (auto inci_re : e->getIncidentRelationships()) {
		int card = inci_re->getCardinality();
		double anglei = double(card - 2)*M_PI / card;
		angleSum += anglei + buffer;
	}

	if (angleSum > 2 * M_PI)
		return false;

	return true;
}

bool ERData::entityHasEnoughAngularSpace(int enId, double buffer) const
{
	Entity *en = m_entityList[enId];
	return entityHasEnoughAngularSpace(en, buffer);
}

double ERData::entityMaxInternalAngle(int enId) const
{
	Entity *en = m_entityList[enId];

	double angle = 0;
	for (auto inci_re : en->getIncidentRelationships()) {
		int card = inci_re->getCardinality();
		double anglei = double(card - 2)*M_PI / card;
		if (anglei > angle)
			angle = anglei;
	}

	return angle;
}

bool ERData::hasEnoughAngularSpaceAtCommonVert(int re_i, int re_j, double bufferangle) const
{
	RE* a = m_relationshipList[re_i];
	RE* b = m_relationshipList[re_j];

	vector<Entity*> commonNodes;
	
	for (int i = 0; i < a->getCardinality(); i++) {
		Entity *e = a->getIncidentEntity(i);
		int inside = isInsideList<Entity>(e, b->getIncidentEntities());
		if (inside != -1)
			commonNodes.push_back(e);
	}

	for (auto e : commonNodes) {
		bool res = entityHasEnoughAngularSpace(e, bufferangle);
		if (!res)
			return false;
	}

	return true;
}

int ERData::getSharedEntityNum(RE * a, RE * b) const
{
	int count = 0;

	for (int i = 0; i < a->getCardinality(); i++) {
		Entity *e = a->getIncidentEntity(i);
		int inside = isInsideList<Entity>(e, b->getIncidentEntities());
		if (inside != -1)
			count++;
	}

	return count;
}

int ERData::getSharedRelationshipNum(int en_i, int en_j) const
{
	return getNumberOfAjacentRelationships(m_entityList[en_i], m_entityList[en_j]);
}

int ERData::getSharedRelationshipNum(EN* a, EN* b)
{
	int sum = 0;
	for (int i = 0; i < a->getIncidentRelationships().size(); i++)
	{
		Relationship* p1 = a->getIncidentRelationship(i);
		int inside = isInsideList<Relationship>(p1, b->getIncidentRelationships());
		if (inside != -1)
			sum++;
	}
	return sum;
}

int ERData::getEnShareNum(Entity* en)
{
	int count = 0;
	for (Entity* adj : en->getAdjacentEntities())
		count += getSharedRelationshipNum(en, adj);
	return count;
}

int ERData::getReShareNum(Relationship* re)
{
	int count = 0;
	for (Relationship* adj : re->getAdjacentRelationships())
		count += getSharedEntityNum(re, adj);
	return count;
}

bool ERData::isTwoEntityAdjacent(int en_i, int en_j) const
{
	return checkIfTwoEntityAdjacent(m_entityList[en_i], m_entityList[en_j]);
}

vector<int> ERData::getEntityIndicesFromIncidentLocs(int ri, const vector<Vec3>& locList) const
{
	return m_relationshipList[ri]->getEntityIndicesFromIncidentLocs(locList);
}

int ERData::getEdgeNum()
{
	/*check the correctness of the indices of entities*/
	updateEntityIndices();

	/*sum up edge count with duplicating*/
	int EdgeCount = 0;
	for (int i = 0; i < getRelationshipNum(); i++) {

		RE *re = m_relationshipList[i];
		int incidentNum = re->getIncidentEntities().size();

		EdgeCount += incidentNum;

	}

	/*remove duplications*/
	struct edge
		// increasing order.
		: public std::array<int, 2>
	{
		// puts v0 and v1 into correct order.
		inline edge(int v0, int v1) {
			if (v0 > v1)
				std::swap(v0, v1);
			(*this)[0] = v0;
			(*this)[1] = v1;
		}
	};
	set<edge>alledges;

	for (int i = 0; i < getRelationshipNum(); i++) {
		RE *re = m_relationshipList[i];
		int incidentNum = re->getIncidentEntities().size();

		if (incidentNum == 1) {
			continue;
		}

		for (int j = 0; j < incidentNum; j++) {
			int index1 = re->getIncidentEntity(j)->getIndex();
			int index2 = re->getIncidentEntity((j + 1) % incidentNum)->getIndex();

			edge e(index1, index2);
			auto res = alledges.insert(e);
			if (res.second == true) {

				ENList enlist;
				enlist.push_back(re->getIncidentEntity(j));
				enlist.push_back(re->getIncidentEntity((j + 1) % incidentNum));

				auto edgeAdj = re->getRelationshipsSharingEntities(enlist);
				int dup = edgeAdj.size();
				EdgeCount -= dup;
			}
		}
	}

	return EdgeCount;
}

Entity* ERData::getEntity(int index)
{
	if (index >= m_entityList.size()) {
		LogConsole::showError("out of range entity index");
		return NULL;
	}
	return m_entityList[index];
}

Relationship* ERData::getRelationship(int index)
{
	if (index >= m_relationshipList.size()) {
		LogConsole::showError("out of range relationship index");
		return NULL;
	}
	return m_relationshipList[index];
}

const Entity* ERData::getConstEntity(int index) const
{
	if (index >= m_entityList.size())
	{
		LogConsole::showError("out of range entity index");
		return NULL;
	}
	return m_entityList[index];
}

const Relationship* ERData::getConstRelationship(int index) const
{
	if (index >= m_relationshipList.size()) {
		LogConsole::showError("out of range relationship index");
		return NULL;
	}
	return m_relationshipList[index];
}

vector<Entity*> ERData::getEntities()
{
	return m_entityList;
}

vector<Entity*> ERData::getFreeEntities()
{
	vector<Entity*> free_list;
	for (Entity* en : m_entityList)
	{
		if (en->flag_Optimizable)
			free_list.push_back(en);
	}
	return free_list;
}

vector<Relationship*> ERData::getRelationships()
{
	return m_relationshipList;
}

vector<Relationship*> ERData::getFreeRelationships()
{
	vector<Relationship*> free_list;
	for (Relationship* re : m_relationshipList)
	{
		if (re->flag_Optimizable)
			free_list.push_back(re);
	}
	return free_list;
}

Entity * ERData::getEntity(string label)
{
	for (auto r : m_entityList) {
		int k = label.compare(r->getLabel());
		if (k == 0)
			return r;
	}

	return NULL;
}

Relationship * ERData::getRelationship(string label)
{
	for (auto r : m_relationshipList) {
		int k = label.compare(r->getLabel());
		if (k == 0)
			return r;
	}

	return NULL;
}

vector<Relationship*> ERData::getRelationships(vector<string> labels)
{
	vector<Relationship*> rlist;

	for (auto label : labels) {
		Relationship *r = getRelationship(label);

		if (r == NULL)
			continue;
		else
		{
			rlist.push_back(r);
		}
	}

	return rlist;
}

Relationship* ERData::getRelationshipByOpidx(int opidx)
{
	for (auto r : m_relationshipList) 
	{
		if (r->getOpidx() == opidx)
			return r;
	}

	return NULL;
}

Entity* ERData::getEntityByOpidx(int opidx)
{
	for (auto e : m_entityList)
	{
		if (e->getOpidx() == opidx)
			return e;
	}

	return NULL;
}

void ERData::calculateEntityDegree(){

     maxEntityDegree = INT_MIN;
     minEntityDegree = INT_MAX;

	 if (m_entityList.empty()) {
		 maxEntityDegree = 0;
		 minEntityDegree = 0;
	 }

	for (int i = 0; i < m_entityList.size(); i++) {
		Entity *entity = m_entityList[i];

		int degree = entity->getDegree();

		if (degree < minEntityDegree)
			minEntityDegree = degree;
		if (degree >maxEntityDegree)
			maxEntityDegree = degree;
	}

}

void ERData::calculateRelationshipDegree() {

     maxRelationshipDegree = INT_MIN;
     minRelationshipDegree = INT_MAX;

	 if (m_relationshipList.empty()) {
		 maxRelationshipDegree = 0;
		 maxRelationshipDegree = 0;
	 }

	for (int i = 0; i < m_relationshipList.size(); i++) {
		Relationship *re = m_relationshipList[i];

		int degree = re->getDegree();

		if (degree < minRelationshipDegree)
			minRelationshipDegree = degree;
		if (degree >maxRelationshipDegree)
			maxRelationshipDegree = degree;
	}
}

void ERData::forceMove(Entity * dst, Entity * source, Vec3 force, double stepT)
{
    forcePair.push_back(pair<int, int>(dst->getIndex(), source->getIndex()));
    LogConsole::writeConsole("src:%d, dst:%d", source->getIndex(), dst->getIndex());

    Vec3 v1 = dst->getLocation();
    Vec3 v2 = source->getLocation();
    Vec3 dir = v2 - v1;

    
    double fv = length(force);  

    double len = length(dir);
    Vec3 f_c = force;
    normalize(f_c);
    normalize(dir);

    double theta2 = acos(dot(f_c, dir));

    double a1 = cos(theta2)*fv + fv*fv*sin(theta2)*sin(theta2)*stepT*stepT / len;
    double a2 = fv;

    Vec3 d1 = 0.5*a1*stepT*stepT*dir;
    Vec3 d2 = 0.5*a2*stepT*stepT*f_c;

    Vec3 newv1 = v1 + d1;
    Vec3 newv2 = v2 + d2;

    dst->setLocation(newv1);

    if(!source->flag_forcedMoved){
        source->setLocation(newv2);
        source->flag_forcedMoved = true;
    }
}

void ERData::forceMove(Entity *e_source, Vec3 force, double stepT)
{
    for (int i = 0; i < m_entityList.size(); i++) {
        m_entityList[i]->flag_forcedMoved = false;
    }

    forcePair.clear();

    for (int i = 0; i < e_source->getAdjacentEntities().size(); i++) {
        Entity * newDst = e_source->getAdjacentEntity(i);
        forceMove(newDst, e_source, force, stepT);
    }

    update2D();
}

int ERData::removeBinaryEnclosedByRelationships() {

	int removeCount = 0;

	for (int i = 0; i < m_relationshipList.size(); i++) {

		if (m_relationshipList[i]->getCardinality() != 2)
			continue;

		Relationship *b = m_relationshipList[i];
		bool duplicated = false;
		//check if the binary relationship is inside one of its incident nary relationship

		vector<Relationship*> adj_re = b->getAdjacentRelationships();
		for (int j = 0; j < adj_re.size(); j++) {

			Relationship *target = adj_re[j];

			if (target->getCardinality() == 2)
				continue;

			if (target == b)
				continue;

			duplicated = isRelationshipInsideRelationship(b, target);
			if (duplicated)
				break;
		}

		if (duplicated) {
			//remove binary relationship
			removeCount++;

			deleteRelationship(b);
			i--;
		}
	}
	return removeCount;
}

int ERData::removeMultipleRelationships()
{
	int removeCount = 0;

	for (int i = 0; i < m_relationshipList.size(); i++) {

		for (int j = i + 1; j < m_relationshipList.size(); j++) {

			if (isTwoRelationshipSame(m_relationshipList[i], m_relationshipList[j])) {
				deleteRelationship(m_relationshipList[j]);
				j--;
				removeCount++;
			}
		}
	}

	return removeCount;
}

int ERData::removeNonMultipleRelationships()
{
	int removeCount = 0;

	for (int i = 0; i < m_relationshipList.size(); i++) {

		auto re = m_relationshipList[i];

		/*check whether i has repeated adjacent polygon*/
		bool hasrepeated = false;
		for (int j = 0; j < re->getAdjacentRelationships().size(); j++) {

			auto readj = re->getAdjacentRelationship(j);
			if (isTwoRelationshipSame(re, readj))
				hasrepeated = true;
		}

		if (!hasrepeated)
		{
			deleteRelationship(m_relationshipList[i]);
			i--;
			removeCount++;
		}
	}

	return removeCount;
}

void ERData::removeUnvisibleElements(int &removedEntities, int &removedRelationships) {

	removedEntities = 0;
	removedRelationships = 0;

	for (int i = 0; i < m_relationshipList.size(); i++) {

		Relationship *b = m_relationshipList[i];
		bool visible = b->getVisibility();

		if (!visible) {
			removedRelationships++;
			deleteRelationship(b);
			i--;
		}
	}

	for (int i = 0; i < m_entityList.size(); i++) {

		Entity *b = m_entityList[i];
		bool visible = b->getVisibility();

		if (!visible) {
			removedEntities++;
			deleteEntity(b);
			i--;
		}
	}
}

void ERData::removePolygonsByCard(int card)
{
	int i = 0;
	int count = 0;
	while (i < m_relationshipList.size()) {

		Relationship *poly = m_relationshipList[i];

		if (poly->getCardinality() == card) {
			deleteRelationship(poly);
			i--;
			count++;
		}
		i++;
	}
	LogConsole::writeConsole("removed relationships:%d", count);
}

void ERData::addEntity(Entity *en, bool set_opid) 
{
	en->setIndex(m_entityList.size());
	if (set_opid)
	{
		en->setOpidx(m_entityLifetime);
		m_entityLifetime++;
	}
	m_entityList.push_back(en);
}

Entity* ERData::addEntity(Vec3 loc) {
	
	Entity *en = new Entity(loc);
	addEntity(en);

	return en;
}

Entity* ERData::addEntity(string label) {
	Entity *en = new Entity();
	en->setLabel(label);
	addEntity(en);

	return en;
}

Entity* ERData::addEntityUnique(string label)
{
	for (Entity* en : m_entityList)
	{
		if (en->getLabel() == label)
			return en;
	}
	return addEntity(label);
}

void ERData::addEntities(int Num, string lable) {

	//srand(1000000);

	for (int i = 0; i < Num; i++) {

		Entity *en = new Entity();
		addEntity(en);

		if (!lable.empty())
		{
			en->setLabel(lable);
		}
		else {
			en->setLabelWithIndex();
		}
	}
}

void ERData::insertEntity(Entity* en)
{
	int idx = en->getIndex();
	if (idx == m_entityList.size())
		m_entityList.push_back(en);
	else
	{
		Entity* temp = m_entityList.at(idx);
		m_entityList.at(idx) = en;
		temp->setIndex(m_entityList.size());
		m_entityList.push_back(temp);
	}
}

void  ERData::addRelationship(Relationship * r, bool set_opid)
{
	r->setIndex(m_relationshipList.size());
	if (set_opid)
	{
		r->setOpidx(m_relationshipLifetime);
		m_relationshipLifetime++;
	}
	m_relationshipList.push_back(r);
}

bool ERData::addRelationship(vector<Entity*> entitylist) {

	if (entitylist.size() <= 1) {
		LogConsole::showError("Please select more than one enities.");
		return false;
	}

	string name = "NULL";
#ifndef LINUXVER
	bool ok;
	QString text = QInputDialog::getText(NULL, "Input",
		"Relationship Label:", QLineEdit::Normal,
		"NULL", &ok);
	if (ok && !text.isEmpty())
		name = text.toStdString();
	else
		return false;

#endif // !LINUXVER

	Relationship *newR = new Relationship(entitylist);
	newR->setLabel(name);
	addRelationship(newR);
	newR->update2D();

	return true;
}

Relationship* ERData::addRelationship(int Cardinality, string label, double radius) {

	vector<Entity*> tempEList;

	double cx = 0.0;
	double cy = 0.0;

	for (int i = 0; i < Cardinality; i++) {

		double x = cx + radius*cos(i * 2.0*M_PI / Cardinality);
		double y = cy + radius*sin(i * 2.0*M_PI / Cardinality);

		if (Cardinality == 1) {
			x = cx;
			y = cy;
		}

		Entity *en = new Entity(Vec3(x, y, 0));
		addEntity(en);
		en->setLabelWithIndex();
		tempEList.push_back(en);
	}

	Relationship *newR = new Relationship(tempEList);
	if (!label.empty())
		newR->setLabel(label);
	else
		newR->setLabelWithIndex();
	addRelationship(newR);

	newR->update2D();

	return newR;
}

void ERData::insertRelationship(Relationship* re)
{
	int idx = re->getIndex();
	if (idx == m_relationshipList.size())
		m_relationshipList.push_back(re);
	else
	{
		Relationship* temp = m_relationshipList.at(idx);
		m_relationshipList.at(idx) = re;
		temp->setIndex(m_relationshipList.size());
		m_relationshipList.push_back(temp);
	}
}


bool ERData::deleteEntity(Entity * en, bool delete_relationships)
{
	int k = isInsideList<Entity>(en, m_entityList);
	if (k == -1)
		return false;

	en->finalizeAdjacency();

	while (en->getIncidentRelationships().size() > 0)
	{
		Relationship* re = en->getIncidentRelationship(0);
		en->removeIncidentRelationship(re);

		int res = isInsideList<Relationship>(re, m_relationshipList);
		if (res < 0)
			continue;

		if (delete_relationships /* || re->getCardinality() == 0*/)
			deleteRelationship(re);
		else
		{
			re->initializeScaffold();
			re->update2D();
			//re->setColor(re->getCardColor(re->getCardinality()));
		}
			
	}

	m_entityList[k] = m_entityList[m_entityList.size() - 1];
	m_entityList[k]->setIndex(k);
	m_entityList.pop_back();

	en->setRemovalOp(NULL);
	en->setMergerOps(std::vector<OperationRecord*>{});
	delete(en);
	return true;

}

bool ERData::deleteEntities(vector<Entity*> entitylist, bool delete_relationships) {

	if (entitylist.size() < 1) {
		LogConsole::showError("Please select at least one enity to delete.");
		return false;
	}

	for (int i = 0; i < entitylist.size(); i++)
	{
		deleteEntity(entitylist[i], delete_relationships);
	}

	return true;
}

/*	Note, the dual entity's dual element will be set to NULL,
	so always first delete, then update dual element*/
bool ERData::deleteRelationship(Relationship* re, bool delete_entities)
{
	int res = isInsideList<Relationship>(re, m_relationshipList);
	if (res == -1)
		return false;

	re->finalizeAdjacency();

	while (re->getIncidentEntities().size() > 0)
	{
		Entity* en = re->getIncidentEntity(0);
		re->removeIncidentEntity(en);

		if (isInsideList<Entity>(en, m_entityList) < 0)
			continue;

		if (delete_entities /* || en->getDegree() == 0 */)
			deleteEntity(en);
		else if (en->getDualRelationship())
		{
			Color4_F color = en->getDiscreteColor(en->getDegree());
			color.w = 1.0;
			en->setColor(color);
		}
	}

	m_relationshipList[res] = m_relationshipList[m_relationshipList.size() - 1];
	m_relationshipList[res]->setIndex(res);
	m_relationshipList.pop_back();

	re->setRemovalOp(NULL);
	re->setMergerOps(std::vector<OperationRecord*>{});
	delete(re);
	return true;
}

bool ERData::deleteRelationships(vector<Relationship*> relationshiplist, bool delete_entities) {

	if (relationshiplist.size() < 1)
		return false;

	for (int i = 0; i < relationshiplist.size(); i++)
	{
		deleteRelationship(relationshiplist[i], delete_entities);
	}

	return true;
}

Relationship* ERData::mergeToRelationship(vector<Entity*> entityList, vector<Relationship*> relationshipList, string name, Entity* dual)
{
	if (name == "") {
		for (auto r : relationshipList) {
			name += r->getLabel();
			if (r != relationshipList.back())
				name += "-";
		}

		if (name.size() > 100) {
			name = name.substr(0, 10);
			name += "_merged...";
		}
	}
	LogConsole::writeConsole("To merged to:%s", name.c_str());

	vector<Entity*> totalEntityList = entityList;

	//get all the entities
	for (int i = 0; i < relationshipList.size(); i++) {
		Relationship *r = relationshipList[i];
		for (int j = 0; j < r->getIncidentEntities().size(); j++) {
			int inside = isInsideList<Entity>(r->getIncidentEntity(j), totalEntityList);
			if (inside >= 0)
				continue;
			totalEntityList.push_back(r->getIncidentEntity(j));
		}
	}

	if (totalEntityList.size() < 1)
		return NULL;

	Relationship *newR = new Relationship(totalEntityList);
	newR->setLabel(name);
	addRelationship(newR);
	newR->update2D();

	/*update the dual elements collapse the vertices*/
	if(dual == NULL && m_dualERData)
	{
		vector<Entity*> dualEns;
		for (int i = 0; i < relationshipList.size(); i++) {
			Entity *e = relationshipList[i]->getDualEntity();
			if(e)
				dualEns.push_back(e);
		}

		if (dualEns.size() == relationshipList.size()) {
			Entity *newdual = m_dualERData->collapseToEntity(dualEns, "", newR);
			newR->setDualElement(newdual);
			if (newdual->getDualRelationship() == NULL) {
				LogConsole::showError("bad!");
			}
		}
		else {
			LogConsole::showError("bad dual data, cannot update dual elements!");
		}
	}
	else if(dual){
		newR->setDualElement(dual);
	}


	for (int i = 0; i < relationshipList.size(); i++) {
		Relationship *r = relationshipList[i];
		vector<RE*>mlist = r->getRelatedMonogons();
		deleteRelationship(r);
		initializeMonogonOrientations(mlist);
	}

	return newR;
}

Entity* ERData::collapseToEntity(vector<Entity*> entityList, string name, Relationship* dual)
{
	if (name == "") {
		for (auto r : entityList) {
			name += r->getLabel();
			if (r != entityList.back())
				name += "-";
		}
	}
	LogConsole::writeConsole("To collapse to:%s", name.c_str());

	/*create a new vertex and update all incident polygons 
	by replacing all vertices inside the entityList with the new vertex*/

	vector<Relationship*> totalReList;
	
    //get all the relationship
	for (int i = 0; i < entityList.size(); i++) {
		Entity *en = entityList[i];
		for (int j = 0; j < en->getIncidentRelationships().size(); j++) {
			int inside = isInsideList<Relationship>(en->getIncidentRelationship(j), totalReList);
			if (inside >= 0)
				continue;
			totalReList.push_back(en->getIncidentRelationship(j));
		}
	}

	Vec3 loc = calculateCenter(entityList);
	Entity *newen = new Entity(loc);
	newen->setLabel(name);
	addEntity(newen);

	for (int i = 0; i < totalReList.size(); i++) {

		Relationship *re = totalReList[i];
		string oldlabel = re->getLabel();

		/*get the enlist with excluding verts inside entityList*/
		vector<Entity*> vlist;
		for (int j = 0; j < re->getIncidentEntities().size(); j++) {
			Entity *en = re->getIncidentEntity(j);
			int k = isInsideList<Entity>(en, entityList);
			if (k >= 0) {
				continue;
			}
			else {
				vlist.push_back(en);
			}
		}

		/*include the new added vert*/
		vlist.push_back(newen);

		/*create new poly with verts*/
		Relationship *newRe = new Relationship(vlist);
		newRe->setLabel(oldlabel);
		addRelationship(newRe);

		Entity *originDualEn = re->getDualEntity();
		deleteRelationship(re);

		if (originDualEn) {
			newRe->setDualElement(originDualEn);
			originDualEn->setDualElement(newRe);
		}
		else
			LogConsole::writeConsole("NULL dual elements");

	}

	newen->updateIncidentRes();

	/*update the dual elements
	collapse the vertices*/
	if (dual == NULL && m_dualERData)
	{
		vector<Relationship*> dualRes;
		for (int i = 0; i < entityList.size(); i++) {
			Relationship *e = entityList[i]->getDualRelationship();
			if (e)
				dualRes.push_back(e);
		}

		if (dualRes.size() == entityList.size()) {
			Relationship *newdual = m_dualERData->mergeToRelationship(vector<Entity*>(), dualRes, name, newen);
			newen->setDualElement(newdual);
			if (newdual->getDualEntity() == NULL) {
				LogConsole::showError("bad!");
			}
		}
		else {
			LogConsole::showError("bad dual data, cannot update dual elements!");
		}
	}
	else if(dual){
		newen->setDualElement(dual);
	}

	deleteEntities(entityList);

	return newen;
}

Relationship* ERData::mergeRelationships(vector<Relationship*> relist, string name)
{
	// get all incident entities
	vector<Entity*> enlist;
	for (Relationship* re : relist)
	{
		for (Entity* en : re->getIncidentEntities())
		{
			int inside = isInsideList<Entity>(en, enlist);
			if (inside < 0)
				enlist.push_back(en);
		}
	}

	Relationship* newR = new Relationship(enlist);
	newR->setLabel(name);
	addRelationship(newR);
	newR->update2D();

	for (Relationship* re : relist)
		deleteRelationship(re);

	return newR;
}

Entity* ERData::mergeEntities(vector<Entity*> enlist, string name, Relationship* dual)
{
	// get incident relationships
	vector<Relationship*> relist;
	for (Entity* en : enlist)
	{
		for (Relationship* re : en->getIncidentRelationships())
		{
			int inside = isInsideList<Relationship>(re, relist);
			if (inside < 0)
				relist.push_back(re);
		}
	}

	Vec3 loc;
	if (dual)
		loc = dual->getCenter(0.0);
	else
		loc = calculateCenter(enlist);

	Entity* newE = new Entity(loc);
	newE->setLabel(name);
	addEntity(newE);

	for (Relationship* re : relist)
	{
		if (re->getCardinality() < 1)
			continue;

		string oldlabel = re->getLabel();
		int oldopidx = re->getOpidx();
		Color4_F oldcolor = re->getColor();
		OperationRecord* op_remove = re->getRemovalOp();
		vector<OperationRecord*> op_merges = re->getMergerOps();

		/*get the enlist with excluding verts inside entityList*/
		vector<Entity*> vlist;
		for (Entity* en : re->getIncidentEntities()) 
		{
			int k = isInsideList<Entity>(en, enlist);
			if (k >= 0)
				continue;
			else
				vlist.push_back(en);
		}

		// include new vertex
		vlist.push_back(newE);

		// create new polygon with verts
		Relationship* newR = new Relationship(vlist);
		newR->setLabel(oldlabel);
		addRelationship(newR);
		newR->setOpidx(oldopidx);
		newR->setRemovalOp(op_remove);
		newR->setMergerOps(op_merges);
		newR->setColor(oldcolor);

		Entity* original_dual = re->getDualEntity();
		deleteRelationship(re);

		if (original_dual)
		{
			newR->setDualElement(original_dual);
			original_dual->setDualElement(newR);
		}
	}

	if (m_dualERData)
	{
		Color4_F color = newE->getDiscreteColor(newE->getDegree());
		color.w = 1.0;
		newE->setColor(color);
	}
		
	newE->updateIncidentRes();
	newE->setDualElement(dual);
	deleteEntities(enlist);
	return newE;
}

bool ERData::addEntityToRelationship(Entity* en, Relationship* r)
{
	vector<Entity*> elist = r->getIncidentEntities();
	elist.push_back(en);
	string label = r->getLabel();
	Relationship* newR = new Relationship(elist);
	newR->update2D();
	addRelationship(newR);
	newR->setLabel(label);

	// remove old relationship
	Entity* dual = r->getDualEntity();
	deleteRelationship(r);
	if (newR && dual)
	{
		newR->setDualElement(dual);
		dual->setDualElement(newR);
	}

	return true;
}

bool ERData::removeEntityFromRelationship(Entity * en, Relationship * r)
{
	vector<Entity*> leftElist;

	//get the enitites left in the relationship
	for (int i = 0; i < r->getIncidentEntities().size(); i++) {

		if (r->getIncidentEntity(i) == en) {
			continue;
		}
		else{
			leftElist.push_back(r->getIncidentEntity(i));
		}
	}

	string label = r->getLabel();

	Relationship *newR = NULL;
	if (leftElist.size() >0) {
		newR = new Relationship(leftElist);
		newR->update2D();
		addRelationship(newR);
		newR->setLabel(label);
	}

	// set monogon angle
	if (r->getCardinality() == 2)
	{
		Vec3 dir = en->getLocation() - r->getCenter(0.0);
		float angle = atan2(dir.y, dir.x);
		if (angle < 0.0)
			angle += 2 * M_PI;
		newR->setMonogonRotation(angle);
	}

	//remove this relationship
	Entity* dual = r->getDualEntity();
	deleteRelationship(r);
	if (newR && dual) {
		newR->setDualElement(dual);
		dual->setDualElement(newR);
	}

	return true;
}

bool ERData::removeEntityFromIncidents(Entity * en)
{
	if (!en)
		return false;

	vector<Relationship*> relist = en->getIncidentRelationships();
	
	for (auto &re : relist) {
		removeEntityFromRelationship(en, re);
	}

	return true;
}

//remove the entities from the current relationship, the left entities sill construct a relationship
bool ERData::removeEntitiesFromRelationship(vector<Entity*> entityList, Relationship * r){

	vector<Entity*> leftElist;

	//get the enitites left in the relationship
	for (int i = 0; i < r->getIncidentEntities().size(); i++) {
		int inside = isInsideList<Entity>(r->getIncidentEntity(i), entityList);

		if (inside != -1) {
			continue;
		}
		leftElist.push_back(r->getIncidentEntity(i));
	}

	string label = r->getLabel();

	Relationship *newR = NULL;
	if (leftElist.size() >0 && !isInsideRelationshipList(leftElist)) {
		newR = new Relationship(leftElist);
		newR->update2D();
		addRelationship(newR);
		newR->setLabel(label);
	}

	//remove this relationship
	Entity* dual = r->getDualEntity();
	deleteRelationship(r);
	if (newR && dual) {
		newR->setDualElement(dual);
		dual->setDualElement(newR);
	}

	return true;
}

//remove entities from the current relationship and create a new relationship among them
bool ERData::divideEntitiesFromRelationship(vector<Entity*> entityList, Relationship * r) {

	vector<Entity*> leftElist;

	//get the enitites left in the relationship
	for (int i = 0; i < r->getIncidentEntities().size(); i++) {
		int inside = isInsideList<Entity>(r->getIncidentEntity(i), entityList);

		if (inside != -1) {
			continue;
		}
		leftElist.push_back(r->getIncidentEntity(i));
	}

	//create a new relationships between left ones
	Relationship *newR = NULL;
	if (leftElist.size() >0 && !isInsideRelationshipList(leftElist)) {
		newR = new Relationship(leftElist);
		newR->update2D();
		addRelationship(newR);
	}

	//create a new relationships between selected ones
	if (entityList.size() >0 && !isInsideRelationshipList(entityList)) {
		Relationship *newR = new Relationship(entityList);
		newR->update2D();
		addRelationship(newR);
	}

	//remove this relationship
	Entity* dual = r->getDualEntity();
	deleteRelationship(r);
	if (newR) {
		newR->setDualElement(dual);
	}
	else {
		LogConsole::showError("Fail to create new relationship");
	}
	if(dual)
		dual->setDualElement(newR);

	return true;
}

bool ERData::removeEnclosingRelationships(Relationship * r)
{
	//find the first enclosed relationship
	vector<Relationship *>enclosedRelationship;
	for (int i = 0; i < r->getAdjacentRelationships().size(); i++) {

		Relationship *temp = r->getAdjacentRelationship(i);
		bool res = isEntityListInsideEntityList(temp->getIncidentEntities(), r->getIncidentEntities());

		if (res) {
			enclosedRelationship.push_back(temp);
		}
	}

	if (enclosedRelationship.size() == 0) {
		LogConsole::writeConsole("No enclosing relationships.");
		return false;
	}

	for (int i = 0; i < enclosedRelationship.size(); i++) {
		deleteRelationship(enclosedRelationship[i]);
	}

	return true;

}

//the relationships enclosed in r will be separated
bool ERData::separateRelationship(Relationship *r) {

	//find the first enclosed relationship
	Relationship *enclosedRelationship = NULL;
	for (int i = 0; i < r->getAdjacentRelationships().size(); i++) {

		Relationship *temp = r->getAdjacentRelationship(i);
		bool res = isEntityListInsideEntityList(temp->getIncidentEntities(), r->getIncidentEntities());

		if (res) {
			enclosedRelationship = temp;
			break;
		}
	}

	if (enclosedRelationship == NULL) {
		LogConsole::showError("No enclosing relationships.");
		return false;
	}

	if (enclosedRelationship->getCardinality() <= 2) {
		LogConsole::showError("the enclosing relationship is a binary relationship, they are already separated");
		return false;
	}

	//get the enitites left in the relationship
	vector<Entity*> leftElist;
	for (int i = 0; i < r->getIncidentEntities().size(); i++) {
		int inside = isInsideList<Entity>(r->getIncidentEntity(i), enclosedRelationship->getIncidentEntities());

		if (inside != -1) {
			continue;
		}
		leftElist.push_back(r->getIncidentEntity(i));
	}

	//add the first two entities from the enclosing entities
	leftElist.push_back(enclosedRelationship->getIncidentEntity(0));
	leftElist.push_back(enclosedRelationship->getIncidentEntity(1));

	//delete the selected relationship
	deleteRelationship(r);

	//create another relationship with two sharing vertices
	if (leftElist.size() >1 && !isInsideRelationshipList(leftElist)) {
		Relationship *newR = new Relationship(leftElist);
		newR->update2D();
		addRelationship(newR);
	}

	return true;
}

void ERData::starrizeRelationships()
{
    for (int i = 0; i < m_relationshipList.size(); i++) {
        Relationship *r = m_relationshipList[i];
        r->starrize();
    }
}

void ERData::updateRelationshipLocation(int index, vector<Vec3> loc)
{
	Relationship *r = m_relationshipList[index];

	int N = r->getIncidentEntities().size() * 2;

	for (int i = 0; i < N / 2; i++)
	{
		r->getIncidentEntity(i)->setLocation(loc[i]);
	}

	gmainwindow->refreshOpenGLWindow();
}

bool ERData::updateEntitiesLocation(vector<Vec3> loc)
{
	if (loc.size() != m_entityList.size()) {
		LogConsole::writeConsole("The input location list does not have same size as the entity list");
		return false;
	}

	maxX = maxY = -DBL_MAX;
	minX = minY = DBL_MAX;

	for (int i = 0; i < m_entityList.size(); i++) {
		m_entityList[i]->setLocation(loc[i]);

		Vec3 loctemp = loc[i];

		if (loctemp.x > maxX)
			maxX = loctemp.x;
		if (loctemp.x < minX)
			minX = loctemp.x;
		if (loctemp.y > maxY)
			maxY = loctemp.y;
		if (loctemp.y < minY)
			minY = loctemp.y;
	}

	return true;
}

bool ERData::updateEntitiesLocation(int N, const double *xinput)
{
	if (N != m_entityList.size()*2) {
		LogConsole::writeConsole("The input location list does not have same size as the entity list");
		return false;
	}

	maxX = maxY = -DBL_MAX;
	minX = minY = DBL_MAX;

	for (int i = 0; i < m_entityList.size(); i++) {
		int index = m_entityList[i]->getIndex();
		assert(index == i);

		m_entityList[i]->setLocation(xinput[2*index], xinput[2 * index + 1]);

		if (xinput[2 * index] > maxX)
			maxX = xinput[2 * index];
		if (xinput[2 * index] < minX)
			minX = xinput[2 * index];
		if (xinput[2 * index + 1] > maxY)
			maxY = xinput[2 * index + 1];
		if (xinput[2 * index + 1] < minY)
			minY = xinput[2 * index + 1];
	}

	return true;

}

bool ERData::updateFreeEntityLocs(int N, const double * xinput)
{
	if (N != getFreeVertCompNum()) {
		LogConsole::writeConsole("The input loc list does not have same size as the free entity list");
		return false;
	}

	maxX = maxY = -DBL_MAX;
	minX = minY = DBL_MAX;

	for (int i = 0; i < m_entityList.size(); i++) {
		int index = m_entityList[i]->getIndex();
		assert(index == i);

		int lix = vertex_index_x_to_list_index(index, fixedVertComp);
		int liy = vertex_index_y_to_list_index(index, fixedVertComp);
		if (lix != -1 && liy != -1) {
			m_entityList[i]->setLocation(xinput[lix], xinput[liy]);
		}

		Vec3 loctemp = m_entityList[i]->getLocation();
		if (loctemp.x > maxX)
			maxX = loctemp.x;
		if (loctemp.x < minX)
			minX = loctemp.x;
		if (loctemp.y > maxY)
			maxY = loctemp.y;
		if (loctemp.y < minY)
			minY = loctemp.y;
	}

	return true;
}

void ERData::updateRelationshipLocation(vector<Vec3> centers)
{
    if (centers.size() != m_relationshipList.size()) {
        LogConsole::showError("update relationships get errors");
        return;
    }

    for (int i = 0; i < m_relationshipList.size(); i++) {
        Relationship *r = m_relationshipList[i];

        r->moveToLocation(centers[i].x, centers[i].y);
    }

}


void ERData::updateMonogonRotations(vector<double> orients)
{
	updateMonogonRotations(orients.size(), &orients[0]);
}

void ERData::updateMonogonRotations(int N, const double * orients)
{
	assert(N == getMonogons().size());

	int i = 0;
	for (auto &re : getMonogons()) {
		if (re->getCardinality() != 1) {
			LogConsole::writeError("wrong monogon list");
			continue;
		}
		re->setMonogonRotation(orients[i]);
		re->create2DScaffolds();
		i++;
	}
}

bool ERData::updateAllLocations(int NV, int NM, const double* x, int offset)
{
	REList monogons = getMonogons();
	if (NV != m_entityList.size() * 2)
	{
		LogConsole::writeConsole("The input location list does not have same size as the entity list");
		return false;
	}
	if (NM != monogons.size())
	{
		LogConsole::writeConsole("The rotation list does not have same size as the monogon list");
		return false;
	}

	//for (int i = 0; i < NV + NM; i++)
	//{
	//	LogConsole::writeConsole("index: %d, value: %.4f", i, x[i]);
	//}

	maxX = maxY = -DBL_MAX;
	minX = minY = DBL_MAX;

	for (int i = 0; i < m_entityList.size(); i++)
	{
		Entity* en = m_entityList[i];
		assert(en->getIndex() == i);

		en->setLocation(x[2 * i + offset], x[2 * i + 1 + offset]);

		if (x[2 * i + offset] > maxX)
			maxX = x[2 * i + offset];
		if (x[2 * i + offset] < minX)
			minX = x[2 * i + offset];
		if (x[2 * i + 1 + offset] > maxY)
			maxY = x[2 * i + 1 + offset];
		if (x[2 * i + 1 + offset] < minY)
			minY = x[2 * i + 1 + offset];
	}

	for (int i = 0; i < NM; i++)
	{
		Relationship* mon = monogons[i];
		assert(mon->getCardinality() == 1);

		mon->setMonogonRotation(x[NV + i + offset]);
		mon->create2DScaffolds();
	}

	return true;
}

bool ERData::updateAllFreeLocations(int NV, int NM, const double* x, int offset)
{
	if (NV != getFreeVertCompNum())
	{
		LogConsole::writeConsole("The input loc list does not have same size as the free entity list");
		return false;
	}
	if (NM != getFreeMonoCompNum())
	{
		LogConsole::writeConsole("The input rotation list does not have the same size as the free monogon list");
		return false;
	}

	//for (int i = 0; i < NV + NM; i++)
	//{
	//	LogConsole::writeConsole("index: %d, value: %.4f", i, x[i]);
	//}

	maxX = maxY = -DBL_MAX;
	minX = minY = DBL_MAX;

	for (int i = 0; i < m_entityList.size(); i++) 
	{
		int index = m_entityList[i]->getIndex();
		assert(index == i);

		int lix = vertex_index_x_to_list_index(index, fixedVertComp);
		int liy = vertex_index_y_to_list_index(index, fixedVertComp);
		if (lix != -1 && liy != -1)
			m_entityList[i]->setLocation(x[lix+offset], x[liy+offset]);
		
		Vec3 loctemp = m_entityList[i]->getLocation();
		if (loctemp.x > maxX)
			maxX = loctemp.x;
		if (loctemp.x < minX)
			minX = loctemp.x;
		if (loctemp.y > maxY)
			maxY = loctemp.y;
		if (loctemp.y < minY)
			minY = loctemp.y;
	}

	REList monogons = getMonogons();
	for (int i = 0; i < monogons.size(); i++)
	{
		Relationship* re = monogons.at(i);
		int listindex = monogon_index_to_list_index(i, NV, getSortedFixedMonoIndices());

		if (listindex != -1)
		{
			re->setMonogonRotation(x[listindex + offset]);
			re->create2DScaffolds();
		}
	}

	return true;
}

void ERData::CheckCollocatedRelationShips()
{
	int count = 0;
	typedef std::set<vector<Entity*>> SetOfEntities;
	SetOfEntities setOfEntities;
	
	for (int i = 0; i < m_relationshipList.size(); i++)
	{
		bool DuplicationFound = false;

		Relationship* currentRelation = m_relationshipList[i];
		const vector<Entity*>& CurrentEntities = currentRelation->getIncidentEntities();

		auto search = setOfEntities.find(CurrentEntities);
		if (search == setOfEntities.end())
		{
			setOfEntities.insert(CurrentEntities);
		}
		else
		{
			count++;
			LogConsole::writeConsole("Found Duplication for the Paper: %d", i);
		}
	}

	LogConsole::writeConsole("Total Duplication Found: %d", count);
}

int ERData::getCurrentLayoutIndex() const
{
	return current_layout_index;
}

int ERData::getSavedLayoutNum() const
{
	return m_backupLayouts.size();
}

int ERData::saveCurrentLayout()
{
	/*clear all layout after current used one*/
	if (current_layout_index != m_backupLayouts.size() - 1) {
		m_backupLayouts.erase(m_backupLayouts.begin() + current_layout_index + 1, m_backupLayouts.end());
	}

	BackupLayout layout;
	for (int i = 0; i < m_entityList.size(); i++) {
		layout.push_back(m_entityList[i]->getLocation());
	}
	m_backupLayouts.push_back(layout);

	if (m_backupLayouts.size() > 10) {
		m_backupLayouts.erase(m_backupLayouts.begin());
	}

	current_layout_index = m_backupLayouts.size() - 1;

	return current_layout_index;
}

bool ERData::resumeLayout(int index)
{
	if(m_backupLayouts.size() == 0)
	{
		LogConsole::writeRedConsole("No saved backup layout");
		return false;
	}

	if (index < 0 || index >= m_backupLayouts.size()) {
		LogConsole::writeError("Invalid backup layout index!");
		return false;
	}

	const BackupLayout &layout = m_backupLayouts[index];
	updateEntitiesLocation(layout);
	update2D();
	return true;
}
void ERData::clearBackupLayouts()
{
	m_backupLayouts.clear();
	current_layout_index -= 1;
}

int ERData::undoLayout()
{
	int old_li = current_layout_index;
	current_layout_index -= 1;
	bool res = resumeLayout(current_layout_index);
	if (!res)
	{
		current_layout_index = old_li;
	}

	return current_layout_index;
}

int ERData::redoLayout()
{
	int old_li = current_layout_index;
	current_layout_index += 1;
	bool res = resumeLayout(current_layout_index);
	if (!res)
	{
		current_layout_index = old_li;
	}

	return current_layout_index;
}

void ERData::calculateBoundingCirle(bool resetZ, bool includeMonogon)
{
	if (m_entityList.size() == 0)
		return;

	Vec3 loc = m_entityList[0]->getLocation();
	maxX = minX = loc.x;
	maxY = minY = loc.y;

	double margin = Element::m_MonogonSize + Element::m_MonogonLength;

	for (int i = 0; i < m_entityList.size(); i++) {

		Vec3 loc = m_entityList[i]->getLocation();

		if (loc.x > maxX)
			maxX = loc.x;
		if (loc.x < minX)
			minX = loc.x;
		if (loc.y > maxY)
			maxY = loc.y;
		if (loc.y < minY)
			minY = loc.y;
	}

	if(includeMonogon)
	{
		//check all points of monogon scaffold
		for (int i = 0; i < getRelationships().size(); i++)
		{
			if (getRelationship(i)->getCardinality() != 1)
				continue;

			RE* re = getRelationship(i);
			Scaffold *s = re->getScaffoldPointer(0);
			for (int j = 0; j < s->m_CurvedLocation.size(); j++) {
				Vec3 loc = s->m_CurvedLocation[j];

				if (loc.x > maxX)
					maxX = loc.x;
				if (loc.x < minX)
					minX = loc.x;
				if (loc.y > maxY)
					maxY = loc.y;
				if (loc.y < minY)
					minY = loc.y;
			}
		}

		maxX += Element::m_VertexSize;
		maxY += Element::m_VertexSize;
		minX -= Element::m_VertexSize;
		minY -= Element::m_VertexSize;
	}

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;

	BC_center = Vec3(midX, midY, 0);

	BC_radius = 0;
	for (int i = 0; i < m_entityList.size(); i++) {
		Vec3 loc = m_entityList[i]->getLocation();

		if(resetZ)
			loc.z = 0;

		double dist = length(loc - BC_center);
		
		if (dist > BC_radius)
			BC_radius = dist;
	}
}

void ERData::jitterLocations()
{
	srand(100000);

	for (int i = 0; i < m_entityList.size(); i++) {

		if (!m_entityList[i]->flag_Optimizable)
			continue;

		double dx = 0.01*(double)rand() / RAND_MAX;
		double dy = 0.01*(double)rand() / RAND_MAX;

		double o_x = m_entityList[i]->getLocation().x;
		double o_y = m_entityList[i]->getLocation().y;

		double n_x = o_x + dx*0.01;
		double n_y = o_y + dy*0.01;

		m_entityList[i]->setLocation(Vec3(n_x, n_y, 0));
	}

	update2D();

	LogConsole::writeGreenConsole("vertices locations are jittered");

}

/*Rescale bounding circle*/
void ERData::rescaleBoundingCirle()
{
    Vec3 old_BC_center = BC_center;
    double old_BC_radius = BC_radius;

    calculateBoundingCirle();

    Vec3 dis = old_BC_center - BC_center;
    double scale = old_BC_radius / BC_radius;

    moveEntities(dis.x, dis.y, &m_entityList);
    scaleEntities(Vec3(scale,scale,0), old_BC_center);

    BC_center = old_BC_center;
    BC_radius = old_BC_radius;
}

bool ERData::isBoundingBoxIntersect(const Relationship * re1, const Relationship * re2) const
{
	if (re1->minX > re2->maxX || re2->minX > re1->maxX)
		return false;

	if (re1->minY > re2->maxY || re2->minY > re1->maxY)
		return false;

	return true;
}

void ERData::updateRelationshipsBoundingBox()
{
	for (int j = 0; j < getRelationshipNum(); j++) {
		m_relationshipList[j]->updateBoundingBox();
	}
}

void ERData::setOptimizableVerticesAndPolygons(vector<Entity*> elist, vector<Relationship*> rlist, bool optimizable)
{
    if (elist.size() == 0 && rlist.size() == 0) {
        for (int i = 0; i < m_entityList.size(); i++) {
            m_entityList[i]->flag_Optimizable = optimizable;
        }

        for (int i = 0; i < m_relationshipList.size(); i++) {
            m_relationshipList[i]->flag_Optimizable = optimizable;
        }
    }
    else {
        for (int i = 0; i < m_entityList.size(); i++) {
            m_entityList[i]->flag_Optimizable = !optimizable;
        }

        for (int i = 0; i < m_relationshipList.size(); i++) {
            m_relationshipList[i]->flag_Optimizable = !optimizable;
        }

        for (int i = 0; i < elist.size(); i++) {
            elist[i]->flag_Optimizable = optimizable;
        }

        for (int i = 0; i < rlist.size(); i++) {
            rlist[i]->flag_Optimizable = optimizable;
            
            for (int j = 0; j < rlist[i]->getIncidentEntities().size(); j++) {
                rlist[i]->getIncidentEntity(j)->flag_Optimizable = optimizable;
            }
        }
    }
}

vector<int> ERData::getEntityIndicesOfRelationships(vector<int> rlist)
{
    set<int> enlist;
    for (int i = 0; i < rlist.size(); i++) {

        int index = rlist[i];
        Relationship *r = m_relationshipList[index];
        
        int incidentECount = r->getIncidentEntities().size();

        for (int j = 0; j < incidentECount; j++) {
            Entity *e = r->getIncidentEntity(j);
            int indexOfEntity = e->getIndex();
            enlist.insert(indexOfEntity);
        }
    }

    vector<int> res;
    for (set<int>::iterator it = enlist.begin(); it != enlist.end(); ++it) {
        LogConsole::writeConsole("entity:%d", *it);
        res.push_back(*it);
    }

    return res;
}

vector<Relationship*> ERData::getEntityEnclosingRelationships(vector<Entity*> elist)
{
	vector<Relationship*> res;
	for (int i = 0; i < m_relationshipList.size(); i++) {

		bool enclosed = isRelationshipEnclosedByEntities(m_relationshipList[i], elist);

		if (enclosed)
		{
			res.push_back(m_relationshipList[i]);
		}
	}

	return res;

}

vector<Relationship*> ERData::getEntityIncidentRelationships(const int vi)
{
	Entity* en = m_entityList[vi];

	return en->getIncidentRelationships();
}

vector<Relationship*> ERData::findPolygonsToMergeGreedy()
{

 vector<Relationship*> res;

 Relationship *re1 = NULL;
 Relationship *re2 = NULL;
 
 int max_adjShared = 2;

 for (int i = 0; i < getRelationshipNum(); i++) {

     RE *r = m_relationshipList[i];
     int card = r->getCardinality();

     for (auto r_adj : r->getAdjacentRelationships()) {

         int shardEns = getSharedEntities(r_adj, r).size();
         if (shardEns > max_adjShared /*|| shardEns == card*/) {
             re1 = r;
             re2 = r_adj;
         }
     }


     if (re1 && re2) {
         res.push_back(re1);
         res.push_back(re2);
         break;
     }
 }

 return res;
}

/*find a polygon whose adjacent polygons define constraint edges 
that construct a loop (enlist) which is smaller than cardinality of poly
The loopRes[0] is the relationship*/
int ERData::findPolygonContainUnavoidableLoop(vector<Entity*> &loop, vector<Relationship*>&loopRes)
{
 int loopSize = -1;
 for (const auto &re : m_relationshipList) {

     loop.clear();
     loopRes.clear();
     loopSize = re->hasEdgeAdjLoop(loop, loopRes);

     if (loopSize!=-1 && loopSize < re->getCardinality()) {
         loopRes.insert(loopRes.begin(), re);
         return loopSize;
     }

 }

 loop.clear();
 loopRes.clear();
 return -1;
}

bool ERData::FindReWithEdgelyAdjacentNodeOverlapping(vector<Entity*>& enlist, vector<Relationship*>& relist)
{
 for (const auto &re : m_relationshipList) {

     for (int i = 0; i < re->getIncidentEntities().size(); i++) {

         Entity *e = re->getIncidentEntity(i);

         vector<Relationship::EdgelyAdjacentRelationship> edgelyAdjacentRes = re->getEdgelyAdjacentRelationships(e);
         if (edgelyAdjacentRes.size() > 2) {

             relist.insert(relist.begin(), re);
             enlist.insert(enlist.begin(), e);

             for (int k = 0; k < edgelyAdjacentRes.size(); k++) {
                 uniqueInsert(edgelyAdjacentRes[k].r1, relist);
             }
             return true;
         }
     }

 }

 return false;
}

void ERData::writeEulerCharacteristics(string vis)
{
 int V = getEntityNum();
 int F = getRelationshipNum();
 int E1 = getEdgeNum();
 int E2 = getTotalCard();

 LogConsole::writeConsole("\n------ Euler Characteristics ------");
 LogConsole::writeConsole("[%s] V = %i", vis.c_str(), V);
 LogConsole::writeConsole("[%s] F = %i", vis.c_str(), F);
 LogConsole::writeConsole("[%s] E1 = %i (connected vertices, no duplicates)", vis.c_str(), E1);
 LogConsole::writeConsole("[%s] E2 = %i (connected vertices, w/ duplicates", vis.c_str(), E2);

 LogConsole::writeConsole("[%s] V-E1+F = %i (Prime Euler Characteristic)", vis.c_str(), V - E1 + F);
 LogConsole::writeConsole("[%s] V-E2+F = %i (Prime Euler Characteristic)", vis.c_str(), V - E2 + F);
}

int ERData::getTotalCard()
{
	int total = 0;
	for (Relationship* re : m_relationshipList)
		total += re->getCardinality();
	return total;
}

int ERData::getTotalDeg()
{
	int total = 0;
	for (Entity* en : m_entityList)
		total += en->getDegree();
	return total;
}

void ERData::retrieveAttributes(vector<vector<string>> attrlist)
{

 for (int j = 0; j < m_entityList.size(); j++) {
     Entity *e = m_entityList[j];
     string label = e->getLabel();

     e->clearAttributes();

     for (int i = 0; i < attrlist.size(); i++) {
         vector<string> attributes = attrlist[i];

         string key = attributes[0];
         int res = key.compare(label);

         if (res == 0) {
             e->setAttributes(attributes);
         }
     }
 }
}

void ERData::appendFileComment(string comment)
{
 FILECOMMENTS += comment;
 FILECOMMENTS += "\n";
}

void ERData::clearComment()
{
 FILECOMMENTS = "";
}




// Multiscale functions

void ERData::generateAtomicOperations(bool nrem, bool nmerge, bool erem, bool emerge, double alpha, double beta, double gamma)
{
	// calculate statistics for priority measure
	calcAllStatsLocally();

	double min_incidence = getMinIncidence();
	double max_incidence = getMaxIncidence();
	double min_adjfactor = getMinAdjacencyFactor();
	double max_adjfactor = getMaxAdjacencyFactor();
	double min_centrality = getMinBetweennessCentrality();
	double max_centrality = getMaxBetweennessCentrality();
	double min_resistance = getMinEffectiveResistance();
	double max_resistance = getMaxEffectiveResistance();

	// clean up any existing operations
	deleteAtomicOperations();
	
	if (nrem || nmerge)
	{
		for (Entity* remove : m_entityList)
		{
			if (nrem)
			{
				OperationRecord* new_remove = new OperationRecord(remove, OPERATION_TYPE::NODE_DELETE);
				calcOperationPriority(new_remove, alpha, beta, gamma);
				remove->setRemovalOp(new_remove);
			}

			if (nmerge)
			{
				for (Entity* retain : remove->getAdjacentEntities())
				{
					OperationRecord* new_merge = new OperationRecord(remove, OPERATION_TYPE::NODE_MERGE, retain);
					bool duplicate = false;
					for (OperationRecord* old_merge : remove->getMergerOps())
					{
						if (mergesIdentical(new_merge, old_merge))
						{
							duplicate = true;
							break;
						}
					}

					if (duplicate)
						delete(new_merge);
					else
					{
						calcOperationPriority(new_merge, alpha, beta, gamma);
						remove->addMergerOp(new_merge);
						retain->addMergerOp(new_merge);
					}
				}
			}
		}
	}

	if (erem || emerge)
	{
		for (Relationship* remove : m_relationshipList)
		{
			if (erem)
			{
				OperationRecord* new_remove = new OperationRecord(remove, OPERATION_TYPE::EDGE_DELETE);
				calcOperationPriority(new_remove, alpha, beta, gamma);
				remove->setRemovalOp(new_remove);
			}

			if (emerge)
			{
				for (Relationship* retain : remove->getAdjacentRelationships())
				{
					OperationRecord* new_merge = new OperationRecord(remove, OPERATION_TYPE::EDGE_MERGE, retain);
					bool duplicate = false;
					for (OperationRecord* old_merge : remove->getMergerOps())
					{
						if (mergesIdentical(new_merge, old_merge))
						{
							duplicate = true; 
							break;
						}
					}

					if (duplicate)
						delete(new_merge);
					else
					{
						calcOperationPriority(new_merge, alpha, beta, gamma);
						remove->addMergerOp(new_merge);
						retain->addMergerOp(new_merge);
					}
				}
			}
		}
	}
}

void ERData::deleteAtomicOperations()
{
	for (Entity* en : m_entityList)
		en->deleteOperations();
	for (Relationship* re : m_relationshipList)
		re->deleteOperations();
}

void ERData::updateOperationPriority(OperationRecord* op, double alpha, double beta, double gamma)
{
	//OPERATION_TYPE type = op->getType();
	//int footprint_size;
	//switch (type)
	//{
	//	case OPERATION_TYPE::NODE_DELETE:
	//	{
	//		Entity* remove = getEntityByOpidx(op->getRemovedIndex());
	//		footprint_size = 1 + remove->getIncidentRelationships().size() + remove->getAdjacentEntities().size();
	//		break;
	//	}

	//	case OPERATION_TYPE::EDGE_DELETE:
	//	{
	//		Relationship* remove = getRelationshipByOpidx(op->getRemovedIndex());
	//		footprint_size = 1 + remove->getIncidentEntities().size() + remove->getAdjacentRelationships().size();
	//		break;
	//	}

	//	case OPERATION_TYPE::NODE_MERGE:
	//	{
	//		Entity* remove = getEntityByOpidx(op->getRemovedIndex());
	//		Entity* retain = getEntityByOpidx(op->getRetainedIndex());
	//		std::unordered_set<int> incidence, adjacency;

	//		for (Relationship* inc : remove->getIncidentRelationships())
	//			incidence.insert(inc->getOpidx());
	//		for (Relationship* inc : retain->getIncidentRelationships())
	//			incidence.insert(inc->getOpidx());
	//		for (Entity* adj : remove->getAdjacentEntities())
	//			adjacency.insert(adj->getOpidx());
	//		for (Entity* adj : retain->getAdjacentEntities())
	//			adjacency.insert(adj->getOpidx());

	//		footprint_size = incidence.size() + adjacency.size();
	//		break;
	//	}

	//	case OPERATION_TYPE::EDGE_MERGE:
	//	{
	//		Relationship* remove = getRelationshipByOpidx(op->getRemovedIndex());
	//		Relationship* retain = getRelationshipByOpidx(op->getRetainedIndex());
	//		std::unordered_set<int> incidence, adjacency;

	//		for (Entity* inc : remove->getIncidentEntities())
	//			incidence.insert(inc->getOpidx());
	//		for (Entity* inc : retain->getIncidentEntities())
	//			incidence.insert(inc->getOpidx());
	//		for (Relationship* adj : remove->getAdjacentRelationships())
	//			adjacency.insert(adj->getOpidx());
	//		for (Relationship* adj : retain->getAdjacentRelationships())
	//			adjacency.insert(adj->getOpidx());

	//		footprint_size = incidence.size() + adjacency.size();
	//		break;
	//	}
	//}

	//double cost = 0.0; //(double)footprint_size / (double)(m_entityArraySize + m_relationshipArraySize);
	//op->setCost(cost);

	double incidence;
	double adjfactor;
	double centrality;

	double min_incidence = getMinIncidence();
	double max_incidence = getMaxIncidence();
	double min_adjfactor = getMinAdjacencyFactor();
	double max_adjfactor = getMaxAdjacencyFactor();
	double min_centrality = getMinBetweennessCentrality();
	double max_centrality = getMaxBetweennessCentrality();

	if (op->getType() == OPERATION_TYPE::NODE_DELETE)
	{
		int remove_id = op->getRemovedIndex();
		Entity* remove_en = getEntityByOpidx(remove_id);

		incidence = 0; //remove_en->getDegree();
		for (Relationship* re : remove_en->getIncidentRelationships())
			incidence = std::max(incidence, double(re->getCardinality()));
		adjfactor = 0;
		for (Entity* adj : remove_en->getAdjacentEntities())
			adjfactor += std::pow(getSharedRelationshipNum(remove_en, adj),gmainwindow->share_power);
		centrality = m_entityBetweennessCentrality[remove_id];
	}
	else if (op->getType() == OPERATION_TYPE::NODE_MERGE)
	{
		int remove_id = op->getRemovedIndex();
		int retain_id = op->getRetainedIndex();
		Entity* remove_en = getEntityByOpidx(remove_id);
		Entity* retain_en = getEntityByOpidx(retain_id);

		incidence = 0; //std::max(remove_en->getDegree(), retain_en->getDegree());
		for (Relationship* re : remove_en->getIncidentRelationships())
			incidence = std::max(incidence, double(re->getCardinality()));
		for (Relationship* re : retain_en->getIncidentRelationships())
			incidence = std::max(incidence, double(re->getCardinality()));
		double remove_adjfactor = 0;
		for (Entity* adj : remove_en->getAdjacentEntities())
			remove_adjfactor += std::pow(getSharedRelationshipNum(remove_en, adj), gmainwindow->share_power);
		double retain_adjfactor = 0;
		for (Entity* adj : retain_en->getAdjacentEntities())
			retain_adjfactor += std::pow(getSharedRelationshipNum(remove_en, adj), gmainwindow->share_power);
		adjfactor = (remove_adjfactor + retain_adjfactor) / 2.0;
		centrality = (m_entityBetweennessCentrality[remove_id] + m_entityBetweennessCentrality[retain_id]) / 2.0;
	}
	else if (op->getType() == OPERATION_TYPE::EDGE_DELETE)
	{
		int remove_id = op->getRemovedIndex();
		Relationship* remove_re = getRelationshipByOpidx(remove_id);

		incidence = 0; //remove_re->getCardinality();
		for (Entity* en : remove_re->getIncidentEntities())
			incidence = std::max(incidence, double(en->getDegree()));
		adjfactor = 0;
		for (Relationship* adj : remove_re->getAdjacentRelationships())
			adjfactor += std::pow(getSharedEntityNum(remove_re, adj), gmainwindow->share_power);
		centrality = m_relationshipBetweennessCentrality[remove_id];
	}
	else if (op->getType() == OPERATION_TYPE::EDGE_MERGE)
	{
		int remove_id = op->getRemovedIndex();
		int retain_id = op->getRetainedIndex();
		Relationship* remove_re = getRelationshipByOpidx(remove_id);
		Relationship* retain_re = getRelationshipByOpidx(retain_id);

		incidence = 0; //std::max(remove_re->getCardinality(), retain_re->getCardinality());
		for (Entity* en : remove_re->getIncidentEntities())
			incidence = std::max(incidence, double(en->getDegree()));
		for (Entity* en : retain_re->getIncidentEntities())
			incidence = std::max(incidence, double(en->getDegree()));
		double remove_adjfactor = 0;
		for (Relationship* adj : remove_re->getAdjacentRelationships())
			remove_adjfactor += std::pow(getSharedEntityNum(remove_re, adj), gmainwindow->share_power);
		double retain_adjfactor = 0;
		for (Relationship* adj : retain_re->getAdjacentRelationships())
			retain_adjfactor += std::pow(getSharedEntityNum(remove_re, adj), gmainwindow->share_power);
		adjfactor = (remove_adjfactor + retain_adjfactor) / 2.0;
		centrality = (m_relationshipBetweennessCentrality[remove_id] + m_relationshipBetweennessCentrality[retain_id]) / 2.0;
	}

	double preserve_incidence = (max_incidence == min_incidence) ? 0.0 : (max_incidence - incidence) / (max_incidence - min_incidence);
	double preserve_adjfactor = (max_adjfactor == min_adjfactor) ? 0.0 : (max_adjfactor - adjfactor) / (max_adjfactor - min_adjfactor);
	double preserve_centrality = (max_centrality == min_centrality) ? 0.0 : (max_centrality - centrality) / (max_centrality - min_centrality);

	double log_alpha = log2(alpha);
	double log_beta = log2(beta);
	double log_gamma = log2(gamma);

	double importance = (log_alpha * preserve_incidence) + (log_beta * preserve_adjfactor) + (log_gamma * preserve_centrality);

	op->setImportance(importance);
	op->setCost(0.0);
}

void ERData::calcOperationPriority(OperationRecord* op, double alpha, double beta, double gamma)
{
	std::unordered_set<int> en_footprint;
	std::unordered_set<int> re_footprint;
	//double sum_incidence = 0.0;
	//double sum_adjfactor = 0.0;
	//double sum_centrality = 0.0;
	double incidence;
	double adjfactor;
	double centrality;

	double min_incidence = getMinIncidence();
	double max_incidence = getMaxIncidence();
	double min_adjfactor = getMinAdjacencyFactor();
	double max_adjfactor = getMaxAdjacencyFactor();
	double min_centrality = getMinBetweennessCentrality();
	double max_centrality = getMaxBetweennessCentrality();

	if (op->getType() == OPERATION_TYPE::NODE_DELETE)
	{
		int remove_id = op->getRemovedIndex();

		en_footprint.insert(remove_id);
		//sum_incidence += m_entityIncidence[remove_id];
		//sum_adjfactor += m_entityAdjacencyFactor[remove_id];
		//sum_centrality += m_entityBetweennessCentrality[remove_id];
		
		incidence = 0; //m_entityIncidence[remove_id];
		adjfactor = m_entityAdjacencyFactor[remove_id];
		centrality = m_entityBetweennessCentrality[remove_id];

		for (int i = 0; i < m_relationshipArraySize; i++)
		{
			if (m_incidenceMatrix[i][remove_id] == 1)
			{
				/*sum_incidence += m_relationshipIncidence[i];
				sum_adjfactor += m_relationshipAdjacencyFactor[i];*/
				//sum_centrality += m_relationshipBetweennessCentrality[i];
				incidence = std::max(incidence, double(m_relationshipIncidence[i]));
				re_footprint.insert(i);
			}
		}

		//for (int i = 0; i < m_entityArraySize; i++)
		//{
		//	if (m_entityAdjacencyMatrix[i][remove_id] == 1)
		//	{
		//		sum_incidence += m_entityIncidence[i];
		//		//sum_adjfactor += m_entityAdjacencyFactor[i];
		//		//sum_centrality += m_entityBetweennessCentrality[i];
		//		en_footprint.insert(i);
		//	}
		//}
	}
	else if (op->getType() == OPERATION_TYPE::NODE_MERGE)
	{
		int remove_id = op->getRemovedIndex();
		int retain_id = op->getRetainedIndex();

		en_footprint.insert(remove_id);
		//sum_incidence += m_entityIncidence[remove_id];
		//sum_adjfactor += m_entityAdjacencyFactor[remove_id];
		//sum_centrality += m_entityBetweennessCentrality[remove_id];

		en_footprint.insert(retain_id);
		//sum_incidence += m_entityIncidence[retain_id];
		//sum_adjfactor += m_entityAdjacencyFactor[retain_id];
		//sum_centrality += m_entityBetweennessCentrality[retain_id];

		incidence = 0; //std::max(m_entityIncidence[remove_id], m_entityIncidence[retain_id]);
		adjfactor = (m_entityAdjacencyFactor[remove_id] + m_entityAdjacencyFactor[retain_id]) / 2.0;
		centrality = (m_entityBetweennessCentrality[remove_id] + m_entityBetweennessCentrality[retain_id]) / 2.0;

		for (int i = 0; i < m_relationshipArraySize; i++)
		{
			if (m_incidenceMatrix[i][remove_id] == 1 || m_incidenceMatrix[i][retain_id] == 1)
			{
				/*sum_incidence += m_relationshipIncidence[i];
				sum_adjfactor += m_relationshipAdjacencyFactor[i];*/
				//sum_centrality += m_relationshipBetweennessCentrality[i];
				incidence = std::max(incidence, double(m_relationshipIncidence[i]));
				re_footprint.insert(i);
			}
		}

		//for (int i = 0; i < m_entityArraySize; i++)
		//{
		//	if (m_entityAdjacencyMatrix[i][remove_id] == 1 || m_entityAdjacencyMatrix[i][retain_id] == 1)
		//	{
		//		sum_incidence += m_entityIncidence[i];
		//		sum_adjfactor += m_entityAdjacencyFactor[i];
		//		//sum_centrality += m_entityBetweennessCentrality[i];
		//		en_footprint.insert(i);
		//	}
		//}

		//for (Cluster* cluster : m_clusterList)
		//{
		//	if (cluster->containsEn(remove_id) || cluster->containsEn(retain_id))
		//	{
		//		double importance = cluster->getImportance();
		//		if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && importance > en_cluster)
		//			en_cluster = importance;
		//		else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && importance > re_cluster)
		//			re_cluster = importance;
		//	}
		//}
	}
	else if (op->getType() == OPERATION_TYPE::EDGE_DELETE)
	{
		int remove_id = op->getRemovedIndex();

		re_footprint.insert(remove_id);
		//sum_incidence += m_relationshipIncidence[remove_id];
		//sum_adjfactor += m_relationshipAdjacencyFactor[remove_id];
		//sum_centrality += m_relationshipBetweennessCentrality[remove_id];

		incidence = 0; //m_relationshipIncidence[remove_id];
		adjfactor = m_relationshipAdjacencyFactor[remove_id];
		centrality = m_relationshipBetweennessCentrality[remove_id];

		for (int i = 0; i < m_entityArraySize; i++)
		{
			if (m_incidenceMatrix[remove_id][i] == 1)
			{
				//sum_incidence += m_entityIncidence[i];
				//sum_adjfactor += m_entityAdjacencyFactor[i];
				//sum_centrality += m_entityBetweennessCentrality[i];
				incidence = std::max(incidence, double(m_entityIncidence[i]));
				en_footprint.insert(i);
			}
		}

		//for (int i = 0; i < m_relationshipArraySize; i++)
		//{
		//	if (m_relationshipAdjacencyMatrix[remove_id][i] == 1)
		//	{
		//		sum_incidence += m_relationshipIncidence[i];
		//		//sum_adjfactor += m_relationshipAdjacencyFactor[i];
		//		//sum_centrality += m_relationshipBetweennessCentrality[i];
		//		en_footprint.insert(i);
		//	}
		//}

		//for (Cluster* cluster : m_clusterList)
		//{
		//	if (cluster->containsRe(remove_id))
		//	{
		//		double importance = cluster->getImportance();
		//		if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && importance > en_cluster)
		//			en_cluster = importance;
		//		else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && importance > re_cluster)
		//			re_cluster = importance;
		//	}
		//}
	}
	else if (op->getType() == OPERATION_TYPE::EDGE_MERGE)
	{
		int remove_id = op->getRemovedIndex();
		int retain_id = op->getRetainedIndex();

		re_footprint.insert(remove_id);
		//sum_incidence += m_relationshipIncidence[remove_id];
		//sum_adjfactor += m_relationshipAdjacencyFactor[remove_id];
		//sum_centrality += m_relationshipBetweennessCentrality[remove_id];

		re_footprint.insert(retain_id);
		//sum_incidence += m_relationshipIncidence[retain_id];
		//sum_adjfactor += m_relationshipAdjacencyFactor[retain_id];
		//sum_centrality += m_relationshipBetweennessCentrality[retain_id];

		incidence = 0; //std::max(m_relationshipIncidence[remove_id], m_relationshipIncidence[retain_id]);
		adjfactor = (m_relationshipAdjacencyFactor[remove_id] + m_relationshipAdjacencyFactor[retain_id]) / 2.0;
		centrality = (m_relationshipBetweennessCentrality[remove_id] + m_relationshipBetweennessCentrality[retain_id]) / 2.0;

		for (int i = 0; i < m_entityArraySize; i++)
		{
			if (m_incidenceMatrix[remove_id][i] == 1 || m_incidenceMatrix[retain_id][i] == 1)
			{
				//sum_incidence += m_entityIncidence[i];
				//sum_adjfactor += m_entityAdjacencyFactor[i];
				//sum_centrality += m_entityBetweennessCentrality[i];
				incidence = std::max(incidence, double(m_entityIncidence[i]));
				en_footprint.insert(i);
			}
		}

		//for (int i = 0; i < m_relationshipArraySize; i++)
		//{
		//	if (m_relationshipAdjacencyMatrix[remove_id][i] == 1 || m_relationshipAdjacencyMatrix[remove_id][i] == 1)
		//	{
		//		sum_incidence += m_relationshipIncidence[i];
		//		//sum_adjfactor += m_relationshipAdjacencyFactor[i];
		//		//sum_centrality += m_relationshipBetweennessCentrality[i];
		//		en_footprint.insert(i);
		//	}
		//}

		//for (Cluster* cluster : m_clusterList)
		//{
		//	if (cluster->containsRe(remove_id) || cluster->containsRe(retain_id))
		//	{
		//		double importance = cluster->getImportance();
		//		if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && importance > en_cluster)
		//			en_cluster = importance;
		//		else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && importance > re_cluster)
		//			re_cluster = importance;
		//	}
		//}
	}

	double footprint_size = (double)(en_footprint.size() + re_footprint.size());
	//incidence = sum_incidence / footprint_size;
	//adjfactor = sum_adjfactor / footprint_size;
	//centrality = sum_centrality;
	//if (op->getType() == OPERATION_TYPE::EDGE_MERGE || op->getType() == OPERATION_TYPE::NODE_MERGE)
	//	centrality /= 2.0;

	double preserve_incidence = (max_incidence == min_incidence) ? 0.0 : (max_incidence - incidence) / (max_incidence - min_incidence);
	double preserve_adjfactor = (max_adjfactor == min_adjfactor) ? 0.0 : (max_adjfactor - adjfactor) / (max_adjfactor - min_adjfactor);
	double preserve_centrality = (max_centrality == min_centrality) ? 0.0 : (max_centrality - centrality) / (max_centrality - min_centrality);

	double log_alpha = log2(alpha);
	double log_beta = log2(beta);
	double log_gamma = log2(gamma);

	double importance = (log_alpha * preserve_incidence) + (log_beta * preserve_adjfactor) + (log_gamma * preserve_centrality);
	double cost = 0;//(double)footprint_size / (double)(m_entityList.size() + m_relationshipList.size());

	// remove this, just for testing
	/*if ((op->getType() == OPERATION_TYPE::EDGE_MERGE) || (op->getType() == OPERATION_TYPE::NODE_MERGE))
		importance = 0.0;*/

	op->setImportance(importance);
	op->setCost(cost);
}

bool ERData::isConnectedWithoutRelationship(Relationship* remove)
{
	if (m_entityList.size() < 2)
		return true;

	int orphan_count = 0;
	int start_point = 0;
	for (Entity* en : remove->getIncidentEntities())
	{
		if (en->getDegree() == 1)
			orphan_count++;
		if (en->getIndex() == start_point)
			start_point++;
	}

	std::unordered_set<int> connected;
	std::queue<int> traverse;
	std::pair<std::unordered_set<int>::iterator, bool> ret;
	connected.insert(m_entityList[0]->getIndex());
	traverse.push(m_entityList[0]->getIndex());

	while (!traverse.empty())
	{
		int idx = traverse.front();
		traverse.pop();
		for (Relationship* re : m_entityList[idx]->getIncidentRelationships())
		{
			if (re == remove)
				continue;

			for (Entity* en : re->getIncidentEntities())
			{
				ret = connected.insert(en->getIndex());
				if (ret.second)
					traverse.push(en->getIndex());
			}
		}
	}

	if (connected.size() == (m_entityList.size() - orphan_count))
		return true;
	else
		return false;
}

bool ERData::isConnectedWithoutEntity(Entity* remove)
{
	if (m_entityList.size() < 2)
		return true;

	int orphan_count = 0;
	int start_point = 0;
	for (Relationship* re : remove->getIncidentRelationships())
	{
		if (re->getCardinality() == 1)
			orphan_count++;
		if (re->getIndex() == start_point)
			start_point++;
	}

	std::unordered_set<int> connected;
	std::queue<int> traverse;
	std::pair<std::unordered_set<int>::iterator, bool> ret;
	connected.insert(m_relationshipList[start_point]->getIndex());
	traverse.push(m_relationshipList[start_point]->getIndex());

	while (!traverse.empty())
	{
		int idx = traverse.front();
		traverse.pop();
		for (Entity* en : m_relationshipList[idx]->getIncidentEntities())
		{
			if (en == remove)
				continue;

			for (Relationship* re : en->getIncidentRelationships())
			{
				ret = connected.insert(re->getIndex());
				if (ret.second)
					traverse.push(re->getIndex());
			}
		}
	}

	if (connected.size() == (m_relationshipList.size()-orphan_count))
		return true;
	else
		return false;
}

bool ERData::isLinear()
{
	if (m_entityList.size() < 2 || m_relationshipList.size() < 2)
		return true;

	for (Relationship* re : m_relationshipList)
	{
		for (Relationship* adj : re->getAdjacentRelationships())
		{
			if (getSharedEntityNum(re, adj) > 2)
				return false;
		}
	}

	for (Entity* en : m_entityList)
	{
		for (Entity* adj : en->getAdjacentEntities())
		{
			if (getSharedRelationshipNum(en, adj) > 2)
				return false;
		}
	}

	return true;
}

bool ERData::isSubgraphLinear(Relationship* root)
{
	std::unordered_set<Relationship*> searched;
	std::queue<Relationship*> to_search({ root });

	while (!to_search.empty())
	{
		Relationship* re = to_search.front();
		to_search.pop();

		for (Relationship* adj : re->getAdjacentRelationships())
		{
			if (getSharedEntityNum(re, adj) > 1)
				return false;
			if (searched.find(adj) == searched.end())
				to_search.push(adj);
		}
			
		searched.insert(re);
	}
	
	return true;
}

bool ERData::isSubgraphLinear(Entity* root)
{
	std::unordered_set<Entity*> searched;
	std::queue<Entity*> to_search({ root });

	while (!to_search.empty())
	{
		Entity* en = to_search.front();
		to_search.pop();

		for (Entity* adj : en->getAdjacentEntities())
		{
			if (getSharedRelationshipNum(en, adj) > 1)
				return false;
			if (searched.find(adj) == searched.end())
				to_search.push(adj);
		}

		searched.insert(en);
	}

	return true;
}

bool ERData::deleteCreatesOrphans(Relationship* remove)
{
	for (Entity* en : remove->getIncidentEntities())
	{
		if (en->getDegree() == 1)
			return true;
	}
	return false;
}

bool ERData::deleteCreatesZerogons(Entity* remove)
{
	for (Relationship* re : remove->getIncidentRelationships())
	{
		if (re->getCardinality() == 1)
			return true;
	}
	return false;
}

bool ERData::deleteBreaksEnConnection(Relationship* remove)
{
	for (int i = 0; i < remove->getCardinality() - 1; i++)
	{
		Entity* en1 = remove->getIncidentEntity(i);
		if (en1->getDegree() == 1)
			return true;

		for (int j = i + 1; j < remove->getCardinality(); j++)
		{
			Entity* en2 = remove->getIncidentEntity(j);
			if (en2->getDegree() == 1)
				return true;
			
			bool connected = checkIfTwoEntityAdjacent(en1, en2, remove);

			if (!connected)
				return true;
		}
	}

	return false;
}

bool ERData::deleteBreaksReConnection(Entity* remove)
{
	for (int i = 0; i < remove->getDegree() - 1; i++)
	{
		Relationship* re1 = remove->getIncidentRelationship(i);
		if (re1->getCardinality() == 1)
			return true;

		for (int j = 0; j < remove->getDegree(); j++)
		{
			Relationship* re2 = remove->getIncidentRelationship(j);
			if (re2->getCardinality() == 1)
				return true;

			bool connected = checkIfTwoRelationAdjacent(re1, re2, remove);

			if (!connected)
				return true;
		}
	}

	return false;
}

bool ERData::enIsBranchEnd(Entity* remove)
{
	return (remove->getAdjacentEntities().size() < 2);
}

bool ERData::reIsBranchEnd(Relationship* remove)
{
	return (remove->getAdjacentRelationships().size() < 2);
}

bool ERData::hasRemovableRelationships()
{
	for (Relationship* re : m_relationshipList)
	{
		if (!deleteBreaksEnConnection(re) && !reIsBranchEnd(re))
			return true;
	}
	
	return false;
}

bool ERData::hasRemovableEntities()
{
	for (Entity* en : m_entityList)
	{
		if (!deleteBreaksReConnection(en) && !enIsBranchEnd(en))
			return true;
	}
	
	return false;
}

//void ERData::lockCurrentVerts()
//{
//	for (Entity* en : m_entityList)
//	{
//		en->flag_Locked = true;
//	}
//}

int ERData::getEntityOpidxById(int id) const
{
	const Entity* en = getConstEntity(id);
	return en->getOpidx();
}

int ERData::getRelationshipOpidxById(int id) const
{
	const Relationship* re = getConstRelationship(id);
	return re->getOpidx();
}

bool ERData::applyRelationshipOperation(Relationship* remove, Relationship* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain)
{
	int res = isInsideList<Relationship>(remove, m_relationshipList);
	if (res == -1)
		return false;

	for (Entity* en : remove->getIncidentEntities())
	{
		std::pair<int, std::string> adj = std::make_pair(en->getOpidx(), en->getLabel());
		incidentRemove->push_back(adj);
	}

	Entity* dual_remove = remove->getDualEntity();

	if (retain)		// operation is a merge
	{
		if (remove == retain)
			return false;

		res = isInsideList<Relationship>(retain, m_relationshipList);
		if (res == -1)
			return false;

		for (Entity* en : retain->getIncidentEntities())
			incidentRetain->push_back(en->getOpidx());

		Entity* dual_retain = retain->getDualEntity();

		std::string merge_name = retain->getLabel();
		int merge_opidx = retain->getOpidx();
		Color4_F merge_color = retain->getColor();
		OperationRecord* op_remove = retain->getRemovalOp();
		vector<OperationRecord*> op_merges = retain->getMergerOps();

		REList to_merge{ remove, retain };
		ENList to_merge_dual{ dual_remove,dual_retain };
		Relationship* merged = mergeRelationships(to_merge, merge_name);
		Entity* merged_dual = NULL;
		if (m_dualERData)
			merged_dual = m_dualERData->mergeEntities(to_merge_dual, merge_name, merged);

		if (merged_dual)
		{
			merged->setDualElement(merged_dual);
			merged_dual->setOpidx(merge_opidx);
		}
		merged->setOpidx(merge_opidx);
		merged->setRemovalOp(op_remove);
		merged->setMergerOps(op_merges);
		merged->setColor(merge_color);
	}
	else	// operation is a delete
	{
		if (dual_remove)
		{
			ENList adjacent = dual_remove->getAdjacentEntities();
			m_dualERData->deleteEntity(dual_remove, false);
			m_dualERData->resetMonogonAngles(adjacent);
		}
		deleteRelationship(remove);
	}

	return true;
}

bool ERData::applyEntityOperation(Entity* remove, Entity* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain)
{
	int res = isInsideList<Entity>(remove, m_entityList);
	if (res == -1)
		return false;

	for (Relationship* re : remove->getIncidentRelationships())
	{
		std::pair<int, std::string> adj = std::make_pair(re->getOpidx(), re->getLabel());
		incidentRemove->push_back(adj);
	}

	Relationship* dual_remove = remove->getDualRelationship();

	if (retain)		// operation is a merge
	{
		if (remove == retain)
			return false;

		res = isInsideList<Entity>(retain, m_entityList);
		if (res == -1)
			return false;

		for (Relationship* re : retain->getIncidentRelationships())
			incidentRetain->push_back(re->getOpidx());

		Relationship* dual_retain = retain->getDualRelationship();

		std::string merge_name = retain->getLabel();
		int merge_opidx = retain->getOpidx();
		OperationRecord* op_remove = retain->getRemovalOp();
		vector<OperationRecord*> op_merges = retain->getMergerOps();

		ENList to_merge{ remove,retain };
		REList to_merge_dual{ dual_remove,dual_retain };
		Relationship* merged_dual = NULL;
		if (m_dualERData)
			merged_dual = m_dualERData->mergeRelationships(to_merge_dual, merge_name);
		Entity* merged = mergeEntities(to_merge, merge_name, merged_dual);

		if (merged_dual)
		{
			merged_dual->setDualElement(merged);
			merged_dual->setOpidx(merge_opidx);
		}
		merged->setOpidx(merge_opidx);
		merged->setRemovalOp(op_remove);
		merged->setMergerOps(op_merges);
		//resetMonogonAngles(vector<Entity*>{merged});
	}
	else	// operation is a delete
	{
		Relationship* dual = remove->getDualRelationship();
		if (dual)
			m_dualERData->deleteRelationship(dual);
		ENList adjacent = remove->getAdjacentEntities();
		deleteEntity(remove, false);
		resetMonogonAngles(adjacent);
	}

	return true;
}

REList ERData::invertRelationshipOperation(std::pair<int, std::string> remove, Relationship* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain, bool is_primal)
{
	Relationship* new_removed = NULL;
	Relationship* new_retained = NULL;
	Entity* dual_retained = NULL;

	if (retain)		// operation is a merge
	{
		dual_retained = retain->getDualEntity();
		ENList entities_removed;
		int removed_num = incidentRemove->size();
		for (int i = 0; i < removed_num; i++)
		{
			int opidx = incidentRemove->at(i).first;

			Entity* en = NULL;
			for (Entity* incident : retain->getIncidentEntities())
			{
				if (incident->getOpidx() == opidx)
				{
					en = incident;
					break;
				}
			}

			if (en)
				entities_removed.push_back(en);
		}

		ENList entities_retained;
		int retained_num = incidentRetain->size();
		for (int i = 0; i < retained_num; i++)
		{
			int opidx = incidentRetain->at(i);

			Entity* en = NULL;
			for (Entity* incident : retain->getIncidentEntities())
			{
				if (incident->getOpidx() == opidx)
				{
					en = incident;
					break;
				}
			}

			if (en)
				entities_retained.push_back(en);
		}

		new_removed = new Relationship(entities_removed);
		new_removed->setLabel(remove.second);
		addRelationship(new_removed);
		new_removed->setOpidx(remove.first);

		new_retained = new Relationship(entities_retained);
		new_retained->setLabel(retain->getLabel());
		addRelationship(new_retained);
		new_retained->setOpidx(retain->getOpidx());

		ENList incident = retain->getIncidentEntities();
		deleteRelationship(retain);
		new_removed->update2D();
		new_retained->update2D();
		resetMonogonAngles(incident);
	}
	else	// operation is a delete
	{
		int num_entities = incidentRemove->size();
		ENList entities(num_entities);
		//std::vector<int> retained;
		for (int i = 0; i < num_entities; i++)
		{
			Entity* en = getEntityByOpidx(incidentRemove->at(i).first);
			entities[i] = en;
		}

		/*if (retained.size() < num_entities)
		{
			if (retained.size() == 1)
			{
				int retained_idx = retained[0];
				Vector3 pos = entities[retained_idx]->getLocation();
				double len = getAverageEdgeLenth();
				double alpha = 0.0;
				double dalpha = 2.0 * M_PI / (double)num_entities;

				int idx = (retained_idx + 1) % num_entities;
				while (idx != retained_idx)
				{
					pos += (Vector3(cos(alpha), sin(alpha), 0.0) * len);
					Entity* new_en = new Entity(pos);
					addEntity(new_en);
					new_en->setLabel(incidentRemove->at(idx).second);
					new_en->setOpidx(incidentRemove->at(idx).first);
					entities[idx] = new_en;

					idx = (idx + 1) % num_entities;
					alpha += dalpha;
				}
			}
			else if (retained.size() == 2)
			{
				int r1_idx = retained[0];
				int r2_idx = retained[1];
				Vector3 r1_pos = entities[r1_idx]->getLocation();
				Vector3 r2_pos = entities[r2_idx]->getLocation();

				Vector3 v12 = r2_pos - r1_pos;
				Vector3 v21 = r1_pos - r2_pos;
				Vector3 dv12 = v12 / (double)(r2_idx-r1_idx);
				Vector3 dv21 = v21 / (double)(num_entities - r2_idx + r1_idx);
				Vector3 off12 = Vector3(v12.y, -v12.x, 0.0);
				Vector3 off21 = Vector3(v21.y, -v21.x, 0.0);
				double len = getAverageEdgeLenth() / 4.0;
				normalize(off12);
				normalize(off21);
				off12 *= len;
				off21 *= len;

				Vector3 interpolate = dv12;
				int idx = r1_idx + 1;
				while (idx != r2_idx)
				{
					Vector3 new_pos = r1_pos + interpolate + off12;
					Entity* new_en = new Entity(new_pos);
					addEntity(new_en);
					new_en->setLabel(incidentRemove->at(idx).second);
					new_en->setOpidx(incidentRemove->at(idx).first);
					entities[idx] = new_en;

					idx++;
					interpolate += dv12;
				}

				interpolate = dv21;
				idx = (r2_idx + 1) % num_entities;
				while (idx != r1_idx)
				{
					Vector3 new_pos = r2_pos + interpolate + off21;
					Entity* new_en = new Entity(new_pos);
					addEntity(new_en);
					new_en->setLabel(incidentRemove->at(idx).second);
					new_en->setOpidx(incidentRemove->at(idx).first);
					entities[idx] = new_en;

					idx = (idx + 1) % num_entities;
					interpolate += dv21;
				}
			}
			else
			{
				for (int i = 0; i < retained.size(); i++)
				{
					int r1_idx = retained[i];
					int r2_idx = (i == retained.size()-1) ? retained[0] : retained[i + 1];
					Vector3 r1_pos = entities[r1_idx]->getLocation();
					Vector3 r2_pos = entities[r2_idx]->getLocation();

					Vector3 v = r2_pos - r1_pos;
					int num_between = (i == retained.size() - 1) ? (num_entities - r1_idx + r2_idx) : (r2_idx - r1_idx);
					Vector3 dv = v / (double)(num_between);

					Vector3 interpolate = dv;
					int idx = (r1_idx + 1) % num_entities;
					while (idx != r2_idx)
					{
						Vector3 new_pos = r1_pos + interpolate;
						Entity* new_en = new Entity(new_pos);
						addEntity(new_en);
						new_en->setLabel(incidentRemove->at(idx).second);
						new_en->setOpidx(incidentRemove->at(idx).first);
						entities[idx] = new_en;

						idx = (idx + 1) % num_entities;
						interpolate += dv;
					}
				}
			}
		}*/

		new_removed = new Relationship(entities);
		new_removed->setLabel(remove.second);
		addRelationship(new_removed);
		new_removed->setOpidx(remove.first);

		if (num_entities == 1)
		{
			Vec3 adjCenter;
			for (Relationship* adj : new_removed->getAdjacentRelationships())
				adjCenter += adj->getCenter(gmainwindow->expectedLength);
			adjCenter /= (double)new_removed->getAdjacentRelationships().size();
			Vec3 dir = entities[0]->getLocation() - adjCenter;
			new_removed->setMonogonRotation(atan2(dir.y, dir.x));
		}

		new_removed->update2D();
	}

	if (m_dualERData && is_primal)
	{
		ENList dual_entities = m_dualERData->invertEntityOperation(remove, dual_retained, incidentRemove, incidentRetain, new_removed, new_retained);
		new_removed->setDualElement(dual_entities.at(0));
		if (new_retained && dual_entities.at(0))
			new_retained->setDualElement(dual_entities.at(1));
	}

	vector<Relationship*> new_relationships{ new_removed, new_retained };
	return new_relationships;
}

ENList ERData::invertEntityOperation(std::pair<int, std::string> remove, Entity* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain, Relationship* dual_remove, Relationship* dual_retain)
{
	if (m_dualERData && !dual_remove)
	{
		if (retain)
			dual_retain = retain->getDualRelationship();
		REList dual_relationships = m_dualERData->invertRelationshipOperation(remove, dual_retain, incidentRemove, incidentRetain, false);
		dual_remove = dual_relationships.at(0);
		dual_retain = dual_relationships.at(1);
	}

	Entity* new_removed = NULL;
	Entity* new_retained = NULL;
	if (retain)		// operation is a merge
	{
		new_removed = new Entity(Vec3(0.0, 0.0, 0.0));
		new_removed->setLabel(remove.second);
		addEntity(new_removed);
		new_removed->setOpidx(remove.first);

		new_retained = new Entity(Vec3(0.0, 0.0, 0.0));
		new_retained->setLabel(retain->getLabel());
		addEntity(new_retained);
		new_retained->setOpidx(retain->getOpidx());

		REList affected = retain->getIncidentRelationships();
		REList updated;

		Vec3 center_removed = Vec3(0.0, 0.0, 0.0);
		int removed_count = 0;
		Vec3 center_retained = Vec3(0.0, 0.0, 0.0);
		int retained_count = 0;
		for (Relationship* re : affected)
		{
			int idx = re->getOpidx();
			ENList entities = re->getIncidentEntities();

			bool add_removed = false;
			for (pair<int, string> adj : *incidentRemove)
			{
				if (adj.first == idx)
				{
					center_removed += re->getCenter(0.0);
					removed_count++;
					entities.push_back(new_removed);
					break;
				}
			}

			bool add_retained = false;
			for (int adj : *incidentRetain)
			{
				if (adj == idx)
				{
					center_retained += re->getCenter(0.0);
					retained_count++;
					entities.push_back(new_retained);
					break;
				}
			}

			string label = re->getLabel();
			Relationship* new_re = new Relationship(entities);
			Entity* new_dual = re->getDualEntity();
			if (new_dual)
			{
				new_re->setDualElement(new_dual);
				new_dual->setDualElement(new_re);
			}
			new_re->setLabel(label);
			addRelationship(new_re);
			new_re->setOpidx(idx);
			updated.push_back(new_re);
			deleteRelationship(re);
		}

		if (!dual_remove)
		{
			center_removed = center_removed / (double)removed_count;
			center_retained = center_retained / (double)retained_count;

			if (center_removed == center_retained)
			{
				double len = getAverageEdgeLenth() / 4.0;
				center_removed.y = center_removed.y - len;
			}
		}
		else
		{
			new_removed->setDualElement(dual_remove);
			new_retained->setDualElement(dual_retain);
			dual_remove->setDualElement(new_removed);
			dual_retain->setDualElement(new_retained);
			center_removed = dual_remove->getCenter(0.0);
			center_retained = dual_retain->getCenter(0.0);
		}

		new_removed->setLocation(center_removed);
		new_retained->setLocation(center_retained);

		deleteEntity(retain, false);
		/*for (Relationship* re : updated)
			re->update2D();*/

		resetMonogonAngles(vector<Entity*>{new_removed, new_retained});
	}
	else	// operation is a delete
	{
		// find candidates for new entity location
		vector<Vec2> locs;
		if (!dual_remove)
		{
			for (pair<int, string> inc : *incidentRemove)
			{
				Relationship* re = getRelationshipByOpidx(inc.first);
				if (re)
					getEnAddLocations(re, locs);
			}
		}

		// create new entity
		new_removed = new Entity();
		new_removed->setLabel(remove.second);
		addEntity(new_removed);
		new_removed->setOpidx(remove.first);

		REList to_update;
		for (pair<int, string> inc : *incidentRemove)
		{
			Relationship* re = getRelationshipByOpidx(inc.first);
			ENList entities;
			int opidx;
			string label;

			if (re)
			{
				entities = re->getIncidentEntities();
			}

			entities.push_back(new_removed);
			Relationship* new_re = new Relationship(entities);
			new_re->setLabel(inc.second);
			addRelationship(new_re);
			new_re->setOpidx(inc.first);
			to_update.push_back(new_re);

			Entity* new_dual = NULL;
			if (m_dualERData)
			{
				if (re)
					new_dual = re->getDualEntity();
				else if (m_dualERData)
					new_dual = m_dualERData->getEntity(new_re->getIndex());

				new_re->setDualElement(new_dual);
				new_dual->setDualElement(new_re);
			}

			if (re)
				deleteRelationship(re);
		}

		// find best location for new entity
		Vec3 new_loc;
		if (dual_remove)
		{
			new_removed->setLocation(dual_remove->getCenter(gmainwindow->expectedLength));
			dual_remove->setDualElement(new_removed);
			new_removed->setDualElement(dual_remove);
		}

		else
			findBestEnLocation(new_removed, locs);

		for (Relationship* re : to_update)
			re->update2D();
		resetMonogonAngles(vector<Entity*>{new_removed});
	}

	vector<Entity*> new_entities{ new_removed, new_retained };
	return new_entities;
}

void ERData::getEnAddLocations(Relationship* re, vector<Vec2>& locs)
{
	int card = re->getCardinality();
	int max_card = getRelationshipIncidence(re);
	Vec2 sum_loc = Vec2(0.0);
	for (Entity* en : re->getIncidentEntities())
		sum_loc += Vec2(en->getLocation());
	Vec2 center = sum_loc / (double)card;
	double rad_ideal = (0.5 * gmainwindow->expectedLength) / sin(M_PI / (double)max_card);

	// find entity angles
	std::vector<double> en_angles;
	if (card > 1)
	{
		for (Entity* en : re->getIncidentEntities())
		{
			Vec2 loc = Vec2(en->getLocation());
			Vec2 dir = loc - center;
			en_angles.push_back(dir.angle());
		}
		std::sort(en_angles.begin(), en_angles.end());
	}
	else
	{
		en_angles.push_back(re->getMonogonRotation() + M_PI);
	}

	// find candidate locations
	for (int i = 0; i < en_angles.size(); i++)
	{
		double a1 = en_angles[i];
		double a2 = en_angles[(i + 1) % en_angles.size()];
		if ((i + 1) == en_angles.size())
			a2 += (2.0 * M_PI);
		double angle = (a1 + a2) / 2.0;
		Vec2 dir = Vec2(cos(angle), sin(angle));
		locs.push_back(center + rad_ideal * dir);
	}
}

void ERData::findBestEnLocation(Entity* en, vector<Vec2>& locs)
{
	double min_energy = DBL_MAX;
	Vec2 best_loc;
	for (Vec2 loc : locs)
	{
		en->setLocation(Vec3(loc, 0.0));
		double sum_overlap = 0.0;
		double sum_regular = 0.0;

		for (Relationship* re : en->getIncidentRelationships())
		{
			if (re->getCardinality() < 2)
				continue;
			sum_regular += calc_PolygonAspectRatio(re);
			for (Relationship* adj : re->getAdjacentRelationships())
			{
				if (adj->getCardinality() < 2)
					continue;
				sum_overlap += calc_PolygonOverlap(re, adj);
			}	
		}
			
		if ((sum_overlap + sum_regular) < min_energy)
		{
			min_energy = sum_overlap + sum_regular;
			best_loc = loc;
		}
	}

	en->setLocation(Vec3(best_loc,0.0));

	//REList monogons = getMonogons();
	//double* x = new double[2];

	//// fix everything except input entity
	//fixedVertComp.clear();
	//for (int i = 0; i < m_entityList.size(); i++)
	//{
	//	if (i != en_id)
	//	{
	//		fixedVertComp.push_back(2 * i);
	//		fixedVertComp.push_back(2 * i + 1);
	//		m_entityList[i]->flag_Optimizable = false;
	//	}
	//	else
	//	{
	//		m_entityList[i]->flag_Optimizable = true;
	//	}
	//}
	//fixedMonoComp.clear();
	//for (int i = 0; i < monogons.size(); i++)
	//{
	//	fixedMonoComp.push_back(2 + i);
	//	monogons[i]->flag_Optimizable = false;
	//}

	//// set energy weights
	//gmainwindow->weight_PR = 0.1;
	//gmainwindow->weight_PA = 0.1;
	//gmainwindow->weight_PS = 0.4;
	//gmainwindow->weight_PI = 0.4;

	//// loop through location candidates
	//LayoutEnergy<double> energy;
	//double min_energy = DBL_MAX;
	//Vec2 best_loc;
	//for (Vec2 loc : locs)
	//{
	//	x[0] = loc.x;
	//	x[1] = loc.y;

	//	energy = evaluate_subgraph_energy(this, 2, 0, x, NULL);
	//	if (energy.total < min_energy)
	//	{
	//		min_energy = energy.total;
	//		best_loc = loc;
	//	}
	//}

	//// set entity location
	//m_entityList[en_id]->setLocation(Vec3(best_loc, 0.0));
	//delete[] x;
}





// Hypergraph statistic functions

void ERData::calcAllStatsLocally()
{
	int nverts = m_entityList.size();
	int nedges = m_relationshipList.size();

	// delete old memory
	if (m_entityIncidence) delete[] m_entityIncidence;
	if (m_entityAdjacency) delete[] m_entityAdjacency;
	if (m_entityAdjacencyFactor) delete[] m_entityAdjacencyFactor;
	if (m_entityBetweennessCentrality) delete[] m_entityBetweennessCentrality;
	if (m_entityEccentricity) delete[] m_entityEccentricity;
	if (m_entityClosenessCentrality) delete[] m_entityClosenessCentrality;
	if (m_entityEffectiveResistance) delete[] m_entityEffectiveResistance;

	if (m_relationshipIncidence) delete[] m_relationshipIncidence;
	if (m_relationshipAdjacency) delete[] m_relationshipAdjacency;
	if (m_relationshipAdjacencyFactor) delete[] m_relationshipAdjacencyFactor;
	if (m_relationshipBetweennessCentrality) delete[] m_relationshipBetweennessCentrality;
	if (m_relationshipEccentricity) delete[] m_relationshipEccentricity;
	if (m_relationshipClosenessCentrality) delete[] m_relationshipClosenessCentrality;
	if (m_relationshipEffectiveResistance) delete[] m_relationshipEffectiveResistance;

	if (m_incidenceMatrix)
	{
		for (int i = 0; i < nedges; i++)
			delete[] m_incidenceMatrix[i];
		delete[] m_incidenceMatrix;
	}
	if (m_entityAdjacencyMatrix)
	{
		for (int i = 0; i < nverts; i++)
			delete[] m_entityAdjacencyMatrix[i];
		delete[] m_entityAdjacencyMatrix;
	}
	if (m_relationshipAdjacencyMatrix)
	{
		for (int i = 0; i < nedges; i++)
			delete[] m_relationshipAdjacencyMatrix[i];
		delete[] m_relationshipAdjacencyMatrix;
	}

	// initialize memory (last two values are min and max)
	m_entityIncidence = new int[nverts + 2];
	m_entityAdjacency = new int[nverts + 2];
	m_entityAdjacencyFactor = new int[nverts + 2];
	m_entityBetweennessCentrality = new float[nverts + 2];
	m_entityEccentricity = new float[nverts + 2];
	m_entityClosenessCentrality = new float[nverts + 2];
	m_entityEffectiveResistance = new float[nverts + 2];

	m_relationshipIncidence = new int[nedges + 2];
	m_relationshipAdjacency = new int[nedges + 2];
	m_relationshipAdjacencyFactor = new int[nedges + 2];
	m_relationshipBetweennessCentrality = new float[nedges + 2];
	m_relationshipEccentricity = new float[nedges + 2];
	m_relationshipClosenessCentrality = new float[nedges + 2];
	m_relationshipEffectiveResistance = new float[nedges + 2];

	m_incidenceMatrix = new int* [nedges];
	for (int i = 0; i < nedges; i++)
		m_incidenceMatrix[i] = new int[nverts];

	m_entityAdjacencyMatrix = new int* [nverts];
	for (int i = 0; i < nverts; i++)
		m_entityAdjacencyMatrix[i] = new int[nverts];

	m_relationshipAdjacencyMatrix = new int* [nedges];
	for (int i = 0; i < nedges; i++)
		m_relationshipAdjacencyMatrix[i] = new int[nedges];

	// calculate statistics
	calcStatistics
	(
		m_entityIncidence,
		m_relationshipIncidence,
		m_entityAdjacency,
		m_relationshipAdjacency,
		m_entityAdjacencyFactor,
		m_relationshipAdjacencyFactor,
		true
	);
	calcCentralities
	(
		m_entityBetweennessCentrality,
		m_entityEccentricity,
		m_entityClosenessCentrality,
		m_relationshipBetweennessCentrality,
		m_relationshipEccentricity,
		m_relationshipClosenessCentrality
	);
	//calcEntityCentralities
	//(
	//	m_entityBetweennessCentrality,
	//	m_entityEccentricity,
	//	m_entityClosenessCentrality
	//);
	//calcRelationshipCentralities
	//(
	//	m_relationshipBetweennessCentrality,
	//	m_relationshipEccentricity,
	//	m_relationshipClosenessCentrality
	//);
	calcEffectiveResistances
	(
		m_entityEffectiveResistance,
		m_relationshipEffectiveResistance
	);

	// normalize betweenness centrality
	//float min = m_entityBetweennessCentrality[nverts];
	//float max = m_entityBetweennessCentrality[nverts + 1];
	//for (int i = 0; i < nverts + 2; i++)
	//{
	//	m_entityBetweennessCentrality[i] = 2*(m_entityBetweennessCentrality[i] - min) / (max - min);
	//}
	//min = m_relationshipBetweennessCentrality[nedges];
	//max = m_relationshipBetweennessCentrality[nedges + 1];
	//for (int i = 0; i < nedges + 2; i++)
	//{
	//	m_relationshipBetweennessCentrality[i] = 2*(m_relationshipBetweennessCentrality[i] - min) / (max - min);
	//}

	m_entityArraySize = nverts;
	m_relationshipArraySize = nedges;
}

// modified betweenness centrality algorithm of U. Brandes
void ERData::calcEntityCentralities(float* bc, float* ec, float* cc)
{
	int nverts = m_entityList.size();

	// initialize arrays
	for (int i = 0; i < nverts; i++)
	{
		if (bc) bc[i] = 0.0;
		if (ec) cc[i] = 0.0;
		if (cc) cc[i] = 0.0;
	}

	bc[nverts] = FLT_MAX;
	bc[nverts + 1] = -FLT_MAX;
	ec[nverts] = FLT_MAX;
	ec[nverts + 1] = -FLT_MAX;
	cc[nverts] = FLT_MAX;
	cc[nverts + 1] = -FLT_MAX;

	std::vector<int> distance(nverts, -1), sigma(nverts, 0);
	std::vector<std::vector<int>> predecessor(nverts, std::vector<int>());
	std::stack<int> found_stack;
	std::queue<int> search_queue;
	std::vector<float> delta(nverts, 0.0);

	// get entity statistics
	for (int s = 0; s < nverts; s++)
	{
		Entity* s_en = m_entityList[s];

		// check arrays
		assert(found_stack.empty());
		assert(search_queue.empty());
		sigma[s] = 1;
		distance[s] = 0;

		// do BFS and accumulate values
		search_queue.push(s);
		while (!search_queue.empty())
		{
			int v = search_queue.front();
			search_queue.pop();
			found_stack.push(v);
			Entity* v_en = m_entityList[v];

			for (Relationship* re : v_en->getIncidentRelationships())
			{		
				for (Entity* w_en : re->getIncidentEntities())
				{
					if (w_en == v_en)
						continue;

					int w = w_en->getIndex();

					// is w found for the first time?
					if (distance[w] < 0)
					{
						search_queue.push(w);
						distance[w] = distance[v] + 1;
					}

					// is the shortest path to w via v?
					if (distance[w] == (distance[v] + 1))
					{
						sigma[w] += sigma[v];
						predecessor[w].push_back(v);
					}
				}
			}
		}

		// stack holds vertices in order of decreasing distance from s
		while (!found_stack.empty())
		{
			int w = found_stack.top();
			found_stack.pop();
			for (int v : predecessor[w])
				delta[v] += ((float)sigma[v] / (float)sigma[w]) * (1.0 + delta[w]);
			if (w != s)
				if (bc) bc[w] += delta[w];
		}

		// computing closeness centrality, eccentricity, and cleaning up
		for (int i = 0; i < nverts; i++)
		{
			if (ec)
				if (distance[i] > ec[s])
					ec[s] = (float)distance[i];
			if (cc) cc[s] += (float)distance[i];

			predecessor[i].clear();
			sigma[i] = 0;
			distance[i] = -1;
			delta[i] = 0.0;
		}
	}

	// get min and max values
	for (int i = 0; i < nverts; i++)
	{
		if (bc[i] < bc[nverts]) bc[nverts] = bc[i];
		if (bc[i] > bc[nverts + 1]) bc[nverts + 1] = bc[i];
		if (cc[i] < cc[nverts]) cc[nverts] = cc[i];
		if (cc[i] > cc[nverts + 1]) cc[nverts + 1] = cc[i];
		if (ec[i] < ec[nverts]) ec[nverts] = ec[i];
		if (ec[i] > ec[nverts + 1]) ec[nverts + 1] = ec[i];
	}
}

// modified betweenness centrality algorithm of U. Brandes
void ERData::calcRelationshipCentralities(float* bc, float* ec, float* cc)
{
	int nedges = m_relationshipList.size();

	// initialize memory
	for (int i = 0; i < nedges; i++)
	{
		if (bc) bc[i] = 0.0;
		if (ec) ec[i] = 0.0;
		if (cc) cc[i] = 0.0;
	}

	bc[nedges] = FLT_MAX;
	bc[nedges + 1] = -FLT_MAX;
	ec[nedges] = FLT_MAX;
	ec[nedges + 1] = -FLT_MAX;
	cc[nedges] = FLT_MAX;
	cc[nedges + 1] = -FLT_MAX;

	std::vector<int> distance(nedges, -1), sigma(nedges, 0);
	std::vector<std::vector<int>> predecessor(nedges, std::vector<int>());
	std::stack<int> found_stack;
	std::queue<int> search_queue;
	std::vector<float> delta(nedges, 0.0);

	// get relationship statistics
	for (int s = 0; s < nedges; s++)
	{
		Relationship* s_re = m_relationshipList[s];

		// initialize arrays
		assert(found_stack.empty());
		assert(search_queue.empty());
		sigma[s] = 1;
		distance[s] = 0;

		// do BFS and accumulate values
		search_queue.push(s);
		while (!search_queue.empty())
		{
			int v = search_queue.front();
			search_queue.pop();
			found_stack.push(v);
			Relationship* v_re = m_relationshipList[v];

			for (Entity* en : v_re->getIncidentEntities())
			{
				for (Relationship* w_re : en->getIncidentRelationships())
				{
					if (w_re == v_re)
						continue;

					int w = w_re->getIndex();

					// is w found for the first time?
					if (distance[w] < 0)
					{
						search_queue.push(w);
						distance[w] = distance[v] + 1;
					}

					// is the shortest path to w via v?
					if (distance[w] == (distance[v] + 1))
					{
						sigma[w] += sigma[v];
						predecessor[w].push_back(v);
					}
				}
			}
		}

		// stack holds vertices in order of decreasing distance from s
		while (!found_stack.empty())
		{
			int w = found_stack.top();
			found_stack.pop();
			for (int v : predecessor[w])
				delta[v] += ((float)sigma[v] / (float)sigma[w]) * (1 + delta[w]);
			if (w != s)
				if (bc) bc[w] += delta[w];
		}

		// computing closeness centrality, eccentricity, and cleaning up
		for (int i = 0; i < nedges; i++)
		{
			if (ec)
				if (distance[i] > ec[s])
					ec[s] = distance[i];
			if (cc) cc[s] += distance[i];

			predecessor[i].clear();
			sigma[i] = 0;
			distance[i] = -1;
			delta[i] = 0.0;
		}
	}

	// get min and max values
	for (int i = 0; i < nedges; i++)
	{
		if (bc[i] < bc[nedges]) bc[nedges] = bc[i];
		if (bc[i] > bc[nedges + 1]) bc[nedges + 1] = bc[i];
		if (cc[i] < cc[nedges]) cc[nedges] = cc[i];
		if (cc[i] > cc[nedges + 1]) cc[nedges + 1] = cc[i];
		if (ec[i] < ec[nedges]) ec[nedges] = ec[i];
		if (ec[i] > ec[nedges + 1]) ec[nedges + 1] = ec[i];
	}
}

// modified betweenness centrality algorithm of U. Brandes
void ERData::calcCentralities(float* en_bc, float* en_ec, float* en_cc, float* re_bc, float* re_ec, float* re_cc)
{
	int nverts = m_entityList.size();
	int nedges = m_relationshipList.size();

	// initialize memory
	for (int i = 0; i < nverts; i++)
	{
		if (en_bc) en_bc[i] = 0.0;
		if (en_ec) en_ec[i] = 0.0;
		if (en_cc) en_cc[i] = 0.0;

	}

	for (int i = 0; i < nedges; i++)
	{
		if (re_bc) re_bc[i] = 0.0;
		if (en_ec) re_ec[i] = 0.0;
		if (en_cc) re_cc[i] = 0.0;
	}

	en_bc[nverts] = FLT_MAX;
	en_bc[nverts + 1] = -FLT_MAX;
	en_ec[nverts] = FLT_MAX;
	en_ec[nverts + 1] = -FLT_MAX;
	en_cc[nverts] = FLT_MAX;
	en_cc[nverts + 1] = -FLT_MAX;
	re_bc[nedges] = FLT_MAX;
	re_bc[nedges + 1] = -FLT_MAX;
	re_ec[nedges] = FLT_MAX;
	re_ec[nedges + 1] = -FLT_MAX;
	re_cc[nedges] = FLT_MAX;
	re_cc[nedges + 1] = -FLT_MAX;

	std::vector<int> distance(nverts + nedges, -1), sigma(nverts + nedges, 0);
	std::vector<std::vector<int>> predecessor(nverts + nedges, std::vector<int>());
	std::stack<int> found_stack;
	std::queue<int> search_queue;
	std::vector<float> delta(nverts + nedges, 0.0);

	// get entity statistics
	for (int s = 0; s < nverts + nedges; s++)
	{
		// check arrays
		assert(found_stack.empty());
		assert(search_queue.empty());
		for (int i = 0; i < nverts + nedges; i++)
		{
			predecessor[i].clear();
			sigma[i] = 0;
			distance[i] = -1;
			delta[i] = 0.0;
		}
		sigma[s] = 1;
		distance[s] = 0;

		// do BFS and accumulate values
		search_queue.push(s);
		while (!search_queue.empty())
		{
			int v = search_queue.front();
			search_queue.pop();
			found_stack.push(v);
			std::vector<int> incident_ids;
			if (v < nverts)
				m_entityList[v]->getIncidentRelationshipIds(incident_ids, nverts);
			else
				m_relationshipList[v - nverts]->getIncidentEntityIds(incident_ids);
			
			for (int w : incident_ids)
			{
				// is w found for the first time?
				if (distance[w] < 0)
				{
					search_queue.push(w);
					distance[w] = distance[v] + 1;
				}

				// is the shortest path to w via v
				if (distance[w] == (distance[v] + 1))
				{
					sigma[w] += sigma[v];
					predecessor[w].push_back(v);
				}
			}
		}


		// stack holds elements in order of decreasing distance from s
		while (!found_stack.empty())
		{
			int w = found_stack.top();
			found_stack.pop();
			for (int v : predecessor[w])
				delta[v] += ((float)sigma[v] / (float)sigma[w]) * (1.0 + delta[w]);
			if (w != s)
			{
				if (w < nverts)
				{
					if (en_bc) 
						en_bc[w] += delta[w];
				}
				else
				{
					if (re_bc) 
						re_bc[w - nverts] += delta[w];
				}
			}
		}

		// computing closeness centrality, eccentricity, and cleaning up
		for (int i = 0; i < nverts + nedges; i++)
		{
			if (s < nverts)
			{
				if (en_ec)
				{
					if (distance[i] > en_ec[s])
						en_ec[s] = (float)distance[i];
				}
				if (en_cc) 
					en_cc[s] += (float)distance[i];
			}
			else
			{
				if (re_ec)
				{
					if (distance[i] > re_ec[s - nverts])
						re_ec[s - nverts] = (float)distance[i];
				}
				if (re_cc) 
					re_cc[s - nverts] += (float)distance[i];
			}
		}
	}

	// get min and max values
	for (int i = 0; i < nverts; i++)
	{
		if (en_bc[i] < en_bc[nverts]) en_bc[nverts] = en_bc[i];
		if (en_bc[i] > en_bc[nverts + 1]) en_bc[nverts + 1] = en_bc[i];
		if (en_cc[i] < en_cc[nverts]) en_cc[nverts] = en_cc[i];
		if (en_cc[i] > en_cc[nverts + 1]) en_cc[nverts + 1] = en_cc[i];
		if (en_ec[i] < en_ec[nverts]) en_ec[nverts] = en_ec[i];
		if (en_ec[i] > en_ec[nverts + 1]) en_ec[nverts + 1] = en_ec[i];
	}
	for (int i = 0; i < nedges; i++)
	{
		if (re_bc[i] < re_bc[nedges]) re_bc[nedges] = re_bc[i];
		if (re_bc[i] > re_bc[nedges + 1]) re_bc[nedges + 1] = re_bc[i];
		if (re_cc[i] < re_cc[nedges]) re_cc[nedges] = re_cc[i];
		if (re_cc[i] > re_cc[nedges + 1]) re_cc[nedges + 1] = re_cc[i];
		if (re_ec[i] < re_ec[nedges]) re_ec[nedges] = re_ec[i];
		if (re_ec[i] > re_ec[nedges + 1]) re_ec[nedges + 1] = re_ec[i];
	}
}

void ERData::calcStatistics(int* en_in, int* re_in, int* en_ad, int* re_ad, int* en_af, int* re_af, bool local)
{
	int nverts = m_entityList.size();
	int nedges = m_relationshipList.size();

	// initialize memory
	en_in[nverts] = INT_MAX;
	en_in[nverts + 1] = -INT_MAX;
	en_ad[nverts] = INT_MAX;
	en_ad[nverts + 1] = -INT_MAX;
	en_af[nverts] = INT_MAX;
	en_af[nverts + 1] = -INT_MAX;
	re_in[nedges] = INT_MAX;
	re_in[nedges + 1] = -INT_MAX;
	re_ad[nedges] = INT_MAX;
	re_ad[nedges + 1] = -INT_MAX;
	re_af[nedges] = INT_MAX;
	re_af[nedges + 1] = -INT_MAX;

	if (local)
	{
		for (int i = 0; i < nedges; i++)
		{
			for (int j = 0; j < nverts; j++)
				m_incidenceMatrix[i][j] = 0;
			for (int k = 0; k < nedges; k++)
				m_relationshipAdjacencyMatrix[i][k] = 0;
		}
		for (int i = 0; i < nverts; i++)
		{
			for (int j = 0; j < nverts; j++)
				m_entityAdjacencyMatrix[i][j] = 0;
		}
	}


	// calculate statistics
	for (int i = 0; i < nverts; i++)
	{
		Entity* en = m_entityList[i];

		en_in[i] = en->getDegree();
		en_ad[i] = en->getCardinality();

		int adj_factor = 0;
		for (Entity* adj : en->getAdjacentEntities())
		{
			adj_factor += (int)std::pow(getSharedRelationshipNum(en, adj),gmainwindow->share_power);
			int j = adj->getIndex();
			if (local)
				m_entityAdjacencyMatrix[i][j] = 1;
		}
		en_af[i] = adj_factor;

		if (en_in[i] < en_in[nverts]) en_in[nverts] = en_in[i];
		if (en_in[i] > en_in[nverts + 1]) en_in[nverts + 1] = en_in[i];
		if (en_ad[i] < en_ad[nverts]) en_ad[nverts] = en_ad[i];
		if (en_ad[i] > en_ad[nverts + 1]) en_ad[nverts + 1] = en_ad[i];
		if (en_af[i] < en_af[nverts]) en_af[nverts] = en_af[i];
		if (en_af[i] > en_af[nverts + 1]) en_af[nverts + 1] = en_af[i];
	}

	for (int i = 0; i < nedges; i++)
	{
		Relationship* re = m_relationshipList[i];

		re_in[i] = re->getCardinality();
		re_ad[i] = re->getDegree();

		int adj_factor = 0;
		for (Relationship* adj : re->getAdjacentRelationships())
		{
			adj_factor += (int)std::pow(getSharedEntityNum(re, adj),gmainwindow->share_power);
			int j = adj->getIndex();
			if (local)
				m_relationshipAdjacencyMatrix[i][j] = 1;
		}
		re_af[i] = adj_factor;

		if (re_in[i] < re_in[nedges]) re_in[nedges] = re_in[i];
		if (re_in[i] > re_in[nedges + 1]) re_in[nedges + 1] = re_in[i];
		if (re_ad[i] < re_ad[nedges]) re_ad[nedges] = re_ad[i];
		if (re_ad[i] > re_ad[nedges + 1]) re_ad[nedges + 1] = re_ad[i];
		if (re_af[i] < re_af[nedges]) re_af[nedges] = re_af[i];
		if (re_af[i] > re_af[nedges + 1]) re_af[nedges + 1] = re_af[i];

		for (Entity* en : re->getIncidentEntities())
		{
			int j = en->getIndex();
			if (local)
				m_incidenceMatrix[i][j] = 1;
		}
	}
}

void ERData::calcEffectiveResistances(float* en_ef, float* re_ef)
{
	// TODO: implement effective resistance for hypergraphs here.
	// for now I am just using vertex and edge weights

	int nverts = m_entityList.size();
	int nedges = m_relationshipList.size();

	// initialize memory
	en_ef[nverts] = FLT_MAX;
	en_ef[nverts + 1] = -FLT_MAX;
	re_ef[nedges] = FLT_MAX;
	re_ef[nedges + 1] = -FLT_MAX;

	for (int i = 0; i < nverts; i++)
	{
		en_ef[i] = 1.0; //m_entityList[i]->getWeight();
		if (en_ef[i] < en_ef[nverts]) en_ef[nverts] = en_ef[i];
		if (en_ef[i] > en_ef[nverts + 1]) en_ef[nverts + 1] = en_ef[i];
	}

	for (int i = 0; i < nedges; i++)
	{
		re_ef[i] = 1.0; // m_relationshipList[i]->getWeight();
		if (re_ef[i] < re_ef[nedges]) re_ef[nedges] = re_ef[i];
		if (re_ef[i] > re_ef[nedges + 1]) re_ef[nedges + 1] = re_ef[i];
	}
}


int ERData::getEntityIncidence(Entity* en) const
{
	if (m_entityIncidence)
		return m_entityIncidence[en->getOpidx()];
	else
		return en->getDegree();
}

int ERData::getEntityIncidence(const Entity* en) const
{
	if (m_entityIncidence)
		return m_entityIncidence[en->getOpidx()];
	else
		return en->getDegree();
}

int ERData::getEntityAdjacency(Entity* en) const
{
	if (m_entityAdjacency)
		return m_entityAdjacency[en->getOpidx()];
	else
		return en->getCardinality();
}

int ERData::getEntityAdjacencyFactor(Entity* en) const
{
	if (m_entityAdjacencyFactor)
		return m_entityAdjacencyFactor[en->getOpidx()];
	else
		return 0;
}

float ERData::getEntityBetweennessCentrality(Entity* en) const
{	
	if (m_entityBetweennessCentrality)
		return 0.5 * m_entityBetweennessCentrality[en->getOpidx()];
	else 
		return 0;
}

float ERData::getEntityEccentricity(Entity* en) const
{
	if (m_entityEccentricity)
		return 0.5 * m_entityEccentricity[en->getOpidx()];
	else
		return 0;
}

float ERData::getEntityClosenessCentrality(Entity* en) const
{
	if (m_entityClosenessCentrality)
		return 0.5 * m_entityClosenessCentrality[en->getOpidx()];
	else
		return 0;
}

float ERData::getEntityEffectiveResistance(Entity* en) const
{
	if (m_entityEffectiveResistance)
		return m_entityEffectiveResistance[en->getOpidx()];
	else
		return 0;
}


int ERData::getRelationshipIncidence(Relationship* re) const
{
	if (m_relationshipIncidence)
		return m_relationshipIncidence[re->getOpidx()];
	else
		return re->getCardinality();
}

int ERData::getRelationshipIncidence(const Relationship* re) const
{
	if (m_relationshipIncidence)
		return m_relationshipIncidence[re->getOpidx()];
	else
		return re->getCardinality();
}

int ERData::getRelationshipAdjacency(Relationship* re) const
{
	if (m_relationshipAdjacency)
		return m_relationshipAdjacency[re->getOpidx()];
	else
		return re->getDegree();
}

int ERData::getRelationshipAdjacencyFactor(Relationship* re) const
{
	if (m_relationshipAdjacencyFactor)
		return m_relationshipAdjacencyFactor[re->getOpidx()];
	else
		return 0;
}

float ERData::getRelationshipBetweennessCentrality(Relationship* re) const
{
	if (m_relationshipBetweennessCentrality)
		return 0.5 * m_relationshipBetweennessCentrality[re->getOpidx()];
	else
		return 0;
}

float ERData::getRelationshipEccentricity(Relationship* re) const
{
	if (m_relationshipEccentricity)
		return 0.5 * m_relationshipEccentricity[re->getOpidx()];
	else
		return 0;
}

float ERData::getRelationshipClosenessCentrality(Relationship* re) const
{
	if (m_relationshipClosenessCentrality)
		return 0.5 * m_relationshipClosenessCentrality[re->getOpidx()];
	else
		return 0;
}

float ERData::getRelationshipEffectiveResistance(Relationship* re) const
{
	if (m_relationshipEffectiveResistance)
		return m_relationshipEffectiveResistance[re->getOpidx()];
	else
		return 0;
}


int ERData::getMinIncidence() const
{
	if (m_entityIncidence && m_relationshipIncidence)
		return std::min(m_entityIncidence[m_entityArraySize], m_relationshipIncidence[m_relationshipArraySize]);
	else
		return 0;
}

int ERData::getMaxIncidence() const
{
	if (m_entityIncidence && m_relationshipIncidence)
		return std::max(m_entityIncidence[m_entityArraySize + 1], m_relationshipIncidence[m_relationshipArraySize + 1]);
	else
		return 0;
}

int ERData::getMinAdjacency() const
{
	if (m_entityAdjacency && m_relationshipAdjacency)
		return std::min(m_entityAdjacency[m_entityArraySize], m_relationshipAdjacency[m_relationshipArraySize]);
	else
		return 0;
}

int ERData::getMaxAdjacency() const
{
	if (m_entityAdjacency && m_relationshipAdjacency)
		return std::max(m_entityAdjacency[m_entityArraySize + 1], m_relationshipAdjacency[m_relationshipArraySize + 1]);
	else
		return 0;
}

int ERData::getMinAdjacencyFactor() const
{
	if (m_entityAdjacencyFactor && m_relationshipAdjacencyFactor)
		return std::min(m_entityAdjacencyFactor[m_entityArraySize], m_relationshipAdjacencyFactor[m_relationshipArraySize]);
	else
		return 0;
}

int ERData::getMaxAdjacencyFactor() const
{
	if (m_entityAdjacencyFactor && m_relationshipAdjacencyFactor)
		return std::max(m_entityAdjacencyFactor[m_entityArraySize + 1], m_relationshipAdjacencyFactor[m_relationshipArraySize + 1]);
	else
		return 0;
}

float ERData::getMinBetweennessCentrality() const
{
	if (m_entityBetweennessCentrality && m_relationshipBetweennessCentrality)
		return std::min(m_entityBetweennessCentrality[m_entityArraySize], m_relationshipBetweennessCentrality[m_relationshipArraySize]);
	else
		return 0;
}

float ERData::getMaxBetweennessCentrality() const
{
	if (m_entityBetweennessCentrality && m_relationshipBetweennessCentrality)
		return std::max(m_entityBetweennessCentrality[m_entityArraySize + 1], m_relationshipBetweennessCentrality[m_relationshipArraySize + 1]);
	else
		return 0;
}

float ERData::getMinEccentricity() const
{
	if (m_entityEccentricity && m_relationshipEccentricity)
		return std::min(m_entityEccentricity[m_entityArraySize], m_relationshipEccentricity[m_relationshipArraySize]);
	else
		return 0;
}

float ERData::getMaxEccentricity() const
{
	if (m_entityEccentricity && m_relationshipEccentricity)
		return std::max(m_entityEccentricity[m_entityArraySize + 1], m_relationshipEccentricity[m_relationshipArraySize + 1]);
	else
		return 0;
}

float ERData::getMinClosenessCentrality() const
{
	if (m_entityClosenessCentrality && m_relationshipClosenessCentrality)
		return std::min(m_entityClosenessCentrality[m_entityArraySize], m_relationshipClosenessCentrality[m_relationshipArraySize]);
	else
		return 0;
}

float ERData::getMaxClosenessCentrality() const
{
	if (m_entityClosenessCentrality && m_relationshipClosenessCentrality)
		return std::max(m_entityClosenessCentrality[m_entityArraySize + 1], m_relationshipClosenessCentrality[m_relationshipArraySize + 1]);
	else
		return 0;
}

float ERData::getMinEffectiveResistance() const
{
	if (m_entityEffectiveResistance && m_relationshipEffectiveResistance)
		return std::min(m_entityEffectiveResistance[m_entityArraySize], m_relationshipEffectiveResistance[m_relationshipArraySize]);
	else
		return 0;
}

float ERData::getMaxEffectiveResistance() const
{
	if (m_entityEffectiveResistance && m_relationshipEffectiveResistance)
		return std::max(m_entityEffectiveResistance[m_entityArraySize + 1], m_relationshipEffectiveResistance[m_relationshipArraySize + 1]);
	else
		return 0;
}


bool ERData::areEntitiesAdjacent(const Entity* eni, const Entity* enj) const
{
	if (m_entityAdjacencyMatrix)
		return (m_entityAdjacencyMatrix[eni->getOpidx()][enj->getOpidx()] == 1);
	else
	{
		for (Relationship* re : eni->getIncidentRelationships())
		{
			int inside = isInsideList<Entity>(enj, re->getIncidentEntities());
			if (inside != -1)
				return true;
		}
		return false;
	}
}

bool ERData::areRelationshipsAdjacent(const Relationship* rei, const Relationship* rej) const
{
	if (m_relationshipAdjacencyMatrix)
		return (m_relationshipAdjacencyMatrix[rei->getOpidx()][rej->getOpidx()] == 1);
	else
	{
		for (Entity* en : rei->getIncidentEntities())
		{
			int inside = isInsideList<Relationship>(rej, en->getIncidentRelationships());
			if (inside != -1)
				return true;
		}
		return false;
	}
}

bool ERData::areEntityRelationshipIncident(const Entity* en, const Relationship* re) const
{
	if (m_incidenceMatrix)
		return (m_incidenceMatrix[re->getOpidx()][en->getOpidx()] == 1);
	else
	{
		int inside = isInsideList<Entity>(en, re->getIncidentEntities());
		if (inside != -1)
			return true;
		return false;
	}
}


void ERData::findCycleElementsPeel(int* enlist, int* relist)
{
	for (int i = 0; i < m_entityList.size(); i++)
		enlist[i] = 1;
	for (int i = 0; i < m_relationshipList.size(); i++)
		relist[i] = 1;

	// iteratively peel off branch ends
	int removed = 1;
	while (removed > 0)
	{
		removed = 0;

		for (Entity* en : m_entityList)
		{
			if (enlist[en->getIndex()] == 0)
				continue;

			ENList tmpAdjList = en->getAdjacentEntities();
			REList tmpIncList = en->getIncidentRelationships();

			// find adjacent entities that have not been peeled
			ENList adjList;
			for (Entity* adj : tmpAdjList)
			{
				if (enlist[adj->getIndex()] == 1)
					adjList.push_back(adj);
			}

			// find incident relationships that have not been peeled
			REList incList;
			for (Relationship* inc : tmpIncList)
			{
				if (relist[inc->getIndex()] == 1)
					incList.push_back(inc);
			}

			// peel if only one adjacenet or incident element
			if (adjList.size() < 2 || incList.size() < 2)
			{
				enlist[en->getIndex()] = 0;
				removed++;
				continue;
			}

			// if all ajacent entities are through one relationship, peel
			for (Relationship* inc : incList)
			{
				ENList incIncList = inc->getIncidentEntities();
				int degree = 0;
				for (Entity* incInc : incIncList)
				{
					if (enlist[incInc->getIndex()] == 1)
						degree++;
				}

				if (degree > adjList.size())
				{
					enlist[en->getIndex()] = 0;
					removed++;
					break;
				}		
			}
		}

		for (Relationship* re : m_relationshipList)
		{
			if (relist[re->getIndex()] == 0)
				continue;

			REList tmpAdjList = re->getAdjacentRelationships();
			ENList tmpIncList = re->getIncidentEntities();

			// find adjacent relationships that have not been peeled
			REList adjList;
			for (Relationship* adj : tmpAdjList)
			{
				if (relist[adj->getIndex()] == 1)
					adjList.push_back(adj);
			}

			// find incident entities that have not been peeled
			ENList incList;
			for (Entity* inc : tmpIncList)
			{
				if (enlist[inc->getIndex()] == 1)
					incList.push_back(inc);
			}

			// peel if only one adjacenet or incident element
			if (adjList.size() < 2 || incList.size() < 2)
			{
				relist[re->getIndex()] = 0;
				removed++;
				continue;
			}

			// if all adjacent relationships are through one entity, peel
			for (Entity* inc : incList)
			{
				REList incIncList = inc->getIncidentRelationships();
				int cardinality = 0;
				for (Relationship* incInc : incIncList)
				{
					if (relist[incInc->getIndex()] == 1)
						cardinality++;
				}

				if (cardinality > adjList.size())
				{
					relist[re->getIndex()] = 0;
					removed++;
					break;
				}
			}
		}
	}

	// add some peeled elements back in
	//vector<int> cycleAdjEns;
	//vector<int> cycleAdjRes;
	//for (Relationship* re : m_relationshipList)
	//{
	//	if (relist[re->getIndex()])
	//		continue;

	//	// get incident entities on a cycle
	//	int IncidentEntitiesOnCycles = 0;
	//	for (Entity* en : re->getIncidentEntities())
	//	{
	//		if (enlist[en->getIndex()])
	//			IncidentEntitiesOnCycles++;
	//	}
	//	
	//	// all incident entities on cycles
	//	if (IncidentEntitiesOnCycles == re->getCardinality())
	//		cycleAdjRes.push_back(re->getIndex());
	//}
	//for (Entity* en : m_entityList)
	//{
	//	if (enlist[en->getIndex()])
	//		continue;

	//	// get incident relationships on a cycle
	//	int incidentRelationshipsOnCycles = 0;
	//	for (Relationship* re : en->getIncidentRelationships())
	//	{
	//		if (relist[re->getIndex()])
	//			incidentRelationshipsOnCycles++;
	//	}
	//	
	//	// all incident relationships on cycles
	//	if (incidentRelationshipsOnCycles == en->getDegree())
	//		cycleAdjEns.push_back(en->getIndex());
	//}

	//for (int idx : cycleAdjEns)
	//	enlist[idx] = 1;
	//for (int idx : cycleAdjRes)
	//	relist[idx] = 1;
}

void ERData::findCycleElementsDFS(int* enlist, int* relist)
{
	for (int i = 0; i < m_entityList.size(); i++)
		enlist[i] = 0;
	for (int i = 0; i < m_relationshipList.size(); i++)
		relist[i] = 0;

	// use the Konig graph for DFS
	ERData* kgraph = this->getBipartiteGraph();
	int knodes = kgraph->getEntityNum();
	int* visited = new int[knodes];
	for (int i = 0; i < knodes; i++)
		visited[i] = 0;

	// take first index node as starting point
	stack<Entity*> tovisit;
	tovisit.push(kgraph->getEntity(0));
	kgraph->getEntity(0)->setOpidx(0);
	int count = 0;

	// depth first search on konig graph
	while (!tovisit.empty())
	{
		// pop nex vertex
		Entity* en = tovisit.top();
		tovisit.pop();
		if (visited[en->getIndex()])
			continue;
		visited[en->getIndex()] = 1;

		for (Entity* adj : en->getAdjacentEntities())
		{
			if (!visited[adj->getIndex()])
			{
				tovisit.push(adj);
				adj->setOpidx(en->getIndex()); // use as parent pointer
			}
			else if (adj->getIndex() != en->getOpidx())
			{
				// there is a non-trivial cycle
				count++;
				int idx = adj->getIndex();
				if (idx < m_entityList.size())
					enlist[idx] = 1;
				else
					relist[idx - m_entityList.size()] = 1;

				Entity* path = en;
				while (path->getIndex() != adj->getIndex())
				{
					idx = path->getIndex();
					if (idx < m_entityList.size())
						enlist[idx] = 1;
					else
						relist[idx - m_entityList.size()] = 1;
					path = kgraph->getEntity(path->getOpidx());
				} 
			}
		}
	}
	

	// Count edgely adjacent polygons and remove from cycle count
	int trivial = 0;
	for (Entity* en : m_entityList)
	{
		for (Entity* adj : en->getAdjacentEntities())
		{
			if (adj->getIndex() < en->getIndex())
				continue;
			trivial += (getSharedRelationshipNum(en, adj) - 1);
			//count = count - shared + 1;
		}
	}
	LogConsole::writeConsole("cycles: %d, trivial: %d", count, trivial);

	// check if hypergraph is connected
	int unvisited = 0;
	for (int i = 0; i < knodes; i++)
	{
		if (!visited[i])
			unvisited++;
	}
	LogConsole::writeConsole("Disconnected Elements: %d",unvisited);

	delete[] visited;
	delete kgraph;
}


// Cluster detection functions

void ERData::constructIncidenceMatrix()
{
	int encount = m_entityList.size();
	int recount = m_relationshipList.size();

	std::vector<Eigen::Triplet<int>> incidentList;
	incidentList.reserve(encount * 2);

	for (Entity* en : m_entityList)
	{
		for (Relationship* re : en->getIncidentRelationships())
		{
			incidentList.push_back(Eigen::Triplet<int>(re->getIndex(), en->getIndex(), 1));
		}
	}

	m_IncidenceMatrix = Eigen::SparseMatrix<int>(recount, encount);
	m_IncidenceMatrix.setFromTriplets(incidentList.begin(), incidentList.end());
}

Eigen::VectorXd ERData::getIncidenceEigenvals()
{
	constructIncidenceMatrix();
	Eigen::MatrixXd im = m_IncidenceMatrix.toDense().cast<double>();
	Eigen::MatrixXd sq = im * im.transpose();
	Eigen::EigenSolver<Eigen::MatrixXd> es(sq);

	Eigen::VectorXd evals = es.eigenvalues().real();
	std::sort(evals.data(), evals.data() + evals.size(), std::greater<double>());
	evals = (1.0e-8 < evals.array().abs()).select(evals, 0.0);

	return evals;
}

void ERData::constructEnAdjMatrix()
{
	int encount = m_entityList.size();
	std::vector<Eigen::Triplet<int>> adjList;
	adjList.reserve(encount);

	for (Entity* e1 : m_entityList)
	{
		int idx = e1->getIndex();
		for (Entity* e2 : e1->getAdjacentEntities())
		{
			int adj = e2->getIndex();
			adjList.push_back(Eigen::Triplet<int>(idx, adj, 1));
		}
	}

	m_enAdjMatrix = Eigen::SparseMatrix<int>(encount, encount);
	m_enAdjMatrix.setFromTriplets(adjList.begin(), adjList.end());
}

Eigen::VectorXd ERData::getEnAdjEigenvals()
{
	constructEnAdjMatrix();
	Eigen::MatrixXd am = m_enAdjMatrix.toDense().cast<double>();
	Eigen::MatrixXd sq = am * am.transpose();
	Eigen::EigenSolver<Eigen::MatrixXd> es(sq);

	Eigen::VectorXd evals = es.eigenvalues().real();
	std::sort(evals.data(), evals.data() + evals.size(), std::greater<double>());
	evals = (1.0e-8 < evals.array().abs()).select(evals, 0.0);

	return evals;
}
	
void ERData::constructReAdjMatrix()
{
	int recount = m_relationshipList.size();
	std::vector<Eigen::Triplet<int>> adjList;
	adjList.reserve(recount);

	for (Relationship* r1 : m_relationshipList)
	{
		int idx = r1->getIndex();
		for (Relationship* r2 : r1->getAdjacentRelationships())
		{
			int adj = r2->getIndex();
			adjList.push_back(Eigen::Triplet<int>(idx, adj, 1));
		}
	}

	m_reAdjMatrix = Eigen::SparseMatrix<int>(recount, recount);
	m_reAdjMatrix.setFromTriplets(adjList.begin(), adjList.end());
}

Eigen::VectorXd ERData::getReAdjEigenvals()
{
	constructReAdjMatrix();
	Eigen::MatrixXd am = m_reAdjMatrix.toDense().cast<double>();
	Eigen::MatrixXd sq = am * am.transpose();
	Eigen::EigenSolver<Eigen::MatrixXd> es(sq);

	Eigen::VectorXd evals = es.eigenvalues().real();
	std::sort(evals.data(), evals.data() + evals.size(), std::greater<double>());
	evals = (1.0e-8 < evals.array().abs()).select(evals, 0.0);

	return evals;
}

int ERData::countNAdjacentClusters()
{
	int count = 0;
	for (int i = 0; i < m_relationshipList.size(); i++)
	{
		for (int j = i + 1; j < m_relationshipList.size(); j++)
		{
			Relationship* rei = m_relationshipList[i];
			Relationship* rej = m_relationshipList[j];
			if (getSharedEntityNum(rei, rej) > 2)
				count++;
		}
	}
	for (int i = 0; i < m_entityList.size(); i++)
	{
		for (int j = i + 1; j < m_entityList.size(); j++)
		{
			Entity* eni = m_entityList[i];
			Entity* enj = m_entityList[j];
			if (getSharedRelationshipNum(eni, enj) > 2)
				count++;
		}
	}
	return count;
}


// Deterministic Layout Algorithms

bool ERData::constructKonigGraph()
{	
	int numens = m_entityList.size();
	int numres = m_relationshipList.size();
	if (m_KonigGraph)
		delete m_KonigGraph;

	m_KonigGraph = new ogdf::Graph();
	m_KonigGraphLayout = new ogdf::GraphAttributes(*m_KonigGraph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
	vector<ogdf::node> nodes;

	for (Entity* en : m_entityList)
	{
		Vec3 loc = en->getLocation();
		ogdf::node v = m_KonigGraph->newNode();
		m_KonigGraphLayout->x(v) = loc.x;
		m_KonigGraphLayout->y(v) = loc.y;
		nodes.push_back(v);
	}
	for (Relationship* re : m_relationshipList)
	{
		Vec3 cen = re->getCenter(re->getMonogonLength());
		//if (re->getCardinality() == 1)
		//{
		//	double angle = re->getMonogonRotation();
		//	double len = re->getMonogonLength();
		//	cen += (len * Vec3(cos(angle), sin(angle)));
		//}
		ogdf::node v = m_KonigGraph->newNode();
		m_KonigGraphLayout->x(v) = cen.x;
		m_KonigGraphLayout->y(v) = cen.y;
		nodes.push_back(v);
	}	
	
	for (Relationship* re : m_relationshipList)
	{
		int re_idx = re->getIndex() + numens;
		vector<Entity*> ens = re->getIncidentEntities();
		for (int i = 0; i < ens.size(); i++)
		{
			Entity* en = ens[i];
			int en_idx = en->getIndex();
			m_KonigGraph->newEdge(nodes[re_idx], nodes[en_idx]);
		}
		//if (ens.size() == 1)
		//{
		//	Entity* en = ens[0];
		//	int en_idx = en->getIndex();
		//	m_KonigGraph->newEdge(nodes[re_idx], nodes[en_idx]);
		//}
		//else if (ens.size() == 2)
		//{
		//	Entity* en1 = ens[0];
		//	Entity* en2 = ens[1];
		//	int en1_idx = en1->getIndex();
		//	int en2_idx = en2->getIndex();
		//	ogdf::edge side = m_KonigGraph->searchEdge(nodes[en1_idx], nodes[en2_idx], false);
		//	if (!side)
		//		m_KonigGraph->newEdge(nodes[en1_idx], nodes[en2_idx]);
		//	m_KonigGraph->delNode(nodes[re_idx]);

		//}
		//else
		//{
		//	for (int i = 0; i < ens.size(); i++)
		//	{
		//		Entity* en1 = ens[i];
		//		Entity* en2 = ens[(i + 1) % ens.size()];
		//		int en1_idx = en1->getIndex();
		//		int en2_idx = en2->getIndex();
		//		ogdf::edge side = m_KonigGraph->searchEdge(nodes[en1_idx], nodes[en2_idx], false);
		//		/*if (!side)
		//			m_KonigGraph->newEdge(nodes[en1_idx], nodes[en2_idx]);*/
		//		m_KonigGraph->newEdge(nodes[re_idx], nodes[en1_idx]);
		//	}
		//}
	}

	return true;
}

bool ERData::isZykovPlanar()
{
	constructKonigGraph();
	ogdf::BoyerMyrvold BMTest;
	return BMTest.isPlanar(*m_KonigGraph);
}

bool ERData::buildKonigLayout(double base_ratio, bool size_optim, bool side_optim)
{
	bool planar = isZykovPlanar();
	//if (!planar)
	//	return false;

	// draw graph in plane
	//if (planar)
	//{
	//	ogdf::PlanarDrawLayout PDL;
	//	PDL.baseRatio(base_ratio);
	//	PDL.sizeOptimization(size_optim);
	//	PDL.sideOptimization(side_optim);
	//	PDL.call(*m_KonigGraphLayout);
	//}
	 
	// fast force directed layout
	ogdf::FMMMLayout FMMML;
	FMMML.useHighLevelOptions(true);
	FMMML.unitEdgeLength(1.0);
	FMMML.qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
	FMMML.call(*m_KonigGraphLayout);

	return planar;
}

bool ERData::applyKonigLayout(double base_ratio, bool size_optim, bool side_optim, double scale)
{
	bool planar = buildKonigLayout(base_ratio, size_optim, side_optim);
	//if (!planar)
	//	return false;

	ERData* dual = getDualERData();

	Vec3 center = Vec3(0, 0, 0);
	int prime_idx = m_entityList.size();
	for (ogdf::node v : m_KonigGraph->nodes)
	{
		int idx = v->index();
		double x = m_KonigGraphLayout->x(v);
		double y = m_KonigGraphLayout->y(v);
		Vec3 loc = Vec3(x, y, 0);
		center += loc;

		if (idx < prime_idx)
			m_entityList[idx]->setLocation(loc);
		else if (dual)
			m_relationshipList[idx - prime_idx]->getDualEntity()->setLocation(loc);
		else
			break;
	}
	if (!dual)
		center = center / float(prime_idx);
	else
		center = center / float(m_KonigGraph->numberOfNodes());

	double len = getAverageEdgeLenth();
	if (dual)
		len = (len + dual->getAverageEdgeLenth()) / 2.0;
	scale *= (1.0 / len);

	//update2D();
	//if (dual)
	//	dual->update2D();

	reCenterScaleData(scale, center);
	if (dual)
		dual->reCenterScaleData(scale, center);

	return planar;
}

bool ERData::overlapAdjustKonigLayout()
{
	for (Relationship* re : m_relationshipList)
	{
		for (Relationship* adj : re->getAdjacentRelationships())
		{
			for (Entity* en : adj->getIncidentEntities())
			{
				bool inside = re->isEntityInInterior(en);
			}
		}
	}
	return true;
}

ogdf::Graph* ERData::getKonigGraph()
{
	return m_KonigGraph;
}

ogdf::GraphAttributes* ERData::getKonigGraphLayout()
{
	return m_KonigGraphLayout;
}

void ERData::reCenterScaleData(double scale, Vec3 center)
{
	for (int i = 0; i < m_entityList.size(); i++)
	{
		Vec3 loc = m_entityList[i]->getLocation();
		loc = (loc - center) * scale;
		m_entityList[i]->setLocation(loc);
	}
	BC_center = Vec3(0.0, 0.0, 0.0);

	//calculateBoundingCirle(false, false);
	//for (int i = 0; i < m_entityList.size(); i++)
	//{
	//	Vec3 loc = m_entityList[i]->getLocation();
	//	m_entityList[i]->setLocation(loc - BC_center);
	//}
	//BC_center = Vec3(0.0, 0.0, 0.0);
	//
	//scale *= (1.0 / getAverageEdgeLenth());
	//scaleEntities(Vec3(scale,scale,0), BC_center);

	update2D();
}


// Bipartite Simplification Operations

bool ERData::removeEntity(Entity* en)
{
	int idx = en->getIndex();
	if (m_entityList.at(idx) != en)
		return false;

	m_entityList[idx] = m_entityList[m_entityList.size() - 1];
	m_entityList[idx]->setIndex(idx);
	m_entityList.pop_back();

	return true;
}

bool ERData::removeRelationship(Relationship* re)
{
	int idx = re->getIndex();
	if (m_relationshipList.at(idx) != re)
		return false;

	m_relationshipList[idx] = m_relationshipList[m_relationshipList.size() - 1];
	m_relationshipList[idx]->setIndex(idx);
	m_relationshipList.pop_back();

	return true;
}
