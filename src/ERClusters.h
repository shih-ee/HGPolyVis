#pragma once

#include "globalVariables.h"
#include "ERData.h"
#include "ElementUtils.h"
#include "ColorGradient.h"


class ERSubgraph;
class Cluster;
class Bridge;





///////////////////////////////////////////////////////////////
// ERSubgraph
///////////////////////////////////////////////////////////////

class ERSubgraph
{
protected:

	std::unordered_set<int> m_reset;
	std::unordered_set<int> m_enset;
	Vec4 m_color = Vec4(0.9375, 0.9375, 0.9375, 0.5);

public:

	ERSubgraph();
	ERSubgraph(int idx, ELEMENTTYPE type);
	~ERSubgraph();

	std::unordered_set<int> getElementSet(ELEMENTTYPE type);
	int getNumElements(ELEMENTTYPE type);
	bool addElement(int idx, ELEMENTTYPE type);
	void removeElement(int idx, ELEMENTTYPE type);
	bool containsElement(int idx, ELEMENTTYPE type);

	Vec4 getColor();
	void setColor(Vec4 color);
};






///////////////////////////////////////////////////////////////
// Bridge
///////////////////////////////////////////////////////////////

class Bridge : public ERSubgraph
{
public:

	Bridge();
	Bridge(int idx, ELEMENTTYPE type);
	~Bridge();
};






///////////////////////////////////////////////////////////////
// Cluster
///////////////////////////////////////////////////////////////

struct ClusterLayout
{
	double radius;
	Vec3 center;
	double barcode_radius;
	double barcode_step;
	double barcode_rotation;

	ClusterLayout()
	{
		radius = 0.0;
		center = Vec3(0.0, 0.0, 0.0);
		barcode_radius = 0.5;
		barcode_step = 0.25;
		barcode_rotation = 0.0;
	}
	
	ClusterLayout(double r, Vec3 c, double br, double bs, double bt)
	{
		radius = r;
		center = c;
		barcode_radius = br;
		barcode_step = bs;
		barcode_rotation = bt;
	}
};

class Cluster : public ERSubgraph
{
private:

	ClusterLayout* m_layout = NULL;

public:
	
	Cluster();
	Cluster(int idx, ELEMENTTYPE type);
	~Cluster();

	void setLayout(ClusterLayout* layout);
	ClusterLayout* getLayout();
};






///////////////////////////////////////////////////////////////
// ERClusterDecomp
///////////////////////////////////////////////////////////////

class ERClusterDecomp
{
private:

	ERData* m_ERData;
	ERData* m_dualERData;
	std::unordered_set<Cluster*> m_Clusters;
	std::unordered_set<Bridge*> m_Bridges;
	std::unordered_map<int, Cluster*> m_reClusterMap;
	std::unordered_map<int, Cluster*> m_enClusterMap;
	std::unordered_map<int, Bridge*> m_reBridgeMap;
	std::unordered_map<int, Bridge*> m_enBridgeMap;

public:
	
	ERClusterDecomp(ERData* erdata);
	~ERClusterDecomp();
	void initEmpty();
	void initDecomp();

	// accessors
	ERData* getERData();
	ERData* getDualData();
	std::vector<Cluster*> getClusters();
	std::vector<Bridge*> getBridges();
	Cluster* getElemCluster(int idx, ELEMENTTYPE type);
	Bridge* getElemBridge(int idx, ELEMENTTYPE type);

	// utilities
	bool addToCluster(int idx, Cluster* c, ELEMENTTYPE type);
	bool addToBridge(int idx, Bridge* b, ELEMENTTYPE type);
	void removeFromCluster(int idx, Cluster* c, ELEMENTTYPE type);
	void removeFromBridge(int idx, Bridge* b, ELEMENTTYPE type);
	void mergeClusters(Cluster* c1, Cluster* c2);
	void mergeBridges(Bridge* b1, Bridge* b2);
	bool entityHasExteriorConnection(Entity* en, ERSubgraph* s);
	int getEntityInteriorDegree(Entity* en, ERSubgraph* s);
	ENList getSubgraphEntities(ERSubgraph* s, ELEMENT_STATISTIC sort = ELEM_NONE, bool ascending = true, Vec3* get_center = NULL);
	REList getSubgraphRelationships(ERSubgraph* s, ELEMENT_STATISTIC sort = ELEM_NONE, bool ascending = true);
	ENList getSubgraphIncidentEntities(ERSubgraph* s, ELEMENT_STATISTIC sort = ELEM_NONE, bool ascending = true, Vec3* get_center = NULL);
	REList getSubgraphIncidentRlationships(ERSubgraph* s, ELEMENT_STATISTIC sort = ELEM_NONE, bool ascending = true);
	Cluster* createCluster(ENList enlist, REList relist);
	
	// algorithms
	void Decompose();

	// visualization
	void createClusterLayouts();
	void createClusterLayout(Cluster* c);
	void updateClusterLayouts();
	void updateClusterLayout(Cluster* c);
	void scaleClusterLayout(Cluster* c, double new_radius);
	void assignColors(ColorGradient cg_c, ColorGradient cg_b);
	void setSubgraphColor(ERSubgraph* s, Vec4 color, bool elements_inherit_color = false);
};


