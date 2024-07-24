#include "Relationship.h"
#include "Entity.h"
#include "ElementUtils.h"
#include "mainwindow.h"
#include "LogConsole.h"
#include <array>

Relationship::Relationship()
{
	m_type = RELATIONSHIP;
	m_MonogonSize = Element::m_MonogonSize;
	m_MonogonRotation = 0;
}

Relationship::Relationship(Entity * en1, Entity * en2, bool init_connections)
{
	m_type = RELATIONSHIP;
	vector<Entity*>enlist;
	enlist.push_back(en1);
	enlist.push_back(en2);
	initialize(enlist, init_connections);
}

Relationship::Relationship(const vector<Entity *>& entityList, bool init_connections)
{
	m_type = RELATIONSHIP;
	initialize(entityList, init_connections);
}

Relationship::Relationship(const vector<Relationship*> &reList, bool init_connections)
{
	m_type = RELATIONSHIP;
	vector<Entity*> enlist = getUniqueIncidentEntities(reList);
	initialize(enlist, init_connections);
}

Relationship::Relationship(const Relationship & src)
{
	m_type = RELATIONSHIP;
	m_index = src.m_index;

	for (int i = 0; i < src.m_incidentEntities.size(); i++) {
		Entity *en = new Entity();
		en->setLocation(src.m_incidentEntities[i]->getLocation());
		en->setIndex(src.m_incidentEntities[i]->getIndex());
		m_incidentEntities.push_back(en);
	}
}

Relationship::Relationship(const vector<int> entityIncides)
{
	m_type = RELATIONSHIP;
	for (int i = 0; i < entityIncides.size(); i++) {
		Entity *en = new Entity();
		en->setIndex(entityIncides[i]);
		m_incidentEntities.push_back(en);
	}
}

void Relationship::initialize(const vector<Entity*>& entityList, bool init_connections)
{
	if (init_connections)
		initalizeAdjacency(entityList);
	else
		m_incidentEntities = entityList;
	initializeScaffold();
	m_center = calculateCenter(entityList);
	m_observer = m_center;
	m_Color = getDiscreteColor(entityList.size());
	m_MonogonSize = Element::m_MonogonSize;
	m_MonogonLength = Element::m_MonogonLength;
	m_MonogonRotation = 0;
}

void Relationship::initializeScaffold()
{
	m_scaffolds.clear();
	//m_backupLayout.scaffolds.clear();

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Scaffold s;
		m_scaffolds.push_back(s);
	}

	//for (int i = 0; i < m_incidentEntities.size(); i++) {
	//	Scaffold s;
	//	m_backupLayout.scaffolds.push_back(s);
	//}
}

void Relationship::initalizeAdjacency(const vector<Entity*>& entityList)
{
    m_incidentEntities = entityList;

    for (int i = 0; i<m_incidentEntities.size(); i++) {
        //get each entities's ajacent entities
        for (int j = i + 1; j < m_incidentEntities.size(); j++) {
            m_incidentEntities[i]->addAdjacentEntity(m_incidentEntities[j]);
        }

        //get each entity's incident relationship
        m_incidentEntities[i]->addIncidentRelationship(this);
    }

    for (int i = 0; i < m_incidentEntities.size(); i++) {
        //get this relationship's adjacent relationships
        for (int j = 0; j < m_incidentEntities[i]->getIncidentRelationships().size(); j++) {
            addAdjacentRelationship(m_incidentEntities[i]->getIncidentRelationship(j));
        }
    }
}

void Relationship::finalizeAdjacency()
{
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Entity *e = m_incidentEntities[i];
		e->removeIncidentRelationship(this);
	}

	while (m_adjacentRelationships.size()>0) {
		Relationship *r = m_adjacentRelationships[0];
		r->removeAjacentRelationship(this);
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {

		for (int j = i; j < m_incidentEntities.size(); j++) {

			int res = checkIfTwoEntityAdjacent(m_incidentEntities[i], m_incidentEntities[j]);

			if (!res) {
				m_incidentEntities[i]->removeAdjacentEntity(m_incidentEntities[j]);
				m_incidentEntities[j]->removeAdjacentEntity(m_incidentEntities[i]);
			}
		}
	}
}

void Relationship::copyToIncidentEntityList(const vector<Entity*>& entityList)
{
    m_incidentEntities = entityList;
}

void Relationship::copyToAdjacentRelationshipList(const vector<Relationship*>& relist)
{
    m_adjacentRelationships = relist;
}



Relationship::~Relationship()
{
	if (mMonogonPipe)
		delete(mMonogonPipe);

	m_incidentEntities.clear();
	m_adjacentRelationships.clear();
}

void Relationship::setLabelWithIndex()
{

	m_label = "P" + to_string(m_index);
}

const vector<Entity*>& Relationship::getIncidentEntities() const {
	return m_incidentEntities;
}

vector<Relationship*> Relationship::getAdjacentRelationships() {
	return m_adjacentRelationships;
}

const vector<Relationship*>& Relationship::getAdjacentRelationships() const
{
	return m_adjacentRelationships;
}

Entity* Relationship::getIncidentEntity(int index) {
	return m_incidentEntities[index];
}

Entity * Relationship::getIncidentEntityByIndex(int index)
{
    for (int i = 0; i < m_incidentEntities.size(); i++) {
        if (m_incidentEntities[i]->getIndex() == index)
            return m_incidentEntities[i];
    }

    return NULL;
}

Relationship* Relationship::getAdjacentRelationship(int index) {
	return m_adjacentRelationships[index];
}

const Relationship* Relationship::getAdjacentRelationship(int index) const
{
	return m_adjacentRelationships[index];
}

int Relationship::getIncidentColorNum()
{
	std::vector<Color4_F> colors;
	for (Entity* en : m_incidentEntities)
	{
		Color4_F color = en->getColor();
		bool duplicate = false;
		for (Color4_F other : colors)
		{
			if (color == other)
			{
				duplicate = true;
				break;
			}
		}
		if (!duplicate)
		{
			colors.push_back(en->getColor());
		}
	}
		
	return colors.size();
}

int Relationship::getEntityInsidePolyIndex(Entity * en)
{
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		if (m_incidentEntities[i]== en)
			return i;
	}
}

vector<Entity*> Relationship::getShardEntities() const
{
    vector<Entity*> sharedVertexList;

    if (m_adjacentRelationships.size() == 0)
        return sharedVertexList;

    for (int i = 0; i < m_incidentEntities.size(); i++) {
        if (m_incidentEntities[i]->getIncidentRelationships().size() != 1)
            sharedVertexList.push_back(m_incidentEntities[i]);
    }

    return sharedVertexList;
}

vector<Scaffold>* Relationship::getScaffoldList()
{
	/*digon could only have one scaffold, which is drawn as curved link*/
	if (m_scaffolds.size() != getCardinality() && getCardinality()!= 2)
		LogConsole::showError("Relationship %d has wrong scaffolds", m_index);

	return &m_scaffolds;
}

const Scaffold& Relationship::getScaffold(int index)
{
	if (index >= m_scaffolds.size()) {
		LogConsole::showError("relationship does not have %dth Scaffold", index);
	}

	return m_scaffolds[index];
}

Scaffold * Relationship::getScaffoldPointer(int index)
{
	if (index >= m_scaffolds.size()) {
		LogConsole::showError("relationship does not have %dth Scaffold", index);
	}

	return &m_scaffolds[index];
}

bool Relationship::isSameToRelationship(Relationship *tar) {
	return isTwoEntityListSame(m_incidentEntities, tar->m_incidentEntities);
}

bool Relationship::isSameToRelationship(const vector<Entity*> &entityList) {
	return isTwoEntityListSame(m_incidentEntities, entityList);
}

bool Relationship::hasMultipleRelationship()
{
	for (auto &re : m_adjacentRelationships) {

		if (re->getCardinality() != getCardinality())
			continue;

		if (isSameToRelationship(re))
			return true;
	}

	return false;
}

bool Relationship::addAdjacentRelationship(Relationship *r, bool symmetric) {

	if (r == this)
		return false;

	int k = isInsideList<Relationship>(r, m_adjacentRelationships);

	if (k != -1)
		return false;

	m_adjacentRelationships.push_back(r);

	if (symmetric)
		r->addAdjacentRelationship(this, false);

	return true;
}

bool Relationship::removeAjacentRelationship(Relationship *r, bool symmetric) {

	int k = isInsideList<Relationship>(r, m_adjacentRelationships);

	if (k == -1)
		return false;

	//remove each one from the other's ajacent relationship list
	m_adjacentRelationships.erase(m_adjacentRelationships.begin() + k);

	if (symmetric)
		r->removeAjacentRelationship(this, false);

	return true;
}

vector<Relationship*> Relationship::getDuplicateRelationships()
{
	vector<Relationship*> res;

	for (int i = 0; i < getAdjacentRelationships().size(); i++) {
		Relationship *adj = getAdjacentRelationship(i);
		if (isTwoRelationshipSame(this, adj))
			res.push_back(adj);
	}

	return res;
}

vector<int> Relationship::getDuplicateRelationshipIndices()
{
	
	vector<Relationship*> res = getDuplicateRelationships();
	vector<int> indics = getIndices<Relationship>(res);
	return indics;
}

bool Relationship::isFirstOfDuplicates()
{
	int myIndex = getIndex();
	vector<int> duplicateIndices = getDuplicateRelationshipIndices();

	if (duplicateIndices.empty())
		return true;

	std::sort(duplicateIndices.begin(), duplicateIndices.end());
	
	if (myIndex < duplicateIndices.front())
		return true;
	else
		return false;
}

bool Relationship::addIncidentEntity(Entity *tar, bool set_adjacency, bool symmetric) {

	int k = isInsideList<Entity>(tar, m_incidentEntities);

	if (k != -1)
		return false;

	m_incidentEntities.push_back(tar);
	
	if (set_adjacency)
		for (Entity* en : m_incidentEntities)
			en->addAdjacentEntity(tar);

	if (symmetric)
		tar->addIncidentRelationship(this,set_adjacency,false);

	return true;
}

bool Relationship::removeIncidentEntity(Entity *tar, bool symmetric) {

	int k = isInsideList<Entity>(tar, m_incidentEntities);

	if (k == -1)
		return false;

	//remove entity
	m_incidentEntities.erase(m_incidentEntities.begin() + k);

	if (symmetric)
		tar->removeIncidentRelationship(this, false);

	return true;
}

void Relationship::removeIncidentEntityList(vector<Entity*> entityList) {

	for (int i = 0; i < entityList.size(); i++) {
		removeIncidentEntity(entityList[i]);
	}
}

bool Relationship::isTwoEntityConsecutive(Entity * en1, Entity * en2) const
{
	int index1 = -1;
	int index2 = -1;

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Entity *entity = m_incidentEntities[i];

		if (entity == en1) {
			index1 = i;
		}
		if (entity == en2) {
			index2 = i;
		}
	}

	if (index1 == -1 || index2 == -1 || (abs(index1 - index2) != 1 && abs(index1 - index2) != (m_incidentEntities.size()-1)))
		return false;

	return true;
}

/*set dual element to NULL*/
void Relationship::removeDualElement()
{
	dualElement = NULL;
}

void Relationship::setDualElement(Entity * e)
{
	if (!e) {
		LogConsole::showWarning("NULL dual element");
	}

	dualElement = e;
}

Entity * Relationship::getDualEntity()
{
	return dualElement;
}

void Relationship::addIncidentEntityList(vector<Entity*> entityList) {

	for (int i = 0; i < entityList.size(); i++) {
		addIncidentEntity(entityList[i]);
	}

	initalizeAdjacency(m_incidentEntities);
	initializeScaffold();
}

int Relationship::getCardinality() const{
	return m_incidentEntities.size();
}

int Relationship::getDegree() const {
	return m_adjacentRelationships.size();
}

int Relationship::getDegreeInRelationshipList(const vector<Relationship*> &re_list)
{
	int sum = 0;
	for (auto adjr : m_adjacentRelationships) {
		int inside = isInsideList<Relationship>(adjr, re_list);
		if (inside != 0)
			sum++;
	}

	return sum;
}

int Relationship::getModularityDegree(const vector<Relationship*>& re_list)
{
	int sum = 0;
	for (auto adjr : m_adjacentRelationships) {
		int inside = isInsideList<Relationship>(adjr, re_list);
		if (inside != 0) {
			int weight = getSharedEntities(adjr, this).size();
			sum += weight;
		}
	}

	return sum;
}

int Relationship::getSumEntityDegree()
{
    int sum = 0; 
    for (int i = 0; i < m_incidentEntities.size(); i++) {
        int d = m_incidentEntities[i]->getDegree();
        sum += d;
    }

    return sum;
}

vector<Entity*> Relationship::getEntitiesByDegree(int degree)
{
	vector<Entity*> res;

	if (degree < 1)
	{
		LogConsole::showError("incorrect degree");
		return res;
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		if (m_incidentEntities[i]->getDegree() == degree) {
			res.push_back(m_incidentEntities[i]);
		}
	}

	return res;
}

void Relationship::clearIncidentEntities() {
	m_incidentEntities.clear();
}

void Relationship::clearAdjacentRelationships() {
	m_adjacentRelationships.clear();
}

/*Get all relationships sharing with vertex e by edge */
vector<Relationship::EdgelyAdjacentRelationship> Relationship::getEdgelyAdjacentRelationships(Entity *e)
{
    vector<EdgelyAdjacentRelationship> results;

    int inside = isInsideList<Entity>(e, m_incidentEntities);
    if (inside == -1) {
        LogConsole::writeConsole("entity is not incident to relationship");
        return results;
    }

    for (int i = 0; i < e->getIncidentRelationships().size(); i++) {
        Relationship *r = e->getIncidentRelationship(i);

		if (r == this)
			continue;

        vector<Entity*> sharedEntities = getSharedEntities(r, this);

        if (sharedEntities.size() != 2)
            continue;
        else {
            int res = isInsideList<Entity>(e, sharedEntities);
            if (res == -1) {
                LogConsole::writeConsole("the shared entities are wrong");
                return results;
            }

            EdgelyAdjacentRelationship found;
            found.e1 = sharedEntities[0];
            found.e2 = sharedEntities[1];
            found.r1 = r;

            results.push_back(found);
        }
    }

    return results;
}

/*get all edgely adjacent relationships incident to ONLY e1 and e2,
note the current relationship is excluded*/
vector<Relationship::EdgelyAdjacentRelationship> Relationship::getEdgelyAdjacentRelationships(Entity * e1, Entity * e2)
{
	vector<EdgelyAdjacentRelationship> results;

	int inside = isInsideList<Entity>(e1, m_incidentEntities);
	if (inside == -1) {
		LogConsole::writeConsole("entity is not incident to relationship");
		return results;
	}

	inside = isInsideList<Entity>(e2, m_incidentEntities);
	if (inside == -1) {
		LogConsole::writeConsole("entity is not incident to relationship");
		return results;
	}

	for (int i = 0; i < e1->getIncidentRelationships().size(); i++) {
		Relationship *r = e1->getIncidentRelationship(i);

		if (r == this)
			continue;

		vector<Entity*> sharedEntities = getSharedEntities(r, this);

		if (sharedEntities.size() != 2)
			continue;
		else {
			int res = isInsideList<Entity>(e1, sharedEntities);
			if (res == -1) {
				LogConsole::writeError("the shared entities are wrong");
				continue;
			}

			res = isInsideList<Entity>(e2, sharedEntities);
			if (res == -1) {
				continue;
			}

			EdgelyAdjacentRelationship found;
			found.e1 = sharedEntities[0];
			found.e2 = sharedEntities[1];
			found.r1 = r;
			results.push_back(found);
		}
	}

	return results;
}

/*Get adjacent relationships which are sharing elist as well*/
vector<Relationship*> Relationship::getRelationshipsSharingEntities(vector<Entity*>elist)
{

	vector<Relationship*>results;
	for (int adjindex = 0; adjindex < m_adjacentRelationships.size(); adjindex++) {
		Relationship* r = m_adjacentRelationships[adjindex];

		bool res = true;
		for (int i = 0; i < elist.size(); i++) {
			int index = isInsideList<Entity>(elist[i], r->getIncidentEntities());
			if (index == -1) {
				res = false;
				break;
			}
		}

		if (res == true) {
			results.push_back(r);
		}
	}

	return results;
}

/*get all edgely adjacent relationships, 
note the current relationship is excluded*/
vector<Relationship::EdgelyAdjacentRelationship> Relationship::getEdgelyAdjacentRelationships()
{
	vector<EdgelyAdjacentRelationship> res;

	for (int i = 0; i < m_adjacentRelationships.size(); i++) {
		Relationship * adj_re = m_adjacentRelationships[i];
		vector<Entity*> sharedEntities = getSharedEntities(adj_re, this);
		if (sharedEntities.size() != 2)
			continue;
		else {
			EdgelyAdjacentRelationship found;
			found.e1 = sharedEntities[0];
			found.e2 = sharedEntities[1];
			found.r1 = adj_re;

			res.push_back(found);
		}
	}
	return res;
}

void Relationship::improveContinuity()
{
    if (m_adjacentRelationships.size() <= 0)
        return;

    for (int i = 0; i < m_incidentEntities.size(); i++) {

        Entity *e = m_incidentEntities[i];
        int num = m_incidentEntities.size();
        int pivot = i;
        int pivotNext = i + 1;
        int pivotPre = i - 1;

        vector<EdgelyAdjacentRelationship> edgelyAdjacentRes= getEdgelyAdjacentRelationships(e);

        if (edgelyAdjacentRes.size() < 1) {
            continue;
        }

        if (edgelyAdjacentRes.size() == 1) {

            int order1 = isInsideList<Entity>(edgelyAdjacentRes[0].e1, m_incidentEntities);
            int order2 = isInsideList<Entity>(edgelyAdjacentRes[0].e2, m_incidentEntities);

            if (order1 != pivot && order2 != pivot)
                LogConsole::showError("wrong shared vertices");

            if (order1 == (order2 + 1) % num)
                continue;
            if (order2 == (order1 + 1) % num)
                continue;

            /*Get closest vertex*/
            Vec3 edgelyAdjReCenter = edgelyAdjacentRes[0].r1->getCenter(0.0);
            Vec3 pnl = m_incidentEntities[pivotNext% num]->getLocation();
            Vec3 ppl = m_incidentEntities[(pivotPre + num) % num]->getLocation();

            double ln = length(edgelyAdjReCenter - pnl);
            double lp = length(edgelyAdjReCenter - ppl);

            int closetOrder;

            if (lp < ln) {
                closetOrder = (pivotPre + num) % num;
            }
            else {
                closetOrder = pivotNext% num;
                i = pivotNext;
            }

            if (order1 == pivot) {
                swapTwoEntitiesWithoutUpdate(order2, closetOrder);
            }
            if (order2 == pivot) {
                swapTwoEntitiesWithoutUpdate(order1, closetOrder);
            }
        }

        if (edgelyAdjacentRes.size() >1) {

            int order1 = isInsideList<Entity>(edgelyAdjacentRes[0].e1, m_incidentEntities);
            int order2 = isInsideList<Entity>(edgelyAdjacentRes[0].e2, m_incidentEntities);

            int order3 = isInsideList<Entity>(edgelyAdjacentRes[1].e1, m_incidentEntities);
            int order4 = isInsideList<Entity>(edgelyAdjacentRes[1].e2, m_incidentEntities);

            if (order1 != pivot && order2 != pivot)
                LogConsole::showError("wrong shared vertices");

            if (order3 != pivot && order4 != pivot)
                LogConsole::showError("wrong shared vertices");

            if (((order1 == (order2 + 1) % num) || (order2 == (order1 + 1) % num)) && ((order3 == (order4 + 1) % num) || (order4 == (order3 + 1) % num)))
                continue;

            int toSwapOf12 = order1 == pivot ? order2 : order1;
            int toSwapOf34 = order3 == pivot ? order4 : order3;

            /*treat as one edgely adjacent poly*/
            if (toSwapOf12 == toSwapOf34)
            {
                int order1 = isInsideList<Entity>(edgelyAdjacentRes[0].e1, m_incidentEntities);
                int order2 = isInsideList<Entity>(edgelyAdjacentRes[0].e2, m_incidentEntities);

                if (order1 != pivot && order2 != pivot)
                    LogConsole::showError("wrong shared vertices");

                if (order1 == (order2 + 1) % num)
                    continue;
                if (order2 == (order1 + 1) % num)
                    continue;

                /*Get closest vertex*/
                Vec3 edgelyAdjReCenter = edgelyAdjacentRes[0].r1->getCenter(0.0);
                Vec3 pnl = m_incidentEntities[pivotNext% num]->getLocation();
                Vec3 ppl = m_incidentEntities[(pivotPre + num) % num]->getLocation();

                double ln = length(edgelyAdjReCenter - pnl);
                double lp = length(edgelyAdjReCenter - ppl);

                int closetOrder;

                if (lp < ln) {
                    closetOrder = (pivotPre + num) % num;
                }
                else {
                    closetOrder = pivotNext% num;
                    i = pivotNext;
                }

                if (order1 == pivot) {
                    swapTwoEntitiesWithoutUpdate(order2, closetOrder);
                }
                if (order2 == pivot) {
                    swapTwoEntitiesWithoutUpdate(order1, closetOrder);
                }
                continue;
            }

            i = pivotNext;
            int closetOrder;
            if (order1 == (order2 + 1) % num) {
                if(order1 == pivot)
                    closetOrder = pivotNext% num;
                else if (order2 == pivot) 
                    closetOrder = (pivotPre + num) % num;
                
                swapTwoEntitiesWithoutUpdate(toSwapOf34, closetOrder);
                continue;
            }
            else if (order2 == (order1 + 1) % num)
            {
                if (order1 == pivot)
                    closetOrder = (pivotPre + num) % num;
                else if (order2 == pivot)
                    closetOrder = pivotNext% num;

                swapTwoEntitiesWithoutUpdate(toSwapOf34, closetOrder);
                continue;
            }

            if (order3 == (order4 + 1) % num)
            {
                if (order3 == pivot)
                    closetOrder = pivotNext% num;
                else if (order4 == pivot)
                    closetOrder = (pivotPre + num) % num;

                swapTwoEntitiesWithoutUpdate(toSwapOf12, closetOrder);
                continue;
            }
            else if (order4 == (order3 + 1) % num)
            {
                if (order3 == pivot)
                    closetOrder = (pivotPre + num) % num;
                else if (order4 == pivot)
                    closetOrder = pivotNext% num;
                
                swapTwoEntitiesWithoutUpdate(toSwapOf12, closetOrder);
                continue;
            }

            /*Get closest vertex*/
            Vec3 edgelyAdjReCenter12 = edgelyAdjacentRes[0].r1->getCenter(0.0);
            Vec3 edgelyAdjReCenter34 = edgelyAdjacentRes[1].r1->getCenter(0.0);

            Vec3 pnl = m_incidentEntities[pivotNext% num]->getLocation();
            Vec3 ppl = m_incidentEntities[(pivotPre + num) % num]->getLocation();

            double ln12 = length(edgelyAdjReCenter12 - pnl);
            double lp12 = length(edgelyAdjReCenter12 - ppl);

            double ln34 = length(edgelyAdjReCenter34 - pnl);
            double lp34 = length(edgelyAdjReCenter34 - ppl);

            if (ln12 < ln34) {
                swapTwoEntitiesWithoutUpdate(toSwapOf12, pivotNext% num);
                swapTwoEntitiesWithoutUpdate(toSwapOf34, (pivotPre + num) % num);
            }
            else {
                swapTwoEntitiesWithoutUpdate(toSwapOf34, pivotNext% num);
                swapTwoEntitiesWithoutUpdate(toSwapOf12, (pivotPre + num) % num);
            }
        }

    }

    starrize();
    starrizeAdjacentPolygons();

    create2DScaffolds();
    createAdjacentRelationships2DScaffolds();
}

/*find adjacent polygons define constraint edges that construct a loop*/
int Relationship::hasEdgeAdjLoop(vector<EN*> &loopVerts, vector<RE*>&loopRes)
{
	if (m_adjacentRelationships.size() <= 0)
		return false;

	int loopSize = -1;
	
	/*find all edgely adjacent relationship*/
	vector<EdgelyAdjacentRelationship> edgeAdjRes = getEdgelyAdjacentRelationships();

	/*check whether the shared vertices construct a loop*/
    // Increasing order.
	struct Edge : public std::array<int, 2>
	{
		// Puts v0 and v1 into correct order.
		inline Edge(int v0, int v1) {
			if (v0 > v1)
				std::swap(v0, v1);
			(*this)[0] = v0;
			(*this)[1] = v1;
		}
	};

	auto findNext = [](const vector<Edge> &constraints, vector<bool> &visitMap, int start) {
		int next = -1;
		for (int i = 0; i < constraints.size(); i++) {
			if (visitMap[i])
				continue;

			int found = -1;
			if (constraints[i][0] == start)
			{
				next = constraints[i][1];
				found = i;
			}
			if (constraints[i][1] == start) {
				next = constraints[i][0];
				found = i;
			}

			if (found != -1) {
				visitMap[found] = true;
				break;
			}
		}

		return next;
	};

	auto isConstraintsALoop = [findNext](const vector<Edge> &constraints) {

		if (constraints.size() < 3)
			return false;

		vector<bool> visitMap = vector<bool>(constraints.size(), false);

		/*always start from the last added edge since previous edges already been checked*/
		int start = constraints.back()[0];
		int next = constraints.back()[1];
		visitMap.back() = true;
		int travel = 1;

		while (true) {
			next = findNext(constraints, visitMap, next);
			travel++;

			if (next == start) {
				if (travel < 3) {
					LogConsole::writeError("wrong loop checking!");
				}

				return true;
			}

			if (next == -1)
				return false;

		}
	};

	auto shortest_cycle = [](const vector<Edge> &constraints) {

		//https://www.geeksforgeeks.org/shortest-cycle-in-an-undirected-unweighted-graph/

		// To store length of the shortest cycle 
		int ans = INT_MAX;

		if (constraints.size() < 3)
			return -1;

		/*convert constraints to adjacency link*/
		vector<int> indexMapToGraphNode;
		for (int i = 0; i < constraints.size(); i++) {
			int index1 = constraints[i][0];
			int index2 = constraints[i][1];

			auto iter1 = std::find(indexMapToGraphNode.begin(), indexMapToGraphNode.end(), index1);
			if (iter1 == indexMapToGraphNode.end()) {
				indexMapToGraphNode.push_back(index1);
			}

			auto iter2 = std::find(indexMapToGraphNode.begin(), indexMapToGraphNode.end(), index2);
			if (iter2 == indexMapToGraphNode.end()) {
				indexMapToGraphNode.push_back(index2);
			}
		}

		vector<int> *gr;
		gr = new vector<int>[indexMapToGraphNode.size()];

		/*update graph*/
		for (int i = 0; i < constraints.size(); i++) {

			int index1 = constraints[i][0];
			int index2 = constraints[i][1];
			int x, y;
			for (int i = 0; i < indexMapToGraphNode.size(); i++) {
				if (index1 == indexMapToGraphNode[i])
					x = i;
				if (index2 == indexMapToGraphNode[i])
					y = i;
			}

			gr[x].push_back(y);
			gr[y].push_back(x);
		}

		// For all vertices 
		int n = indexMapToGraphNode.size();
		for (int i = 0; i < n; i++) {

			// Make distance maximum 
			vector<int> dist(n, (int)(1e9));

			// Take a imaginary parent 
			vector<int> par(n, -1);

			// Distance of source to source is 0 
			dist[i] = 0;
			queue<int> q;

			// Push the source element 
			q.push(i);

			// Continue until queue is not empty 
			while (!q.empty()) {

				// Take the first element 
				int x = q.front();
				q.pop();

				// Traverse for all it's childs 
				for (int child : gr[x]) {

					// If it is not visited yet 
					if (dist[child] == (int)(1e9)) {

						// Increase distance by 1 
						dist[child] = 1 + dist[x];

						// Change parent 
						par[child] = x;

						// Push into the queue 
						q.push(child);
					}

					// If it is already visited 
					else if (par[x] != child && par[child] != x)
						ans = min(ans, dist[x] + dist[child] + 1);
				}
			}
		}

		delete[] gr;

		// If graph contains no cycle 
		if (ans == INT_MAX)
			return -1;

		// If graph contains cycle 
		else
			return ans;

	};

	vector<Edge> constraints;
	for (int i = 0; i < edgeAdjRes.size(); i++) {

		int index1 = edgeAdjRes[i].e1->getIndex();
		int index2 = edgeAdjRes[i].e2->getIndex();

		Edge edge(index1, index2);

		/*if edge is inside constraints, skip since it is a duplicate*/
		auto iter = std::find(constraints.begin(), constraints.end(), edge);
		if (iter != constraints.end()) {
			continue;
		}

		constraints.push_back(edge);

		loopSize = shortest_cycle(constraints);
		if (loopSize != -1) {
			loopRes.push_back(edgeAdjRes[i].r1);
			loopVerts.push_back(edgeAdjRes[i].e1);
			loopVerts.push_back(edgeAdjRes[i].e2);
			return loopSize;
		}
	}

	return -1;
}

bool Relationship::swapEntities(vector<Entity*> swapList, bool starrize_flag, bool update_scaffold) {

	if (swapList.size() != 2) {
		LogConsole::writeConsole("please select two entities with right click");
		return false;
	}

	bool wronglist;
	for (int i = 0; i < swapList.size(); i++) {
		Entity *entity = swapList[i];
		
		int inside = isInsideList<Entity>(entity, m_incidentEntities);

		if (inside==-1){
			LogConsole::writeConsole("wrong swap list");
			return false;
		}
	}


	int index1, index2;
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Entity *entity = m_incidentEntities[i];

		if (entity == swapList[0]) {
			index1 = i;
		}
		if (entity == swapList[1]) {
			index2 = i;
		}
	}

    swapTwoEntitiesWithoutUpdate(index1, index2);

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }

	return true;
}

void Relationship::swapEntities(int index1, int index2, bool starrize_flag, bool update_scaffold) {

    int card = getCardinality();

    if (index1 > card || index2 > card) {
        LogConsole::writeConsole("please select two entities with right click");
        return;
    }

    swapTwoEntitiesWithoutUpdate(index1, index2);

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::swapTwoEntitiesWithoutUpdate(int ith_incident, int jth_incident)
{
	Vec3 loci = m_incidentEntities[ith_incident]->getLocation();
	Vec3 locj = m_incidentEntities[jth_incident]->getLocation();

	std::iter_swap(m_incidentEntities.begin() + ith_incident, m_incidentEntities.begin() + jth_incident);
	m_incidentEntities[ith_incident]->setLocation(loci);
	m_incidentEntities[jth_incident]->setLocation(locj);
}

void Relationship::update2D(bool starrize_flag, bool update_scaffold, bool cardinality_changed)
{
	if (cardinality_changed)
	{
		if (getCardinality() == 1)
		{
			Vec2 dir = Vec2(getCenter(m_MonogonLength) - m_incidentEntities[0]->getLocation());
			m_MonogonRotation = dir.angle();
		}
		if (update_scaffold)
			initializeScaffold();
	}
	if (starrize_flag)
        starrize();
	if (update_scaffold) 
        create2DScaffolds();
}

void Relationship::starrize(Vec3 observer) 
{
	m_observer = observer;

	if (getCardinality() < 4)
		return;

	vector<int>indices(getCardinality());
	vector<Entity*>copyIncidentlist = m_incidentEntities;
	vector<Vec3> locs(getCardinality());
	for (int i = 0; i < getCardinality(); i++) {
		indices[i] = i;
		locs[i] = m_incidentEntities[i]->getLocation();
	}

	vector<int> sorted = getSortedVerticesFromLocs(m_observer, indices, locs);

	m_incidentEntities.clear();
	for (int i = 0; i < sorted.size(); i++) {
		m_incidentEntities.push_back(copyIncidentlist[sorted[i]]);
	}

}

/*reorder entities with using center as observer*/
void Relationship::starrize()
{
	m_center = calculateCenter(m_incidentEntities);

	if (getCardinality() <= 2)
		return;
	
	starrize(m_center);
}

void Relationship::starrizeAdjacentPolygons()
{
    for (int i = 0; i < m_adjacentRelationships.size(); i++) {
        m_adjacentRelationships[i]->starrize();
    }
}

void Relationship::rotateCWPolygon(double rotateAngle, bool starrize_flag, bool update_scaffold) {

	double angle = -M_PI*(rotateAngle / 180.0);

	if (getCardinality() == 1) {
		m_MonogonRotation += angle;
		create2DScaffolds();
		return;
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 entity1 = m_incidentEntities[i]->getLocation();
		Vec3 dir1 = entity1 - m_center;
		dir1 = rotate(dir1, angle);
		entity1 = m_center + dir1;
		m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::rotatePolygon(double rotateAngle, Vec3 pivot, bool starrize_flag, bool update_scaffold) {
	
	if (getCardinality() == 1) {
		m_MonogonRotation += rotateAngle;
		create2DScaffolds();
		return;
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 entity1 = m_incidentEntities[i]->getLocation();
		Vec3 dir1 = entity1 - pivot;
		dir1 = rotate(dir1, rotateAngle);
		entity1 = pivot + dir1;
		m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}


void Relationship::flipPolygonAcrossTwoVertices(int index1, int index2, bool starrize_flag, bool update_scaffold)
{
    int card = getCardinality();
    if (index1 > card || index2 > card) {
        LogConsole::writeConsole("please select two entities with right click");
        return;
    }

    Vec3 v1 = m_incidentEntities[index1]->getLocation();
    Vec3 v2 = m_incidentEntities[index2]->getLocation();

    Vec3 m = v1 - v2;

    for (int i = 0; i < m_incidentEntities.size(); i++) {
        Vec3 v = m_incidentEntities[i]->getLocation();
        v -= v1;
        Vec3 newloc = 2.0*m*(dot(m, v) / dot(m, m)) - v;
        newloc += v1;
        m_incidentEntities[i]->setLocation(newloc);
    }

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::flipPolygonAcrossTwoVertices(Vec3 v1, Vec3 v2, bool starrize_flag, bool update_scaffold)
{
    Vec3 m = v1 - v2;

    for (int i = 0; i < m_incidentEntities.size(); i++) {
        Vec3 v = m_incidentEntities[i]->getLocation();
        v -= v1;
        Vec3 newloc = 2.0*m*(dot(m, v) / dot(m, m)) - v;
        newloc += v1;
        m_incidentEntities[i]->setLocation(newloc);
    }

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::PermutePolygonEntities(bool starrize_flag, bool update_scaffold) {

	int *indiceOrder = new int[m_incidentEntities.size()];
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		indiceOrder[i] = i;
	}

	int card = m_incidentEntities.size();
	int *myints = indiceOrder;
	std::next_permutation(myints, myints + 3);


	for (int i = 0; i < card; i++) {
		LogConsole::writeConsole("%d", myints[i]);
	}

	vector<Vec3> oldLocations;
	for (int i = 0; i < card; i++) {
		oldLocations.push_back(m_incidentEntities[i]->getLocation());
	}

	for (int i = 0; i < card; i++) {
		m_incidentEntities[i]->setLocation(Vec3(oldLocations[myints[i]].a, oldLocations[myints[i]].b, 0));
	}

	for (int i = 0; i < card; i++) {
		m_incidentEntities.push_back(m_incidentEntities[myints[i]]);
	}

	m_incidentEntities.erase(m_incidentEntities.begin(), m_incidentEntities.begin() + card);
	if (m_incidentEntities.size() != card)
		LogConsole::writeConsole("the polygon is not correct");

	delete[] indiceOrder;

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

double Relationship::getRadiusByDegree(double minimumlength) {
	int degree = getDegree();
	double radius = sqrt(degree)*minimumlength;
	return radius;
}

double Relationship::getRadiusByCardinality(double minimumlength) {
	int card = getCardinality();
	double radius = sqrt(card)*minimumlength;
	return radius;
}

void Relationship::IncreasePolygon(bool starrize_flag, bool update_scaffold) {

	double k = 1.1;

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 entity1 = m_incidentEntities[i]->getLocation();
		Vec3 dir1 = entity1 - m_center;
		entity1 = m_center + dir1*k;
		m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::ScalePolygon(double k, bool starrize_flag, bool update_scaffold)
{
    if (k == 1.0)
        return;

    k = abs(k);

	if (getCardinality() == 1) {
		this->m_MonogonSize *= k;
		this->m_MonogonLength *= k;
		create2DScaffolds();
		this->monogonPipeReconstructed = false;
		return;
	}


    for (int i = 0; i < m_incidentEntities.size(); i++) {
        Vec3 entity1 = m_incidentEntities[i]->getLocation();
        Vec3 dir1 = entity1 - m_center;
        entity1 = m_center + dir1*k;
        m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
    }

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::ScalePolygonWithoutUpdateAdjcent(double k)
{
	if (k == 1.0)
		return;

	k = abs(k);

	if (getCardinality() == 1) {
		this->m_MonogonSize *= k;
		this->m_MonogonLength *= k;
		create2DScaffolds();
		return;
	}


	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 entity1 = m_incidentEntities[i]->getLocation();
		Vec3 dir1 = entity1 - m_center;
		entity1 = m_center + dir1*k;
		m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
	}
	
	create2DScaffolds();
}

void Relationship::DecreasePolygon(bool starrize_flag, bool update_scaffold) {

	double k = 0.9;

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 entity1 = m_incidentEntities[i]->getLocation();
		Vec3 dir1 = entity1 - m_center;
		entity1 = m_center + dir1*k;
		m_incidentEntities[i]->setLocation(Vec3(entity1.a, entity1.b, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}


void Relationship::regularizePolygon(double length, bool starrize_flag, bool update_scaffold) {


	length = 0.05;
	double radius = getRadiusByCardinality(length);
	length = radius;

	double startAngle = 0;
	for (int j = 0; j < m_incidentEntities.size(); j++) {
			Entity *entity = m_incidentEntities[j];

		double angle = 2 * M_PI / m_incidentEntities.size();

		double dx = length*cos(startAngle + angle*j);
		double dy = length*sin(startAngle + angle*j);

		entity->setLocation(Vec3(m_center.x + dx, m_center.y + dy, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::translateLocation(double ddx, double ddy, bool starrize_flag, bool update_scaffold)
{

	for (int i = 0; i< m_incidentEntities.size(); i++) {
		Entity *en = m_incidentEntities[i];
		en->setLocation(Vec3(en->getLocation().x + ddx, en->getLocation().y + ddy, 0));
	}

    if (starrize_flag) {
        starrize();
        starrizeAdjacentPolygons();
    }

    if (update_scaffold) {
        create2DScaffolds();
        createAdjacentRelationships2DScaffolds();
    }
}

void Relationship::moveToLocation(double tx, double ty, bool starrize_flag, bool update_scaffold)
{

    Vec3 oldcenter = calculateCenter(m_incidentEntities);
    Vec3 newcenter(tx, ty, 0);

    double dx = newcenter.x - oldcenter.x;
    double dy = newcenter.y - oldcenter.y;

    translateLocation(dx, dy, starrize_flag, update_scaffold);
}

void Relationship::randomeCreateNewObserver()
{
    calcConvexHull();
    m_observer = generateRandomLocInPolygon(convexHull);
    starrize(m_observer);
    create2DScaffolds();
}

void Relationship::createAdjacentRelationships2DScaffolds()
{
	for (int i = 0; i < m_adjacentRelationships.size(); i++) {
		Relationship *r = m_adjacentRelationships[i];
		r->create2DScaffolds();
	}
}

void Relationship::calcConvexHull()
{
	convexHull.clear();

	bool res = getConvexHull(m_incidentEntities, convexHull);
	if (!res || convexHull.size() < 3) {
		LogConsole::writeConsole("%d'sconvex hull is constructed with %d! points", m_index, convexHull.size());
		return;
	}
}

vector<Vec3> Relationship::getAllEntititiesLocations()
{
    vector<Vec3> locs;

    if (m_incidentEntities.size() == 2) {

        for (int i = 0; i < m_scaffolds.size(); i++) {
            for (int j = 0; j < m_scaffolds[i].m_CurvedLocation.size(); j++) {
                locs.push_back(m_scaffolds[i].m_CurvedLocation[j]);
            }
        }

    }
    else {
        for (int i = 0; i < m_incidentEntities.size(); i++) {
            locs.push_back(m_incidentEntities[i]->getLocation());
        }
    }

    return locs;
}

vector<Vec3> Relationship::getConvexHullVertices()
{
	return convexHull;
}

void Relationship::clearConvexHullVertices()
{
	convexHull.clear();
}

Vec3 Relationship::getCenter(double r)
{
	if (m_incidentEntities.size() > 1)
		return m_center;

	double a = m_MonogonRotation;
	return m_center + r * Vec3(cos(a), sin(a), 0);
}

Vec3 Relationship::getConstCenter() const
{
	return m_center;
}

void Relationship::setCenter(Vec3 center)
{
    m_center = center;
}

Vec3 Relationship::getObserver()
{
    return m_observer;
}

void Relationship::setObserver(Vec3 loc)
{
    m_observer = loc;
}

void Relationship::createMonogonPipe(double width)
{
	if (this->m_MonogonSize == Element::m_MonogonSize)
		return;

	if (mMonogonPipe) {
		delete(mMonogonPipe);
		mMonogonPipe = NULL;
	}

	if (!mMonogonPipe) {
		mMonogonPipe = new PipeMesh::Pipe();
	}

	double b = this->m_MonogonSize;
	double a = this->m_MonogonLength;

	double inclinedAngle = 0;

	PipeMesh::Vector3 nodeLoc = PipeMesh::Vector3(0, 0, 0);

	PipeMesh::Vector3 m_observer = PipeMesh::Vector3(nodeLoc.x + a*cos(inclinedAngle), nodeLoc.y + a*sin(inclinedAngle), 0.0);

	double x = m_observer.x;
	double y = m_observer.y;

	vector<PipeMesh::Vector3> scaffoldsPoints;
	scaffoldsPoints.push_back(nodeLoc);

	double startAngle = -M_PI*0.5 - asin(b / a);
	double endAngle = M_PI*0.5 + asin(b / a);
	double angleRange = endAngle - startAngle;

	for (int n = 0; n <= Element::m_MonogonSegments; ++n) {
		double const t = startAngle + angleRange * (double)n / (double)Element::m_MonogonSegments;

		double ex = cos(t) * b;
		double ey = sin(t) * b;

		Eigen::Vector2d loc = Eigen::Rotation2D<double>(inclinedAngle) * Eigen::Vector2d(ex, ey);

		scaffoldsPoints.push_back(PipeMesh::Vector3(x + loc.x(), y + loc.y(), 0));
	}
	scaffoldsPoints.push_back(nodeLoc);

	// generate points of a circle on xy plane
	auto buildCircle = [](float radius, int steps)
	{
		std::vector<PipeMesh::Vector3> points;
		if (steps < 2) return points;

		const float PI2 = acos(-1) * 2.0f;
		float x, y, a;
		for (int i = 0; i <= steps; ++i)
		{
			a = PI2 / steps * i;
			x = radius * cosf(a);
			y = radius * sinf(a);
			points.push_back(PipeMesh::Vector3(x, y, 0));
		}
		return points;
	};

	std::vector<PipeMesh::Vector3> circle = buildCircle(width*0.05*Element::m_VertexSize, 48); // radius, segments

	mMonogonPipe->set(scaffoldsPoints, circle);
	monogonPipeReconstructed = true;
}

PipeMesh::Pipe * Relationship::getMonogonPipe()
{
	if (getCardinality() != 1)
		return NULL;

	return mMonogonPipe;
}

void Relationship::createPolygonScaffold() {

	double expL = gmainwindow->expectedLength;

	for (int i = 0; i < m_incidentEntities.size(); i++) {

		Entity *entity1 = m_incidentEntities[i];
		Entity *entity2 = m_incidentEntities[(i + 1) % (m_incidentEntities.size())];

		vector<Relationship*> sharedRelationships;
		int numEdgelyAdjacentPolys = 1;

		if (gmainwindow->flag_drawPolyCurvedScaffolds) {
			numEdgelyAdjacentPolys = getNumberOfAjacentRelationships(entity1, entity2, sharedRelationships);
		}

		if (numEdgelyAdjacentPolys >= 2) {

			Vec3 middle = (entity1->getLocation() + entity2->getLocation()) * 0.5;
			Vec3 dir = m_observer - middle;

			std::sort(sharedRelationships.begin(), sharedRelationships.end(), [](Relationship *i, Relationship *j){
				return (i->getIndex()<j->getIndex()); 
			});

			int digonNum = 0;
			vector<Relationship*> notDigonSharedRes;
			for (auto re : sharedRelationships) {
				if (re->getCardinality() == 2) {
					digonNum++;
				}
				else {
					notDigonSharedRes.push_back(re);
				}
			}

			assert(notDigonSharedRes.size() + digonNum == sharedRelationships.size());

			/*start from biggest digons's width, then curved more*/
			double radius = gmainwindow->m_scaffoldMaxCurve*length(dir);

			if (digonNum != 0) {

				double lenAngle = (Element::m_DigonLensAngle) / 180.0*M_PI;
				lenAngle = lenAngle + (digonNum - 1)*(Element::m_MultipleDigonOffset / 180.0*M_PI);
				Vec3 edgedir = entity1->getLocation() - entity2->getLocation();
				double chordlen = length(edgedir);
				double lenRadius = chordlen / (2.0*sin(lenAngle / 2.0));
				double len_half_width = lenRadius - cos(lenAngle / 2.0)*lenRadius;

				vector<Relationship*> sameSideRes;
				vector<Relationship*> othersideRes;
				for (int i = 1; i < notDigonSharedRes.size(); i++) {

					Vec3 dir1 = notDigonSharedRes[0]->getObserver() - middle;
					Vec3 dir2 = notDigonSharedRes[i]->getObserver() - middle;
					if (dot(dir1, dir2) > 0)
						sameSideRes.push_back(notDigonSharedRes[i]);
					else
						othersideRes.push_back(notDigonSharedRes[i]);
				}
				assert(othersideRes.size() + sameSideRes.size() + 1 == notDigonSharedRes.size());

				int order = getOrderOfRelationship(this, notDigonSharedRes) /*+ 1*/;
				int sameSideOrder = getOrderOfRelationship(this, sameSideRes);
				int otherSideOrder = getOrderOfRelationship(this, othersideRes);
				int bendorder = -1;

				/*if total order is 0, no curve*/
				int maxBenNum = -1;
				if (order == 0) {
					assert(sameSideOrder == -1);
					assert(otherSideOrder == -1);
					bendorder = 1;
				}
				else {
					/*if total order is not 0, check the order of the same side of 0 or opposite of 0*/
					if (sameSideOrder != -1)
					{
						bendorder = sameSideOrder + 2;
						maxBenNum = sameSideRes.size();
					}
					else {
						bendorder = otherSideOrder + 1;
						maxBenNum = othersideRes.size();
					}
				}

				if (bendorder == -1)
					LogConsole::showError("wrong sharing relationship list");

				radius = len_half_width + gmainwindow->m_scaffoldMaxCurve*bendorder;

				if (radius / length(dir) > 0.8) {
					double offset = (0.8 *length(dir) - len_half_width*1.2) / maxBenNum;
					radius = len_half_width*1.2 + offset*bendorder*length(dir);
				}
			}
			else
			{
				/*get the number of polygons on the same side of the first shared poly*/
				vector<Relationship*> sameSideRes;
				vector<Relationship*> othersideRes;
				for (int i = 1; i < sharedRelationships.size(); i++) {

					Vec3 dir1 = sharedRelationships[0]->getObserver() - middle;
					Vec3 dir2 = sharedRelationships[i]->getObserver() - middle;
					if (dot(dir1, dir2) > 0)
						sameSideRes.push_back(sharedRelationships[i]);
					else
						othersideRes.push_back(sharedRelationships[i]);
				}

				assert(othersideRes.size() + sameSideRes.size() + 1 == sharedRelationships.size());

				int order = getOrderOfRelationship(this, sharedRelationships) /*+ 1*/;
				int sameSideOrder = getOrderOfRelationship(this, sameSideRes);
				int otherSideOrder = getOrderOfRelationship(this, othersideRes);
				int bendorder = -1;

				/*if total order is 0, no curve*/
				int maxBenNum = -1;
				if (order == 0) {
					assert(sameSideOrder == -1);
					assert(otherSideOrder == -1);
					bendorder = 0;
				}
				else {
					/*if total order is not 0, check the order of the same side of 0 or opposite of 0*/
					if (sameSideOrder != -1)
					{
						bendorder = sameSideOrder + 1;
						maxBenNum = sameSideRes.size();
					}
					else {
						bendorder = otherSideOrder + 1;
						maxBenNum = othersideRes.size();
					}
				}

				if (bendorder == -1)
					LogConsole::showError("wrong sharing relationship list");

				radius = gmainwindow->m_scaffoldMaxCurve*bendorder;
				if (radius / length(dir) > 0.8) {
					double offset = 0.8 *length(dir) / maxBenNum;
					radius = offset*bendorder*length(dir);
				}
			}

            normalize(dir);
			m_scaffolds[i].ControlPoint = middle + dir*radius;
			vector<Vec3> currentListToAdd;
			currentListToAdd.push_back(entity1->getLocation());
			currentListToAdd.push_back(m_scaffolds[i].ControlPoint);
			currentListToAdd.push_back(entity2->getLocation());
			UpdateBezierLocation(currentListToAdd);
			m_scaffolds[i].m_CurvedLocation = currentListToAdd;

		}
		else {
			m_scaffolds[i].ControlPoint = 0.5*(entity1->getLocation() + entity2->getLocation());
			vector<Vec3> currentListToAdd;
			currentListToAdd.push_back(entity1->getLocation());
			currentListToAdd.push_back(entity2->getLocation());
			m_scaffolds[i].m_CurvedLocation = currentListToAdd;
		}
	}
}


void Relationship::createDigonScaffold() {

	Entity *entity1 = m_incidentEntities[0];
	Entity *entity2 = m_incidentEntities[1];



	Vec3 loc1 = entity1->getLocation();
	Vec3 loc2 = entity2->getLocation();
	Vec3 dir = loc1 - loc2;
	Vec3 middle = (loc1 + loc2) * 0.5;	
	
	if (!gmainwindow->flag_drawPolyCurvedScaffolds)
	{
		vector<Vec3> points{loc1, loc2};

		m_scaffolds.clear();
		Scaffold S;
		S.m_Curved = false;
		S.m_CurvedLocation = points;
		S.ControlPoint = middle;
		m_scaffolds.push_back(S);
		return;
	}

	double chordlen = length(dir);
	double lenAngle = (Element::m_DigonLensAngle) / 180.0*M_PI;

	if (gmainwindow->flag_drawPolyCurvedScaffolds) {
		/*check if there are more digons*/
		vector<Relationship*> sharedRelationships;
		int numEdgelyAdjacentPolys = getNumberOfAjacentDigons(entity1, entity2, sharedRelationships);
		if (numEdgelyAdjacentPolys >= 2) {
			int order = getOrderOfRelationship(this, sharedRelationships);
			lenAngle = lenAngle + order*(Element::m_MultipleDigonOffset / 180.0*M_PI);
		}
	}

	double lenRadius = chordlen / (2.0*sin(lenAngle / 2.0));
	double d = chordlen*0.5*tan(lenAngle*0.5);
	Vec3 random = Vec3(0.0, 0.0, 1.0);
	Vec3 norm = cross(random, dir);
	normalize(norm);

	m_scaffolds[0].ControlPoint = middle + norm*d;

	if(m_scaffolds.size() > 1)
		m_scaffolds[1].ControlPoint = middle - norm*d;

	double l_chortToCenter = cos(lenAngle*0.5)*lenRadius;
	Vec3 center = middle + norm*l_chortToCenter;
	Vec3 startDir = m_incidentEntities[0]->getLocation() - center;
	Vec3 endDir = m_incidentEntities[1]->getLocation() - center;

	double startAngle = atan2(startDir.y, startDir.x);
	double endAngle = atan2(endDir.y, endDir.x);

	vector<Vec3> scaffoldsPoints = getArc(center.x, center.y, lenRadius, endAngle, lenAngle, 10);
	m_scaffolds[0].m_CurvedLocation = scaffoldsPoints;

	center = middle - norm*l_chortToCenter;
	startDir = m_incidentEntities[0]->getLocation() - center;
	endDir = m_incidentEntities[1]->getLocation() - center;

	startAngle = atan2(startDir.y, startDir.x);
	endAngle = atan2(endDir.y, endDir.x);

	scaffoldsPoints = getArc(center.x, center.y, lenRadius, startAngle, lenAngle, 10);

	if (m_scaffolds.size() > 1)
		m_scaffolds[1].m_CurvedLocation = scaffoldsPoints;

	//update center location of diagon
	vector<Vec3> locs;
	for (int s = 0; s < m_scaffolds.size(); s++) {
		locs.push_back(m_scaffolds[s].ControlPoint);
	}
	m_observer = m_center;
}

double Relationship::getDigonLength() const
{
	if(getCardinality() != 2)
		return 0.0;

	Entity *entity1 = m_incidentEntities[0];
	Entity *entity2 = m_incidentEntities[1];

	Vec3 dir = entity1->getLocation() - entity2->getLocation();
	double chordlen = length(dir);

	return chordlen;
}

double Relationship::getDigonRadius() const
{
	if (getCardinality() != 2)
		return 0.0;

	double lenAngle = (Element::m_DigonLensAngle) / 180.0*M_PI;
	double chordlen = getDigonLength();
	double lenRadius = chordlen / (2.0*sin(lenAngle / 2.0));

	return lenRadius;
}

double Relationship::getDigonWidth() const
{
	if (getCardinality() != 2)
		return 0.0;

	double lenRadius = getDigonRadius();
	double lenAngle = (Element::m_DigonLensAngle) / 180.0*M_PI;
	double len_half_width = lenRadius - cos(lenAngle / 2.0)*lenRadius;

	return len_half_width*2.0;
}

double Relationship::getMonogonInclinedAngle() const
{
	Entity *entity1 = m_incidentEntities[0];
	vector<Relationship*> sharedRelationships;

	int numEdgelyAdjacentPolys = getNumberOfAjacentMonogon(entity1, sharedRelationships);
	double angleStep = 2 * M_PI / numEdgelyAdjacentPolys;
	if (numEdgelyAdjacentPolys == 0)
		angleStep = 0;


	/*get the average center of its incident polygons*/
	double offset = 0.0;
	Vec3 center;
	int count = 0;
	for (int i = 0; i < entity1->getIncidentRelationships().size(); i++) {
		Relationship *inc = entity1->getIncidentRelationship(i);
		if (inc->getCardinality() == 1) {
			continue;
		}
		else {
			center += calculateCenter(inc->getIncidentEntities());
			count++;
		}
	}

	center /= double(count);

	Vec3 dir = entity1->getLocation() - center;
	offset = atan2(dir.y, dir.x);

	if (numEdgelyAdjacentPolys == 2)
		offset += M_PI / 2.0;

	if (count == 0)
		offset = 0;

	if (numEdgelyAdjacentPolys >= 2) {
		int order = getOrderOfRelationship(this, sharedRelationships);
		return offset + order*angleStep;
	}
	else {
		return offset;
	}
}

void Relationship::setMonogonRotation(double angle)
{
	m_MonogonRotation = angle;
}

double Relationship::getMonogonRotation()
{
	return m_MonogonRotation;
}

/*Draw cone shape*/
void Relationship::createMonogonScaffold()
{
	Entity *entity1 = m_incidentEntities[0];
	double b = this->m_MonogonSize;
	double a = this->m_MonogonLength;
	Vec3 nodeLoc = entity1->getLocation();
	m_observer = Vec3(nodeLoc.x + a*cos(m_MonogonRotation), nodeLoc.y + a*sin(m_MonogonRotation), 0.0);

	double x = m_observer.x;
	double y = m_observer.y;

	vector<Vec3> scaffoldsPoints;
	scaffoldsPoints.push_back(nodeLoc);

	double startAngle = -M_PI*0.5 - asin(b / a);
	double endAngle = M_PI*0.5 + asin(b / a);
	double angleRange = endAngle - startAngle;

	for (int n = 0; n <= m_MonogonSegments; ++n) {
		double const t = startAngle + angleRange * (double)n / (double)m_MonogonSegments;
		double ex = cos(t) * b;
		double ey = sin(t) * b;
		Eigen::Vector2d loc = Eigen::Rotation2D<double>(m_MonogonRotation) * Eigen::Vector2d(ex, ey);
		scaffoldsPoints.push_back(Vec3(x + loc.x(), y + loc.y(), 0));
	}
	scaffoldsPoints.push_back(nodeLoc);

	m_scaffolds[0].m_CurvedLocation = scaffoldsPoints;

}

void Relationship::initializeMonogonOrientation()
{
	if (getCardinality() != 1)
		return;

	double offset = getMonogonInclinedAngle();

	m_MonogonRotation = offset;
}

//update all monogons adjacent to this polygons's adjacent polygons
void Relationship::updateRelatedMonogons()
{
	for (auto re : getRelatedMonogons()) {
		re->initializeMonogonOrientation();
		re->create2DScaffolds();
	}
}

vector<RE*> Relationship::getRelatedMonogons()
{
	vector<RE*> res;
	for (int i = 0; i < getAdjacentRelationships().size(); i++) {
		RE *rei = getAdjacentRelationship(i);
		for (int j = 0; j < rei->getAdjacentRelationships().size(); j++) {
			RE *re = rei->getAdjacentRelationship(j);
			if (re->getCardinality() == 1)
			{
				res.push_back(re);
			}
		}
	}
	return res;
}

void Relationship::create2DScaffolds()
{
	if (getCardinality() == 1) {
		createMonogonScaffold();
	}
	else if (getCardinality() == 2) {
		m_center = calculateCenter(m_incidentEntities);
		createDigonScaffold();
	}
	else {
		createPolygonScaffold();
	}
}

double Relationship::targetInteriorAngle()
{
	return M_PI*(getCardinality() - 2) / getCardinality();
}

double Relationship::area() const
{
    double sum = 0;

    if (getCardinality() == 2) {
        double theta = (Element::m_DigonLensAngle) / 180.0*M_PI;
        Vec3 l1 = m_incidentEntities[0]->getLocation();
        Vec3 l2 = m_incidentEntities[1]->getLocation();
        Vec3 u = l2 - l1;
        double M_a = length(u)* length(u);

        sum = ((theta - sin(theta)) * M_a) / (4.0*(sin(theta / 2.0)*sin(theta / 2.0)));
    }
    else {

        for (int i = 0; i < m_incidentEntities.size(); i++) {
            Vec3 l1 = m_incidentEntities[i]->getLocation();
            Vec3 l2 = m_incidentEntities[(i + 1) % getCardinality()]->getLocation();

            sum += l1.x*l2.y - l1.y*l2.x;
        }

        sum = abs(sum) / 2.0;
    }
    return sum;
}

double Relationship::area2()
{
    double sum = 0;

    if (getCardinality() == 2) {
        double theta = (Element::m_DigonLensAngle) / 180.0*M_PI;
        Vec3 l1 = m_incidentEntities[0]->getLocation();
        Vec3 l2 = m_incidentEntities[1]->getLocation();
        Vec3 u = l2 - l1;
        double M_a = length(u)* length(u);

        sum = ((theta - sin(theta)) * M_a) / (4.0*(sin(theta / 2.0)*sin(theta / 2.0)));
    }
    else {

        Vec3 center = calculateCenter(m_incidentEntities);

        for (int i = 0; i < m_incidentEntities.size(); i++) {
            Vec3 l1 = m_incidentEntities[i]->getLocation();
            Vec3 l2 = m_incidentEntities[(i + 1) % getCardinality()]->getLocation();

            double area = CalcTriangleArea(center, l1, l2);
            sum += area;
        }
    }
    return sum;
}

double Relationship::perimeter() const
{
    double sum = 0;

    if (getCardinality() == 2) {

        double theta = (Element::m_DigonLensAngle) / 180.0*M_PI;
        Vec3 l1 = m_incidentEntities[0]->getLocation();
        Vec3 l2 = m_incidentEntities[1]->getLocation();
        Vec3 u = l2 - l1;
        double L = length(u);

        sum = L*theta / (2.0*sin(theta*0.5));
    }
    else {
        for (int i = 0; i < m_incidentEntities.size(); i++) {
            Vec3 l1 = m_incidentEntities[i]->getLocation();
            Vec3 l2 = m_incidentEntities[(i + 1) % getCardinality()]->getLocation();

            double x1 = l1.x;
            double x2 = l2.x;
            double y1 = l1.y;
            double y2 = l2.y;
            
            double len = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            sum += len;
        }
    }

    return sum;
}

double Relationship::aspectRatio() const
{
	double a = area();
	double p = perimeter();
	int num = getCardinality();
	double C = 4.0 * (double)num*tan(M_PI / (double)num);
	double aspectRatio = p*p - C*a;

	return aspectRatio;
}

bool Relationship::isEntityInInterior(Entity* en)
{
	int crossn = 0;
	int n = m_incidentEntities.size();
	Vec3 p0 = en->getLocation();
	
	for (int i = 0; i < n; i++)
	{
		Vec3 p1 = m_incidentEntities[i]->getLocation();
		Vec3 p2 = m_incidentEntities[(i + 1) % n]->getLocation();
		if (((p1.y <= p0.y) && (p2.y > p0.y)) ||
			((p1.y > p0.y) && (p2.y <= p0.y)))
		{
			// compute edge-ray intersection x-coordinate
			double vt = (p0.y - p1.y) / (p2.y - p1.y);
			if (p0.x < (p1.x + (vt * (p2.x - p1.x))))
				crossn++;
		}
	}
	return (crossn % 2);
}


double Relationship::calcRegularityEnergy_edgeVariance()
{
	double sum = 0;
    int card = m_incidentEntities.size();
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 l = m_incidentEntities[i]->getLocation() - m_incidentEntities[(i + 1)% card]->getLocation();
		double len = length(l);
        sum += len;
	}
    double expectedvalue = sum / card;
    double variance = 0;
    for (int i = 0; i < m_incidentEntities.size(); i++) {
        Vec3 l = m_incidentEntities[i]->getLocation() - m_incidentEntities[(i + 1) % card]->getLocation();
        double len = length(l);
        variance += (len - expectedvalue)*(len - expectedvalue);
    }
	return variance;
}

double Relationship::calcRegularityEnergy_angleVariance()
{
	double sum = 0;
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 l1 = m_incidentEntities[(i+1) % getCardinality()]->getLocation() - m_incidentEntities[i]->getLocation();
		Vec3 l2 = m_incidentEntities[(i-1+ getCardinality())%getCardinality()]->getLocation() - m_incidentEntities[i]->getLocation();
		double angle = ccwAngle(l1, l2);
		sum+=(angle - targetInteriorAngle())*(angle - targetInteriorAngle());
	}
	return sum;
}

double Relationship::calcUniformAreaEnergy(double expectedArea)
{
	double A = this->area();
	double en =  (A- expectedArea)*(A - expectedArea);
	LogConsole::writeConsole("R%d uniform area energy: %f", m_index, en);
	return en;
}

void Relationship::showGeometryInfo()
{
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 l = m_incidentEntities[i]->getLocation() - m_incidentEntities[(i + 1)% getCardinality()]->getLocation();
		double len = length(l);
		LogConsole::writeConsole("R%d edge(%3d,%3d) length: %.16f", m_index, m_incidentEntities[i]->getIndex(), m_incidentEntities[(i + 1) % getCardinality()]->getIndex(), len);
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		Vec3 l1 = m_incidentEntities[(i + 1) % getCardinality()]->getLocation() - m_incidentEntities[i]->getLocation();
		Vec3 l2 = m_incidentEntities[(i - 1 + getCardinality()) % getCardinality()]->getLocation() - m_incidentEntities[i]->getLocation();
		double angle = ccwAngle(l1, l2);
		LogConsole::writeConsole("R%d: angle %3d: %.16f", m_index, m_incidentEntities[i]->getIndex(), angle);
	}
}

vector<int> Relationship::getEntityIndices() const
{
	int size = m_incidentEntities.size();
	vector<int> indicesList(size);
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		indicesList[i] = m_incidentEntities[i]->getIndex();
	}
	return indicesList;
}

/*loclist is the locs of the incident vertices*/
vector<int> Relationship::getEntityIndicesFromIncidentLocs(const vector<Vec3> &locList) const
{
	if (locList.size() != getCardinality())
	{
		LogConsole::showError("wrong loclist");
		return vector<int>();
	}

	vector<int>indices(getCardinality());
	for (int i = 0; i < getCardinality(); i++)
		indices[i] = m_incidentEntities[i]->getIndex();

	if (indices.size() < 3)
		return indices;
	else
		return getSortedVerticesFromLocs(indices, locList);
}

vector<int> Relationship::getSortedVerticesFromLocs(vector<int> indices, const vector<Vec3>& locList)
{
	Vec3 m_pivot = calculateCenter(locList);

	return getSortedVerticesFromLocs(m_pivot, indices, locList);

}

vector<int> Relationship::getSortedVerticesFromLocs(Vec3 m_pivot, vector<int> indices, const vector<Vec3>& locList)
{
	assert(indices.size() == locList.size());

	vector<pair<double, int>> temp_Entities;
	temp_Entities.resize(locList.size());

	for (int i = 0; i < locList.size(); i++) {

		Vec3 dir = locList[i] - m_pivot;

		//calculate the angles between each vertex and the x axis
		double angle = atan2(dir.y, dir.x);
		temp_Entities[i] = pair<double, int>(angle, indices[i]);
	}

	//reorder vertices based on the angles
	std::sort(temp_Entities.begin(), temp_Entities.end(),
		[](pair<double, int> x, pair<double, int> y) { return x.first < y.first; });

	//return vertices indices
	vector<int> indicesList;
	indicesList.resize(temp_Entities.size());
	for (int i = 0; i < temp_Entities.size(); i++) {
		indicesList[i] = temp_Entities[i].second;
	}

	return indicesList;
}

vector<Vec3> Relationship::getSortedLayout()
{
	Vec3 center = calculateCenter(m_incidentEntities);
	vector<Vec3> locs;
	for (Entity* en : m_incidentEntities)
		locs.push_back(en->getLocation());
	
	if (locs.size() == 1)
		return locs;

	if (locs.size() == 2)
	{
		Vec3 vdiff = (locs[1] - locs[0]) * 0.5;
		Vec3 voff(vdiff.y, -vdiff.x, 0); normalize(voff);
		double a = Element::m_DigonLensAngle * M_PI / 360.0;
		double b = length(vdiff);
		double c = tan(a) * b;

		locs.push_back(locs[0] + vdiff + c * voff);
		locs.push_back(locs[0] + vdiff - c * voff);
	}

	for (Vec3& point : locs)
	{
		Vec3 dir = point - center;
		point.z = atan2(dir.y, dir.x);
	}

	std::sort(locs.begin(), locs.end(),
		[](Vec3 a, Vec3 b) {return a.z < b.z;});

	return locs;
}

//void Relationship::backupLocation(vector<pair<int, Vec3>> &locList)
//{
//    locList.clear();
//    for (int i = 0; i < m_incidentEntities.size(); i++) {
//        locList.push_back(pair<int, Vec3>(m_incidentEntities[i]->getIndex(), m_incidentEntities[i]->getLocation()));
//    }
//}
//
//void Relationship::resumeLocation(const vector<pair<int, Vec3>> locList)
//{
//    if (locList.size() != m_incidentEntities.size()) {
//        LogConsole::writeConsole("cannot resume locations from different size backup");
//        return;
//    }
//
//    for (int i = 0; i < locList.size(); i++) {
//
//        int index = locList[i].first;
//        bool found = false;
//        for (int j = 0; j < m_incidentEntities.size(); j++) {
//            if (m_incidentEntities[j]->getIndex() == index) {
//                m_incidentEntities[j]->setLocation(locList[i].second);
//                found = true;
//                break;
//            }
//        }
//
//        if (!found) {
//            LogConsole::writeConsole("wrong backup locations");
//            return;
//        }
//    }
//}
//
//void Relationship::backupLayout()
//{
//    m_backupLayout.locList.clear();
//    for (int i = 0; i < m_incidentEntities.size(); i++) {
//        m_backupLayout.locList.push_back(pair<int, Vec3>(m_incidentEntities[i]->getIndex(), m_incidentEntities[i]->getLocation()));
//    }
//
//    for (int i = 0; i < m_scaffolds.size(); i++) {
//        m_backupLayout.scaffolds[i] = m_scaffolds[i];
//    }
//
//    m_backupLayout.center = m_center;
//    m_backupLayout.observer = m_observer;
//}
//
//void Relationship::resumeLayout()
//{
//    if (m_backupLayout.locList.size() != m_incidentEntities.size()) {
//        LogConsole::writeConsole("cannot resume layout from different size backup");
//        return;
//    }
//
//    int num = m_incidentEntities.size();
//
//    for (int i = 0; i < m_backupLayout.locList.size(); i++) {
//        int index = m_backupLayout.locList[i].first;
//        bool found = false;
//        for (int j = 0; j < m_incidentEntities.size(); j++) {
//            if (m_incidentEntities[j]->getIndex() == index) {
//                Entity *etemp = m_incidentEntities[j];
//                etemp->setLocation(m_backupLayout.locList[i].second);
//                m_incidentEntities.push_back(etemp);
//                found = true;
//                break;
//            }
//        }
//
//        if (!found) {
//            LogConsole::writeConsole("wrong backup locations");
//            return;
//        }
//    }
//
//    m_incidentEntities.erase(m_incidentEntities.begin(), m_incidentEntities.begin() + num);
//
//    if(m_incidentEntities.size() != num)
//        LogConsole::showError("resum layout fails");
//
//    for (int i = 0; i < m_scaffolds.size(); i++) {
//        m_scaffolds[i] = m_backupLayout.scaffolds[i];
//    }
//
//    m_center = m_backupLayout.center;
//    m_observer = m_backupLayout.observer;
//}
//
//void Relationship::backupLocation()
//{
//    m_backupLayout.locList.clear();
//    for (int i = 0; i < m_incidentEntities.size(); i++) {
//        m_backupLayout.locList.push_back(pair<int, Vec3>(m_incidentEntities[i]->getIndex(), m_incidentEntities[i]->getLocation()));
//    }
//}
//
//void Relationship::resumeLocation()
//{
//    if (m_backupLayout.locList.size() != m_incidentEntities.size()) {
//        LogConsole::writeConsole("wrong backup locations");
//        return;
//    }
//
//    for (int i = 0; i < m_backupLayout.locList.size(); i++) {
//
//        int index = m_backupLayout.locList[i].first;
//        bool found = false;
//        for (int j = 0; j < m_incidentEntities.size(); j++) {
//            if(m_incidentEntities[j]->getIndex() == index){
//                m_incidentEntities[j]->setLocation(m_backupLayout.locList[i].second);
//                found = true;
//                break;
//            }
//        }
//
//        if(!found){
//            LogConsole::writeConsole("wrong backup locations");
//            return;
//        }
//    }
//}

int Relationship::getNextEntityIndex(Entity * e)
{
	int num = m_incidentEntities.size();
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		if (e == m_incidentEntities[i])
			return m_incidentEntities[(i + 1) % num]->getIndex();
	}

	LogConsole::writeConsole("entity not inside relationship");
	return -1;
}

int Relationship::getPreviousEntityIndex(Entity * e)
{
	int num = m_incidentEntities.size();
	for (int i = 0; i < m_incidentEntities.size(); i++) {
		if (e == m_incidentEntities[i])
			return m_incidentEntities[(i - 1 + num) % num]->getIndex();
	}

	LogConsole::writeConsole("entity not inside relationship");
	return -1;
}

float Relationship::getDiagonInnerCircle()
{
	Entity *entity1 = m_incidentEntities[0];
	Entity *entity2 = m_incidentEntities[1];

	Vec3 dir = entity1->getLocation() - entity2->getLocation();
	Vec3 middle = (entity1->getLocation() + entity2->getLocation()) * 0.5;

	double chordlen = length(dir);
	double lenAngle = (Element::m_DigonLensAngle) / 180.0*M_PI;

	if (gmainwindow->flag_drawPolyCurvedScaffolds) {
		/*check if there are more digons*/
		vector<Relationship*> sharedRelationships;
		int numEdgelyAdjacentPolys = getNumberOfAjacentDigons(entity1, entity2, sharedRelationships);
		if (numEdgelyAdjacentPolys >= 2) {
			int order = getOrderOfRelationship(this, sharedRelationships);
			lenAngle = lenAngle + order*(5.0 / 180.0*M_PI);
		}
	}

	double lenRadius = chordlen / (2.0*sin(lenAngle / 2.0));
	double l_chortToCenter = cos(lenAngle*0.5)*lenRadius;
	return lenRadius - l_chortToCenter;
}

double Relationship::getMonogonSize()
{
	return this->m_MonogonSize;
}

void Relationship::setMonogonSize(float size)
{
	this->m_MonogonSize = size;
}

double Relationship::getMonogonLength()
{
	return this->m_MonogonLength;
}

void Relationship::setMonogonLength(float size)
{
	this->m_MonogonLength = size;
}

void Relationship::updateBoundingBox()
{
	if (getCardinality() == 1) {
		maxX = m_incidentEntities[0]->getLocation().x + this->m_MonogonSize + this->m_MonogonLength  + this->m_MonogonSize;
		minX = m_incidentEntities[0]->getLocation().x - (this->m_MonogonSize + this->m_MonogonLength + this->m_MonogonSize);
		maxY = m_incidentEntities[0]->getLocation().y + this->m_MonogonSize + this->m_MonogonLength + this->m_MonogonSize;
		minY = m_incidentEntities[0]->getLocation().y - (this->m_MonogonSize + this->m_MonogonLength + this->m_MonogonSize);
		return;
	}

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		if (i == 0) {
			maxX = m_incidentEntities[i]->getLocation().x;
			minX = m_incidentEntities[i]->getLocation().x;
			maxY = m_incidentEntities[i]->getLocation().y;
			minY = m_incidentEntities[i]->getLocation().y;
		}
		else {
			maxX = m_incidentEntities[i]->getLocation().x > maxX ? m_incidentEntities[i]->getLocation().x : maxX;
			minX = m_incidentEntities[i]->getLocation().x < minX ? m_incidentEntities[i]->getLocation().x : minX;
			maxY = m_incidentEntities[i]->getLocation().y > maxY ? m_incidentEntities[i]->getLocation().y : maxY;
			minY = m_incidentEntities[i]->getLocation().y < minY ? m_incidentEntities[i]->getLocation().y : minY;
		}
	}

}

double Relationship::calcCircumscribedCircleRadius()
{
	m_center = calculateCenter(m_incidentEntities);
	double ccr = 0;
	for (int i = 0; i < m_incidentEntities.size(); i++) {

		Vec3 dir = m_incidentEntities[i]->getLocation() - m_center;
		double dist = length(dir);
		if (dist > ccr)
			ccr = dist;
	}

	return ccr;
}

void Relationship::getIncidentEntityIds(vector<int>& ids, int offset)
{
	for (Entity* en : m_incidentEntities)
		ids.push_back(en->getIndex() + offset);
}




// optimization related

template<class mytype>
inline void Relationship::reorderEntityIndices(const mytype * x)
{
	{
		vector<Vec3> locList;
		for (int i = 0; i < m_incidentEntities.size(); i++) {
			int index = m_incidentEntities[i]->getIndex();
			int xindex = 2 * index;
			int yindex = 2 * index + 1;
			m_incidentEntities[i]->setLocation(Vec3(x[xindex], x[yindex], 0));
		}
		m_center = calculateCenter(m_incidentEntities);
		m_observer = m_center;
		starrize(m_observer);
		return;
	}
}

/*x is the locs of all verts of the whole ERdata, including the fixed verts*/
template<class mytype>
inline vector<int> Relationship::getEntityIndices(const mytype * x) const
{
	int size = m_incidentEntities.size();
	vector<Vec3> locList(size);

	for (int i = 0; i < m_incidentEntities.size(); i++) {
		int index = m_incidentEntities[i]->getIndex();

		int xindex = 2 * index;
		int yindex = 2 * index + 1;
		locList[i] = Vec3(x[xindex], x[yindex], 0);
	}

	vector<int> reorderedIndiced = getEntityIndicesFromIncidentLocs(locList);
	return reorderedIndiced;
}