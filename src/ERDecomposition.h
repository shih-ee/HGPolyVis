#pragma once

#include "globalVariables.h"
#include "ERData.h"
#include "ElementUtils.h"
#include "ERDecompStructures.h"
#include "BipartOperation.h"

class ERCycleDecomp
{
private:

	ERData* m_ERData;
	std::vector<ERBicoComp*> m_BicoComps;
	std::vector<ERTreeComp*> m_TreeComps;
	std::vector<ERCycle*> m_Cycles;
	std::vector<ERCluster*> m_Clusters;

	std::unordered_map <Element*, std::vector<ERBicoComp*>> m_ElemBicoMap;
	std::unordered_map <Element*, ERTreeComp*> m_ElemTreeMap;
	std::unordered_map<Element*, std::vector<ERCycle*>> m_ElemCycleMap;
	std::unordered_map<ERCluster*, ERBicoComp*> m_ClusterBicoMap;

	bool** m_edges = NULL;
	std::unordered_map<int, int> m_adjacency;
	std::vector<std::pair<Element*, Element*>> m_cutEdges;

public:

	// constructors
	ERCycleDecomp(ERData* erdata);
	~ERCycleDecomp();

	// accessors
	ERData* getERData();
	std::vector<ERCycle*> getCycles(int max_len = 0);
	ERCycle* getCycle(int idx);
	std::vector<ERCluster*> getClusters();

	// algorithms
	void Decompose();
	void InitSimplifications();
	void computeBiconnectedComponents();
	void computeCycleBasis(ERBicoComp* comp);
	void initIncidenceMatrix();
	ERCycle* backTraceCycle(Element* en, Element* re, ERBicoComp* comp, int num_ens, int num_res);
	void processLongCycle(ERCycle* cycle, std::pair<Entity*, Relationship*> edge, ERBicoComp* bcomp);
	
	void findCutEdges(ERBicoComp* bcomp);
	void processCutEdges(std::vector<std::pair<Element*, Element*>> edges, ERBicoComp* bcomp);

	// utility
	void highlightElement(Element* e);
	int m_bchighlight = -1;
	void highlightNextBComp();
	void highlightAllBComps();
	int m_tchighlight = -1;
	void highlightNextTComp();
	void highlightAllTComps();
	int m_cyhighlight = -1;
	void highlightNextCycle();
	void highlightAllCycles();
	int m_clhighlight = -1;
	void highlightNextCluster();
	void highlightAllClusters();
	void resetHighlights();

	void addBicoComp(ERBicoComp* bcomp);
	void addBicoElement(ERBicoComp* bcomp, Element* e);
	void addTreeComp(ERTreeComp* tcomp);
	void addTreeElement(ERTreeComp* tcomp, Element* e);
	ERTreeComp* mergeTreeComps(ERTreeComp* t1, ERTreeComp* t2);
	std::vector<Element*> getTreeRoots(ERTreeComp* tcomp);

	void addCycle(ERCycle* cycle, ERBicoComp* bcomp = NULL);
	void removeCycle(ERCycle* cycle);
	void addCluster(ERCluster* cl, ERBicoComp* bcomp);
	
	int m_clusterSimpIndex = 0;
	bool m_recomputeForbiddens = true;
	int getNumClusters();
	BipartOperation* getNextClusterOp();
	ERBicoComp* getBicoComp(ERForbidden* fb);
	int getMergeDirection(std::vector<std::pair<Element*, Element*>> merges, ERBicoComp* bcomp);
	ERForbidden* getNextForbiddenSimp();
	ERForbidden* getNextWorstForbidden(ERCluster* cl);
	bool replaceElements(std::vector<Element*> elems, Element* replacement);
	void updateCycleAdjacency();
	void nullifyCycle(ERCycle* cycle);
	void nullifyCycles(std::pair<Element*, Element*> merge, ERBicoComp* bcomp);
	
	std::pair<Element*, Element*> findClusterConnection(ERCluster* cl1, ERCluster* cl2);
	Element* findClusterConnection(Element* e1, ERCluster* cl);
	std::pair<Entity*, Relationship*> getNextCutEdge();

	void printDecompStats();
};