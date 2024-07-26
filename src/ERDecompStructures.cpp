#include "ERDecompStructures.h"
#include "LogConsole.h"
#include "PDOUtil.h"
#include <queue>



///////////////////////////////////////////////////////////////
// ERBicoComp
///////////////////////////////////////////////////////////////

ERBicoComp::ERBicoComp()
{

}

ERBicoComp::~ERBicoComp()
{

}

int ERBicoComp::getIndex()
{
	return m_index;
}

void ERBicoComp::setIndex(int idx)
{
	m_index = idx;
}

bool ERBicoComp::addElement(Element* e)
{
	bool result = m_elemset.insert(e).second;
	if (result)
		m_elements.push_back(e);
	return result;
}

bool ERBicoComp::containsElement(Element* e)
{
	return (m_elemset.find(e) != m_elemset.end());
}

bool ERBicoComp::replaceElement(Element* olde, Element* newe)
{
	if (!containsElement(olde))
		return false;

	// replace element in component set
	m_elemset.erase(olde);
	auto it = std::find(m_elements.begin(), m_elements.end(), olde);
	if (m_elemset.insert(newe).second)
	{
		int loc = std::distance(m_elements.begin(), it);
		m_elements.at(loc) = newe;
	}
	else
	{
		m_elements.erase(it);
	}
	
	// replace element in cycles
	std::vector<ERCycle*> cycles = m_elemCycleMap.at(olde);
	m_elemCycleMap.erase(olde);
	bool append = (m_elemCycleMap.find(newe) != m_elemCycleMap.end());
	for (ERCycle* c : cycles)
	{
		c->replaceElement(olde, newe);
		if (append)
			m_elemCycleMap.at(newe).push_back(c);
	}
	if (!append)
		m_elemCycleMap.insert(std::make_pair(newe, cycles));

	return true;
}

std::vector<Element*> ERBicoComp::getElements()
{
	return m_elements;
}

Element* ERBicoComp::getElement(int idx)
{
	return m_elements.at(idx);
}

int ERBicoComp::getNumElements()
{
	return m_elements.size();
}

void ERBicoComp::getElemCounts(int &num_ens, int &num_res)
{
	num_ens = 0; num_res = 0;
	for (Element* e : m_elements)
	{
		if (e->getType() == ELEMENTTYPE::ENTITY)
			num_ens++;
		else
			num_res++;
	}
}

void ERBicoComp::addIncidentBicoComp(ERBicoComp* comp)
{
	m_incidentBicoComps.insert(comp);
}

void ERBicoComp::addIncidentTreeComp(ERTreeComp* comp)
{
	m_incidentTreeComps.insert(comp);
}

void ERBicoComp::addCycle(ERCycle* cycle)
{
	m_cycles.push_back(cycle);

	for (Element* e : cycle->getElements())
	{
		if (m_elemCycleMap.find(e) != m_elemCycleMap.end())
			m_elemCycleMap.at(e).push_back(cycle);
		else
		{
			std::vector<ERCycle*> v{cycle};
			m_elemCycleMap.insert(std::make_pair(e, v));
		}
	}
}

std::vector<ERCycle*> ERBicoComp::getCycles()
{
	return m_cycles;
}

int ERBicoComp::getNumCycles()
{
	return m_cycles.size();
}

bool ERBicoComp::checkLinearIndependent(ERCycle* cycle, Element* root)
{
	if (m_elemCycleMap.find(root) == m_elemCycleMap.end())
		return true;

	int len = cycle->getLength();
	for (ERCycle* other : m_elemCycleMap.at(root))
	{
		int common = getNumCommonEdges(cycle, other);
		if (common == len)
			return false;
	}

	return true;
}

void ERBicoComp::initAdjacencyMatrix()
{
	// clear any previous adjacency
	m_cycleForbiddenMap.clear();
	for (ERCycle* cycle : m_cycles)
		cycle->clearAdjacency();
	m_adjacency.clear();

	int num = m_cycles.size();
	if (num < 2)
		return;

	std::vector<ERForbidden*> nadjacent;
	for (int i = 0; i < num - 1; i++)
	{
		ERCycle* c = m_cycles.at(i);
		int cidx = c->getIndex();
		int len = c->getLength();
		for (int k = 0; k < len; k++)
		{
			Element* curr = c->getElement(k);
			Element* next = c->getElement((k + 1) % len);
			for (ERCycle* a : m_elemCycleMap.at(curr))
			{
				int aidx = a->getIndex();
				if (aidx <= cidx)
					continue;

				if (a->containsEdge(curr, next))
				{
					a->addAdjacentCycle(c, true);

					int hash = pair_hash(cidx, aidx);
					if (!m_adjacency.insert(std::make_pair(hash, 1)).second &&
						c->getLength() == 4 && a->getLength() == 4)
					{
						// if there is a double edge, save it as a forbidden
						ERForbidden* fb = new ERForbidden({ c,a });
						nadjacent.push_back(fb);
					}
				}
			}
		}
	}

	processForbiddens(nadjacent, false, false, true);
}

void ERBicoComp::printAdjacencyMatrix()
{
	int num = m_cycles.size();
	for (int i = 0; i < num - 1; i++)
	{
		ERCycle* c = m_cycles.at(i);
		if (c->getLength() > 4)
			continue;
		for (int j = i + 1; j < num; j++)
		{
			ERCycle* a = m_cycles.at(j);
			if (a->getLength() > 4)
				continue;
			int hash = pair_hash(c->getIndex(), a->getIndex());
			if (m_adjacency.find(hash) != m_adjacency.end())
				LogConsole::writeConsole("%d,%d", c->getIndex(), a->getIndex());
		}
	}
}

void ERBicoComp::addForbidden(ERForbidden* fb)
{
	m_forbiddens.push_back(fb);

	for (ERCycle* c : fb->getCycles())
	{
		if (m_cycleForbiddenMap.find(c) != m_cycleForbiddenMap.end())
			m_cycleForbiddenMap.at(c).push_back(fb);
		else
		{
			std::vector<ERForbidden*> v{fb};
			m_cycleForbiddenMap.insert(std::make_pair(c, v));
		}
	}
}

void ERBicoComp::addCycleToForbidden(ERCycle* c, ERForbidden* fb)
{
	fb->addCycle(c);
	if (m_cycleForbiddenMap.find(c) != m_cycleForbiddenMap.end())
	{
		m_cycleForbiddenMap.at(c).push_back(fb);
	}
	else
	{
		std::vector<ERForbidden*> v{fb};
		m_cycleForbiddenMap.insert(std::make_pair(c, v));
	}
}

void ERBicoComp::findForbiddens()
{
	int num = m_cycles.size();
	if (num < 2)
		return;

	initAdjacencyMatrix();

	std::queue<ERCycle*> search;
	std::unordered_set<ERCycle*> visited; visited.reserve(num);

	// get a working copy of the adjacency matrix
	std::unordered_map<int, int> adjacency = m_adjacency;

	int search_idx = 0;
	while (search_idx < num)
	{
		// search each connected component of cycle adjacency graph
		ERCycle* c = m_cycles.at(search_idx);
		if (c->getLength() == 4 && visited.find(c) == visited.end())
		{
			search.push(c);
			visited.insert(c);
		}
		search_idx++;

		std::vector<ERForbidden*> candidates;
		while (!search.empty())
		{
			c = search.front();
			int c_idx = c->getIndex();
			search.pop();

			for (ERCycle* a : c->getAdjacentCycles())
			{
				if (a->getLength() != 4)
					continue;

				int a_idx = a->getIndex();
				int hash = pair_hash(c_idx, a_idx);
				if (adjacency.at(hash) < 1)
					continue;
				
				if (visited.find(a) != visited.end()) // cycle detected
				{
					//if (m_cycleForbiddenMap.find(c) == m_cycleForbiddenMap.end() ||
					//	m_cycleForbiddenMap.find(a) == m_cycleForbiddenMap.end())
					//{
					//	ERForbidden* fb = backTraceForbidden(c, a, adjacency);
					//	candidates.push_back(fb);
					//}

					ERForbidden* fb = backTraceForbidden(c, a, adjacency);
					candidates.push_back(fb);
				}
				else // no cycle detected
				{
					search.push(a);
					visited.insert(a);
				}
				
				adjacency.at(hash) -= 1;
			}
		}

		processForbiddens(candidates, false, false, true);
	}
}

ERForbidden* ERBicoComp::backTraceForbidden(ERCycle* source, ERCycle* target, std::unordered_map<int, int> adjacency)
{
	int num = m_cycles.size();
	std::queue<ERCycle*> search;
	std::unordered_set<ERCycle*> visited; visited.reserve(num);
	std::unordered_map<ERCycle*, ERCycle*> parents; parents.reserve(num);
	parents.insert(std::make_pair(source, nullptr));

	search.push(source);
	visited.insert(source);

	ERForbidden* fb = new ERForbidden();
	while (!search.empty())
	{
		ERCycle* c = search.front();
		int c_idx = c->getIndex();
		search.pop();
		for (ERCycle* a : c->getAdjacentCycles())
		{
			if (a->getLength() > 4)
				continue;

			int a_idx = a->getIndex();
			int hash = pair_hash(c_idx, a_idx);
			if (adjacency.at(hash) == m_adjacency.at(hash)) // edge not used in first search or already used in current search
				continue;
			adjacency.at(hash) += 1;

			if (a == target) // minimum cycle found
			{
				//addCycleToForbidden(target, fb);
				fb->addCycle(target);
				ERCycle* p = c;
				while (p)
				{
					//addCycleToForbidden(p, fb);
					fb->addCycle(p);
					p = parents.at(p);
				}
				search = {};
				break;
			}
			else // cycle not found
			{
				if (visited.insert(a).second)
					search.push(a);
				parents.insert(std::make_pair(a, c));
			}
		}
	}

	return fb;
}

void ERBicoComp::processForbiddens(std::vector<ERForbidden*> candidates, bool merge_strangled, bool merge_saturated, bool merge_nadjacent)
{
	if (candidates.empty())
		return;

	// verify that candidates have at least one common element
	// and classify according to number of common elements
	std::vector<ERForbidden*> strangled;
	std::vector<ERForbidden*> saturated;
	std::vector<ERForbidden*> nadjacent;
	for (ERForbidden* fb : candidates)
	{
		int common = fb->findCommonElements();

		if (common < 1)
			delete(fb);
		else if (common == 1)
		{
			fb->setType(FORBIDDENTYPE::STRANGLED);
			strangled.push_back(fb);
		}
		else if (common == 2 && 
			fb->getCommonElement(0)->getType() !=
			fb->getCommonElement(1)->getType())
		{
			fb->setType(FORBIDDENTYPE::SATURATED);
			saturated.push_back(fb);
		}
		else
		{
			fb->setType(FORBIDDENTYPE::NADJACENT);
			nadjacent.push_back(fb);
		}
	}

	// merge strangled subgraphs if they have the same common element
	if (strangled.size() > 1 && merge_strangled)
	{
		for (int i = 0; i < strangled.size() - 1; i++)
		{
			if (!strangled.at(i))
				continue;
			ERForbidden* fbi = strangled.at(i);
			for (int j = i + 1; j < strangled.size(); j++)
			{
				if (strangled.at(j))
					continue;
				ERForbidden* fbj = strangled.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					strangled.at(j) = NULL;
				}
			}
		}
	}

	// merge saturated subgraphs if they have the same common elements
	if (saturated.size() > 1 && merge_saturated)
	{
		for (int i = 0; i < saturated.size() - 1; i++)
		{
			if (!saturated.at(i))
				continue;
			ERForbidden* fbi = saturated.at(i);
			for (int j = i + 1; j < saturated.size(); j++)
			{
				if (!saturated.at(j))
					continue;
				ERForbidden* fbj = saturated.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					saturated.at(j) = NULL;
				}
			}
		}
	}

	// merge n-adjacent subgraphs if they have a matching pair of common elements
	if (nadjacent.size() > 1 && merge_nadjacent)
	{
		for (int i = 0; i < nadjacent.size() - 1; i++)
		{
			if (!nadjacent.at(i))
				continue;
			ERForbidden* fbi = nadjacent.at(i);
			for (int j = i + 1; j < nadjacent.size(); j++)
			{
				if (!nadjacent.at(j))
					continue;
				ERForbidden* fbj = nadjacent.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					nadjacent.at(j) = NULL;
				}
			}
		}
	}
	
	// add merged forbiddens
	for (ERForbidden* fb : strangled)
	{
		if (fb)
			addForbidden(fb);
	}
	for (ERForbidden* fb : saturated)
	{
		if (fb)
			addForbidden(fb);
	}
	for (ERForbidden* fb : nadjacent)
	{
		if (fb)
			addForbidden(fb);
	}
}

std::vector<ERForbidden*> ERBicoComp::getForbiddens()
{
	return m_forbiddens;
}

int ERBicoComp::getNumForbiddens()
{
	return m_cycleForbiddenMap.size();
}

void ERBicoComp::addCluster(ERCluster* cluster)
{
	m_clusters.push_back(cluster);

	for (ERForbidden* fb : cluster->getForbiddens())
	{
		if (m_forbiddenClusterMap.find(fb) != m_forbiddenClusterMap.end())
			m_forbiddenClusterMap.at(fb) = cluster;
		else
			m_forbiddenClusterMap.insert(std::make_pair(fb, cluster));
	}
}

void ERBicoComp::findClusters()
{
	int num = m_forbiddens.size();
	if (num < 1)
		return;

	std::unordered_set<ERForbidden*> visited; visited.reserve(num);
	std::stack<ERForbidden*> search;

	int search_idx = 0;
	while (search_idx < num)
	{
		ERForbidden* fb = m_forbiddens.at(search_idx);
		search_idx++;
		if (visited.find(fb) != visited.end())
			continue;
		
		ERCluster* cluster = new ERCluster({fb});
		visited.insert(fb);
		search.push(fb);

		while (!search.empty())
		{
			fb = search.top();
			search.pop();

			for (ERCycle* c : fb->getCycles())
			{
				for (ERForbidden* fj : m_cycleForbiddenMap.at(c))
				{
					if (!visited.insert(fj).second)
						continue;
					cluster->addForbidden(fj);
					search.push(fj);
				}
			}
		}

		addCluster(cluster);
	}
}

std::vector<ERCluster*> ERBicoComp::getClusters()
{
	return m_clusters;
}

ERCluster* ERBicoComp::getCluster(int idx)
{
	return m_clusters.at(idx);
}

int ERBicoComp::getNumClusters()
{
	return m_clusters.size();
}

void ERBicoComp::initAdjacency()
{
	for (ERCycle* cycle : m_cycles)
		cycle->clearAdjacency();

	int num = m_cycles.size();
	if (num < 2)
		return;

	for (int i = 0; i < num - 1; i++)
	{
		ERCycle* c = m_cycles.at(i);
		int len = c->getLength();
		for (int k = 0; k < len; k++)
		{
			Element* curr = c->getElement(k);
			Element* next = c->getElement((k + 1) % len);
			for (ERCycle* a : m_elemCycleMap.at(curr))
			{
				if (a->getIndex() < c->getIndex())
					continue;

				if (a->containsEdge(curr, next))
					a->addAdjacentCycle(c, true);
			}
		}
	}
}

void ERBicoComp::updateAdjacency()
{
	int num = m_cycles.size();
	if (num < 2)
		return;

	std::vector<ERCycle*> keep;
	std::vector<ERCycle*> discard;
	std::unordered_set<Element*> update;
	std::unordered_set<ERCycle*> checkset;
	keep.reserve(num);
	discard.reserve(num);
	for (ERCycle* c : m_cycles)
	{
		int len = c->getLength();
		if (c->to_delete && len == 4)
		{
			for (ERCycle* a : c->getAdjacentCycles())
			{
				a->removeAdjacentCycle(c);
				if (!a->to_delete)
					checkset.insert(a);
			}

			if (m_cycleClusterMap.find(c) != m_cycleClusterMap.end())
			{
				/*for (ERCluster* cluster : m_cycleClusterMap.at(c))
					cluster->removeCycle(c);*/
				ERCluster* cl = m_cycleClusterMap.at(c);
				cl->removeCycle(c);
				m_cycleClusterMap.erase(c);
			}

			for (Element* e : c->getElements())
				update.insert(e);

			discard.push_back(c);
		}
		else
		{
			keep.push_back(c);
		}
	}

	for (Element* e : update)
		updateElemCycleMap(e);

	for (ERCycle* c : discard)
		delete (c);

	if (checkset.size() > 1)
	{
		std::vector<ERCycle*> checkadj;
		checkadj.insert(checkadj.end(), checkset.begin(), checkset.end());
		checkNewAdjacency(checkadj);
	}
	
	m_cycles = keep;
}

void ERBicoComp::updateElemCycleMap(Element* e)
{
	std::vector<ERCycle*> cycles;
	for (ERCycle* c : m_elemCycleMap.at(e))
	{
		if (!c->to_delete)
			cycles.push_back(c);
	}
	m_elemCycleMap.at(e) = cycles;
}

void ERBicoComp::checkNewAdjacency(std::vector<ERCycle*> cycles)
{
	int num = cycles.size();
	if (num < 2)
		return;

	for (int i = 0; i < num - 1; i++)
	{
		ERCycle* c = cycles.at(i);
		for (int j = i + 1; j < num; j++)
		{
			ERCycle* a = cycles.at(j);
			if (getNumCommonEdges(c, a) > 0)
				c->addAdjacentCycle(a, true);
		}
	}
}

void ERBicoComp::findBiconnectedClusters()
{
	int num = m_cycles.size();
	if (num < 2)
		return;

	initAdjacency();

	std::unordered_map<ERCycle*, int> dis; dis.reserve(num);
	std::unordered_map<ERCycle*, int> low; low.reserve(num);
	std::unordered_map<ERCycle*, ERCycle*> par; par.reserve(num);
	std::unordered_map<ERCycle*, int> chi; chi.reserve(num);

	for (ERCycle* c : m_cycles)
	{
		dis.insert(std::make_pair(c, -1));
		low.insert(std::make_pair(c, -1));
		par.insert(std::make_pair(c, nullptr));
		chi.insert(std::make_pair(c, 0));
	}

	std::stack<ERCycle*> to_search;
	std::stack<ERCycle*> searched;

	// Do depth first search following Hopcroft and Tarjan's algorithm
	for (int i = 0; i < num; i++)
	{
		ERCycle* c = m_cycles.at(i);
		if (c->getLength() > 4)
			continue;
		if (dis.at(c) == -1)
			to_search.push(c);
		ERCycle* p;

		std::vector<ERCluster*> clusters;

		int iter = -1;
		while (!to_search.empty())
		{
			c = to_search.top();
			to_search.pop();
			if (dis.at(c) != -1)
				continue;

			p = par.at(c);
			if (!searched.empty() && p)
			{
				if (searched.top() != p)
					searched.push(p);
			}

			iter++;
			searched.push(c);
			dis.at(c) = iter;
			low.at(c) = iter;

			for (ERCycle* a : c->getAdjacentCycles())
			{
				if (a->getLength() > 4)
					continue;

				if (dis.at(a) == -1)
				{
					p = par.at(a);
					if (p)
						chi.at(p) -= 1;
					par.at(a) = c;
					chi.at(c) += 1;
					to_search.push(a);
				}
				else if (par.at(c) != a)
				{
					low.at(c) = std::min(low.at(c), dis.at(a));
				}
			}

			while (chi.at(c) == 0) // we have reached a leaf or back edge
			{
				p = par.at(c);
				if (p)
				{
					low.at(p) = std::min(low.at(c), low.at(p));
					chi.at(p) -= 1;

					if (low.at(c) == dis.at(p)) // biconnected component
					{
						ERCluster* cluster = new ERCluster();
						clusters.push_back(cluster);

						while (searched.top() != p)
						{
							cluster->addCycle(searched.top());
							//addCycleToCluster(cluster, searched.top());
							searched.pop();
							if (searched.empty())
								break;
						}
						cluster->addCycle(p);
						//addCycleToCluster(cluster, p);
					}
					else if (low.at(c) > dis.at(p)) // tree component, save only if there is a double edge
					{
						/*int c_idx = c->getIndex();
						int p_idx = p->getIndex();
						int hash = pair_hash(c_idx, p_idx);
						if (m_adjacency.at(hash) == 2)*/
						if (getNumCommonEdges(c, p) > 1)
						{
							ERCluster* cluster = new ERCluster();
							clusters.push_back(cluster);
							cluster->addCycle(c);
							//addCycleToCluster(cluster, c);
							cluster->addCycle(p);
							//addCycleToCluster(cluster, p);
						}
						searched.pop();
					}
					
					c = p;
				}
				else
				{
					break;
				}
			}
		}

		processClusters(clusters);
	}
}

void ERBicoComp::addCycleToCluster(ERCluster* cl, ERCycle* cy)
{
	bool result = cl->addCycle(cy);
	if (!result)
		return;

	m_cycleClusterMap.insert(std::make_pair(cy, cl));

	//if (m_cycleClusterMap.find(cy) != m_cycleClusterMap.end())
	//{
	//	m_cycleClusterMap.at(cy).push_back(cl);
	//}
	//else
	//{
	//	std::vector<ERCluster*> v{cl};
	//	m_cycleClusterMap.insert(std::make_pair(cy, v));
	//}
}

void ERBicoComp::processClusters(std::vector<ERCluster*> clusters)
{
	int num = clusters.size();
	if (num < 1)
		return;
	if (num < 2)
	{
		ERCluster* cl = clusters.at(0);
		for (ERCycle* cy : cl->getCycles())
			m_cycleClusterMap.insert(std::make_pair(cy, cl));
		m_clusters.push_back(clusters.at(0));
		return;
	}

	for (int i = 0; i < num - 1; i++)
	{
		if (!clusters.at(i))
			continue;
		ERCluster* cli = clusters.at(i);
		for (int j = i + 1; j < num; j++)
		{
			if (!clusters.at(j))
				continue;
			ERCluster* clj = clusters.at(j);
			if (cli->tryMerge(clj))
			{
				delete(clj);
				clusters.at(j) = NULL;
			}
		}
	}

	for (ERCluster* cl : clusters)
	{
		if (cl)
		{
			cl->calcEntanglement();
			for (ERCycle* cy : cl->getCycles())
				m_cycleClusterMap.insert(std::make_pair(cy, cl));
			m_clusters.push_back(cl);
		}
	}
}

ERCluster* ERBicoComp::getElementCluster(Element* e)
{
	ERCluster* cl = NULL;
	for (ERCycle* cy : m_elemCycleMap.at(e))
	{
		if (cy->getLength() > 4)
			continue;
		if (m_cycleClusterMap.find(cy) == m_cycleClusterMap.end())
			continue;

		cl = m_cycleClusterMap.at(cy);
		break;
	}
	return cl;
}

int ERBicoComp::getNumLongCycles(int len)
{
	int num = 0;
	for (ERCycle* cy : m_cycles)
	{
		if (cy->getLength() > len)
			num++;
	}
	return num;
}


CycleCount ERBicoComp::getCycleCountsForEdge(std::pair<Element*, Element*> edge)
{
	CycleCount cc;
	
	Element* e1 = edge.first;
	Element* e2 = edge.second;

	for (ERCycle* cy : m_elemCycleMap.at(e1))
	{
		int len = cy->getLength();
		if (len == 4)
			continue;
		if (cy->containsEdge(e1, e2))
		{
			cc.long_count++;
			cc.cycles.push_back(cy);
			cc.avg_len += len;
			if (cc.longest_len < len)
				cc.longest_len = len;
			if (cc.shortest_len > len)
				cc.shortest_len = len;
		}
	}

	cc.avg_len = cc.avg_len / (float)cc.long_count;
	return cc;
}

CycleCount ERBicoComp::getCycleCountsForPair(Element* e1, Element* e2)
{
	int count = 0;
	int len_sum = 0;
	int longest = 0;
	int shortest = INT_MAX;
	for (ERCycle* cy : m_elemCycleMap.at(e1))
	{
		int len = cy->getLength();
		if (len == 4)
			continue;
		if (cy->findElement(e2) != -1)
		{
			count++;
			len_sum += cy->getLength();
			if (longest < len)
				longest = len;
			if (shortest > len)
				shortest = len;
		}
	}

	float len_avg = (float)len_sum / (float)count;
	return CycleCount(count, shortest, longest, len_avg);
}

std::vector<int> ERBicoComp::getCyclesForPair(Element* e1, Element* e2)
{
	std::vector<int> cycles;
	for (ERCycle* cy : m_elemCycleMap.at(e1))
	{
		if (cy->findElement(e2) != -1)
			cycles.push_back(cy->getIndex());
	}
	return cycles;
}



///////////////////////////////////////////////////////////////
// ERTreeComp
///////////////////////////////////////////////////////////////

ERTreeComp::ERTreeComp()
{

}

ERTreeComp::~ERTreeComp()
{

}

int ERTreeComp::getIndex()
{
	return m_index;
}

void ERTreeComp::setIndex(int idx)
{
	m_index = idx;
}

bool ERTreeComp::addElement(Element* e)
{
	return m_elements.insert(e).second;
}

bool ERTreeComp::containsElement(Element* e)
{
	return (m_elements.find(e) != m_elements.end());
}

bool ERTreeComp::replaceElement(Element* olde, Element* newe)
{
	if (!containsElement(olde))
		return false;

	m_elements.erase(olde);
	m_elements.insert(newe);

	return true;
}

std::unordered_set<Element*> ERTreeComp::getElements()
{
	return m_elements;
}

void ERTreeComp::addIncidentBicoComp(ERBicoComp* comp)
{
	m_incidentBicoComps.insert(comp);
}



///////////////////////////////////////////////////////////////
// ERBridge
///////////////////////////////////////////////////////////////

ERBridge::ERBridge()
{
}

ERBridge::ERBridge(std::vector<Element*> elements)
{
	m_elements = elements;
}

ERBridge::ERBridge(Entity* en)
{
	m_elements.push_back(en);

	std::vector<Relationship*> relist = en->getIncidentRelationships();
	m_incidentElements.insert(m_incidentElements.end(), relist.begin(), relist.end());
}

ERBridge::ERBridge(Relationship* re)
{
	m_elements.push_back(re);

	std::vector<Entity*> enlist = re->getIncidentEntities();
	m_incidentElements.insert(m_incidentElements.end(), enlist.begin(), enlist.end());
}

ERBridge::~ERBridge()
{

}

void ERBridge::setIndex(int idx)
{
	m_index = idx;
}

int ERBridge::getIndex()
{
	return m_index;
}

int ERBridge::getNumElements()
{
	return m_elements.size();
}

std::vector<Element*> ERBridge::getElements()
{
	return m_elements;
}

int ERBridge::findElement(Element* e)
{
	auto it = std::find(m_elements.begin(), m_elements.end(), e);
	if (it != m_elements.end())
		return std::distance(m_elements.begin(), it);
	else
		return -1;
}

void ERBridge::addElement(Element* e)
{
	m_elements.push_back(e);
}

std::vector<ERCycle*> ERBridge::getIncidentCycles()
{
	return m_incidentCycles;
}

void ERBridge::addIncidentCycle(ERCycle* c, bool symmetric)
{
	if (std::find(m_incidentCycles.begin(), m_incidentCycles.end(), c) != m_incidentCycles.end())
		return;

	m_incidentCycles.push_back(c);

	if (symmetric)
		c->addIncidentBridge(this, false);
}

void ERBridge::clearCycleIncidence()
{
	m_incidentCycles.clear();
}

std::vector<Element*> ERBridge::getIncidentElements()
{
	return m_incidentElements;
}

int ERBridge::getNumIncidentElements()
{
	return m_incidentElements.size();
}

Element* ERBridge::getIncidentElement(int idx)
{
	return m_incidentElements[idx];
}

void ERBridge::addIncidentElement(Element* e)
{
	if (std::find(m_incidentElements.begin(), m_incidentElements.end(), e) == m_incidentElements.end())
		m_incidentElements.push_back(e);
}



///////////////////////////////////////////////////////////////
// ERCycle
///////////////////////////////////////////////////////////////

ERCycle::ERCycle()
{

}

ERCycle::ERCycle(std::vector<Element*> elements)
{
	m_elements = elements;
}

ERCycle::~ERCycle()
{

}

void ERCycle::setIndex(int idx)
{
	m_index = idx;
}

int ERCycle::getIndex()
{
	return m_index;
}

int ERCycle::getLength()
{
	return m_elements.size();
}

Element* ERCycle::getElement(int idx)
{
	return m_elements.at(idx);
}

int ERCycle::findElement(Element* e)
{
	auto it = std::find(m_elements.begin(), m_elements.end(), e);
	if (it != m_elements.end())
		return std::distance(m_elements.begin(), it);
	else
		return -1;
}

void ERCycle::replaceElement(Element* olde, Element* newe)
{
	int loc = findElement(olde);
	m_elements[loc] = newe;
}

std::vector<Element*> ERCycle::getElements()
{
	return m_elements;
}

std::vector<ERCycle*> ERCycle::getAdjacentCycles()
{
	return m_adjacent;
}

bool ERCycle::containsEdge(Element* e1, Element* e2)
{
	int idx = findElement(e1);
	if (idx == -1)
		return false;
	
	int len = getLength();
	int next = (idx + 1) % len;
	int prev = (idx + len - 1) % len;
	if (getElement(next) == e2 || getElement(prev) == e2)
		return true;
	else
		return false;
}

bool ERCycle::containsElement(Element* e)
{
	if (std::find(m_elements.begin(), m_elements.end(), e) == m_elements.end())
		return false;
	else
		return true;
}

void ERCycle::addAdjacentCycle(ERCycle* c, bool symmetric)
{
	if (c == this)
		return;

	if (std::find(m_adjacent.begin(), m_adjacent.end(), c) != m_adjacent.end())
		return;

	m_adjacent.push_back(c);

	if (symmetric)
		c->addAdjacentCycle(this, false);
}

void ERCycle::removeAdjacentCycle(ERCycle* c)
{
	auto it = std::find(m_adjacent.begin(), m_adjacent.end(), c);
	m_adjacent.erase(it);
}

void ERCycle::clearAdjacency()
{
	m_adjacent.clear();
}

void ERCycle::addIncidentBridge(ERBridge* b, bool symmetric)
{
	if (std::find(m_incident.begin(), m_incident.end(), b) != m_incident.end())
		return;

	m_incident.push_back(b);

	if (symmetric)
		b->addIncidentCycle(this, false);
}

void ERCycle::clearIncidence()
{
	m_incident.clear();
}



///////////////////////////////////////////////////////////////
// ERForbidden
///////////////////////////////////////////////////////////////

ERForbidden::ERForbidden()
{

}

ERForbidden::ERForbidden(std::vector<ERCycle*> cycles)
{
	m_Cycles = cycles;
}

ERForbidden::~ERForbidden()
{

}

void ERForbidden::setCycles(std::vector<ERCycle*> cycles)
{
	m_Cycles = cycles;
}

std::vector<ERCycle*> ERForbidden::getCycles()
{
	return m_Cycles;
}

ERCycle* ERForbidden::getCycle(int idx)
{
	return m_Cycles.at(idx);
}

int ERForbidden::getNumCycles()
{
	return m_Cycles.size();
}

void ERForbidden::addCycle(ERCycle* cycle)
{
	m_Cycles.push_back(cycle);
}

std::vector<Element*> ERForbidden::getElements()
{
	std::unordered_set<Element*> element_set;
	for (ERCycle* c : m_Cycles)
	{
		for (Element* elem : c->getElements())
			element_set.insert(elem);
	}

	std::vector<Element*> elements;
	std::copy(element_set.begin(), element_set.end(), std::back_inserter(elements));

	return elements;
}

int ERForbidden::findCommonElements()
{
	for (Element* e : m_Cycles[0]->getElements())
	{
		bool common = true;
		for (int i = 1; i < m_Cycles.size(); i++)
		{
			ERCycle* c = m_Cycles.at(i);
			if (c->findElement(e) == -1)
			{
				common = false;
				break;
			}
		}
		if (common)
			m_CommonElems.push_back(e);
	}

	return m_CommonElems.size();
}

void ERForbidden::setCommonElements(std::vector<Element*> common)
{
	m_CommonElems = common;
}

std::vector<Element*> ERForbidden::getCommonElements()
{
	return m_CommonElems;
}

Element* ERForbidden::getCommonElement(int idx)
{
	return m_CommonElems.at(idx);
}

void ERForbidden::setIndex(int idx)
{
	m_index = idx;
}

int ERForbidden::getIndex()
{
	return m_index;
}

void ERForbidden::setType(FORBIDDENTYPE type)
{
	m_type = type;
}

FORBIDDENTYPE ERForbidden::getType()
{
	return m_type;
}

bool ERForbidden::merge(ERForbidden* other)
{
	// must have at least one common element to merge
	//int num_re_common = 0;
	//int num_en_common = 0;
	std::vector<Element*> common_common;
	for (Element* e : other->getCommonElements())
	{
		if (std::find(m_CommonElems.begin(), m_CommonElems.end(), e) != m_CommonElems.end())
		{
			common_common.push_back(e);
			/*if (e->getType() == ELEMENTTYPE::ENTITY)
				num_en_common++;
			else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
				num_re_common++;*/
		}
	}

	if (!common_common.empty()/*num_re_common > 1 || num_en_common > 1*/)
	{
		std::unordered_set<ERCycle*> cycles;
		for (ERCycle* cycle : m_Cycles)
			cycles.insert(cycle);
		for (ERCycle* cycle : other->getCycles())
			cycles.insert(cycle);
		m_Cycles.clear();
		for (ERCycle* cycle : cycles)
			m_Cycles.push_back(cycle);

		m_CommonElems = common_common;
		return true;
	}
	else
		return false;
}

bool ERForbidden::tryMerge(ERForbidden* other)
{
	FORBIDDENTYPE o_type = other->getType();
	if (m_type != o_type)
		return false;

	// strangled subgraphs must have same common element
	if (m_type == FORBIDDENTYPE::STRANGLED &&
		m_CommonElems.at(0) != other->getCommonElement(0))
		return false;

	// saturated subgraphs must have same common elements
	if (m_type == FORBIDDENTYPE::SATURATED)
	{
		Element* m0 = m_CommonElems.at(0);
		Element* m1 = m_CommonElems.at(1);
		Element* o0 = other->getCommonElement(0);
		Element* o1 = other->getCommonElement(1);
		if ((m0 != o0 || m1 != o1) && (m0 != o1 || m1 != o0))
			return false;
	}

	// n-adjacent subgraphs must have a pair of matching common elements
	if (m_type == FORBIDDENTYPE::NADJACENT)
	{
		int num_re_common = 0;
		int num_en_common = 0;
		std::vector<Element*> common_common;
		for (Element* e : other->getCommonElements())
		{
			if (std::find(m_CommonElems.begin(), m_CommonElems.end(), e) != m_CommonElems.end())
			{
				common_common.push_back(e);
				if (e->getType() == ELEMENTTYPE::ENTITY)
					num_en_common++;
				else if (e->getType() == ELEMENTTYPE::RELATIONSHIP)
					num_re_common++;
			}
		}

		if (num_en_common < 2 && num_re_common < 2)
			return false;
		else
			m_CommonElems = common_common;
	}

	// do the merging
	std::unordered_set<ERCycle*> cycles;
	for (ERCycle* cycle : m_Cycles)
		cycles.insert(cycle);
	for (ERCycle* cycle : other->getCycles())
		cycles.insert(cycle);
	m_Cycles.clear();
	for (ERCycle* cycle : cycles)
		m_Cycles.push_back(cycle);

	return true;
}



///////////////////////////////////////////////////////////////
// ERCluster
///////////////////////////////////////////////////////////////

ERCluster::ERCluster()
{

}

ERCluster::ERCluster(std::vector<ERForbidden*> forbiddens)
{
	m_forbiddens = forbiddens;
}

ERCluster::~ERCluster()
{

}

void ERCluster::setIndex(int idx)
{
	m_index = idx;
}

int ERCluster::getIndex()
{
	return m_index;
}

bool ERCluster::addCycle(ERCycle* cycle)
{
	bool result = m_cycleset.insert(cycle).second;
	if (result)
		m_cycles.push_back(cycle);
	return result;
}

void ERCluster::removeCycle(ERCycle* cycle)
{
	if (m_cycles.empty())
		return;
	m_cycleset.erase(cycle);
	auto it = std::find(m_cycles.begin(), m_cycles.end(), cycle);
	m_cycles.erase(it);
}

std::vector<ERCycle*> ERCluster::getCycles()
{
	return m_cycles;
}

int ERCluster::getNumCycles()
{
	return m_cycles.size();
}

bool ERCluster::containsCycle(ERCycle* cycle)
{
	return (m_cycleset.find(cycle) != m_cycleset.end());
}

void ERCluster::addForbidden(ERForbidden* fb)
{
	fb->setIndex(m_forbiddens.size());
	m_forbiddens.push_back(fb);
}

void ERCluster::removeForbidden(ERForbidden* fb)
{
	auto it = std::find(m_forbiddens.begin(), m_forbiddens.end(), fb);
	if (it != m_forbiddens.end())
		m_forbiddens.erase(it);
}

std::vector<ERForbidden*> ERCluster::getForbiddens()
{
	return m_forbiddens;
}

ERForbidden* ERCluster::getForbidden(int idx)
{
	return m_forbiddens.at(idx);
}

int ERCluster::getNumForbiddens()
{
	return m_forbiddens.size();
}

std::vector<Element*> ERCluster::getElements()
{
	std::unordered_set<Element*> element_set;
	if (!m_cycles.empty())
	{
		for (ERCycle* c : m_cycles)
		{
			for (Element* e : c->getElements())
				element_set.insert(e);
		}
			
	}
	else if (!m_forbiddens.empty())
	{
		for (ERForbidden* fb : m_forbiddens)
		{
			for (ERCycle* cl : fb->getCycles())
			{
				for (Element* e : cl->getElements())
					element_set.insert(e);
			}
		}
	}

	std::vector<Element*> elements;
	if (!element_set.empty())
		std::copy(element_set.begin(), element_set.end(), std::back_inserter(elements));

	return elements;
}

std::unordered_set<Element*> ERCluster::getElemSet()
{
	std::unordered_set<Element*> element_set;
	if (!m_cycles.empty())
	{
		for (ERCycle* c : m_cycles)
		{
			for (Element* e : c->getElements())
				element_set.insert(e);
		}
	}
	else if (!m_forbiddens.empty())
	{
		for (ERForbidden* fb : m_forbiddens)
		{
			for (ERCycle* cl : fb->getCycles())
			{
				for (Element* e : cl->getElements())
					element_set.insert(e);
			}
		}
	}
	return element_set;
}

int ERCluster::getNumElements()
{
	std::unordered_set<Element*> element_set;
	if (!m_cycles.empty())
	{
		for (ERCycle* c : m_cycles)
		{
			for (Element* e : c->getElements())
				element_set.insert(e);
		}

	}
	else if (!m_forbiddens.empty())
	{
		for (ERForbidden* fb : m_forbiddens)
		{
			for (ERCycle* cl : fb->getCycles())
			{
				for (Element* e : cl->getElements())
					element_set.insert(e);
			}
		}
	}
	
	return element_set.size();
}

bool ERCluster::tryMerge(ERCluster* other)
{
	// clusters must share a common cycle
	bool common_cycle = false;
	if (this->getNumCycles() < other->getNumCycles())
	{
		for (ERCycle* c : this->getCycles())
			if (other->containsCycle(c))
				common_cycle = true;
	}
	else
	{
		for (ERCycle* c : other->getCycles())
			if (this->containsCycle(c))
				common_cycle = true;
	}

	if (!common_cycle)
		return false;

	// do the merging
	for (ERCycle* c : other->getCycles())
		this->addCycle(c);
	
	return true;
}

void ERCluster::initAdjacencyMatrix()
{
	m_adjacency.clear();
	m_forbiddens.clear();

	if (m_cycles.size() < 2)
		return;

	std::vector<ERForbidden*> nadjacent;
	for (ERCycle* c : m_cycles)
	{
		for (ERCycle* a : c->getAdjacentCycles())
		{
			if (a->getLength() > 4)
				continue;
			
			if (a->getIndex() < c->getIndex())
				continue;
			
			int common = getNumCommonEdges(c, a);
			if (common > 1)
			{
				ERForbidden* fb = new ERForbidden({ c,a });
				nadjacent.push_back(fb);
			}

			int hash = pair_hash(c->getIndex(), a->getIndex());
			m_adjacency.insert(std::make_pair(hash, 1));
		}
	}

	processForbiddens(nadjacent,false,false,true);
}

bool ERCluster::findForbiddens()
{
	initAdjacencyMatrix();

	int num = m_cycles.size();
	std::queue<ERCycle*> search;
	std::unordered_set<ERCycle*> visited; visited.reserve(num);

	// get a working copy of the adjacency matrix
	std::unordered_map<int, int> adjacency = m_adjacency;

	int search_idx = 0;
	while (search_idx < num)
	{
		// search each connected component of cycle adjacency graph
		ERCycle* c = m_cycles.at(search_idx);
		if (c->getLength() == 4 && visited.find(c) == visited.end())
		{
			search.push(c);
			visited.insert(c);
		}
		search_idx++;

		std::vector<ERForbidden*> candidates;
		while (!search.empty())
		{
			c = search.front();
			int c_idx = c->getIndex();
			search.pop();

			for (ERCycle* a : c->getAdjacentCycles())
			{
				if (a->getLength() != 4)
					continue;
				if (m_cycleset.find(a) == m_cycleset.end())
					continue;

				int a_idx = a->getIndex();
				int hash = pair_hash(c_idx, a_idx);
				if (adjacency.at(hash) < 1)
					continue;

				if (visited.find(a) != visited.end()) // cycle detected
				{
					//if (m_cycleForbiddenMap.find(c) == m_cycleForbiddenMap.end() ||
					//	m_cycleForbiddenMap.find(a) == m_cycleForbiddenMap.end())
					//{
					//	ERForbidden* fb = backTraceForbidden(c, a, adjacency);
					//	candidates.push_back(fb);
					//}

					ERForbidden* fb = backTraceForbidden(c, a, adjacency);
					candidates.push_back(fb);
				}
				else // no cycle detected
				{
					search.push(a);
					visited.insert(a);
				}

				adjacency.at(hash) -= 1;
			}
		}

		processForbiddens(candidates, false, false, true);
	}

	return (!m_forbiddens.empty());
}

ERForbidden* ERCluster::backTraceForbidden(ERCycle* source, ERCycle* target, std::unordered_map<int, int> adjacency)
{
	int num = m_cycles.size();
	std::queue<ERCycle*> search;
	std::unordered_set<ERCycle*> visited; visited.reserve(num);
	std::unordered_map<ERCycle*, ERCycle*> parents; parents.reserve(num);
	parents.insert(std::make_pair(source, nullptr));

	search.push(source);
	visited.insert(source);

	ERForbidden* fb = new ERForbidden();
	while (!search.empty())
	{
		ERCycle* c = search.front();
		int c_idx = c->getIndex();
		search.pop();
		for (ERCycle* a : c->getAdjacentCycles())
		{
			if (a->getLength() > 4)
				continue;
			if (m_cycleset.find(a) == m_cycleset.end())
				continue;

			int a_idx = a->getIndex();
			int hash = pair_hash(c_idx, a_idx);
			if (adjacency.at(hash) == m_adjacency.at(hash)) // edge not used in first search or already used in current search
				continue;
			adjacency.at(hash) += 1;

			if (a == target) // minimum cycle found
			{
				fb->addCycle(target);
				ERCycle* p = c;
				while (p)
				{
					fb->addCycle(p);
					p = parents.at(p);
				}
				search = {};
				break;
			}
			else // cycle not found
			{
				if (visited.insert(a).second)
					search.push(a);
				parents.insert(std::make_pair(a, c));
			}
		}
	}

	return fb;
}

void ERCluster::processForbiddens(std::vector<ERForbidden*> candidates, bool merge_strangled, bool merge_saturated, bool merge_nadjacent)
{
	if (candidates.empty())
		return;

	std::vector<ERForbidden*> strangled;
	std::vector<ERForbidden*> saturated;
	std::vector<ERForbidden*> nadjacent;
	for (ERForbidden* fb : candidates)
	{
		int common = fb->findCommonElements();

		if (common < 1)
			delete(fb);
		else if (common == 1)
		{
			fb->setType(FORBIDDENTYPE::STRANGLED);
			strangled.push_back(fb);
		}
		else if (common == 2 &&
			fb->getCommonElement(0)->getType() !=
			fb->getCommonElement(1)->getType())
		{
			fb->setType(FORBIDDENTYPE::SATURATED);
			saturated.push_back(fb);
		}
		else
		{
			fb->setType(FORBIDDENTYPE::NADJACENT);
			nadjacent.push_back(fb);
		}
	}

	// merge strangled subgraphs if they have the same common element
	if (strangled.size() > 1 && merge_strangled)
	{
		for (int i = 0; i < strangled.size() - 1; i++)
		{
			if (!strangled.at(i))
				continue;
			ERForbidden* fbi = strangled.at(i);
			for (int j = i + 1; j < strangled.size(); j++)
			{
				if (strangled.at(j))
					continue;
				ERForbidden* fbj = strangled.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					strangled.at(j) = NULL;
				}
			}
		}
	}

	// merge saturated subgraphs if they have the same common elements
	if (saturated.size() > 1 && merge_saturated)
	{
		for (int i = 0; i < saturated.size() - 1; i++)
		{
			if (!saturated.at(i))
				continue;
			ERForbidden* fbi = saturated.at(i);
			for (int j = i + 1; j < saturated.size(); j++)
			{
				if (!saturated.at(j))
					continue;
				ERForbidden* fbj = saturated.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					saturated.at(j) = NULL;
				}
			}
		}
	}

	// merge n-adjacent subgraphs if they have a matching pair of common elements
	if (nadjacent.size() > 1 && merge_nadjacent)
	{
		for (int i = 0; i < nadjacent.size() - 1; i++)
		{
			if (!nadjacent.at(i))
				continue;
			ERForbidden* fbi = nadjacent.at(i);
			for (int j = i + 1; j < nadjacent.size(); j++)
			{
				if (!nadjacent.at(j))
					continue;
				ERForbidden* fbj = nadjacent.at(j);
				if (fbi->tryMerge(fbj))
				{
					delete(fbj);
					nadjacent.at(j) = NULL;
				}
			}
		}
	}

	// add merged forbiddens
	for (ERForbidden* fb : strangled)
	{
		if (fb)
			addForbidden(fb);
	}
	for (ERForbidden* fb : saturated)
	{
		if (fb)
			addForbidden(fb);
	}
	for (ERForbidden* fb : nadjacent)
	{
		if (fb)
			addForbidden(fb);
	}
}

void ERCluster::calcEntanglement()
{
	std::unordered_set<Element*> element_set;
	std::unordered_set<std::pair<Element*, Element*>> edge_set;
	if (!m_cycles.empty())
	{
		for (ERCycle* c : m_cycles)
		{
			int len = c->getLength();
			for (int i = 0; i < len; i++)
			{
				Element* e1 = c->getElement(i);
				Element* e2 = c->getElement((i + 1)%len);
				element_set.insert(e1);
				edge_set.insert(std::make_pair(e1, e2));
			}
		}

	}
	else if (!m_forbiddens.empty())
	{
		for (ERForbidden* fb : m_forbiddens)
		{
			for (ERCycle* cl : fb->getCycles())
			{
				int len = c->getLength();
				for (int i = 0; i < len; i++)
				{
					Element* e1 = c->getElement(i);
					Element* e2 = c->getElement((i + 1) % len);
					element_set.insert(e1);
					edge_set.insert(std::make_pair(e1, e2));
				}
			}
		}
	}

	m_entanglement = float(edge_set.size() - element_set.size() + 1) / float(element_set.size());
}

float ERCluster::getEntanglement()
{
	return m_entanglement;
}


///////////////////////////////////////////////////////////////
// Structure Utilities
///////////////////////////////////////////////////////////////

int getNumCommonEdges(ERCycle* c, ERCycle* a)
{
	int count = 0;
	int num = c->getLength();
	for (int i = 0; i < num; i++)
	{
		int j = (i + 1) % num;
		Element* e1 = c->getElement(i);
		Element* e2 = c->getElement(j);
		if (a->containsEdge(e1,e2))
			count++;
	}
	return count;
}