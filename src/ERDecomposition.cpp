#include "ERDecomposition.h"
#include "LogConsole.h"
#include "PDOUtil.h"
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <queue>

///////////////////////////////////////////////////////////////
// ERCycleDecomp
///////////////////////////////////////////////////////////////

// constructors

ERCycleDecomp::ERCycleDecomp(ERData* erdata)
{
	m_ERData = erdata;
}

ERCycleDecomp::~ERCycleDecomp()
{
	for (ERCycle* c : m_Cycles)
		if (c)
			delete(c);
	for (ERCluster* c : m_Clusters)
		delete(c);
	for (ERTreeComp* t : m_TreeComps)
		delete(t);
	for (ERBicoComp* b : m_BicoComps)
		delete(b);

	m_ElemBicoMap.clear();
	m_ElemTreeMap.clear();
	m_ElemCycleMap.clear();
	m_cutEdges.clear();

	if (m_edges)
	{
		for (int i = 0; i < m_ERData->getEntityNum(); i++)
			delete[] m_edges[i];
		delete[] m_edges;
		m_edges = NULL;
	}
	m_adjacency.clear();

	m_ERData = NULL;
}


// accessors

ERData* ERCycleDecomp::getERData()
{
	return m_ERData;
}

std::vector<ERCycle*> ERCycleDecomp::getCycles(int max_len)
{
	if (max_len == 0)
		return m_Cycles;

	std::vector<ERCycle*> cycles;
	if (max_len > 0 && max_len < 4)
	{
		LogConsole::writeError("Invalid maximum cycle length");
		return cycles;
	}

	for (ERCycle* c : m_Cycles)
	{
		if (c->getLength() <= max_len)
			cycles.push_back(c);
	}

	return cycles;
}

ERCycle* ERCycleDecomp::getCycle(int idx)
{
	return m_Cycles.at(idx);
}

std::vector<ERCluster*> ERCycleDecomp::getClusters()
{
	return m_Clusters;
}


// algorithms

void ERCycleDecomp::Decompose()
{
	computeBiconnectedComponents();
	initIncidenceMatrix();
	for (ERBicoComp* comp : m_BicoComps) 
	{
		if (comp->getNumElements() == 4) // components with one trivial cycle
		{
			ERCycle* cycle = new ERCycle(comp->getElements());
			addCycle(cycle, comp);
		}
		else
		{
			// get a cycle basis
			computeCycleBasis(comp);
			
			if (comp->getNumCycles() > 1)
			{
				comp->findBiconnectedClusters();
				for (ERCluster* cl : comp->getClusters())
					addCluster(cl, comp);
			}
		}
	}

	printDecompStats();
}

void ERCycleDecomp::InitSimplifications()
{
	// sort forbidden clusters by entanglement
	std::sort(m_Clusters.begin(), m_Clusters.end(),
		[](ERCluster* a, ERCluster* b)
		{
			return a->getEntanglement() > b->getEntanglement();
		});

	for (ERBicoComp* comp : m_BicoComps)
		findCutEdges(comp);
}

// Biconnected-Tree Decomposition
void ERCycleDecomp::computeBiconnectedComponents()
{
	for (ERBicoComp* bcomp : m_BicoComps)
		delete bcomp;
	m_BicoComps.clear();
	for (ERTreeComp* tcomp : m_TreeComps)
		delete tcomp;
	m_TreeComps.clear();
	m_ElemTreeMap.clear();

	int num_ens = m_ERData->getEntityNum();
	int num_res = m_ERData->getRelationshipNum();

	std::unordered_map<Element*, int> dis; dis.reserve(num_ens + num_res);
	std::unordered_map<Element*, int> low; low.reserve(num_ens + num_res);
	std::unordered_map<Element*, int> chi; chi.reserve(num_ens + num_res);
	std::unordered_map<Element*, Element*> par; par.reserve(num_ens + num_res);

	std::vector<Element*> elements; elements.reserve(num_ens + num_res);
	for (Entity* en : m_ERData->getEntities())
	{
		elements.push_back(en);
		dis.insert(std::make_pair(en, -1));
		low.insert(std::make_pair(en, -1));
		chi.insert(std::make_pair(en, 0));
		par.insert(std::make_pair(en, nullptr));
		m_ElemTreeMap.insert(std::make_pair(en, nullptr));
		std::vector<ERBicoComp*> empty;
		m_ElemBicoMap.insert(std::make_pair(en, empty));
	}
	for (Relationship* re : m_ERData->getRelationships())
	{
		elements.push_back(re);
		dis.insert(std::make_pair(re, -1));
		low.insert(std::make_pair(re, -1));
		chi.insert(std::make_pair(re, 0));
		par.insert(std::make_pair(re, nullptr));
		m_ElemTreeMap.insert(std::make_pair(re, nullptr));
		std::vector<ERBicoComp*> empty;
		m_ElemBicoMap.insert(std::make_pair(re, empty));
	}

	std::stack<Element*> to_search;
	std::stack<Element*> searched;
	
	ERBicoComp* bcomp;
	ERTreeComp* tcomp;

	// Do depth first search following Hopcroft and Tarjan's algorithm
	for (int i = 0; i < elements.size(); i++)
	{
		Element* e = elements.at(i);
		if (dis.at(e) == -1)
			to_search.push(e);
		Element* p;

		int iter = -1;
		while (!to_search.empty())
		{
			e = to_search.top();
			to_search.pop();
			if (dis.at(e) != -1)
				continue;

			p = par.at(e);
			if (!searched.empty() && p)
			{
				if (searched.top() != p)
					searched.push(p);
			}

			iter++;
			searched.push(e);
			dis.at(e) = iter;
			low.at(e) = iter;

			if (e->getType() == ELEMENTTYPE::ENTITY)
			{
				Entity* en = m_ERData->getEntity(e->getIndex());
				for (Relationship* re : en->getIncidentRelationships())
				{
					if (dis.at(re) == -1)
					{
						p = par.at(re);
						if (p)
							chi.at(p) -= 1;
						par.at(re) = en;
						chi.at(en) += 1;
						to_search.push(re);
					}
					else if (par.at(en) != re)
					{
						low.at(en) = std::min(low.at(en),dis.at(re));
					}
				}
			}
			else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
			{
				Relationship* re = m_ERData->getRelationship(e->getIndex());
				for (Entity* en : re->getIncidentEntities())
				{
					if (dis.at(en) == -1)
					{
						p = par.at(en);
						if (p)
							chi.at(p) -= 1;
						par.at(en) = re;
						chi.at(re) += 1;
						to_search.push(en);
					}
					else if (par.at(re) != en)
					{
						low.at(re) = std::min(low.at(re),dis.at(en));
					}
				}
			}
			
			while (chi.at(e) == 0) // we have reached a leaf or back edge
			{
				p = par.at(e);
				if (p)
				{
					low.at(p) = std::min(low.at(e), low.at(p));
					chi.at(p) -= 1;

					if (low.at(e) == dis.at(p)) // biconnected component
					{
						//LogConsole::writeConsole("Biconnected Component Found");
						bcomp = new ERBicoComp();
						addBicoComp(bcomp);

						while (searched.top() != p)
						{
							//LogConsole::writeConsole(searched.top()->getLabel().c_str());
							//bcomp->addElement(searched.top());
							addBicoElement(bcomp, searched.top());
							searched.pop();
							if (searched.empty())
								break;
						}
						//LogConsole::writeConsole(p->getLabel().c_str());
						//bcomp->addElement(p);
						addBicoElement(bcomp, p);
					}
					else if (low.at(e) > dis.at(p)) // tree component
					{
						//LogConsole::writeConsole("Tree Component Found");
						
						ERTreeComp* etree = m_ElemTreeMap.at(e);
						ERTreeComp* ptree = m_ElemTreeMap.at(p);
						if (!etree && !ptree)
						{
							tcomp = new ERTreeComp();
							addTreeComp(tcomp);
						}
						else if (!etree)
							tcomp = ptree;
						else if (!ptree)
							tcomp = etree;
						else if (etree == ptree)
							tcomp = etree;
						else
							tcomp = mergeTreeComps(etree, ptree);
						

						//LogConsole::writeConsole(e->getLabel().c_str());
						//tcomp->addElement(e);
						//m_ElemTreeMap.at(e) = tcomp;
						addTreeElement(tcomp, e);
						searched.pop();
						//LogConsole::writeConsole(p->getLabel().c_str());
						//tcomp->addElement(p);
						//m_ElemTreeMap.at(p) = tcomp;
						addTreeElement(tcomp, p);
					}
					
					e = p;
				}
				else
				{
					break;
				}	
			}
		}
	}
}

void ERCycleDecomp::initIncidenceMatrix()
{
	int num_ens = m_ERData->getEntityNum();
	int num_res = m_ERData->getRelationshipNum();

	if (!m_edges)
		m_edges = new bool* [num_ens];

	for (int i = 0; i < num_ens; i++)
	{
		m_edges[i] = new bool[num_res];
		for (int j = 0; j < num_res; j++)
			m_edges[i][j] = 0;
	}
}

// Minimum Cycle Basis Extraction

void ERCycleDecomp::computeCycleBasis(ERBicoComp* comp)
{
	int num_ens = 0; int num_res = 0;
	comp->getElemCounts(num_ens, num_res);

	std::queue<Element*> search;
	std::unordered_set<Entity*> visited_ens;
	visited_ens.reserve(num_ens);
	std::unordered_set<Relationship*> visited_res;
	visited_res.reserve(num_res);

	Element* start = comp->getElement(0);
	search.push(start);
	if (start->getType() == RELATIONSHIP)
		visited_res.insert(m_ERData->getRelationship(start->getIndex()));
	else if (start->getType() == ENTITY)
		visited_ens.insert(m_ERData->getEntity(start->getIndex()));

	// do BFS traversal to find non-spanning tree edges 
	// (each one corresponds to a linearly independent cycle)
	std::vector<ERCycle*> long_cycles;
	std::vector<std::pair<Entity*, Relationship*>> long_cycle_edges;
	while (!search.empty())
	{
		Element* current = search.front();
		search.pop();

		if (current->getType() == RELATIONSHIP)
		{
			Relationship* re = m_ERData->getRelationship(current->getIndex());
			for (Entity* en : re->getIncidentEntities())
			{
				if (!comp->containsElement(en)) // Entity not in component
					continue;

				int en_idx = en->getIndex(); int re_idx = re->getIndex();
				if (m_edges[en_idx][re_idx]) // edge already used
					continue;

				if (visited_ens.find(en) == visited_ens.end()) // no cycle
				{
					search.push(en);
					visited_ens.insert(en);
				}
				else // cycle found
				{
					ERCycle* cycle = backTraceCycle(re, en, comp, num_ens, num_res);
					if (cycle->getLength() == 4)
						addCycle(cycle, comp);
					else
					{
						long_cycles.push_back(cycle);
						long_cycle_edges.push_back(std::make_pair(en, re));
					}
				}

				m_edges[en_idx][re_idx] = 1; // mark edge as used
			}
		}
		else if (current->getType() == ENTITY)
		{
			Entity* en = m_ERData->getEntity(current->getIndex());
			for (Relationship* re : en->getIncidentRelationships())
			{
				if (!comp->containsElement(re)) // Relationship not in component
					continue;

				int en_idx = en->getIndex(); int re_idx = re->getIndex();
				if (m_edges[en_idx][re_idx]) // edge already used
					continue;

				if (visited_res.find(re) == visited_res.end()) // no cycle
				{
					search.push(re);
					visited_res.insert(re);
				}
				else // cycle found
				{
					ERCycle* cycle = backTraceCycle(en, re, comp, num_ens, num_res);
					if (cycle->getLength() == 4)
						addCycle(cycle, comp);
					else
					{
						long_cycles.push_back(cycle);
						long_cycle_edges.push_back(std::make_pair(en, re));
					}
				}

				m_edges[en_idx][re_idx] = 1; // mark edge as used
			}
		}
	}

	for (int i = 0; i < long_cycles.size(); i++)
	{
		ERCycle* cycle = long_cycles.at(i);
		std::pair<Entity*, Relationship*> edge = long_cycle_edges.at(i);
		processLongCycle(cycle, edge, comp);
	}
}

ERCycle* ERCycleDecomp::backTraceCycle(Element* source, Element* target, ERBicoComp* comp, int num_ens, int num_res)
{
	std::queue<Element*> search;
	std::unordered_set<Entity*> visited_ens; visited_ens.reserve(num_ens);
	std::unordered_set<Relationship*> visited_res; visited_res.reserve(num_res);
	std::unordered_map<Element*, Element*> parents; parents.reserve(num_ens + num_res);
	parents.insert(std::make_pair(source, nullptr));

	search.push(source);
	if (source->getType() == RELATIONSHIP)
		visited_res.insert(m_ERData->getRelationship(source->getIndex()));
	else if (source->getType() == ENTITY)
		visited_ens.insert(m_ERData->getEntity(source->getIndex()));
	int source_idx = source->getIndex();
	int target_idx = target->getIndex();

	std::vector<Element*> elements;

	//if (source->getType() == ELEMENTTYPE::ENTITY)
	//{
	//	if ((source_idx == 32 || source_idx == 48 || source_idx == 60) &&
	//		(target_idx == 19 || target_idx == 27 || target_idx == 80))
	//		bool debug1 = true;
	//}
	//else if (source->getType() == ELEMENTTYPE::RELATIONSHIP)
	//{
	//	if ((target_idx == 32 || target_idx == 48 || target_idx == 60) &&
	//		(source_idx == 19 || source_idx == 27 || source_idx == 80))
	//		bool debug2 = true;
	//}

	while (!search.empty())
	{
		Element* current = search.front();
		search.pop();
		if (current->getType() == ELEMENTTYPE::RELATIONSHIP)
		{
			Relationship* re = m_ERData->getRelationship(current->getIndex());
			for (Entity* en : re->getIncidentEntities())
			{
				if (!comp->containsElement(en)) // Entity not in component
					continue;

				int en_idx = en->getIndex(); int re_idx = re->getIndex();
				if (!m_edges[en_idx][re_idx]) // edge not used in first search
					continue;
				if (parents.at(re) == en) // edge already used in current search
					continue;

				if (en_idx == target_idx && target->getType() == ELEMENTTYPE::ENTITY) // cycle found
				{
					elements.push_back(target);
					Element* p = re;
					while (p)
					{
						elements.push_back(p);
						p = parents.at(p);
					}

					search = {};
					break;
				}
				else // no cycle
				{
					if (visited_ens.insert(en).second)
						search.push(en);
					parents.insert(std::make_pair(en, re));
				}
			}
		}
		else if (current->getType() == ELEMENTTYPE::ENTITY)
		{
			Entity* en = m_ERData->getEntity(current->getIndex());
			for (Relationship* re : en->getIncidentRelationships())
			{
				if (!comp->containsElement(re)) // Relationship not in component
					continue;

				int en_idx = en->getIndex(); int re_idx = re->getIndex();
				if (!m_edges[en_idx][re_idx]) // edge not used in first search
					continue;
				if (parents.at(en) == re) // edge already used in current search
					continue;

				if (re_idx == target_idx && target->getType() == ELEMENTTYPE::RELATIONSHIP) // cycle found
				{
					elements.push_back(target);
					Element* p = en;
					while (p)
					{
						elements.push_back(p);
						p = parents.at(p);
					}

					search = {};
					break;
				}
				else // no cycle
				{
					if (visited_res.insert(re).second)
						search.push(re);
					parents.insert(std::make_pair(re, en));
				}
			}
		}
	}

	//if (elements.size() == 4)
	//{
	//	bool inset = true;
	//	for (Element* e : elements)
	//	{
	//		int eidx = e->getIndex();
	//		if (e->getType() == ELEMENTTYPE::ENTITY)
	//		{
	//			if (eidx != 32 && eidx != 48 && eidx != 60)
	//			{
	//				inset = false;
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (eidx != 19 && eidx != 27 && eidx != 80 && eidx != 71)
	//			{
	//				inset = false;
	//				break;
	//			}
	//		}
	//	}
	//	if (inset)
	//		bool debug = true;
	//}


	ERCycle* cycle = new ERCycle(elements);
	return cycle;
}

void ERCycleDecomp::processLongCycle(ERCycle* cycle, std::pair<Entity*, Relationship*> edge, ERBicoComp* bcomp)
{
	Entity* en1 = edge.first;
	Relationship* re1 = edge.second;
	int len = cycle->getLength();

	// check every edge in the cycle. If one of the edges 
	for (int i = 0; i < len; i++)
	{
		int j = (i + 1) % len;
		Element* ei = cycle->getElement(i);
		Element* ej = cycle->getElement(j);

		Entity* en; Relationship* re;
		if (ei->getType() == ELEMENTTYPE::ENTITY)
		{
			en = m_ERData->getEntity(ei->getIndex());
			re = m_ERData->getRelationship(ej->getIndex());
		}
		else
		{
			en = m_ERData->getEntity(ej->getIndex());
			re = m_ERData->getRelationship(ei->getIndex());
		}

		bool k22found = false;
		for (Entity* adj : en->getAdjacentEntities())
		{
			std::vector<Relationship*> common = getSharedRelationships(en, adj);
			if (common.size() < 2)
				continue;
			if (std::find(common.begin(), common.end(), re) != common.end())
			{
				k22found = true;
				break;
			}
		}

		if (!k22found)
		{
			addCycle(cycle, bcomp);
			return;
		}
	}

	// get the 1-ring neighborhood from the cycle elements
	std::unordered_map<Element*,std::vector<Element*>> ringmap;
	std::vector<Element*> ringset;
	for (Element* e : cycle->getElements())
	{
		if (e->getType() == ELEMENTTYPE::ENTITY)
		{
			Entity* en = m_ERData->getEntity(e->getIndex());
			for (Relationship* re : en->getIncidentRelationships())
			{
				if (cycle->containsElement(re))
					continue;
				if (ringmap.find(re) == ringmap.end())
				{
					std::vector<Element*> v{en};
					ringmap.insert(std::make_pair(re, v));
					ringset.push_back(re);
				}
				else
				{
					ringmap.at(re).push_back(en);
				}
			}
		}
		else
		{
			Relationship* re = m_ERData->getRelationship(e->getIndex());
			for (Entity* en : re->getIncidentEntities())
			{
				if (cycle->containsElement(en))
					continue;
				if (ringmap.find(en) == ringmap.end())
				{
					std::vector<Element*> v{re};
					ringmap.insert(std::make_pair(en, v));
					ringset.push_back(en);
				}
				else
				{
					ringmap.at(en).push_back(re);
				}
			}
		}
	}

	// find the connected components of the 1-ring neighborhood
	bool keep_old_cycle = true;
	int search_idx = 0;
	std::unordered_set<Element*> component;
	std::unordered_set<Element*> incident_ens;
	std::unordered_set<Element*> incident_res;
	while (search_idx < ringset.size())
	{
		std::queue<Element*> search;
		component.clear();
		incident_ens.clear();
		incident_res.clear();

		Element* e = ringset.at(search_idx);
		search.push(e);
		component.insert(e);
		for (Element* inc : ringmap.at(e))
		{
			if (inc->getType() == ELEMENTTYPE::ENTITY)
				incident_ens.insert(inc);
			else
				incident_res.insert(inc);
		}
			
		while (!search.empty())
		{
			e = search.front();
			search.pop();

			if (e->getType() == ELEMENTTYPE::ENTITY)
			{
				Entity* en = m_ERData->getEntity(e->getIndex());
				for (Relationship* re : en->getIncidentRelationships())
				{
					Element* re_elem = re;
					if (ringmap.find(re_elem) == ringmap.end())
						continue;
					
					if (component.insert(re_elem).second)
					{
						for (Element* inc : ringmap.at(re_elem))
						{
							if (inc->getType() == ELEMENTTYPE::ENTITY)
								incident_ens.insert(inc);
							else
								incident_res.insert(inc);
						}
						search.push(re_elem);
					}
				}
			}
			else
			{
				Relationship* re = m_ERData->getRelationship(e->getIndex());
				for (Entity* en : re->getIncidentEntities())
				{
					Element* en_elem = en;
					if (ringmap.find(en_elem) == ringmap.end())
						continue;

					if (component.insert(en_elem).second)
					{
						for (Element* inc : ringmap.at(en_elem))
						{
							if (inc->getType() == ELEMENTTYPE::ENTITY)
								incident_ens.insert(inc);
							else
								incident_res.insert(inc);
						}
						search.push(en_elem);
					}
				}
			}
		}

		if (incident_ens.size() == (len/2) ||
			incident_res.size() == (len/2))
		{
			keep_old_cycle = false;
			break;
		}
		else if ((incident_ens.size() + incident_res.size()) > (len / 2) &&
			(incident_ens.size() + incident_res.size()) < len)
		{
			keep_old_cycle = false;
			break;
		}

		search_idx++;
	}

	// keep the original cycle if a component in the 1-ring neighborhood
	// with sufficient connections was not found
	if (keep_old_cycle)
	{
		addCycle(cycle, bcomp);
		return;
	}
	
	// Otherwise, find minimal cycle from non-spanning edge of original cycle
	for (Relationship* re2 : re1->getAdjacentRelationships())
	{
		std::vector<Entity*> shared = getSharedEntities(re1, re2);
		if (shared.size() < 2)
			continue;
		if (std::find(shared.begin(), shared.end(), en1) == shared.end())
			continue;
		for (Entity* en2 : shared)
		{
			if (en2 == en1)
				continue;
			std::vector<Element*> elements{en1, re1, en2, re2};
			ERCycle* new_cycle = new ERCycle(elements);
			if (bcomp->checkLinearIndependent(new_cycle, en1))
			{
				addCycle(new_cycle, bcomp);
				delete(cycle);
				return;
			}
			else
				delete (new_cycle);
		}
	}
}


// biconnected component simplification
void ERCycleDecomp::findCutEdges(ERBicoComp* bcomp)
{
	// check if it has enough cycles to contain K5 or K33
	int num_cycles = bcomp->getNumLongCycles(4);
	if (num_cycles < 4)
		return;

	// build ogdf graph with collapsed clusters
	ogdf::Graph G;
	std::vector<Element*> elements = bcomp->getElements();
	std::unordered_map<Element*, ogdf::node> elemNodeMap;
	std::unordered_map<ogdf::node, Element*> nodeElemMap;
	std::unordered_map<ERCluster*, ogdf::node> clusterNodeMap;
	std::unordered_map<ogdf::node, ERCluster*> nodeClusterMap;
	std::unordered_map<Element*, ERCluster*> elemClusterMap;
	
	// create the graph nodes
	for (Element* e : elements)
	{
		ERCluster* cl = bcomp->getElementCluster(e);
		if (cl)
		{
			elemClusterMap.insert(std::make_pair(e, cl));
			if (clusterNodeMap.find(cl) == clusterNodeMap.end())
			{
				ogdf::node v = G.newNode();
				clusterNodeMap.insert(std::make_pair(cl, v));
				nodeClusterMap.insert(std::make_pair(v, cl));
			}
		}
		else
		{
			elemClusterMap.insert(std::make_pair(e, nullptr));
			ogdf::node v = G.newNode();
			elemNodeMap.insert(std::make_pair(e, v));
			nodeElemMap.insert(std::make_pair(v, e));
		}
	}

	// create the graph edges
	Entity* en; Relationship* re;
	for (int i = 0; i < elements.size()-1; i++)
	{
		Element* e = elements.at(i);

		ogdf::node ev;
		ERCluster* ecl = elemClusterMap.at(e);
		if (ecl)
			ev = clusterNodeMap.at(ecl);
		else
			ev = elemNodeMap.at(e);

		ELEMENTTYPE etype = e->getType();
		if (etype == ELEMENTTYPE::ENTITY)
			en = m_ERData->getEntity(e->getIndex());
		else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
			re = m_ERData->getRelationship(e->getIndex());
		
		for (int j = i + 1; j < elements.size(); j++)
		{
			Element* a = elements.at(j);
			
			ELEMENTTYPE atype = a->getType();
			if (etype == atype)
				continue;
			else if (atype == ELEMENTTYPE::ENTITY)
				en = m_ERData->getEntity(a->getIndex());
			else if (atype == ELEMENTTYPE::RELATIONSHIP)
				re = m_ERData->getRelationship(a->getIndex());

			if (!m_ERData->areEntityRelationshipIncident(en, re))
				continue;
			
			ogdf::node av;
			ERCluster* acl = elemClusterMap.at(a);
			if (acl)
				av = clusterNodeMap.at(acl);
			else
				av = elemNodeMap.at(a);

			if (ev != av)
				G.newEdge(ev, av);
		}
	}

	// apply subgraph planarization crossing minimization
	ogdf::SubgraphPlanarizer SP;
	SP.setSubgraph(new ogdf::PlanarSubgraphFast<int>);
	SP.setInserter(new ogdf::VariableEmbeddingInserter);

	int crossNum;
	ogdf::PlanRep PR(G);
	SP.call(PR, 0, crossNum);
	LogConsole::writeConsole("%d crossings computed",crossNum);
	if (crossNum == 0)
		return;
	
	// iterate over crossing nodes
	std::vector<std::pair<Element*, Element*>> cut_candidates;
	ogdf::node crossing = PR.lastNode();
	ogdf::node original = PR.original(crossing);
	int nulledge = 0;
	while (original == NULL)
	{	
		//LogConsole::writeConsole("======== Crossing Found: ========");
		ogdf::adjEntry first = crossing->firstAdj();
		ogdf::adjEntry last = crossing->lastAdj();
		for (ogdf::adjEntry entry : {first, last})
		{
			ogdf::edge edge = entry->theEdge();
			ogdf::edge gedge = PR.original(edge);
			ogdf::node n1 = gedge->source();
			ogdf::node n2 = gedge->target();
			Element* e1 = NULL; Element* e2 = NULL;
			ERCluster* cl1 = NULL; ERCluster* cl2 = NULL;
			
			// find element at source end
			if (nodeElemMap.find(n1) != nodeElemMap.end())
				e1 = nodeElemMap.at(n1);
			else if (nodeClusterMap.find(n1) != nodeClusterMap.end())
				cl1 = nodeClusterMap.at(n1);

			// find element at target
			if (nodeElemMap.find(n2) != nodeElemMap.end())
				e2 = nodeElemMap.at(n2);
			else if (nodeClusterMap.find(n2) != nodeClusterMap.end())
				cl2 = nodeClusterMap.at(n2);

			if (cl1 && cl2)
			{
				std::pair<Element*, Element*> e12;
				e12 = findClusterConnection(cl1, cl2);
				e1 = e12.first;
				e2 = e12.second;
			}
			else if (cl1)
				e1 = findClusterConnection(e2, cl1);
			else if (cl2)
				e2 = findClusterConnection(e1, cl2);

			cut_candidates.push_back(std::make_pair(e1, e2));
			
			/*if (e1->getType() == ELEMENTTYPE::ENTITY)
				LogConsole::writeConsole("E%d", e1->getIndex());
			else
				LogConsole::writeConsole("R%d", e1->getIndex());
			if (e2->getType() == ELEMENTTYPE::ENTITY)
				LogConsole::writeConsole("E%d", e2->getIndex());
			else
				LogConsole::writeConsole("R%d", e2->getIndex());*/
		}
		
		crossing = crossing->pred();
		original = PR.original(crossing);
	}

	processCutEdges(cut_candidates, bcomp);
}

void ERCycleDecomp::processCutEdges(std::vector<std::pair<Element*, Element*>> edges, ERBicoComp* bcomp)
{
	std::unordered_set<ERCycle*> to_cut;

	for (int i = 0; i < edges.size(); i+=2)
	{
		std::pair<Element*, Element*> edge1 = edges.at(i);
		std::pair<Element*, Element*> edge2 = edges.at(i + 1);
		CycleCount count1 = bcomp->getCycleCountsForEdge(edge1);
		CycleCount count2 = bcomp->getCycleCountsForEdge(edge2);

		bool unique = false;
		if (count1.long_count < count2.long_count ||
			(count1.long_count == count2.long_count && count1.avg_len >= count2.avg_len))
		{
			for (ERCycle* cycle : count1.cycles)
			{
				if (to_cut.find(cycle) == to_cut.end())
				{
					unique = true;
					to_cut.insert(cycle);
				}
			}

			if (unique)
			{
				m_cutEdges.push_back(edge1);
				continue;
			}

			for (ERCycle* cycle : count2.cycles)
			{
				if (to_cut.find(cycle) == to_cut.end())
				{
					unique = true;
					to_cut.insert(cycle);
				}
			}

			if (unique)
			{
				m_cutEdges.push_back(edge2);
				continue;
			}

			/*if (std::find(m_cutEdges.begin(), m_cutEdges.end(), edge1) == m_cutEdges.end())
				m_cutEdges.push_back(edge1);
			else
				m_cutEdges.push_back(edge2);*/
		}
		else
		{
			for (ERCycle* cycle : count2.cycles)
			{
				if (to_cut.find(cycle) == to_cut.end())
				{
					unique = true;
					to_cut.insert(cycle);
				}
			}

			if (unique)
			{
				m_cutEdges.push_back(edge2);
				continue;
			}

			for (ERCycle* cycle : count1.cycles)
			{
				if (to_cut.find(cycle) == to_cut.end())
				{
					unique = true;
					to_cut.insert(cycle);
				}
			}

			if (unique)
			{
				m_cutEdges.push_back(edge1);
				continue;
			}

			/*if (std::find(m_cutEdges.begin(), m_cutEdges.end(), edge2) == m_cutEdges.end())
				m_cutEdges.push_back(edge2);
			else
				m_cutEdges.push_back(edge1);*/
		}
	}

	LogConsole::writeConsole("%d valid edge crossings", m_cutEdges.size());
}


// utility

void ERCycleDecomp::highlightElement(Element* e)
{
	e->setSelected(true);
	if (e->getType() == ENTITY)
	{
		Entity* en = m_ERData->getEntity(e->getIndex());
		if (en->getDualRelationship())
			en->getDualRelationship()->setSelected(true);
	}
	else if (e->getType() == RELATIONSHIP)
	{
		Relationship* re = m_ERData->getRelationship(e->getIndex());
		if (re->getDualEntity())
			re->getDualEntity()->setSelected(true);
	}
}

void ERCycleDecomp::highlightNextBComp()
{
	m_bchighlight = (m_bchighlight + 1) % (m_BicoComps.size() + 1);

	if (m_bchighlight >= 0 && m_bchighlight < m_BicoComps.size())
	{
		ERBicoComp* bcomp = m_BicoComps.at(m_bchighlight);
		for (Element* e : bcomp->getElements())
			highlightElement(e);
	}
}

void ERCycleDecomp::highlightAllBComps()
{
	for (Entity* en : m_ERData->getEntities())
	{
		if (m_ElemBicoMap.at(en).empty())
			continue;
		en->setSelected(true);
		if (en->getDualRelationship())
			en->getDualRelationship()->setSelected(true);
	}

	for (Relationship* re : m_ERData->getRelationships())
	{
		if (m_ElemBicoMap.at(re).empty())
			continue;
		re->setSelected(true);
		if (re->getDualEntity())
			re->getDualEntity()->setSelected(true);
	}
}

void ERCycleDecomp::highlightNextTComp()
{
	m_tchighlight = (m_tchighlight + 1) % (m_TreeComps.size() + 1);

	if (m_tchighlight >= 0 && m_tchighlight < m_TreeComps.size())
	{
		ERTreeComp* tcomp = m_TreeComps.at(m_tchighlight);
		for (Element* e : tcomp->getElements())
			highlightElement(e);
	}
}

void ERCycleDecomp::highlightAllTComps()
{
	for (Entity* en : m_ERData->getEntities())
	{
		if (!m_ElemTreeMap.at(en))
			continue;
		en->setSelected(true);
		if (en->getDualRelationship())
			en->getDualRelationship()->setSelected(true);
	}

	for (Relationship* re : m_ERData->getRelationships())
	{
		if (!m_ElemTreeMap.at(re))
			continue;
		re->setSelected(true);
		if (re->getDualEntity())
			re->getDualEntity()->setSelected(true);
	}
}

void ERCycleDecomp::highlightNextCycle()
{
	ERCycle* cycle;

	m_cyhighlight = (m_cyhighlight + 1) % (m_Cycles.size() + 1);
	if (m_cyhighlight >= 0 && m_cyhighlight < m_Cycles.size())
	{
		ERCycle* cycle = m_Cycles.at(m_cyhighlight);
		for (Element* e : cycle->getElements())
			highlightElement(e);
	}

	//while (true)
	//{
	//	m_cyhighlight = (m_cyhighlight + 1) % (m_Cycles.size() + 1);
	//	if (m_cyhighlight >= 0 && m_cyhighlight < m_Cycles.size())
	//	{
	//		cycle = m_Cycles.at(m_cyhighlight);
	//		if (!cycle)
	//			continue;
	//		if (cycle->getLength() > 4)
	//			break;
	//	}
	//	if (m_cyhighlight == m_Cycles.size())
	//		return;
	//}
	//for (Element* e : cycle->getElements())
	//	highlightElement(e);
}

void ERCycleDecomp::highlightAllCycles()
{
	for (ERCycle* cycle : m_Cycles)
	{
		/*if (cycle->getLength() == 4)
			continue;*/

		for (Element* e : cycle->getElements())
			highlightElement(e);
	}

	//for (auto value : m_ElemCycleMap)
	//{
	//	Element* e = value.first;
	//	e->setSelected(true);
	//	if (e->getType() == ENTITY)
	//	{
	//		Entity* en = m_ERData->getEntity(e->getIndex());
	//		if (en->getDualRelationship())
	//			en->getDualRelationship()->setSelected(true);
	//	}
	//	else if (e->getType() == RELATIONSHIP)
	//	{
	//		Relationship* re = m_ERData->getRelationship(e->getIndex());
	//		if (re->getDualEntity())
	//			re->getDualEntity()->setSelected(true);
	//	}
	//}
}

void ERCycleDecomp::highlightNextCluster()
{
	m_clhighlight = (m_clhighlight + 1) % (m_Clusters.size() + 1);

	if (m_clhighlight >= 0 && m_clhighlight < m_Clusters.size())
	{
		ERCluster* cluster = m_Clusters.at(m_clhighlight);
		for (Element* e : cluster->getElements())
			highlightElement(e);
	}
}

void ERCycleDecomp::highlightAllClusters()
{
	for (ERCluster* cluster : m_Clusters)
	{
		for (Element* e : cluster->getElements())
			highlightElement(e);
	}
}

void ERCycleDecomp::resetHighlights()
{
	for (Entity* en : m_ERData->getEntities())
	{
		en->setSelected(false);
		if (en->getDualRelationship())
			en->getDualRelationship()->setSelected(false);
	}
	for (Relationship* re : m_ERData->getRelationships())
	{
		re->setSelected(false);
		if (re->getDualEntity())
			re->getDualEntity()->setSelected(false);
	}

	m_bchighlight = -1;
	m_tchighlight = -1;
	m_cyhighlight = -1;
	m_clhighlight = -1;
}



void ERCycleDecomp::addBicoComp(ERBicoComp* bcomp)
{
	bcomp->setIndex(m_BicoComps.size());
	m_BicoComps.push_back(bcomp);
}

void ERCycleDecomp::addBicoElement(ERBicoComp* bcomp, Element* e)
{
	bool result = bcomp->addElement(e);
	if (!result)
		return;

	for (ERBicoComp* erbc : m_ElemBicoMap.at(e))
	{
		bcomp->addIncidentBicoComp(erbc);
		erbc->addIncidentBicoComp(bcomp);
	}
	m_ElemBicoMap.at(e).push_back(bcomp);

	if (m_ElemTreeMap.at(e))
	{
		ERTreeComp* tcomp = m_ElemTreeMap.at(e);
		bcomp->addIncidentTreeComp(tcomp);
		tcomp->addIncidentBicoComp(bcomp);
	}
}

void ERCycleDecomp::addTreeComp(ERTreeComp* tcomp)
{
	tcomp->setIndex(m_TreeComps.size());
	m_TreeComps.push_back(tcomp);
}

void ERCycleDecomp::addTreeElement(ERTreeComp* tcomp, Element* e)
{
	bool result = tcomp->addElement(e);
	if (!result)
		return;

	m_ElemTreeMap.at(e) = tcomp;

	if (m_ElemBicoMap.find(e) != m_ElemBicoMap.end())
	{
		for (ERBicoComp* bcomp : m_ElemBicoMap.at(e))
		{
			tcomp->addIncidentBicoComp(bcomp);
			bcomp->addIncidentTreeComp(tcomp);
		}
	}
}

ERTreeComp* ERCycleDecomp::mergeTreeComps(ERTreeComp* t1, ERTreeComp* t2)
{
	for (Element* e : t2->getElements())
	{
		t1->addElement(e);
		m_ElemTreeMap.at(e) = t1;
	}

	int idx = t2->getIndex();
	if (idx < m_TreeComps.size() - 1)
	{
		ERTreeComp* temp = m_TreeComps.back();
		temp->setIndex(idx);
		m_TreeComps.at(idx) = temp;
		m_TreeComps.pop_back();
		delete t2;
	}

	return t1;
}

std::vector<Element*> ERCycleDecomp::getTreeRoots(ERTreeComp* tcomp)
{
	std::vector<Element*> roots;
	for (Element* e : tcomp->getElements())
	{
		if (m_ElemBicoMap.find(e) != m_ElemBicoMap.end())
			roots.push_back(e);
	}
}


void ERCycleDecomp::addCycle(ERCycle* cycle, ERBicoComp* bcomp)
{
	cycle->setIndex(m_Cycles.size());
	m_Cycles.push_back(cycle);

	if (bcomp)
	{
		bcomp->addCycle(cycle);
	}
	//else
	//{
	//	for (Element* e : cycle->getElements())
	//	{
	//		if (m_ElemCycleMap.find(e) != m_ElemCycleMap.end())
	//			m_ElemCycleMap.at(e).push_back(cycle);
	//		else
	//		{
	//			std::vector<ERCycle*> v{cycle};
	//			m_ElemCycleMap.insert(std::make_pair(e, v));
	//		}
	//	}
	//}
}

void ERCycleDecomp::removeCycle(ERCycle* cycle)
{
	m_Cycles.at(cycle->getIndex()) = NULL;
}

void ERCycleDecomp::addCluster(ERCluster* cl, ERBicoComp* bcomp)
{
	cl->setIndex(m_Clusters.size());
	m_Clusters.push_back(cl);
	m_ClusterBicoMap.insert(std::make_pair(cl, bcomp));
}

int ERCycleDecomp::getNumClusters()
{
	return m_Clusters.size();
}



BipartOperation* ERCycleDecomp::getNextClusterOp()
{
	ERForbidden* fb = getNextForbiddenSimp();
	if (!fb)
	{
		return NULL;
		m_recomputeForbiddens = true;
	}

	ERBicoComp* bcomp = getBicoComp(fb);
	ERCluster* cluster = m_Clusters.at(m_clusterSimpIndex);
	FORBIDDENTYPE fbtype = fb->getType();
	std::vector<std::pair<Element*, Element*>> merges;
	int chosen;
	ERCycle* to_collapse;
	switch (fbtype)
	{
		case FORBIDDENTYPE::NADJACENT:
		{
			// get the shared merge first
			std::vector<Element*> ens;
			std::vector<Element*> res;
			for (Element* e : fb->getCommonElements())
			{
				if (e->getType() == ELEMENTTYPE::ENTITY)
					ens.push_back(e);
				else
					res.push_back(e);
			}
			if (ens.size() == 2)
				merges.push_back(std::make_pair(ens.at(0), ens.at(1)));
			else if (res.size() == 2)
				merges.push_back(std::make_pair(res.at(0), res.at(1)));

			CycleCount count = bcomp->getCycleCountsForPair(merges.at(0).first, merges.at(0).second);
			if (count.long_count == 0) // merging common elements does not affect larger cycles
			{
				chosen = 0;
				to_collapse = fb->getCycle(chosen);
				//merge_type = merges.at(0).first->getType();
				//for (ERCycle* cy : fb->getCycles())
				//{
				//	cy->to_delete = true;
				//	nullifyCycle(cy);
				//}
			}
			else // get the other merge options
			{
				for (ERCycle* cy : fb->getCycles())
				{
					if (cy->getElement(0) == merges.at(0).first ||
						cy->getElement(0) == merges.at(0).second)
					{
						Element* e1 = cy->getElement(1);
						Element* e2 = cy->getElement(3);
						merges.push_back(std::make_pair(e1, e2));
					}
					else
					{
						Element* e1 = cy->getElement(0);
						Element* e2 = cy->getElement(2);
						merges.push_back(std::make_pair(e1, e2));
					}
				}

				// get cycle to collapse based on chosen merge direction
				chosen = getMergeDirection(merges, bcomp);
				if (chosen == -1)
				{
					cluster->removeForbidden(fb);
					m_recomputeForbiddens = false;
					return NULL;
				}
				//merge_type = merges.at(chosen).first->getType();
				if (chosen == 0)
				{
					to_collapse = fb->getCycle(0);
					//for (ERCycle* cy : fb->getCycles())
					//{
					//	cy->to_delete = true;
					//	nullifyCycle(cy);
					//}
				}
				else
				{
					int idx = (chosen - 1) / 2;
					to_collapse = fb->getCycle(idx);
					//to_collapse->to_delete = true;
					//nullifyCycle(to_collapse);
				}
			}
			
			break;
		}
		case FORBIDDENTYPE::SATURATED:
		{
			// get all possible merges
			for (ERCycle* cy : fb->getCycles())
			{
				merges.push_back(std::make_pair(cy->getElement(0), cy->getElement(2)));
				merges.push_back(std::make_pair(cy->getElement(1), cy->getElement(3)));
			}

			// get cycle to collapse based on chosen merge direction
			chosen = getMergeDirection(merges,bcomp);
			if (chosen == -1)
			{
				cluster->removeForbidden(fb);
				m_recomputeForbiddens = false;
				return NULL;
			}
			//merge_type = merges.at(chosen).first->getType();
			int idx = chosen / 2;
			to_collapse = fb->getCycle(idx);
			/*to_collapse->to_delete = true;
			nullifyCycle(to_collapse);*/

			break;
		}
		case FORBIDDENTYPE::STRANGLED:
		{
			// get only merges with central element
			Element* e0 = fb->getCommonElement(0);
			for (ERCycle* cy : fb->getCycles())
			{
				for (Element* e1 : cy->getElements())
				{
					if (e1 != e0 && e1->getType() == e0->getType())
						merges.push_back(std::make_pair(e0, e1));
				}
			}

			// get cycle to collapse based on chosen merge direction
			chosen = getMergeDirection(merges,bcomp);
			if (chosen == -1)
			{
				cluster->removeForbidden(fb);
				m_recomputeForbiddens = false;
				return NULL;
			}
			//merge_type = merges.at(chosen).first->getType();
			to_collapse = fb->getCycle(chosen);
			//to_collapse->to_delete = true;
			//nullifyCycle(to_collapse);

			break;
		}
	}

	// 
	std::pair<Element*, Element*> merge = merges.at(chosen);
	ELEMENTTYPE merge_type = merge.first->getType();
	nullifyCycles(merge, bcomp);

	// get edges of the cycle to collapse
	std::vector<std::pair<Entity*, Relationship*>> edges;
	std::vector<Element*> ens; std::vector<Element*> res;
	for (Element* e : to_collapse->getElements())
	{
		if (e->getType() == ELEMENTTYPE::ENTITY)
			ens.push_back(e);
		else
			res.push_back(e);
	}
	edges.push_back(std::make_pair(m_ERData->getEntity(ens[0]->getIndex()), m_ERData->getRelationship(res[0]->getIndex())));
	edges.push_back(std::make_pair(m_ERData->getEntity(ens[0]->getIndex()), m_ERData->getRelationship(res[1]->getIndex())));
	edges.push_back(std::make_pair(m_ERData->getEntity(ens[1]->getIndex()), m_ERData->getRelationship(res[0]->getIndex())));
	edges.push_back(std::make_pair(m_ERData->getEntity(ens[1]->getIndex()), m_ERData->getRelationship(res[1]->getIndex())));

	// create bipartite operation to apply
	BipartOperation* bopt = new BipartOperation(edges, BIPART_OP_TYPE::K22_COLLAPSE, merge_type);
	if (merge_type == ELEMENTTYPE::ENTITY)
	{
		// initialize replacement element
		Vec3 loc = (edges[0].first->getLocation() + edges[3].first->getLocation()) / 2.0;
		Entity* en = new Entity(loc);
		en->setLabel(edges[0].first->getLabel() + edges[3].first->getLabel());
		en->setOpidx(edges[0].first->getOpidx());
		bopt->setEnResult(en);
		Element* result = bopt->getResult();
		if (replaceElements(ens, result))
		{
			m_recomputeForbiddens = false;
			return NULL;
		}
			
	}
	else if (merge_type == ELEMENTTYPE::RELATIONSHIP)
	{
		// initialize replacement element
		Relationship* re = new Relationship(edges[0].second->getIncidentEntities(), false);
		re->setLabel(edges[0].second->getLabel() + edges[3].second->getLabel());
		re->setOpidx(edges[0].second->getOpidx());
		bopt->setReResult(re);
		Element* result = bopt->getResult();
		if (replaceElements(res, result))
		{
			m_recomputeForbiddens = false;
			return NULL;
		}
	}

	m_recomputeForbiddens = true;
	return bopt;
}

ERBicoComp* ERCycleDecomp::getBicoComp(ERForbidden* fb)
{
	ERCycle* cy = fb->getCycle(0);
	Element* e1 = cy->getElement(0);
	Element* e2 = cy->getElement(1);

	std::vector<ERBicoComp*> bcomps1 = m_ElemBicoMap.at(e1);
	std::vector<ERBicoComp*> bcomps2 = m_ElemBicoMap.at(e2);
	if (bcomps1.size() == 1)
		return bcomps1.at(0);
	if (bcomps2.size() == 1)
		return bcomps2.at(0);

	for (ERBicoComp* comp : bcomps1)
	{
		if (std::find(bcomps2.begin(), bcomps2.end(), comp) != bcomps2.end())
			return comp;
	}

	return NULL;
}

int ERCycleDecomp::getMergeDirection(std::vector<std::pair<Element*, Element*>> merges, ERBicoComp* bcomp)
{
	int chosen = -1;
	int fewest_longs = INT_MAX;
	int longest_short = 0;
	float best_avg = 0;

	for (int i = 0; i < merges.size(); i++)
	{
		std::pair<Element*, Element*> merge = merges.at(i);
		CycleCount count = bcomp->getCycleCountsForPair(merge.first, merge.second);
		if (count.shortest_len == 6)
			continue;
		if (count.long_count < fewest_longs)
		{
			chosen = i;
			fewest_longs = count.long_count;
			longest_short = count.shortest_len;
			best_avg = count.avg_len;
		}
		else if (count.long_count == fewest_longs)
		{
			if (count.shortest_len > longest_short)
			{
				chosen = i;
				fewest_longs = count.long_count;
				longest_short = count.shortest_len;
				best_avg = count.avg_len;
			}
			else if (count.shortest_len == longest_short)
			{
				if (count.avg_len > best_avg)
				{
					chosen = i;
					fewest_longs = count.long_count;
					longest_short = count.shortest_len;
					best_avg = count.avg_len;
				}
			}
		}
	}
		
	return chosen;
}

ERForbidden* ERCycleDecomp::getNextForbiddenSimp()
{
	ERForbidden* fb = NULL;
	while (m_clusterSimpIndex < m_Clusters.size())
	{
		ERCluster* cl = m_Clusters.at(m_clusterSimpIndex);
		fb = getNextWorstForbidden(cl);
		if (fb)
			break;
		else
			m_clusterSimpIndex++;
	}

	return fb;
}

ERForbidden* ERCycleDecomp::getNextWorstForbidden(ERCluster* cl)
{
	if (m_recomputeForbiddens)
		cl->findForbiddens();
	int num = cl->getNumForbiddens();
	if (num == 0)
		return NULL;

	ERForbidden* worst = NULL;
	int worst_card = 0;
	for (int i = 0; i < num; i++)
	{
		ERForbidden* fb = cl->getForbidden(i);

		if (fb->getType() == FORBIDDENTYPE::NADJACENT) // n-adjacent cluster is always worst
		{
			if (!worst)
				worst = fb;
			else if (worst->getNumCycles() < fb->getNumCycles() || 
				( (worst->getNumCycles() == fb->getNumCycles()) && 
					(worst->getIndex() > fb->getIndex()) ) )
				worst = fb;
		}
		else if (fb->getType() == FORBIDDENTYPE::SATURATED) // severity of saturated depends on card of central element
		{
			if (!worst)
				worst = fb;
			else if (worst->getType() == FORBIDDENTYPE::NADJACENT) 
				return worst; // nadjacent are extracted first
			
			int card = 0;
			Entity* en; Relationship* re;
			Element* e1 = fb->getCommonElement(0);
			Element* e2 = fb->getCommonElement(1);
			if (e1->getType() == ELEMENTTYPE::ENTITY)
			{
				en = m_ERData->getEntity(e1->getIndex());
				re = m_ERData->getRelationship(e2->getIndex());
			}
			else
			{
				en = m_ERData->getEntity(e2->getIndex());
				re = m_ERData->getRelationship(e1->getIndex());
			}
			if (en->getDegree() > re->getCardinality())
			{
				card = en->getDegree();
				fb->setCommonElements({ en,re });
			}
			else
			{
				card = re->getCardinality();
				fb->setCommonElements({ re,en });
			}
			
			if (card > worst_card)
			{
				worst = fb;
				worst_card = card;
			}
		}
		else if (fb->getType() == FORBIDDENTYPE::STRANGLED) // severity of strangled depends on card of central element
		{
			if (!worst)
				worst = fb;
			if (worst->getType() == FORBIDDENTYPE::NADJACENT)
				return worst;

			int card = 0;
			Element* e = fb->getCommonElement(0);
			if (e->getType() == ELEMENTTYPE::ENTITY)
				card = m_ERData->getEntity(e->getIndex())->getDegree();
			else
				card = m_ERData->getRelationship(e->getIndex())->getCardinality();

			if (card <= fb->getNumCycles() && worst == fb)
			{
				worst = NULL;
				worst_card = 0;
			}
			else if (card > worst_card)
			{
				worst = fb;
				worst_card = card;
			}
		}
	}
	
	return worst;
}

bool ERCycleDecomp::replaceElements(std::vector<Element*> elems, Element* replacement)
{
	if (elems.at(0) == elems.at(1))
		return false;

	// find biconnected components and trees that the elements belong to
	std::unordered_set<ERBicoComp*> bcompset;
	ERTreeComp* tcomp = NULL;
	for (Element* e : elems)
	{
		for (ERBicoComp* bcomp : m_ElemBicoMap.at(e))
		{
			bcomp->replaceElement(e, replacement);
			bcompset.insert(bcomp);
		}
		m_ElemBicoMap.erase(e);

		ERTreeComp* t = m_ElemTreeMap.at(e);
		if (t)
		{
			t->replaceElement(e, replacement);
			tcomp = t;
		}
		m_ElemTreeMap.erase(e);
	}

	std::vector<ERBicoComp*> bcomps;
	bcomps.insert(bcomps.begin(), bcompset.begin(), bcompset.end());
	m_ElemBicoMap.insert(std::make_pair(replacement, bcomps));
	m_ElemTreeMap.insert(std::make_pair(replacement, tcomp));

	// update cut edges
	for (std::pair<Element*, Element*> edge : m_cutEdges)
	{
		if (edge.first == elems.at(0) || edge.first == elems.at(1))
			edge.first = replacement;
		if (edge.second == elems.at(0) || edge.second == elems.at(1))
			edge.second = replacement;
	}
}

void ERCycleDecomp::updateCycleAdjacency()
{
	ERCluster* cl = m_Clusters.at(m_clusterSimpIndex);
	ERBicoComp* bc = m_ClusterBicoMap.at(cl);
	bc->updateAdjacency();
	//cl->findForbiddens();
}

void ERCycleDecomp::nullifyCycle(ERCycle* cycle)
{
	m_Cycles.at(cycle->getIndex()) = NULL;
}

void ERCycleDecomp::nullifyCycles(std::pair<Element*, Element*> merge, ERBicoComp* bcomp)
{
	std::vector<int> cycles = bcomp->getCyclesForPair(merge.first, merge.second);
	for (int idx : cycles)
	{
		m_Cycles.at(idx)->to_delete = true;
		if (m_Cycles.at(idx)->getLength() == 4)
			m_Cycles.at(idx) = NULL;
	}
}

std::pair<Element*, Element*> ERCycleDecomp::findClusterConnection(ERCluster* cl1, ERCluster* cl2)
{
	std::unordered_set<Element*> eset1 = cl1->getElemSet();
	std::unordered_set<Element*> eset2 = cl2->getElemSet();

	if (eset1.size() <= eset2.size())
	{
		for (Element* e : eset1)
		{
			if (e->getType() == ELEMENTTYPE::ENTITY)
			{
				Entity* en = m_ERData->getEntity(e->getIndex());
				for (Relationship* re : en->getIncidentRelationships())
				{
					if (eset2.find(re) != eset2.end())
					{
						Element* e1 = en;
						Element* e2 = re;
						return std::make_pair(e1, e2);
					}
				}
			}
			else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
			{
				Relationship* re = m_ERData->getRelationship(e->getIndex());
				for (Entity* en : re->getIncidentEntities())
				{
					if (eset2.find(en) != eset2.end())
					{
						Element* e1 = re;
						Element* e2 = en;
						return std::make_pair(e1, e2);
					}
				}
			}
		}
	}
	else
	{
		for (Element* e : eset2)
		{
			if (e->getType() == ELEMENTTYPE::ENTITY)
			{
				Entity* en = m_ERData->getEntity(e->getIndex());
				for (Relationship* re : en->getIncidentRelationships())
				{
					if (eset1.find(re) != eset1.end())
					{
						Element* e2 = en;
						Element* e1 = re;
						return std::make_pair(e1, e2);
					}
				}
			}
			else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
			{
				Relationship* re = m_ERData->getRelationship(e->getIndex());
				for (Entity* en : re->getIncidentEntities())
				{
					if (eset1.find(en) != eset1.end())
					{
						Element* e2 = re;
						Element* e1 = en;
						return std::make_pair(e1, e2);
					}
				}
			}
		}
	}
}

Element* ERCycleDecomp::findClusterConnection(Element* e1, ERCluster* cl)
{
	std::unordered_set<Element*> eset = cl->getElemSet();
	if (e1->getType() == ELEMENTTYPE::ENTITY)
	{
		Entity* en = m_ERData->getEntity(e1->getIndex());
		for (Relationship* re : en->getIncidentRelationships())
		{
			if (eset.find(re) != eset.end())
				return re;
		}
	}
	else if (e1->getType() == ELEMENTTYPE::RELATIONSHIP)
	{
		Relationship* re = m_ERData->getRelationship(e1->getIndex());
		for (Entity* en : re->getIncidentEntities())
		{
			if (eset.find(en) != eset.end())
				return en;
		}
	}
	return NULL;
}

std::pair<Entity*, Relationship*> ERCycleDecomp::getNextCutEdge()
{
	std::pair<Entity*, Relationship*> cut = std::make_pair(nullptr, nullptr);
	if (!m_cutEdges.empty())
	{
		std::pair<Element*, Element*> edge = m_cutEdges.back();
		if (edge.first->getType() == ELEMENTTYPE::ENTITY)
		{
			cut.first = m_ERData->getEntity(edge.first->getIndex());
			cut.second = m_ERData->getRelationship(edge.second->getIndex());
		}
		else
		{
			cut.first = m_ERData->getEntity(edge.second->getIndex());
			cut.second = m_ERData->getRelationship(edge.first->getIndex());
		}
		m_cutEdges.pop_back();
	}
	return cut;
}


void ERCycleDecomp::printDecompStats()
{



	int nodes = m_ERData->getEntityNum() + m_ERData->getRelationshipNum();
	int edges = 0;
	for (Relationship* re : m_ERData->getRelationships())
		edges += re->getCardinality();
	int beta1 = edges - nodes + 1;
	float eta = (float)beta1 / (float)(nodes);

	int bridges = 0;
	int branches = 0;
	for (ERTreeComp* tcomp : m_TreeComps)
	{
		std::vector<Element*> roots = getTreeRoots(tcomp);
		if (roots.size() > 1)
			bridges++;
		else
			branches++;
	}

	int long_cycles = 0;
	int min_cycles = 0;
	for (ERCycle* cy : m_Cycles)
	{
		if (cy->getLength() == 4)
			min_cycles++;
		else
			long_cycles++;
	}

	LogConsole::writeConsole("======== Decomposition Information ========");
	LogConsole::writeConsole("Topological Blocks: %d", m_BicoComps.size());
	LogConsole::writeConsole("Bridges: %d", bridges);
	LogConsole::writeConsole("Branches: %d", branches);
	LogConsole::writeConsole("Minimal Cycles: %d", min_cycles);
	LogConsole::writeConsole("Long Cycles: %d", long_cycles);
	LogConsole::writeConsole("Forbidden Clusters: %d", m_Clusters.size());
	
	LogConsole::writeConsole("\n");

	LogConsole::writeConsole("Total Cycles: %d", m_Cycles.size());
	LogConsole::writeConsole("Beta1: %d", beta1);
	LogConsole::writeConsole("Entanglement: %.4f", eta);
	LogConsole::writeConsole("===========================================");
}