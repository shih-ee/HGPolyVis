#include "Entity.h"
#include "Relationship.h"
#include "ElementUtils.h"
#include "mainwindow.h"
#include <math.h>
#include "LogConsole.h"

Entity::Entity()
{
	m_type = ENTITY;

	float x = (float)rand() / (float)RAND_MAX;
	float y = (float)rand() / (float)RAND_MAX;

	x = x*2.0 - 1.0;
	y = y*2.0 - 1.0;

	Vec3 loc = Vec3(x, y, 0);
	m_Color = Vec4(0.1, 0.1, 0.1, 1.0);
	m_Location = loc;
    m_Size = Element::m_VertexSize;
}

Entity::Entity(Vec3 loc) 
{
	m_type = ENTITY;

	m_Color = Vec4(0.1, 0.1, 0.1, 1.0);
	m_Location = loc;
	m_Size = Element::m_VertexSize;
}

Entity::Entity(const Entity & src)
{
	m_type = ENTITY;

	m_index = src.m_index;
    m_Location = src.m_Location;
}

Entity::~Entity()
{
	m_adjacentEntities.clear();
	m_incidentRelationships.clear();
}

void Entity::setLabelWithIndex()
{
	m_label = "A" + to_string(m_index);
}

bool Entity::addAdjacentEntity(Entity* e, bool symmetric) {

	if (e == this)
		return false;

	int k = isInsideList<Entity>(e, m_adjacentEntities);

	if (k != -1)
		return false;

	m_adjacentEntities.push_back(e);

	if (symmetric)
		e->addAdjacentEntity(this, false);

	return true;
}

bool Entity::addIncidentRelationship(Relationship * r, bool set_adjacency, bool symmetric) {
	
	int k = isInsideList<Relationship>(r, m_incidentRelationships);

	if (k != -1)
		return false;

	m_incidentRelationships.push_back(r);

	if (set_adjacency)
		for (Relationship* re : m_incidentRelationships)
			re->addAdjacentRelationship(r);

	if (symmetric)
		r->addIncidentEntity(this, set_adjacency, false);

	return true;
}

bool Entity::removeAdjacentEntity(Entity *e, bool symmetric) {

	int i = isInsideList<Entity>(e, m_adjacentEntities);

	if(i == -1)
		return false;

	m_adjacentEntities.erase(m_adjacentEntities.begin() + i);

	if (symmetric)
		e->removeAdjacentEntity(this, false);

	return true;
}

bool Entity::removeIncidentRelationship(Relationship *r, bool symmetric) {

	int i = isInsideList<Relationship>(r, m_incidentRelationships);

	if (i == -1)
		return false;

	m_incidentRelationships.erase(m_incidentRelationships.begin() + i);

	if (symmetric)
		r->removeIncidentEntity(this, false);

	return true;
}

void Entity::removeDualElement()
{
	dualElement = NULL;
}

void Entity::setDualElement(Relationship * r)
{
	if (!r) {
		//LogConsole::showWarning("NULL dual element");
	}

	dualElement = r;
}

Relationship* Entity::getDualRelationship()
{
	return dualElement;
}

void Entity::copyToIncidentRelationshipList(const vector<Relationship*>& rList)
{
    m_incidentRelationships = rList;
}

void Entity::copyToAdjacentEntityList(const vector<Entity*>& elist)
{
    m_adjacentEntities = elist;
}

Relationship* Entity::getIncidentRelationship(const vector<Entity*>& otherEntities) {

	for (int i = 0; i < m_incidentRelationships.size(); i++) {
		
		Relationship *tempRe = m_incidentRelationships[i];
		bool isSame = isTwoEntityListSame(tempRe->getIncidentEntities(), otherEntities);

		if (isSame)
			return tempRe;
	}

	return NULL;
}

void Entity::finalizeAdjacency()
{
	for (int i = 0; i < m_incidentRelationships.size(); i++)
	{
		Relationship* r = m_incidentRelationships[i];
		r->removeIncidentEntity(this);
	}

	while (m_adjacentEntities.size() > 0)
	{
		Entity* e = m_adjacentEntities[0];
		e->removeAdjacentEntity(this);
	}

	for (int i = 0; i < m_incidentRelationships.size(); i++)
	{
		for (int j = i; j < m_incidentRelationships.size(); j++)
		{
			int res = checkIfTwoRelationAdjacent(m_incidentRelationships[i], m_incidentRelationships[j]);

			if (!res)
			{
				m_incidentRelationships[i]->removeAjacentRelationship(m_incidentRelationships[j]);
				m_incidentRelationships[j]->removeAjacentRelationship(m_incidentRelationships[i]);
			}
		}
	}
}

/*
	check whether the entity is connected to relationship target by a binary relationship
	binaryRe: the founded binary relationship, return null if no such relationship 
*/
bool Entity::isConnectToReByBiRe(Relationship* target, Relationship * &binaryRe){

	//check all the digons of node1
	for (int i = 0; i < this->m_incidentRelationships.size(); i++) {

		Relationship *r = this->m_incidentRelationships[i];
		if (r->getCardinality() != 2)
			continue;
		
		//if the incident relationship is a binary relationship
		//get the other entity
		Entity *e = (this == r->getIncidentEntity(0)) ? r->getIncidentEntity(1) : r->getIncidentEntity(0);

		for (int j = 0; j < e->m_incidentRelationships.size(); j++) {

			Relationship *cr = e->m_incidentRelationships[j];

			if (cr == target) {
				binaryRe = cr;
				return true;
			}
		}
	}

	binaryRe = NULL;
	return false;

}

void Entity::setLocation(Vec3 v) {
	m_Location = v;
}

void Entity::setLocation(double x, double y, double z) {
    m_Location.x = x;
    m_Location.y = y;
    m_Location.z = z;
}

void Entity::setLocationZ(double z)
{
    m_Location.z = z;
}

Vec3 Entity::getLocation() {
	return m_Location;
}

void Entity::updateRelatedMonogons()
{
	for (int i = 0; i < getAdjacentEntities().size(); i++) {

		Entity *en = getAdjacentEntity(i);

		for (int j = 0; j < en->getIncidentRelationships().size(); j++) {

			RE *re = en->getIncidentRelationship(j);
			if (re->getCardinality() == 1)
			{
				re->initializeMonogonOrientation();
				re->create2DScaffolds();
			}
		}
	}
}

bool Entity::distributeMonogons()
{
	vector<Relationship*> monogons;
	vector<pair<double, double>> angles_covered;
	vector<pair<double, double>> angles_gap;

	for (auto re : getIncidentRelationships())
	{
		if (re->getCardinality() == 1)
		{
			monogons.push_back(re);
			continue;
		}
		else if (re->getCardinality() == 2)
		{
			double angle_offset = (re->m_DigonLensAngle/2.0) * (M_PI/180.0);
			
			Vec3 dir = re->getCenter(0.0) - getLocation();

			double angle1 = atan2(dir.y, dir.x) - angle_offset;
			if (angle1 < 0.0)
				angle1 += 2*M_PI;

			double angle2 = atan2(dir.y, dir.x) + angle_offset;
			if (angle2 < 0.0)
				angle2 += 2*M_PI;

			angles_covered.push_back(pair<double, double>(angle1, angle2));
		}
		else
		{
			double min1 = DBL_MAX; double min2 = DBL_MAX;
			Entity* near1 = NULL; Entity* near2 = NULL;
			for (auto adjen : re->getIncidentEntities())
			{
				double dist = length(adjen->getLocation() - getLocation());
				if (dist == 0.0)
				{
					continue;
				}
				if (dist < min1)
				{
					near2 = near1;
					min2 = min1;
					near1 = adjen;
					min1 = dist;
				}
				else if (dist < min2)
				{
					near2 = adjen;
					min2 = dist;
				}
			}

			Vec3 dir1 = near1->getLocation() - getLocation();
			double angle1 = atan2(dir1.y, dir1.x);
			if (angle1 < 0.0)
				angle1 += 2*M_PI;

			Vec3 dir2 = near2->getLocation() - getLocation();
			double angle2 = atan2(dir2.y, dir2.x);
			if (angle2 < 0.0)
				angle2 += 2*M_PI;

			double angle_diff = angle2 - angle1;
			if (angle_diff < 0.0)
				angle_diff += 2*M_PI;

			if (angle_diff < M_PI)
			{
				if (angle2 < angle1)
					angle2 += 2 * M_PI;
				angles_covered.push_back(pair<double, double>(angle1, angle2));
			}
			else
			{
				if (angle1 < angle2)
					angle1 += 2 * M_PI;
				angles_covered.push_back(pair<double, double>(angle2, angle1));
			}
		}
	}

	if (angles_covered.size() == 0)
		return false;
	else if (monogons.size() == 0)
		return true;

	std::sort(angles_covered.begin(), angles_covered.end());
	int start_idx = 0;
	int end_idx = start_idx + 1;
	int loop = end_idx / angles_covered.size();
	end_idx = end_idx % angles_covered.size();
	double gap_total = 0;
	bool end = false;
	while (!end)
	{
		end = (end_idx == 0);
		double gap_start = angles_covered[start_idx].second;
		double gap_end = angles_covered[end_idx].first;
		if (gap_start > gap_end && loop == 1)
			gap_end += 2 * M_PI;

		if (gap_start < gap_end)
		{
			double diff = gap_end - gap_start;
			if (diff < 0)
				diff += 2*M_PI;
			angles_gap.push_back(pair<double, double>(diff, gap_start));
			gap_total += diff;

			start_idx = end_idx;
			end_idx = start_idx + 1;
			loop = end_idx / angles_covered.size();
			end_idx = end_idx % angles_covered.size();
		}
		else if (gap_start < angles_covered[end_idx].second)
		{
			start_idx = end_idx;
			end_idx = start_idx + 1;
			loop = end_idx / angles_covered.size();
			end_idx = end_idx % angles_covered.size();
		}
		else
		{
			end_idx++;
			loop = end_idx / angles_covered.size();
			end_idx = end_idx % angles_covered.size();
		}
	}

	int total_monogons = monogons.size();
	if (gap_total < (total_monogons * 0.3696))
		return false;

	std::sort(angles_gap.begin(), angles_gap.end());

	for (pair<double, double> gap : angles_gap)
	{
		double ratio = gap.first / gap_total;
		int num_monogons = (int)(ratio * total_monogons + 0.5);
		if (num_monogons < 1)
		{
			gap_total -= gap.first;
			continue;
		}

		double angle_per_monogon = gap.first / (double)(num_monogons+1);
		double angle = gap.second + angle_per_monogon;
		for (int i = 0; i < num_monogons; i++)
		{
			if (monogons.empty())
				continue;
			monogons.back()->setMonogonRotation(angle);
			angle += angle_per_monogon;
			monogons.pop_back();
		}
	}

	return true;
}

void Entity::setSwapFlag(bool res) {
	m_SelectedForSwap = res;
}

bool Entity::getSwapFlag() {
	return m_SelectedForSwap;
}

Entity* Entity::getAdjacentEntity(int index) {
	return m_adjacentEntities[index];
}

Relationship* Entity::getIncidentRelationship(int index) {
	return m_incidentRelationships[index];
}

int Entity::getCardinality() const {
	return m_adjacentEntities.size();
}

int Entity::getDegree() const {
	return m_incidentRelationships.size();
}

bool Entity::checkIncident(Relationship* re)
{
	return isRelationshipInsideRelationshipList(re, getIncidentRelationships());
}

bool Entity::checkAdjacent(Entity* en)
{
	return isEntityInsideEntityList(en, getAdjacentEntities());
}

void Entity::clearAdjacentEntities() {
	m_adjacentEntities.clear();
}

void Entity::clearIncidentRelationships() {
	m_incidentRelationships.clear();
}

void Entity::moveToLocation(double x, double y, STARRIZATIONR_FLAG starrizeFlag, UPDATESCAFFOLD_FLAG updateScaffoldFlag) {

    setLocation(x, y);
	if (starrizeFlag) {
        starrizeIncidentRes();
	}
    if(updateScaffoldFlag)
        createIncidentRe2DScaffolds();
}

void Entity::translate(double dx, double dy, STARRIZATIONR_FLAG flag1, UPDATESCAFFOLD_FLAG flag2)
{
    double x = m_Location.x;
    double y = m_Location.y;
    moveToLocation(x+dx, y+dy, flag1, flag2);
}

void Entity::translate(Vec3 d, STARRIZATIONR_FLAG starrizeFlag, UPDATESCAFFOLD_FLAG updateScaffoldFlag)
{
    setLocation(m_Location + d);

	if (starrizeFlag) {
        starrizeIncidentRes();
	}
    if (updateScaffoldFlag)
        createIncidentRe2DScaffolds();
}

void Entity::updateIncidentRes()
{
	for (int i = 0; i < m_incidentRelationships.size(); i++) {
		Relationship *r = m_incidentRelationships[i];
		r->update2D();
	}
}

void Entity::starrizeIncidentRes()
{
    for (int i = 0; i < m_incidentRelationships.size(); i++) {
        Relationship *r = m_incidentRelationships[i];
        r->starrize();
    }
}

void Entity::createIncidentRe2DScaffolds() {

	for (int i = 0; i < m_incidentRelationships.size(); i++) {
		Relationship *r = m_incidentRelationships[i];
        r->create2DScaffolds();
	}
}

vector<string> Entity::getAttributes()
{
	return attributes;
}

void Entity::clearAttributes()
{
	attributes.clear();
}

void Entity::setAttributes(vector<string> attri)
{
	attributes = attri;
}

//void Entity::backupLocation()
//{
//    backup_loc = getLocation();
//}
//
//void Entity::resumeLocation()
//{
//    setLocation(backup_loc);
//}

void Entity::getIncidentRelationshipIds(vector<int>& ids, int offset)
{
	for (Relationship* re : m_incidentRelationships)
		ids.push_back(re->getIndex() + offset);
}