#pragma once

#include "globalVariables.h"
#include "ERData.h"
#include "ElementUtils.h"

class ERBicoComp;
class ERTreeComp;
class ERBridge;
class ERCycle;
class ERForbidden;
class ERCluster;

struct CycleCount
{
	std::vector<ERCycle*> cycles;

	int long_count = 0;
	int shortest_len = INT_MAX;
	int longest_len = 0;
	float avg_len = 0;

	CycleCount() {}
	CycleCount(int count, int shortest, int longest, float avg) :
		long_count(count),
		shortest_len(shortest),
		longest_len(longest),
		avg_len(avg) {}
};

class ERBicoComp
{
private:
	int m_index = 0;
	std::unordered_set<Element*> m_elemset;
	std::vector<Element*> m_elements;

	std::unordered_set<ERBicoComp*> m_incidentBicoComps;
	std::unordered_set<ERTreeComp*> m_incidentTreeComps;

	std::vector<ERCycle*> m_cycles;
	std::unordered_map<Element*, std::vector<ERCycle*>> m_elemCycleMap;
	std::unordered_map<int, int> m_adjacency;

	std::vector<ERForbidden*> m_forbiddens;
	std::unordered_map<ERCycle*, std::vector<ERForbidden*>> m_cycleForbiddenMap;
	std::vector<ERCluster*> m_clusters;
	std::unordered_map<ERForbidden*, ERCluster*> m_forbiddenClusterMap;

	std::unordered_map<ERCycle*, ERCluster*> m_cycleClusterMap;
	
public:
	
	ERBicoComp();
	~ERBicoComp();

	void setIndex(int idx);
	int getIndex();
	bool addElement(Element* e);
	bool containsElement(Element* e);
	bool replaceElement(Element* olde, Element* newe);
	std::vector<Element*> getElements();
	Element* getElement(int idx);
	int getNumElements();
	void getElemCounts(int &num_ens, int &num_res);

	void addIncidentBicoComp(ERBicoComp* bcomp);
	void addIncidentTreeComp(ERTreeComp* tcomp);

	void addCycle(ERCycle* cycle);
	std::vector<ERCycle*> getCycles();
	int getNumCycles();
	bool checkLinearIndependent(ERCycle* cycle, Element* root);
	
	void initAdjacency();
	void initAdjacencyMatrix();
	void printAdjacencyMatrix();
	void addForbidden(ERForbidden* fb);
	void addCycleToForbidden(ERCycle* c, ERForbidden* fb);
	void findForbiddens();
	ERForbidden* backTraceForbidden(ERCycle* source, ERCycle* target, std::unordered_map<int, int> adjacency);
	void processForbiddens(std::vector<ERForbidden*> candidates, bool merge_strangled, bool merge_saturated, bool merge_nadjacent);
	std::vector<ERForbidden*> getForbiddens();
	int getNumForbiddens();

	void addCluster(ERCluster* cluster);
	void findClusters();
	std::vector<ERCluster*> getClusters();
	ERCluster* getCluster(int idx);
	int getNumClusters();

	void updateAdjacency();
	void updateElemCycleMap(Element* e);
	void checkNewAdjacency(std::vector<ERCycle*> cycles);
	void findBiconnectedClusters();
	void addCycleToCluster(ERCluster* cl, ERCycle* cy);
	void processClusters(std::vector<ERCluster*> clusters);

	ERCluster* getElementCluster(Element* e);
	int getNumLongCycles(int len);

	CycleCount getCycleCountsForEdge(std::pair<Element*, Element*> edge);
	CycleCount getCycleCountsForPair(Element* e1, Element* e2);
	std::vector<int> getCyclesForPair(Element* e1, Element* e2);
};

class ERTreeComp
{
private:
	int m_index = 0;
	std::unordered_set<Element*> m_elements;

	std::unordered_set<ERBicoComp*> m_incidentBicoComps;

public:

	ERTreeComp();
	~ERTreeComp();

	void setIndex(int idx);
	int getIndex();
	bool addElement(Element* e);
	bool containsElement(Element* e);
	bool replaceElement(Element* olde, Element* newe);
	std::unordered_set<Element*> getElements();

	void addIncidentBicoComp(ERBicoComp* bcomp);
};



class ERBridge
{
private:
	int m_index = 0;
	std::vector<Element*> m_elements;
	std::vector<ERCycle*> m_incidentCycles;
	std::vector<Element*> m_incidentElements;

public:

	ERBridge();
	ERBridge(std::vector<Element*> elements);
	ERBridge(Entity* en);
	ERBridge(Relationship* re);
	~ERBridge();

	void setIndex(int idx);
	int getIndex();
	int getNumElements();
	std::vector<Element*> getElements();
	int findElement(Element* e);
	void addElement(Element* e);
	std::vector<ERCycle*> getIncidentCycles();
	void addIncidentCycle(ERCycle* c, bool symmetric = true);
	void clearCycleIncidence();
	std::vector<Element*> getIncidentElements();
	int getNumIncidentElements();
	Element* getIncidentElement(int idx);
	void addIncidentElement(Element* e);
};



class ERCycle
{
private:

	int m_index;
	std::vector<Element*> m_elements;
	std::vector<ERCycle*> m_adjacent;
	std::vector<ERBridge*> m_incident;

public:

	bool to_delete = false;

public:

	ERCycle();
	ERCycle(std::vector<Element*> elements);
	~ERCycle();

	void setIndex(int idx);
	int getIndex();
	int getLength();
	std::vector<Element*> getElements();
	Element* getElement(int idx);
	int findElement(Element* e);
	void replaceElement(Element* olde, Element* newe);
	std::vector<ERCycle*> getAdjacentCycles();
	bool containsEdge(Element* e1, Element* e2);
	bool containsElement(Element* e1);
	void addAdjacentCycle(ERCycle* c, bool symmetric = true);
	void removeAdjacentCycle(ERCycle* c);
	void clearAdjacency();
	void addIncidentBridge(ERBridge* b, bool symmetric = true);
	void clearIncidence();
};



enum FORBIDDENTYPE
{
	NADJACENT,
	STRANGLED,
	SATURATED,
};

class ERForbidden
{
private:

	int m_index;
	FORBIDDENTYPE m_type;
	std::vector<ERCycle*> m_Cycles;
	std::vector<Element*> m_CommonElems;

public:

	ERForbidden();
	ERForbidden(std::vector<ERCycle*> cycles);
	~ERForbidden();

	void setCycles(std::vector<ERCycle*> cycles);
	std::vector<ERCycle*> getCycles();
	ERCycle* getCycle(int idx);
	int getNumCycles();
	void addCycle(ERCycle* cycle);
	std::vector<Element*> getElements();
	int findCommonElements();
	void setCommonElements(std::vector<Element*> common);
	std::vector<Element*> getCommonElements();
	Element* getCommonElement(int idx);
	void setIndex(int idx);
	int getIndex();
	void setType(FORBIDDENTYPE type);
	FORBIDDENTYPE getType();
	bool merge(ERForbidden* other);
	bool tryMerge(ERForbidden* other);
	
};



class ERCluster
{
private:

	int m_index;
	float m_entanglement;
	std::unordered_set<ERCycle*> m_cycleset;
	std::vector<ERCycle*> m_cycles;
	std::unordered_map<int, int> m_adjacency;
	std::vector<ERForbidden*> m_forbiddens;

public:

	ERCluster();
	ERCluster(std::vector<ERForbidden*> forbiddens);
	~ERCluster();

	void setIndex(int idx);
	int getIndex();
	bool addCycle(ERCycle* cycle);
	void removeCycle(ERCycle* cycle);
	std::vector<ERCycle*> getCycles();
	int getNumCycles();
	bool containsCycle(ERCycle* cycle);
	void addForbidden(ERForbidden* fb);
	void removeForbidden(ERForbidden* fb);
	std::vector<ERForbidden*> getForbiddens();
	ERForbidden* getForbidden(int idx);
	int getNumForbiddens();
	std::vector<Element*> getElements();
	std::unordered_set<Element*> getElemSet();
	int getNumElements();
	bool tryMerge(ERCluster* other);

	void initAdjacencyMatrix();
	bool findForbiddens();
	ERForbidden* backTraceForbidden(ERCycle* source, ERCycle* target, std::unordered_map<int, int> adjacency);
	void processForbiddens(std::vector<ERForbidden*> forbiddens, bool merge_strangled, bool merge_saturated, bool merge_nadjacent);

	void calcEntanglement();
	float getEntanglement();
};



// Structure Utilities
int getNumCommonEdges(ERCycle* c, ERCycle* a);