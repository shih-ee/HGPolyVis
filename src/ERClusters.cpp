#include "ERClusters.h"
#include "mainwindow.h"


///////////////////////////////////////////////////////////////
// ERSubgraph
///////////////////////////////////////////////////////////////

ERSubgraph::ERSubgraph()
{
}

ERSubgraph::ERSubgraph(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		m_reset.insert(idx);
	else if (type == ENTITY)
		m_enset.insert(idx);
}

ERSubgraph::~ERSubgraph()
{
}


std::unordered_set<int> ERSubgraph::getElementSet(ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reset;
	else if (type == ENTITY)
		return m_enset;
}

int ERSubgraph::getNumElements(ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reset.size();
	else if (type == ENTITY)
		return m_enset.size();
}

bool ERSubgraph::addElement(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reset.insert(idx).second;
	else if (type == ENTITY)
		return m_enset.insert(idx).second;

	return false;
}

void ERSubgraph::removeElement(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		m_reset.erase(idx);
	else if (type == ENTITY)
		m_enset.erase(idx);
}

bool ERSubgraph::containsElement(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reset.find(idx) != m_reset.end();
	else if (type == ENTITY)
		return m_enset.find(idx) != m_enset.end();
}


Vec4 ERSubgraph::getColor()
{
	return m_color;
}

void ERSubgraph::setColor(Vec4 color)
{
	m_color = color;
}






///////////////////////////////////////////////////////////////
// Bridge
///////////////////////////////////////////////////////////////

Bridge::Bridge()
	:ERSubgraph()
{
}

Bridge::Bridge(int idx, ELEMENTTYPE type)
	: ERSubgraph(idx, type)
{
}

Bridge::~Bridge()
{
}






///////////////////////////////////////////////////////////////
// Cluster
///////////////////////////////////////////////////////////////

Cluster::Cluster()
	:ERSubgraph()
{
}

Cluster::Cluster(int idx, ELEMENTTYPE type)
	: ERSubgraph(idx, type)
{
}

Cluster::~Cluster()
{
}

void Cluster::setLayout(ClusterLayout* layout)
{
	if (m_layout)
		delete(m_layout);
	m_layout = layout;
}

ClusterLayout* Cluster::getLayout()
{
	return m_layout;
}






///////////////////////////////////////////////////////////////
// ERClusterDecomp
///////////////////////////////////////////////////////////////

ERClusterDecomp::ERClusterDecomp(ERData* erdata)
{
	m_ERData = erdata;
	m_dualERData = NULL;
}

ERClusterDecomp::~ERClusterDecomp()
{
	for (Cluster* c : m_Clusters)
		delete(c);

	for (Bridge* b : m_Bridges)
		delete(b);

	m_ERData = NULL;
	m_dualERData = NULL;
}

void ERClusterDecomp::initEmpty()
{
	for (Relationship* re : m_ERData->getRelationships())
	{
		int idx = re->getIndex();
		m_reClusterMap.insert(std::make_pair(idx, nullptr));
		m_reBridgeMap.insert(std::make_pair(idx, nullptr));
	}

	for (Entity* en : m_ERData->getEntities())
	{
		int idx = en->getIndex();
		m_enClusterMap.insert(std::make_pair(idx, nullptr));
		m_enBridgeMap.insert(std::make_pair(idx, nullptr));
	}
}

void ERClusterDecomp::initDecomp()
{
	for (Relationship* re : m_ERData->getRelationships())
	{
		int idx = re->getIndex();

		Cluster* c = new Cluster(idx, RELATIONSHIP);
		Bridge* b = new Bridge(idx, RELATIONSHIP);

		m_reClusterMap.insert(std::make_pair(idx, c));
		m_reBridgeMap.insert(std::make_pair(idx, b));

		m_Clusters.insert(c);
		m_Bridges.insert(b);
	}

	for (Entity* en : m_ERData->getEntities())
	{
		int idx = en->getIndex();

		Cluster* c = new Cluster(idx, ENTITY);
		Bridge* b = new Bridge(idx, ENTITY);

		m_enClusterMap.insert(std::make_pair(idx, c));
		m_enBridgeMap.insert(std::make_pair(idx, b));

		m_Clusters.insert(c);
		m_Bridges.insert(b);
	}
}


// accessors

ERData* ERClusterDecomp::getERData()
{
	return m_ERData;
}

ERData* ERClusterDecomp::getDualData()
{
	return m_dualERData;
}

std::vector<Cluster*> ERClusterDecomp::getClusters()
{
	std::vector<Cluster*> clusters;
	clusters.reserve(m_Clusters.size());
	clusters.insert(clusters.end(), m_Clusters.begin(), m_Clusters.end());
	return clusters;
}

std::vector<Bridge*> ERClusterDecomp::getBridges()
{
	std::vector<Bridge*> bridges;
	bridges.reserve(m_Bridges.size());
	bridges.insert(bridges.end(), m_Bridges.begin(), m_Bridges.end());
	return bridges;
}

Cluster* ERClusterDecomp::getElemCluster(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reClusterMap.at(idx);
	else if (type == ENTITY)
		return m_enClusterMap.at(idx);
}

Bridge* ERClusterDecomp::getElemBridge(int idx, ELEMENTTYPE type)
{
	if (type == RELATIONSHIP)
		return m_reBridgeMap.at(idx);
	else if (type == ENTITY)
		return m_enBridgeMap.at(idx);
}


// utilities

bool ERClusterDecomp::addToCluster(int idx, Cluster* c, ELEMENTTYPE type)
{
	m_Clusters.insert(c);
	bool added = c->addElement(idx, type);
	if (type == RELATIONSHIP)
		m_reClusterMap.at(idx) = c;
	else if (type == ENTITY)
		m_enClusterMap.at(idx) = c;
	return added;
}

bool ERClusterDecomp::addToBridge(int idx, Bridge* b, ELEMENTTYPE type)
{
	m_Bridges.insert(b);
	bool added = b->addElement(idx, type);
	if (type == RELATIONSHIP)
		m_reBridgeMap.at(idx) = b;
	else if (type == ENTITY)
		m_enBridgeMap.at(idx) = b;
	return added;
}

void ERClusterDecomp::removeFromCluster(int idx, Cluster* c, ELEMENTTYPE type)
{
	c->removeElement(idx, type);

	if (type == RELATIONSHIP)
		m_reClusterMap.at(idx) = NULL;
	else if (type == ENTITY)
		m_enClusterMap.at(idx) = NULL;

	if (c->getNumElements(RELATIONSHIP) == 0 &&
		c->getNumElements(ENTITY) == 0)
	{
		m_Clusters.erase(c);
		delete(c);
	}
}

void ERClusterDecomp::removeFromBridge(int idx, Bridge* b, ELEMENTTYPE type)
{
	b->removeElement(idx, type);
	
	if (type == RELATIONSHIP)
		m_reBridgeMap.at(idx) = NULL;
	else if (type == ENTITY)
		m_enBridgeMap.at(idx) = NULL;

	if (b->getNumElements(RELATIONSHIP) == 0 && 
		b->getNumElements(ENTITY) == 0)
	{
		m_Bridges.erase(b);
		delete(b);
	}
}

void ERClusterDecomp::mergeClusters(Cluster* c1, Cluster* c2)
{
	if (c1 == c2)
		return;

	for (int re_idx : c2->getElementSet(RELATIONSHIP))
		addToCluster(re_idx, c1, RELATIONSHIP);
	
	for (int en_idx : c2->getElementSet(ENTITY))
		addToCluster(en_idx, c1, ENTITY);

	m_Clusters.erase(c2);
	delete(c2);
}

void ERClusterDecomp::mergeBridges(Bridge* b1, Bridge* b2)
{
	if (b1 == b2)
		return;

	for (int re_idx : b2->getElementSet(RELATIONSHIP))
		addToBridge(re_idx, b1, RELATIONSHIP);

	for (int en_idx : b2->getElementSet(ENTITY))
		addToBridge(en_idx, b1, ENTITY);

	m_Bridges.erase(b2);
	delete(b2);
}

bool ERClusterDecomp::entityHasExteriorConnection(Entity* en, ERSubgraph* s)
{
	for (Relationship* re : en->getIncidentRelationships())
		if (!s->containsElement(re->getIndex(), RELATIONSHIP))
			return true;

	return false;
}

int ERClusterDecomp::getEntityInteriorDegree(Entity* en, ERSubgraph* s)
{
	int degree = 0;
	for (Relationship* re : en->getIncidentRelationships())
		if (s->containsElement(re->getIndex(), RELATIONSHIP))
			degree += 1;
	return degree;
}

ENList ERClusterDecomp::getSubgraphEntities(ERSubgraph* s, ELEMENT_STATISTIC sort, bool ascending, Vec3* get_center)
{
	ENList enlist; enlist.reserve(s->getNumElements(ENTITY));
	Vec3 center = Vec3(0, 0, 0);
	for (int idx : s->getElementSet(ENTITY))
	{
		Entity* en = m_ERData->getEntity(idx);
		enlist.push_back(en);
		center += en->getLocation();
	}
	center = center / (double)enlist.size();

	if (sort == ELEM_INTERIOR_INCIDENCE)
		std::sort(enlist.begin(), enlist.end(), [this, s, &ascending](Entity* left, Entity* right)
			{
				return ascending ? this->getEntityInteriorDegree(left, s) < this->getEntityInteriorDegree(right, s)
					: this->getEntityInteriorDegree(left, s) > this->getEntityInteriorDegree(right, s);
				/*int ldeg = this->getEntityInteriorDegree(left, s);
				int rdeg = this->getEntityInteriorDegree(right, s);
				if (ldeg == rdeg)
					return ascending ? left->getIndex() < right->getIndex() : left->getIndex() > right->getIndex();
				else
					return ascending ? ldeg < rdeg : ldeg > rdeg;*/
			});
	else if (sort != ELEM_NONE)
		std::sort(enlist.begin(), enlist.end(), EntityCompare(sort, ascending, center));

	if (get_center)
	{
		get_center->x = center.x;
		get_center->y = center.y;
		get_center->z = center.z;
	}
	return enlist;
}

REList ERClusterDecomp::getSubgraphRelationships(ERSubgraph* s, ELEMENT_STATISTIC sort, bool ascending)
{
	REList relist; relist.reserve(s->getNumElements(RELATIONSHIP));
	for (int idx : s->getElementSet(RELATIONSHIP))
		relist.push_back(m_ERData->getRelationship(idx));
	if (sort != ELEM_NONE)
		std::sort(relist.begin(), relist.end(), RelationshipCompare(sort, ascending));
	return relist;
}

ENList ERClusterDecomp::getSubgraphIncidentEntities(ERSubgraph* s, ELEMENT_STATISTIC sort, bool ascending, Vec3* get_center)
{
	ENList enlist;
	Vec3 center = Vec3(0, 0, 0);
	for (int idx : s->getElementSet(RELATIONSHIP))
	{
		Relationship* re = m_ERData->getRelationship(idx);
		for (Entity* en : re->getIncidentEntities())
		{
			if (!isEntityInsideEntityList(en, enlist))
			{
				enlist.push_back(en);
				center += en->getLocation();
			}
		}	
	}
	center = center / (double)enlist.size();

	if (sort == ELEM_INTERIOR_INCIDENCE)
		std::sort(enlist.begin(), enlist.end(), [this, s, &ascending](Entity* left, Entity* right)
			{
				return ascending ? this->getEntityInteriorDegree(left, s) < this->getEntityInteriorDegree(right, s)
					: this->getEntityInteriorDegree(left, s) > this->getEntityInteriorDegree(right, s);
				/*int ldeg = this->getEntityInteriorDegree(left, s);
				int rdeg = this->getEntityInteriorDegree(right, s);
				if (ldeg == rdeg)
					return ascending ? left->getIndex() < right->getIndex() : left->getIndex() > right->getIndex();
				else
					return ascending ? ldeg < rdeg : ldeg > rdeg;*/
			});
	else if (sort != ELEM_NONE)
		std::sort(enlist.begin(), enlist.end(), EntityCompare(sort, true, center));

	if (get_center)
	{
		get_center->x = center.x;
		get_center->y = center.y;
		get_center->z = center.z;
	}
	return enlist;
}

REList ERClusterDecomp::getSubgraphIncidentRlationships(ERSubgraph* s, ELEMENT_STATISTIC sort, bool ascending)
{
	REList relist;
	for (int idx : s->getElementSet(ENTITY))
	{
		Entity* en = m_ERData->getEntity(idx);
		for (Relationship* re : en->getIncidentRelationships())
			if (!isRelationshipInsideRelationshipList(re, relist))
				relist.push_back(re);
	}
	if (sort != ELEM_NONE)
		std::sort(relist.begin(), relist.end(), RelationshipCompare(sort, ascending));
	return relist;
}


// manual decomposition

Cluster* ERClusterDecomp::createCluster(ENList enlist, REList relist)
{
	Cluster* c = new Cluster();
	
	for (Entity* en : enlist)
	{
		int idx = en->getIndex();
		c->addElement(idx, ENTITY);
		m_enClusterMap.at(idx) = c;
	}
	for (Relationship* re : relist)
	{
		int idx = re->getIndex();
		c->addElement(idx, RELATIONSHIP);
		m_reClusterMap.at(idx) = c;
	}
	
	m_Clusters.insert(c);
	return c;
}


// algorithms

void ERClusterDecomp::Decompose()
{

	for (Relationship* re : m_ERData->getRelationships())
	{
		int re_idx = re->getIndex();
		Cluster* re_c = getElemCluster(re_idx, RELATIONSHIP);
		Bridge* re_b = getElemBridge(re_idx, RELATIONSHIP);

		for (Relationship* adj : re->getAdjacentRelationships())
		{
			int adj_idx = adj->getIndex();
			if (adj_idx < re_idx)
				continue;

			Cluster* adj_c = getElemCluster(adj_idx, RELATIONSHIP);
			Bridge* adj_b = getElemBridge(adj_idx, RELATIONSHIP);

			ENList shared = getSharedEntities(re, adj);
			if (shared.size() > 2)
			{
				mergeClusters(re_c, adj_c);
				if (re_b)
				{
					removeFromBridge(re_idx, re_b, RELATIONSHIP);
					re_b = NULL;
				}
				if (adj_b)
					removeFromBridge(adj_idx, adj_b, RELATIONSHIP);

				for (Entity* en : shared)
				{
					int en_idx = en->getIndex();
					Cluster* en_c = getElemCluster(en_idx, ENTITY);
					Bridge* en_b = getElemBridge(en_idx, ENTITY);
					mergeClusters(re_c, en_c);
					if (en_b)
						removeFromBridge(en_idx, en_b, ENTITY);
				}
			}
		}
	}

	for (Entity* en : m_ERData->getEntities())
	{
		int en_idx = en->getIndex();
		Cluster* en_c = getElemCluster(en_idx, ENTITY);
		Bridge* en_b = getElemBridge(en_idx, ENTITY);

		for (Entity* adj : en->getAdjacentEntities())
		{
			int adj_idx = adj->getIndex();
			if (adj_idx < en_idx)
				continue;

			Cluster* adj_c = getElemCluster(adj_idx, ENTITY);
			Bridge* adj_b = getElemBridge(adj_idx, ENTITY);

			REList shared = getSharedRelationships(en, adj);
			if (shared.size() > 2)
			{
				mergeClusters(en_c, adj_c);
				if (en_b)
				{
					removeFromBridge(en_idx, en_b, ENTITY);
					en_b = NULL;
				}
				if (adj_b)
					removeFromBridge(adj_idx, adj_b, ENTITY);

				for (Relationship* re : shared)
				{
					int re_idx = re->getIndex();
					Cluster* re_c = getElemCluster(re_idx, RELATIONSHIP);
					Bridge* re_b = getElemBridge(re_idx, RELATIONSHIP);
					mergeClusters(en_c, re_c);
					if (re_b)
						removeFromBridge(re_idx, re_b, RELATIONSHIP);
				}
			}
		}
	}

	std::vector<Cluster*> clusters;
	clusters.insert(clusters.end(), m_Clusters.begin(), m_Clusters.end());
	for (Cluster* c : clusters)
	{
		if (c->getNumElements(RELATIONSHIP) > 1 || c->getNumElements(ENTITY) > 1)
			continue;

		for (int idx : c->getElementSet(RELATIONSHIP))
			m_reClusterMap.at(idx) = NULL;
		for (int idx : c->getElementSet(ENTITY))
			m_enClusterMap.at(idx) = NULL;

		m_Clusters.erase(c);
		delete(c);
	}

	for (Relationship* re : m_ERData->getRelationships())
	{
		int re_idx = re->getIndex();
		if (getElemCluster(re_idx, RELATIONSHIP))
			continue;

		Bridge* re_b = getElemBridge(re_idx, RELATIONSHIP);

		for (Entity* en : re->getIncidentEntities())
		{
			int en_idx = en->getIndex();
			if (getElemCluster(en_idx, ENTITY))
				continue;

			Bridge* en_b = getElemBridge(en_idx, ENTITY);
			mergeBridges(re_b, en_b);
		}
	}
}


// visualization

void ERClusterDecomp::createClusterLayouts()
{
	for (Cluster* c : m_Clusters)
		createClusterLayout(c);
}

void ERClusterDecomp::createClusterLayout(Cluster* c)
{
	Vec3* temp = new Vec3(0, 0, 0);
	ENList enlist = getSubgraphIncidentEntities(c, ELEM_NONE, true, temp);

	double min_rad = DBL_MAX;
	double max_rad = 0;
	for (Entity* en : enlist)
	{
		Vec3 diff = en->getLocation() - *temp;
		double rad = length(diff);
		if (rad > max_rad)
			max_rad = rad;
		if (rad < min_rad)
			min_rad = rad;
	}

	int num_layers = c->getNumElements(RELATIONSHIP) + 3;
	double bar_step = min_rad / (double)num_layers;
	double bar_radius = 2 * bar_step;

	ClusterLayout* layout = new ClusterLayout(max_rad, *temp, bar_radius, bar_step, 0.0);
	c->setLayout(layout);

	delete(temp);
}

void ERClusterDecomp::updateClusterLayouts()
{
	for (Cluster* c : m_Clusters)
		updateClusterLayout(c);
}

void ERClusterDecomp::updateClusterLayout(Cluster* c)
{
	ClusterLayout* layout = c->getLayout();
	if (!layout)
		return;

	Vec3* temp = new Vec3(0, 0, 0);
	ENList enlist = getSubgraphIncidentEntities(c, ELEM_NONE, true, temp);

	double min_rad = DBL_MAX;
	double max_rad = 0;
	for (Entity* en : enlist)
	{
		Vec3 diff = en->getLocation() - *temp;
		double rad = length(diff);
		if (rad > max_rad)
			max_rad = rad;
		if (rad < min_rad)
			min_rad = rad;
	}

	int num_layers = c->getNumElements(RELATIONSHIP) + 3;
	
	layout->radius = max_rad;
	layout->center = *temp;
	layout->barcode_step = min_rad / (double)num_layers;
	layout->barcode_radius = layout->barcode_step * 2.0;
	
	delete(temp);
}

void ERClusterDecomp::scaleClusterLayout(Cluster* c, double new_radius)
{
	ENList enlist = getSubgraphIncidentEntities(c);
	ClusterLayout* layout = c->getLayout();
	layout->radius = new_radius;

	unordered_set<Relationship*> affected;
	for (Entity* en : enlist)
	{
		for (Relationship* re : en->getIncidentRelationships())
			affected.insert(re);

		Vec3 dir = en->getLocation() - layout->center;
		normalize(dir);
		Vec3 new_loc = layout->center + dir * new_radius;
		en->setLocation(new_loc);
	}

	for (Relationship* re : affected)
		re->update2D();
}

void ERClusterDecomp::assignColors(ColorGradient cgc, ColorGradient cgb)
{
	int max = m_Bridges.size();
	int bid = 0;
	for (Bridge* br : m_Bridges)
	{
		float r, g, b;
		float value = 2 * float(bid) / float(max);
		cgb.getColorAtValue(value, r, g, b);

		setSubgraphColor(br, Vec4(r, g, b, 0.5), gmainwindow->flag_subgraphsInheritColor);

		bid++;
	}

	max = m_Clusters.size();
	int cid = 0;
	for (Cluster* cl : m_Clusters)
	{
		float r, g, b;
		float value = 2 * float(cid) / float(max);
		cgc.getColorAtValue(value, r, g, b);

		setSubgraphColor(cl, Vec4(r, g, b, 0.5), gmainwindow->flag_subgraphsInheritColor);

		cid++;
	}
}

void ERClusterDecomp::setSubgraphColor(ERSubgraph* s, Vec4 color, bool elements_inherit_color)
{
	s->setColor(color);
	if (elements_inherit_color)
	{
		for (Relationship* re : getSubgraphRelationships(s))
			re->setColor(color);
		for (Entity* en : getSubgraphEntities(s))
			en->setColor(color);
	}
}