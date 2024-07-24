#include "ERProgressive.h"
#include "LogConsole.h"
#include "ElementUtils.h"
#include "mainwindow.h"

ERProgressive::ERProgressive()
{
	m_ERData = NULL;
	m_dualERData = NULL;
}

ERProgressive::~ERProgressive()
{
	clean_operations();

	m_ERData = NULL;
	m_dualERData = NULL;
}

ERData* ERProgressive::getERData()
{
	return m_ERData;
}

ERData* ERProgressive::getDualData()
{
	return m_dualERData;
}

void ERProgressive::setSimilarityCoefficients(double alpha, double beta, double gamma)
{
	m_alpha = alpha;
	m_beta = beta;
	m_gamma = gamma;
}

void ERProgressive::setOps(bool erem, bool emerge, bool nrem, bool nmerge)
{
	m_erem = erem;
	m_emerge = emerge;
	m_nrem = nrem;
	m_nmerge = nmerge;
}

void ERProgressive::clean_operations()
{
	while(!m_opqueue.empty())
	{
		OperationRecord* op = m_opqueue.back();
		m_opqueue.pop_back();
		delete op;
	}
	while (!m_applied.empty())
	{
		OperationRecord* op = m_applied.top();
		m_applied.pop();
		delete op;
	}
	while (!m_inverted.empty())
	{
		OperationRecord* op = m_inverted.top();
		m_inverted.pop();
		delete op;
	}
}

void ERProgressive::clear_opqueue()
{
	while (!m_opqueue.empty())
	{
		OperationRecord* op = m_opqueue.back();
		m_opqueue.pop_back();
		delete op;
	}
}

bool ERProgressive::initialize_operation_queue(ERData* erdata)
{
	if (erdata)
		m_ERData = erdata;

	// cleanup
	while (!m_opqueue.empty())
	{
		OperationRecord* op = m_opqueue.back();
		m_opqueue.pop_back();
		delete op;
	}

	int nverts = m_ERData->getEntityNum();
	int nedges = m_ERData->getRelationshipNum();
	

	// construct set of legal operations
	std::unordered_set<OperationRecord*> op_set;
	for (Entity* remove : m_ERData->getEntities())
	{
		if (m_nrem)
		{
			//bool creates_orphans = m_ERData->deleteCreatesZerogons(remove);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (remove->getDegree() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(remove) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(remove) : false;
			if (/*!creates_orphans &&*/ is_leaf && !breaks_connection && !is_branch_end)
			{
				OperationRecord* op = remove->getRemovalOp();
				op->setLegal(true);
				op_set.insert(op);
			}
		}

		if (m_nmerge)
		{
			for (OperationRecord* op : remove->getMergerOps())
			{
				Entity* retain = m_ERData->getEntityByOpidx(op->getRetainedIndex());
				if (retain == remove)
					continue;
				int share_num = m_ERData->getSharedRelationshipNum(remove, retain);
				if (share_num > m_share /*&& (share_num == remove->getDegree() || share_num == retain->getDegree())*/)
				{
					op->setLegal(true);
					op_set.insert(op);
				}
			}
		}
	}

	for (Relationship* remove : m_ERData->getRelationships())
	{
		if (m_erem)
		{
			//bool creates_orphans = m_ERData->deleteCreatesOrphans(remove);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (remove->getCardinality() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(remove) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(remove) : false;
			if (/*!creates_orphans &&*/ is_leaf && !breaks_connection && !is_branch_end)
			{
				OperationRecord* op = remove->getRemovalOp();
				op->setLegal(true);
				op_set.insert(op);
			}
		}

		if (m_emerge)
		{
			for (OperationRecord* op : remove->getMergerOps())
			{
				Relationship* retain = m_ERData->getRelationshipByOpidx(op->getRetainedIndex());
				if (retain == remove)
					continue;
				int share_num = m_ERData->getSharedEntityNum(remove, retain);
				if (share_num > m_share /*&& (share_num == remove->getCardinality() || share_num == retain->getCardinality())*/)
				{
					op->setLegal(true);
					op_set.insert(op);
				}
			}
		}
	}

	if (op_set.size() > 0)
	{
		std::copy(op_set.begin(), op_set.end(), std::back_inserter(m_opqueue));
		std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
		return true;
	}
	else
		return false;
}

bool ERProgressive::apply_operation(int max_ops, double priority_thresh, int min_edges, int min_nodes, OperationRecord* OR)
{
	OperationRecord* op = OR;
	bool from_queue;
	if (op)
	{
		for (OperationRecord* qop : m_opqueue)
		{
			if (op == qop)
			{
				from_queue = true;
				break;
			}
		}
		if (!from_queue)
		{
			return false;
		}
	}
	else
	{
		if (!m_inverted.empty())
		{
			op = m_inverted.top();
			m_inverted.pop();
			from_queue = false;
		}
		else if (!m_opqueue.empty())
		{
			op = m_opqueue.front();
			std::pop_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
			m_opqueue.pop_back();
			from_queue = true;
		}
		else
		{
			return false;
		}
	}

	// check termination criteria
	if (from_queue)
	{
		bool linear, opmax, thresh, emin, nmin;
		linear = m_ERData->isLinear();
		opmax = (m_applied.size() >= max_ops);
		thresh = (op->getPriority() < priority_thresh);
		emin = (m_ERData->getRelationshipNum() <= min_edges);
		nmin = (m_ERData->getEntityNum() <= min_nodes);

		if (linear || opmax || thresh || (emin && nmin))
		{
			LogConsole::writeGreenConsole("Termination criteria reached");
			if (linear)
				LogConsole::writeConsole("hypergraph is linear");
			if (opmax)
				LogConsole::writeConsole("exceeded maximum operations");
			if (thresh)
				LogConsole::writeConsole("reached priority threshold");
			if (emin && nmin)
				LogConsole::writeConsole("reached minimum elements");
			return false;
		}
	}

	bool result;
	vector<pair<int, std::string>> incidentRemoved;
	vector<int> incidentRetained;
	if (op->getType() == OPERATION_TYPE::EDGE_MERGE && m_emerge)
	{
		Relationship* remove = m_ERData->getRelationshipByOpidx(op->getRemovedIndex());
		Relationship* retain = m_ERData->getRelationshipByOpidx(op->getRetainedIndex());

		if (!remove || !retain)
			result = false;
		else if (m_ERData->isSubgraphLinear(remove))
			result = false;
		else
		{
			//LogConsole::writeConsole("merging %s with %s", remove->getLabel().c_str(), retain->getLabel().c_str());

			vector<int> adjacent;
			unordered_set<int> adjacent_set;
			for (Relationship* re : remove->getAdjacentRelationships())
				if (re != retain) adjacent_set.insert(re->getOpidx());
			for (Relationship* re : retain->getAdjacentRelationships())
				if (re != remove) adjacent_set.insert(re->getOpidx());
			std::copy(adjacent_set.begin(), adjacent_set.end(), std::back_inserter(adjacent));

			vector<int> incident;
			unordered_set<int> incident_set;
			for (Entity* en : remove->getIncidentEntities())
				incident_set.insert(en->getOpidx());
			for (Entity* en : retain->getIncidentEntities())
				incident_set.insert(en->getOpidx());
			std::copy(incident_set.begin(), incident_set.end(), std::back_inserter(incident));

			result = m_ERData->applyRelationshipOperation(remove, retain, &incidentRemoved, &incidentRetained);
			if (result && from_queue)
			{
				op->setLegal(false);
				op->setApplied(true);
				updateQueueEdgeMerge(op->getRemovedIndex(), op->getRetainedIndex(), adjacent, incident);
			}
		}
	}
	else if (op->getType() == OPERATION_TYPE::EDGE_DELETE && m_erem)
	{
		Relationship* remove = m_ERData->getRelationshipByOpidx(op->getRemovedIndex());
		Relationship* retain = NULL;
		
		if (!remove)
			result = false;
		else if (m_ERData->isSubgraphLinear(remove))
			result = false;
		else
		{
			//LogConsole::writeConsole("removing %s", remove->getLabel().c_str());

			vector<int> adjacent;
			for (Relationship* re : remove->getAdjacentRelationships())
				adjacent.push_back(re->getOpidx());

			vector<int> incident;
			for (Entity* en : remove->getIncidentEntities())
				incident.push_back(en->getOpidx());

			result = m_ERData->applyRelationshipOperation(remove, retain, &incidentRemoved, &incidentRetained);
			if (result && from_queue)
			{	
				op->setApplied(true);
				op->setLegal(false);
				updateQueueEdgeDelete(op->getRemovedIndex(), adjacent, incident);
			}
		}
	}
	else if (op->getType() == OPERATION_TYPE::NODE_MERGE && m_nmerge)
	{
		Entity* remove = m_ERData->getEntityByOpidx(op->getRemovedIndex());
		Entity* retain = m_ERData->getEntityByOpidx(op->getRetainedIndex());

		if (!remove || !retain)
			result = false;
		else if (m_ERData->isSubgraphLinear(remove))
			result = false;
		else
		{
			//LogConsole::writeConsole("merging %s with %s", remove->getLabel().c_str(), retain->getLabel().c_str());

			vector<int> incident;
			unordered_set<int> incident_set;
			for (Relationship* re : remove->getIncidentRelationships())
				incident_set.insert(re->getOpidx());
			for (Relationship* re : retain->getIncidentRelationships())
				incident_set.insert(re->getOpidx());
			std::copy(incident_set.begin(), incident_set.end(), std::back_inserter(incident));

			vector<int> adjacent;
			unordered_set<int> adjacent_set;
			for (Entity* en : remove->getAdjacentEntities())
				if (en != retain) adjacent_set.insert(en->getOpidx());
			for (Entity* en : retain->getAdjacentEntities())
				if (en != remove) adjacent_set.insert(en->getOpidx());
			std::copy(adjacent_set.begin(), adjacent_set.end(), std::back_inserter(adjacent));

			result = m_ERData->applyEntityOperation(remove, retain, &incidentRemoved, &incidentRetained);
			if (result && from_queue)
			{
				op->setApplied(true);
				op->setLegal(false);
				updateQueueNodeMerge(op->getRemovedIndex(), op->getRetainedIndex(), adjacent, incident);
			}
		}
	}
	else if (op->getType() == OPERATION_TYPE::NODE_DELETE && m_nrem)
	{
		Entity* remove = m_ERData->getEntityByOpidx(op->getRemovedIndex());
		Entity* retain = NULL;

		if (!remove)
			result = false;
		else if (m_ERData->isSubgraphLinear(remove))
			result = false;
		else
		{
			//LogConsole::writeConsole("removing %s", remove->getLabel().c_str());

			vector<int> incident;
			for (Relationship* re : remove->getIncidentRelationships())
				incident.push_back(re->getOpidx());

			vector<int> adjacent;
			for (Entity* en : remove->getAdjacentEntities())
				adjacent.push_back(en->getOpidx());

			result = m_ERData->applyEntityOperation(remove, retain, &incidentRemoved, &incidentRetained);
			if (result && from_queue)
			{
				op->setApplied(true);
				op->setLegal(false);
				updateQueueNodeDelete(op->getRemovedIndex(), adjacent, incident);
			}
		}
	}
	else
	{
		result = false;
	}

	if (result) // operation suceeded
	{
		if (from_queue)
		{
			op->setIncidentRemoved(incidentRemoved);
			op->setIncidentRetained(incidentRetained);
			removeIllegalOperations();
		}

		m_applied.push(op);
		m_ERData->update2D();
		return true;
	}
	else // operation failed
	{
		op->setLegal(false);
		removeIllegalOperations();
		if (!m_opqueue.empty())
		{
			return apply_operation(max_ops, priority_thresh, min_edges, min_nodes);
		}
		else
		{
			return false;
		}
	}
}

bool ERProgressive::invert_operation(std::vector<int>* enlist, std::vector<int>* relist)
{
	OperationRecord* op;
	if (!m_applied.empty())
		op = m_applied.top();
	else
		return false;

	bool result = false;

	std::unordered_set<Entity*> enset;
	std::unordered_set<Relationship*> reset;
	vector<pair<int, std::string>> incidentRemoved = op->getIncidentRemoved();
	vector<int> incidentRetained = op->getIncidentRetained();

	OPERATION_TYPE type = op->getType();
	if (type == OPERATION_TYPE::EDGE_MERGE || type == OPERATION_TYPE::EDGE_DELETE)
	{
		Relationship* retain = NULL;
		if (type == OPERATION_TYPE::EDGE_MERGE)	// merge operation
			retain = m_ERData->getRelationshipByOpidx(op->getRetainedIndex());

		REList res = m_ERData->invertRelationshipOperation(op->getRemoved(), retain, &incidentRemoved, &incidentRetained, true);

		for (Relationship* re : res)
		{
			if (re)
			{
				reset.insert(re);
				for (Entity* en : re->getIncidentEntities())
					enset.insert(en);
				for (Relationship* adj : re->getAdjacentRelationships())
					reset.insert(adj);

				//if (reset.insert(re).second)
				//{
				//	for (Entity* en : re->getIncidentEntities())
				//		enset.insert(en);
				//}
				//for (Relationship* adj : re->getAdjacentRelationships())
				//{
				//	if (reset.insert(adj).second)
				//	{
				//		for (Entity* en : adj->getIncidentEntities())
				//			enset.insert(en);
				//	}
				//}
			}
		}

		if (res.at(0))
		{
			m_applied.pop();
			m_inverted.push(op);
			result = true;
		}
	}
	else if (type == OPERATION_TYPE::NODE_MERGE || type == OPERATION_TYPE::NODE_DELETE)
	{
		Entity* retain = NULL;
		if (type == OPERATION_TYPE::NODE_MERGE)	// merge operation
			retain = m_ERData->getEntityByOpidx(op->getRetainedIndex());

		ENList ens = m_ERData->invertEntityOperation(op->getRemoved(), retain, &incidentRemoved, &incidentRetained, NULL, NULL);

		for (Entity* en : ens)
		{
			if (en)
			{
				enset.insert(en);
				for (Relationship* re : en->getIncidentRelationships())
					reset.insert(re);
				for (Entity* adj : en->getAdjacentEntities())
					enset.insert(adj);
				
				/*if (enset.insert(en).second)
				{
					for (Relationship* re : en->getIncidentRelationships())
						reset.insert(re);
				}
				for (Entity* adj : en->getAdjacentEntities())
				{
					if (enset.insert(adj).second)
					{
						for (Relationship* re : adj->getIncidentRelationships())
							reset.insert(re);
					}
				}*/
			}
		}

		if (ens.at(0))
		{
			m_applied.pop();
			m_inverted.push(op);
			result = true;
		}
	}

	for (Entity* en : enset)
		enlist->push_back(en->getIndex());
	for (Relationship* re : reset)
		relist->push_back(re->getIndex());

	LogConsole::writeConsole("%i inversions remaining", m_applied.size());
	return result;
}

void ERProgressive::updateQueueEdgeDelete(int removed, vector<int> adjacent, vector<int> incident)
{
	for (int adj : adjacent)
	{
		// check removals of adjacent edges
		Relationship* re = m_ERData->getRelationshipByOpidx(adj);
		//bool creates_orphans = m_ERData->deleteCreatesOrphans(re);
		bool is_leaf = (gmainwindow->flag_pruneOnly) ? (re->getCardinality() == 1) : true;
		bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(re) : false;
		bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(re) : false;
		OperationRecord* removal = re->getRemovalOp();
		if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
			removal->setLegal(false);
		else
		{
			m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
			if (!removal->isLegal())
				m_opqueue.push_back(removal);
			removal->setLegal(true);
		}

		// check mergers on adjacent edges
		for (OperationRecord* merger : re->getMergerOps())
		{
			if (merger->getRemovedIndex() == removed || merger->getRetainedIndex() == removed)
				merger->setLegal(false);
			else
			{
				if (merger->isLegal())
				{
					Relationship* re2 = m_ERData->getRelationshipByOpidx(merger->getRemovedIndex());
					if (re == re2)
						re2 = m_ERData->getRelationshipByOpidx(merger->getRetainedIndex());
					if (re2)
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
						merger->setLegal(false);
				}
			}
		}
	}

	for (int inc : incident)
	{
		// check removals of incident entities
		Entity* en = m_ERData->getEntityByOpidx(inc);
		OperationRecord* removal = en->getRemovalOp();
		if (removal)
		{
			//bool creates_orphans = m_ERData->deleteCreatesZerogons(en);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (en->getDegree() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(en) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(en) : false;
			if (/*creates_orphans ||*/!is_leaf || breaks_connection || is_branch_end)
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}
		
		// check mergers on incident entities
		for (OperationRecord* merger : en->getMergerOps())
		{
			if (merger->isLegal())
			{
				Entity* en2 = m_ERData->getEntityByOpidx(merger->getRemovedIndex());
				if (en == en2)
					en2 = m_ERData->getEntityByOpidx(merger->getRetainedIndex());
				if (en2)
				{
					int share_num = m_ERData->getSharedRelationshipNum(en, en2);
					if (share_num > m_share /*&& (share_num == en->getDegree() || share_num == en2->getDegree())*/)
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
						merger->setLegal(false);
				}
				else
					merger->setLegal(false);
			}
		}
	}
}

void ERProgressive::updateQueueNodeDelete(int removed, vector<int> adjacent, vector<int> incident)
{
	for (int adj : adjacent)
	{
		// check removals of adjacent nodes
		Entity* en = m_ERData->getEntityByOpidx(adj);
		///*bool creates_orphans = m_ERData->deleteCreatesZerogons(en);*/
		bool is_leaf = (gmainwindow->flag_pruneOnly) ? (en->getDegree() == 1) : true;
		bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(en) : false;
		bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(en) : false;
		OperationRecord* removal = en->getRemovalOp();
		if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
			removal->setLegal(false);
		else
		{
			m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
			if (!removal->isLegal())
				m_opqueue.push_back(removal);
			removal->setLegal(true);
		}

		// check mergers on adjacent nodes
		for (OperationRecord* merger : en->getMergerOps())
		{
			if (merger->getRemovedIndex() == removed || merger->getRetainedIndex() == removed)
				merger->setLegal(false);
			else
			{
				if (merger->isLegal())
				{
					Entity* en2 = m_ERData->getEntityByOpidx(merger->getRemovedIndex());
					if (en == en2)
						en2 = m_ERData->getEntityByOpidx(merger->getRetainedIndex());
					if (en2)
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
						merger->setLegal(false);
				}
			}
		}
	}

	for (int inc : incident)
	{
		// check removals of incident relationships
		Relationship* re = m_ERData->getRelationshipByOpidx(inc);
		OperationRecord* removal = re->getRemovalOp();
		if (removal)
		{		
			//bool creates_orphans = m_ERData->deleteCreatesOrphans(re);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (re->getCardinality() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(re) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(re) : false;
			if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}

		// check mergers of incident relationships
		for (OperationRecord* merger : re->getMergerOps())
		{
			if (merger->isLegal())
			{
				Relationship* re2 = m_ERData->getRelationshipByOpidx(merger->getRemovedIndex());
				if (re == re2)
					re2 = m_ERData->getRelationshipByOpidx(merger->getRetainedIndex());
				if (re2)
				{
					int share_num = m_ERData->getSharedEntityNum(re, re2);
					if (share_num > m_share /*&& (share_num == re->getCardinality() || share_num == re2->getCardinality())*/)
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
						merger->setLegal(false);
				}
				else
					merger->setLegal(false);
			}
		}
	}
}

void ERProgressive::updateQueueEdgeMerge(int removed, int retained, vector<int> adjacent, vector<int> incident)
{
	// check removal of retained edge
	Relationship* retained_re = m_ERData->getRelationshipByOpidx(retained);
	OperationRecord* retained_removal = retained_re->getRemovalOp();
	if (retained_removal)
	{
		//bool creates_orphans = m_ERData->deleteCreatesOrphans(retained_re);
		bool is_leaf = (gmainwindow->flag_pruneOnly) ? (retained_re->getCardinality() == 1) : true;
		bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(retained_re) : false;
		bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(retained_re) : false;
		if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
			retained_removal->setLegal(false);
		else
		{
			m_ERData->updateOperationPriority(retained_removal, m_alpha, m_beta, m_gamma);
			if (!retained_removal->isLegal())
				m_opqueue.push_back(retained_removal);
			retained_removal->setLegal(true);
		}
	}

	for (int adj : adjacent)
	{
		// check removal of adjacent edges
		Relationship* re = m_ERData->getRelationshipByOpidx(adj);
		OperationRecord* removal = re->getRemovalOp();
		if (removal)
		{
			//bool creates_orphans = m_ERData->deleteCreatesOrphans(re);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (re->getCardinality() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(re) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(re) : false;
			if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}

		// check mergers on adjacent edges
		for (OperationRecord* merger : re->getMergerOps())
		{
			// replace references to removed edge with references to retained edge
			Relationship* to_merge;
			if (merger->getRemovedIndex() == removed)
			{
				merger->setRemoved(std::make_pair(retained, retained_re->getLabel()));
				to_merge = retained_re;
			}
			else if (merger->getRetainedIndex() == removed)
			{
				merger->setRetainedIndex(retained);
				to_merge = retained_re;
			}
			else
			{
				to_merge = m_ERData->getRelationshipByOpidx(merger->getRemovedIndex());
				if (re == to_merge)
					to_merge = m_ERData->getRelationshipByOpidx(merger->getRetainedIndex());
			}

			if (!to_merge)
			{
				merger->setLegal(false);
			}
			else
			{
				int share_num = m_ERData->getSharedEntityNum(re, to_merge);
				if (share_num > m_share /* && (share_num == re->getCardinality() || share_num == to_merge->getCardinality())*/)
				{
					if (merger->isLegal())
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
					{
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
						merger->setLegal(true);
						m_opqueue.push_back(merger);
					}

					// check for duplicates
					if (to_merge == retained_re)
					{
						for (OperationRecord* other_merger : retained_re->getMergerOps())
						{
							if (merger != other_merger && mergesIdentical(other_merger, merger) && (other_merger->isLegal() || other_merger->isApplied()))
							{
								merger->setLegal(false);
								break;
							}
						}
					}
				}
				else
				{
					merger->setLegal(false);
				}
			}
		}
	}

	for (int inc : incident)
	{
		// check removals of incident entities
		Entity* en = m_ERData->getEntityByOpidx(inc);
		OperationRecord* removal = en->getRemovalOp();
		if (removal)
		{
			//bool creates_orphans = m_ERData->deleteCreatesZerogons(en);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (en->getDegree() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(en) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(en) : false;
			if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}

		// check mergers on incident entities
		for (OperationRecord* merger : en->getMergerOps())
		{
			Entity* en2 = m_ERData->getEntityByOpidx(merger->getRemovedIndex());
			if (en == en2)
				en2 = m_ERData->getEntityByOpidx(merger->getRetainedIndex());
			if (en2)
			{
				int share_num = m_ERData->getSharedRelationshipNum(en, en2);
				if (share_num > m_share /*&& (share_num == en->getDegree() || share_num == en2->getDegree())*/)
				{
					if (merger->isLegal())
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
					{
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
						merger->setLegal(true);
						m_opqueue.push_back(merger);
					}
				}
				else
				{
					merger->setLegal(false);
				}
			}
			else
			{
				merger->setLegal(false);
			}
		}
	}

	// find newly available node merges
	if (m_nmerge)
	{
		for (int i = 0; i < incident.size() - 1; i++)	
		{
			for (int j = i + 1; j < incident.size(); j++)
			{
				int opid1 = incident.at(i);
				int opid2 = incident.at(j);
				if (opid1 == opid2)
					continue;

				Entity* en1 = m_ERData->getEntityByOpidx(opid1);
				Entity* en2 = m_ERData->getEntityByOpidx(opid2);
				int share_num = m_ERData->getSharedRelationshipNum(en1, en2);
				if (share_num > m_share /*&& (share_num == en1->getDegree() || share_num == en2->getDegree())*/)
				{
					OperationRecord* new_merge = new OperationRecord(en1, OPERATION_TYPE::NODE_MERGE, en2);

					bool duplicate = false;
					for (OperationRecord* old_merge : en1->getMergerOps())
					{
						if (new_merge != old_merge && mergesIdentical(new_merge, old_merge) && (old_merge->isLegal() || old_merge->isApplied()))
						{
							duplicate = true;
							break;
						}
					}

					if (duplicate)
						delete(new_merge);
					else
					{
						en1->addMergerOp(new_merge);
						en2->addMergerOp(new_merge);
						m_ERData->updateOperationPriority(new_merge, m_alpha, m_beta, m_gamma);
						new_merge->setLegal(true);
						m_opqueue.push_back(new_merge);
					}
				}
			}
		}
	}
}

void ERProgressive::updateQueueNodeMerge(int removed, int retained, vector<int> adjacent, vector<int> incident)
{
	// check removal of retained node
	Entity* retained_en = m_ERData->getEntityByOpidx(retained);
	OperationRecord* retained_removal = retained_en->getRemovalOp();
	if (retained_removal)
	{
		//bool creates_orphans = m_ERData->deleteCreatesZerogons(retained_en);
		bool is_leaf = (gmainwindow->flag_pruneOnly) ? (retained_en->getDegree() == 1) : true;
		bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(retained_en) : false;
		bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(retained_en) : false;
		if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
			retained_removal->setLegal(false);
		else
		{
			m_ERData->updateOperationPriority(retained_removal, m_alpha, m_beta, m_gamma);
			if (!retained_removal->isLegal())
				m_opqueue.push_back(retained_removal);
			retained_removal->setLegal(true);
		}
	}

	for (int adj : adjacent)
	{
		// check removal of adjacent nodes
		Entity* en = m_ERData->getEntityByOpidx(adj);
		OperationRecord* removal = en->getRemovalOp();
		if (removal)
		{
			//bool creates_orphans = m_ERData->deleteCreatesZerogons(en);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (en->getDegree() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksReConnection(en) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->enIsBranchEnd(en) : false;
			if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)		// can make new node removals legal, but not existing ones illegal
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}

		// check mergers on adjacent nodes
		for (OperationRecord* merger : en->getMergerOps())
		{
			Entity* to_merge;
			if (merger->getRemovedIndex() == removed)
			{
				merger->setRemoved(std::make_pair(retained, retained_en->getLabel()));
				to_merge = retained_en;
			}
			else if (merger->getRetainedIndex() == removed)
			{
				merger->setRetainedIndex(retained);
				to_merge = retained_en;
			}
			else
			{
				to_merge = m_ERData->getEntityByOpidx(merger->getRemovedIndex());
				if (en == to_merge)
					to_merge = m_ERData->getEntityByOpidx(merger->getRetainedIndex());
			}

			if (!to_merge)
			{
				merger->setLegal(false);
			}
			else
			{
				int share_num = m_ERData->getSharedRelationshipNum(en, to_merge);
				if (share_num > m_share /*&& (share_num == en->getDegree() || share_num == to_merge->getDegree())*/)
				{
					if (merger->isLegal())
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
					{
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
						merger->setLegal(true);
						m_opqueue.push_back(merger);
					}

					// check for duplicates
					if (to_merge == retained_en)
					{
						for (OperationRecord* other_merger : retained_en->getMergerOps())
						{
							if (merger != other_merger && mergesIdentical(other_merger, merger) && (other_merger->isLegal() || other_merger->isApplied()))
							{
								merger->setLegal(false);
								break;
							}
						}
					}
				}
				else
				{
					merger->setLegal(false);
				}
			}
		}
	}

	for (int inc : incident)
	{
		// check removals of incident relationships
		Relationship* re = m_ERData->getRelationshipByOpidx(inc);
		OperationRecord* removal = re->getRemovalOp();
		if (removal)
		{
			//bool creates_orphans = m_ERData->deleteCreatesOrphans(re);
			bool is_leaf = (gmainwindow->flag_pruneOnly) ? (re->getCardinality() == 1) : true;
			bool breaks_connection = (gmainwindow->flag_preserveConnections) ? m_ERData->deleteBreaksEnConnection(re) : false;
			bool is_branch_end = (gmainwindow->flag_preserveBranches) ? m_ERData->reIsBranchEnd(re) : false;
			if (/*creates_orphans ||*/ !is_leaf || breaks_connection || is_branch_end)
				removal->setLegal(false);
			else
			{
				m_ERData->updateOperationPriority(removal, m_alpha, m_beta, m_gamma);
				if (!removal->isLegal())
					m_opqueue.push_back(removal);
				removal->setLegal(true);
			}
		}

		// check mergers on incident relationships
		for (OperationRecord* merger : re->getMergerOps())
		{
			Relationship* re2 = m_ERData->getRelationshipByOpidx(merger->getRemovedIndex());
			if (re == re2)
				re2 = m_ERData->getRelationshipByOpidx(merger->getRetainedIndex());
			if (re2)
			{
				int share_num = m_ERData->getSharedEntityNum(re, re2);
				if (share_num > m_share /*&& (share_num == re->getCardinality() || share_num == re2->getCardinality())*/)
				{
					if (merger->isLegal())
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
					else
					{
						m_ERData->updateOperationPriority(merger, m_alpha, m_beta, m_gamma);
						merger->setLegal(true);
						m_opqueue.push_back(merger);
					}
				}
				else
				{
					merger->setLegal(false);
				}
			}
			else
			{
				merger->setLegal(false);
			}
		}
	}

	// find newly available edge merges
	if (m_emerge)
	{
		for (int i = 0; i < incident.size() - 1; i++)
		{
			for (int j = i + 1; j < incident.size(); j++)
			{
				int opid1 = incident.at(i);
				int opid2 = incident.at(j);
				if (opid1 == opid2)
					continue;

				Relationship* re1 = m_ERData->getRelationshipByOpidx(opid1);
				Relationship* re2 = m_ERData->getRelationshipByOpidx(opid2);
				int share_num = m_ERData->getSharedEntityNum(re1, re2);
				if (share_num > m_share /*&& (share_num == re1->getCardinality() || share_num == re2->getCardinality())*/)
				{
					OperationRecord* new_merge = new OperationRecord(re1, OPERATION_TYPE::EDGE_MERGE, re2);

					bool duplicate = false;
					for (OperationRecord* old_merge : re1->getMergerOps())
					{
						if (new_merge != old_merge && mergesIdentical(new_merge, old_merge) && (old_merge->isLegal() || old_merge->isApplied()))
						{
							duplicate = true;
							break;
						}
					}

					if (duplicate)
						delete(new_merge);
					else
					{
						re1->addMergerOp(new_merge);
						re2->addMergerOp(new_merge);
						m_ERData->updateOperationPriority(new_merge, m_alpha, m_beta, m_gamma);
						new_merge->setLegal(true);
						m_opqueue.push_back(new_merge);
					}
				}
			}
		}
	}
}

void ERProgressive::removeIllegalOperations()
{
	vector<OperationRecord*> new_queue;
	new_queue.reserve(m_opqueue.size());
	for (OperationRecord* op : m_opqueue)
	{
		if (op->isLegal())
			new_queue.push_back(op);
	}

	m_opqueue = new_queue;
	std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
}

int ERProgressive::getAppliedOperationNum()
{
	return m_applied.size();
}


// Depricated

//bool ERProgressive::generate_operations(ERData* erdata, int max_emerge_size)
//{
//	if (erdata)
//		m_ERData = erdata;
//
//	while (!m_opqueue.empty())
//	{
//		OperationRecord* op = m_opqueue.back();
//		m_opqueue.pop_back();
//		delete op;
//	}
//
//	double max_recluster = 0.0;
//	double min_recluster = DBL_MAX;
//	for (Relationship* re : m_ERData->getRelationships())
//	{
//		double importance = std::pow(m_ERData->getReShareNum(re), 2.0);
//		if (importance > max_recluster)
//			max_recluster = importance;
//		if (importance < min_recluster)
//			min_recluster = importance;
//	}
//
//	double max_encluster = 0.0;
//	double min_encluster = DBL_MAX;
//	for (Entity* en : m_ERData->getEntities())
//	{
//		double importance = en->getDegree();
//		if (importance > max_encluster)
//			max_encluster = importance;
//		if (importance < min_encluster)
//			min_encluster = importance;
//	}
//
//	bool erem, emerge, nrem, nmerge;
//	erem = emerge = nrem = nmerge = false;
//	if (m_erem)
//		erem = generate_operations_edge_delete();
//	if (m_emerge)
//		emerge = generate_operations_edge_merge(max_emerge_size);
//	if (m_nrem)
//		nrem = generate_operations_node_delete();
//	if (m_nmerge)
//		nmerge = generate_operations_node_merge();
//
//	return (erem || emerge || nrem || nmerge);
//}
//
//bool ERProgressive::generate_operations_edge_delete()
//{
//	if (m_ERData == NULL)
//		return false;
//
//	for (Relationship* re : m_ERData->getRelationships())
//	{
//		OperationRecord* new_record = new OperationRecord(re, OPERATION_TYPE::EDGE_DELETE);
//		double priority = getReRemPriority(re);
//		new_record->setImportance(priority);
//		m_opqueue.push_back(new_record);
//	}
//
//	if (m_opqueue.size() > 0)
//	{
//		std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//		return true;
//	}
//	else
//		return false;
//}
//
//bool ERProgressive::generate_operations_edge_merge(int max_emerge_size)
//{
//	if (m_ERData == NULL)
//		return false;
//
//	for (Relationship* remove : m_ERData->getRelationships())
//	{
//		int card = remove->getCardinality();
//		int min_unshared = INT_MAX;
//		Relationship* retain;
//		for (Relationship* adj : remove->getAdjacentRelationships())
//		{
//			int shared = m_ERData->getSharedEntityNum(remove, adj);
//			int adj_card = adj->getCardinality();
//			int unshared = card - shared + adj_card - shared;
//			if (unshared < min_unshared)
//			{
//				min_unshared = unshared;
//				retain = adj;
//			}
//		}
//
//		int adj_card = retain->getCardinality();
//		int shared = m_ERData->getSharedEntityNum(remove, retain);
//		int merged_size = card + adj_card - shared;
//
//		if (merged_size <= max_emerge_size)
//		{
//			OperationRecord* new_record = new OperationRecord(remove, OPERATION_TYPE::EDGE_MERGE, retain);
//			double priority = getReMergePriority(remove, retain);
//			new_record->setImportance(priority);
//			m_opqueue.push_back(new_record);
//		}
//	}
//
//	if (m_opqueue.size() > 0)
//	{
//		std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//		return true;
//	}
//	else
//		return false;
//}
//
//bool ERProgressive::generate_operations_node_delete()
//{
//	if (m_ERData == NULL)
//		return false;
//
//	for (Entity* en : m_ERData->getEntities())
//	{
//		OperationRecord* new_record = new OperationRecord(en, OPERATION_TYPE::NODE_DELETE);
//		double priority = getEnRemPriority(en);
//		new_record->setImportance(priority);
//		m_opqueue.push_back(new_record);
//	}
//
//	if (m_opqueue.size() > 0)
//	{
//		std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//		return true;
//	}
//	else
//		return false;
//}
//
//bool ERProgressive::generate_operations_node_merge()
//{
//	if (m_ERData == NULL)
//		return false;
//
//	for (Entity* remove : m_ERData->getEntities())
//	{
//		int degree = remove->getDegree();
//		int min_share = INT_MAX;
//		Entity* retain;
//		for (Entity* adj : remove->getAdjacentEntities())
//		{
//			int shared = m_ERData->getSharedRelationshipNum(remove, adj);
//			int adj_degree = adj->getDegree();
//			int share_factor = degree - shared + adj_degree - shared;
//			if (share_factor < min_share)
//			{
//				min_share = share_factor;
//				retain = adj;
//			}
//		}
//		//int card = remove->getDegree();
//		//ENList adj_ens = remove->getAdjacentEntities();
//		//std::vector<std::pair<int, int>> adj_merge_factor;
//		//for (int i = 0; i < adj_ens.size(); i++)
//		//{
//		//	Entity* adj = adj_ens[i];
//		//	int shared = m_ERData->getSharedRelationshipNum(remove, adj);
//		//	int adj_card = adj->getDegree();
//		//	int share_factor = card - shared + adj_card - shared;
//		//	adj_merge_factor.push_back(std::make_pair(share_factor, i));
//		//}
//
//		//std::sort(adj_merge_factor.begin(), adj_merge_factor.end());
//		//Entity* retain = adj_ens.at(adj_merge_factor[0].second);
//
//		OperationRecord* new_record = new OperationRecord(remove, OPERATION_TYPE::NODE_MERGE, retain);
//		double priority = getEnMergePriority(remove, retain);
//		new_record->setImportance(priority);
//		m_opqueue.push_back(new_record);
//	}
//
//	if (m_opqueue.size() > 0)
//	{
//		std::make_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//		return true;
//	}
//	else
//		return false;
//}
//
//bool ERProgressive::apply_next_operation(TERM_CRITERIA term_criteria, int min_edges, int min_nodes, int max_emerge_size, bool preserve_topology, bool prevent_orphans)
//{
//	OperationRecord* op;
//	bool from_queue;
//	if (!m_inverted.empty())
//	{
//		op = m_inverted.top();
//		from_queue = false;
//	}
//	else if (!m_opqueue.empty())
//	{
//		op = m_opqueue.front();
//		from_queue = true;
//	}
//	else
//	{
//		return false;
//	}
//
//	bool result;
//	vector<pair<int, std::string>> incidentRemoved;
//	vector<int> incidentRetained;
//	if (op->getType() == OPERATION_TYPE::EDGE_MERGE)
//	{
//		if (m_ERData->getRelationshipNum() < 2)
//			return false;
//
//		Relationship* remove = m_ERData->getRelationshipByOpidx(op->getRemovedIndex());
//		Relationship* retain = m_ERData->getRelationshipByOpidx(op->getRetainedIndex());
//
//		int remove_card = remove->getCardinality();
//		int retain_card = retain->getCardinality();
//		int shared = m_ERData->getSharedEntityNum(remove, retain);
//		int merged_size = remove_card + retain_card - shared;
//		
//		if (merged_size > max_emerge_size)
//		{
//			result = false;
//		}
//		else
//		{
//			// update all operations involving the relationship to be removed
//			for (OperationRecord* affected : m_opqueue)
//			{
//				if (affected == op)
//					continue;
//				if (affected->getRemoved().first == op->getRemoved().first)
//					affected->setRemoved(std::make_pair(retain->getOpidx(), retain->getLabel()));
//				if (affected->getRetainedIndex() == op->getRemoved().first)
//					affected->setRetainedIndex(retain->getOpidx());
//			}
//
//			result = m_ERData->applyRelationshipOperation(remove, retain, &incidentRemoved, &incidentRetained);
//		}
//	}
//	else if (op->getType() == OPERATION_TYPE::EDGE_DELETE)
//	{
//		if (m_ERData->getRelationshipNum() < 2)
//			return false;
//
//		Relationship* remove = m_ERData->getRelationshipByOpidx(op->getRemovedIndex());
//		Relationship* retain = NULL;
//
//		bool creates_orphans = false;
//		bool breaks_topology = false;
//		bool disconnects = false;
//		if (from_queue)
//		{
//			if (prevent_orphans)
//				creates_orphans = m_ERData->deleteCreatesOrphans(remove);
//			if (preserve_topology)
//			{
//				breaks_topology = m_ERData->deleteBreaksEnConnection(remove);
//				disconnects = breaks_topology;
//			}
//			else
//			{
//				disconnects = !m_ERData->isConnectedWithoutRelationship(remove);
//			}
//		}
//
//		// apply operation
//		if (creates_orphans || breaks_topology || disconnects)
//			result = false;
//		else
//			result = m_ERData->applyRelationshipOperation(remove, retain, &incidentRemoved, &incidentRetained);
//
//	}
//	else if (op->getType() == OPERATION_TYPE::NODE_MERGE)
//	{
//		if (m_ERData->getEntityNum() < 2)
//			return false;
//
//		Entity* remove = m_ERData->getEntityByOpidx(op->getRemovedIndex());
//		Entity* retain = m_ERData->getEntityByOpidx(op->getRetainedIndex());
//
//		// update all operations involving the relationship to be removed
//		for (OperationRecord* affected : m_opqueue)
//		{
//			if (affected == op)
//				continue;
//			if (affected->getRemoved().first == op->getRemoved().first)
//				affected->setRemoved(std::make_pair(retain->getOpidx(), retain->getLabel()));
//			if (affected->getRetainedIndex() == op->getRemoved().first)
//				affected->setRetainedIndex(retain->getOpidx());
//		}
//
//		result = m_ERData->applyEntityOperation(remove, retain, &incidentRemoved, &incidentRetained);
//
//	}
//	else if (op->getType() == OPERATION_TYPE::NODE_DELETE)
//	{
//		if (m_ERData->getEntityNum() < 2)
//			return false;
//
//		Entity* remove = m_ERData->getEntityByOpidx(op->getRemovedIndex());
//		Entity* retain = NULL;
//
//		bool creates_orphans = false;
//		bool breaks_topology = false;
//		bool disconnects = false;
//		if (from_queue)
//		{
//			if (prevent_orphans)
//				creates_orphans = m_ERData->deleteCreatesZerogons(remove);
//			if (preserve_topology)
//			{
//				breaks_topology = m_ERData->deleteBreaksReConnection(remove);
//				disconnects = breaks_topology;
//			}
//			else
//			{
//				disconnects = !m_ERData->isConnectedWithoutEntity(remove);
//			}
//		}
//
//		// apply operation
//		if (creates_orphans || breaks_topology || disconnects)
//			result = false;
//		else
//			result = m_ERData->applyEntityOperation(remove, retain, &incidentRemoved, &incidentRetained);
//
//	}
//
//	if (result)		// operation was successful
//	{
//		if (from_queue)
//		{
//			op->setIncidentRemoved(incidentRemoved);
//			op->setIncidentRetained(incidentRetained);
//			std::pop_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//			m_opqueue.pop_back();
//
//			// Check termination criteria
//			bool terminate = false;
//			if (term_criteria == TERM_CRITERIA::LINEAR)
//				terminate = m_ERData->isLinear();
//			else if (term_criteria == TERM_CRITERIA::EDGE_MIN)
//				terminate = (m_ERData->getRelationshipNum() <= min_edges);
//			else if (term_criteria == TERM_CRITERIA::NODE_MIN)
//				terminate = (m_ERData->getEntityNum() <= min_nodes);
//
//			if (terminate)
//			{
//				LogConsole::writeGreenConsole("Termination criteria reached");
//				while (!m_opqueue.empty())
//				{
//					OperationRecord* op = m_opqueue.back();
//					m_opqueue.pop_back();
//					delete op;
//				}
//			}
//			else if (m_opqueue.empty())
//			{
//				generate_operations(NULL, max_emerge_size);
//			}
//		}
//		else
//		{
//			m_inverted.pop();
//		}
//
//		m_applied.push(op);
//	}
//	else		// operation failed
//	{
//		if (from_queue)
//		{
//			std::pop_heap(m_opqueue.begin(), m_opqueue.end(), CompareOperationRecords());
//			m_opqueue.pop_back();
//			delete op;
//
//			if (m_opqueue.empty())
//			{
//				bool removable_elements = true;
//				if (m_erem) //m_optype == OPERATION_TYPE::EDGE_DELETE)
//					removable_elements = m_ERData->hasRemovableRelationships();
//				if (!removable_elements && m_nrem) //m_optype == OPERATION_TYPE::NODE_DELETE)
//					removable_elements = m_ERData->hasRemovableEntities();
//
//				if (removable_elements)
//					generate_operations(NULL, max_emerge_size);
//			}
//		}
//
//		return apply_next_operation(term_criteria, min_edges, min_nodes, max_emerge_size, preserve_topology, prevent_orphans);
//	}
//
//	m_ERData->update2D();
//	return true;
//}
//
//bool ERProgressive::invert_last_operation()
//{
//	OperationRecord* op;
//	if (!m_applied.empty())
//	{
//		op = m_applied.top();
//	}
//	else
//	{
//		return false;
//	}
//
//	OPERATION_TYPE type = op->getType();
//	if (type == OPERATION_TYPE::EDGE_MERGE || type == OPERATION_TYPE::EDGE_DELETE)
//	{
//		Relationship* retain = NULL;
//		if (type == OPERATION_TYPE::EDGE_MERGE)	// merge operation
//			retain = m_ERData->getRelationshipByOpidx(op->getRetainedIndex());
//
//		vector<pair<int, std::string>> incidentRemoved = op->getIncidentRemoved();
//		vector<int> incidentRetained = op->getIncidentRetained();
//		REList res = m_ERData->invertRelationshipOperation(op->getRemoved(), retain, &incidentRemoved, &incidentRetained, true);
//
//		if (res.at(0))
//		{
//			m_applied.pop();
//			m_inverted.push(op);
//			return true;
//		}
//	}
//	else if (type == OPERATION_TYPE::NODE_MERGE || type == OPERATION_TYPE::NODE_DELETE)
//	{
//		Entity* retain = NULL;
//		if (type == OPERATION_TYPE::NODE_MERGE)	// merge operation
//			retain = m_ERData->getEntityByOpidx(op->getRetainedIndex());
//
//		vector<pair<int, std::string>> incidentRemoved = op->getIncidentRemoved();
//		vector<int> incidentRetained = op->getIncidentRetained();
//		ENList ens = m_ERData->invertEntityOperation(op->getRemoved(), retain, &incidentRemoved, &incidentRetained, NULL, NULL);
//
//		if (ens.at(0))
//		{
//			m_applied.pop();
//			m_inverted.push(op);
//			return true;
//		}
//	}
//}

//double ERProgressive::getEnRemPriority(Entity* en)
//{
//	int en_idx = en->getOpidx();
//
//	int affected_ens = 1;
//	int affected_res = 0;
//	double sum_en_clustering = m_encluster[en_idx];
//	double sum_re_clustering = 0.0;
//
//	for (Relationship* re : en->getIncidentRelationships())
//	{
//		sum_re_clustering += m_recluster[re->getOpidx()];
//		affected_res++;
//	}
//	
//	for (Entity* adj : en->getAdjacentEntities())
//	{
//		sum_en_clustering += m_encluster[adj->getOpidx()];
//		affected_ens++;
//	}
//
//	double avg_en_clustering = sum_en_clustering / (double)affected_ens;
//	double avg_re_clustering = sum_re_clustering / (double)affected_res;
//
//	double en_cluster = 0.0;
//	double re_cluster = 0.0;
//	CLUSTERTYPE cluster_type = CLUSTERTYPE::NONE;
//	for (Cluster* cluster : m_ERData->m_clusterList)
//	{
//		if (cluster->containsEn(en->getOpidx()))
//		{
//			if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && cluster->getImportance() > en_cluster)
//			{
//				en_cluster = cluster->getImportance();
//				cluster_type = CLUSTERTYPE::NODE_CENTER;
//			}
//
//			else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && cluster->getImportance() > re_cluster)
//			{
//				re_cluster = cluster->getImportance();
//				cluster_type = CLUSTERTYPE::POLY_CENTER;
//			}
//		}
//	}
//
//	double en_clustering = std::max(avg_en_clustering, en_cluster);
//	double re_clustering = std::max(avg_re_clustering, re_cluster);
//	double centrality = m_encentrality[en_idx];
//	double weight = m_enweight[en_idx];
//
//	double preserve_enclusters = (max_encluster == min_encluster) ? 0.0 : (max_encluster - en_clustering) / (max_encluster - min_encluster);
//	double preserve_reclusters = (max_recluster == min_recluster) ? 0.0 : (max_recluster - re_clustering) / (max_recluster - min_recluster);
//	double preserve_structure = (max_encentrality == min_encentrality) ? 0.0 : (max_encentrality - centrality) / (max_encentrality / min_encentrality);
//	double preserve_by_weight = (max_enweight == min_enweight) ? 0.0 : (max_enweight - weight) / (max_enweight - min_enweight);
//
//
//	return (m_alpha * preserve_enclusters) + (m_beta * preserve_reclusters) + (m_gamma * preserve_structure) + (m_other * preserve_by_weight);
//}
//
//double ERProgressive::getEnMergePriority(Entity* en1, Entity* en2)
//{
//	/*std::unordered_set<int> affected_ens;
//	std::unordered_set<int> affected_res;
//	affected_ens.insert(en1->getIndex());
//	affected_ens.insert(en2->getIndex());
//	double importance = getEntityImportance(en1) + getEntityImportance(en2);
//	double ens_importance = importance;
//	double res_importance = 0.0;
//
//	for (Relationship* re : en1->getIncidentRelationships())
//	{
//		if (affected_res.insert(re->getIndex()).second)
//			res_importance += getRelationshipImportance(re);
//	}
//
//	for (Relationship* re : en2->getIncidentRelationships())
//	{
//		if (affected_res.insert(re->getIndex()).second)
//			res_importance += getRelationshipImportance(re);
//	}
//
//	for (Entity* adj : en1->getAdjacentEntities())
//	{
//		if (affected_ens.insert(adj->getIndex()).second)
//			ens_importance += getEntityImportance(adj);
//	}
//
//	for (Entity* adj : en2->getAdjacentEntities())
//	{
//		if (affected_ens.insert(adj->getIndex()).second)
//			ens_importance += getEntityImportance(adj);
//	}
//
//	double avg_en_importance = ens_importance / (double)affected_ens.size();
//	double avg_re_importance = res_importance / (double)affected_res.size();
//
//	double en_cluster_importance = 0.0;
//	double re_cluster_importance = 0.0;
//	CLUSTERTYPE cluster_type = CLUSTERTYPE::NONE;
//	for (Cluster* cluster : m_ERData->m_clusterList)
//	{
//		if (cluster->containsEn(en1->getOpidx()) && cluster->containsEn(en2->getOpidx()))
//		{
//			if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && cluster->getPriority() > en_cluster_importance)
//			{
//				en_cluster_importance = cluster->getPriority();
//				cluster_type = CLUSTERTYPE::NODE_CENTER;
//			}
//
//			else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && cluster->getPriority() > re_cluster_importance)
//			{
//				re_cluster_importance = cluster->getPriority();
//				cluster_type = CLUSTERTYPE::POLY_CENTER;
//			}
//		}
//	}
//
//	double en_importance = std::max(avg_en_importance, en_cluster_importance);
//	double re_importance = std::max(avg_re_importance, re_cluster_importance);
//	double weight = std::max(en1->getWeight(), en2->getWeight());
//
//	double preserve_important_entities = max_en_importance + min_en_importance - (en_importance * m_alpha);
//	double preserve_important_relationships = max_re_importance + min_re_importance - (re_importance * m_beta);
//	double preserve_average_elements = (en_importance + re_importance) * m_gamma;
//	double preserve_by_weight = max_en_weight + min_en_weight - (weight * m_other);
//
//	return 0;
//
//	if (cluster_type == CLUSTERTYPE::NONE)
//	{	
//		return preserve_important_entities + preserve_important_relationships + preserve_average_elements + preserve_by_weight;
//	}
//	else
//	{
//		return preserve_important_entities + preserve_important_relationships + preserve_average_elements + preserve_by_weight - 1.0;
//	}*/
//
//	return 1.0 / (double)(m_ERData->getEntityEccentricity(en1->getOpidx()) + m_ERData->getEntityEccentricity(en2->getOpidx()));
//}
//
//double ERProgressive::getReRemPriority(Relationship* re)
//{
//	int re_idx = re->getOpidx();
//
//	int affected_ens = 0;
//	int affected_res = 1;
//	double sum_en_clustering = 0.0;
//	double sum_re_clustering = m_recluster[re_idx];
//
//	for (Entity* en : re->getIncidentEntities())
//	{
//		sum_en_clustering += m_encluster[en->getOpidx()];
//		affected_ens++;
//	}
//
//	for (Relationship* adj : re->getAdjacentRelationships())
//	{
//		sum_re_clustering += m_recluster[adj->getOpidx()];
//		affected_res++;
//	}
//
//	double avg_en_clustering = sum_en_clustering / (double)affected_ens;
//	double avg_re_clustering = sum_re_clustering / (double)affected_res;
//
//	double en_cluster = 0.0;
//	double re_cluster = 0.0;
//	CLUSTERTYPE cluster_type = CLUSTERTYPE::NONE;
//	for (Cluster* cluster : m_ERData->m_clusterList)
//	{
//		if (cluster->containsRe(re->getOpidx()))
//		{
//			if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && cluster->getImportance() > en_cluster)
//			{
//				en_cluster = cluster->getImportance();
//				cluster_type = CLUSTERTYPE::NODE_CENTER;
//			}
//
//			else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && cluster->getImportance() > re_cluster)
//			{
//				re_cluster = cluster->getImportance();
//				cluster_type = CLUSTERTYPE::POLY_CENTER;
//			}
//		}
//	}
//
//	double en_clustering = std::max(avg_en_clustering, en_cluster);
//	double re_clustering = std::max(avg_re_clustering, re_cluster);
//	double centrality = m_recentrality[re_idx];
//	double weight = m_reweight[re_idx];
//
//	double preserve_enclusters = (max_encluster == min_encluster) ? 0.0 : (max_encluster - en_clustering) / (max_encluster - min_encluster);
//	double preserve_reclusters = (max_recluster == min_recluster) ? 0.0 : (max_recluster - re_clustering) / (max_recluster - min_recluster);
//	double preserve_structure = (max_encentrality == min_encentrality) ? 0.0 : (max_encentrality - centrality) / (max_encentrality / min_encentrality);
//	double preserve_by_weight = (max_enweight == min_enweight) ? 0.0 : (max_enweight - weight) / (max_enweight - min_enweight);
//
//	return (m_alpha * preserve_enclusters) + (m_beta * preserve_reclusters) + (m_gamma * preserve_structure) + (m_other * preserve_by_weight);
//}
//
//double ERProgressive::getReMergePriority(Relationship* re1, Relationship* re2)
//{
//	//std::unordered_set<int> affected_ens;
//	//std::unordered_set<int> affected_res;
//	//affected_res.insert(re1->getIndex());
//	//affected_res.insert(re2->getIndex());
//	//double importance = getRelationshipImportance(re1) + getRelationshipImportance(re2);
//	//double ens_importance = 0.0;
//	//double res_importance = importance;
//
//	//for (Entity* en : re1->getIncidentEntities())
//	//{
//	//	if (affected_ens.insert(en->getIndex()).second)
//	//		ens_importance += getEntityImportance(en);
//	//}
//
//	//for (Entity* en : re2->getIncidentEntities())
//	//{
//	//	if (affected_ens.insert(en->getIndex()).second)
//	//		ens_importance += getEntityImportance(en);
//	//}
//
//	//for (Relationship* adj : re1->getAdjacentRelationships())
//	//{
//	//	if (affected_res.insert(adj->getIndex()).second)
//	//		res_importance += getRelationshipImportance(adj);
//	//}
//
//	//for (Relationship* adj : re1->getAdjacentRelationships())
//	//{
//	//	if (affected_res.insert(adj->getIndex()).second)
//	//		res_importance += getRelationshipImportance(adj);
//	//}
//
//	//double avg_en_importance = ens_importance / (double)affected_ens.size();
//	//double avg_re_importance = res_importance / (double)affected_res.size();
//
//	//double en_cluster_importance = 0.0;
//	//double re_cluster_importance = 0.0;
//	//CLUSTERTYPE cluster_type = CLUSTERTYPE::NONE;
//	//for (Cluster* cluster : m_ERData->m_clusterList)
//	//{
//	//	if (cluster->containsRe(re1->getOpidx()) && cluster->containsRe(re2->getOpidx()))
//	//	{
//	//		if (cluster->getClusterType() == CLUSTERTYPE::NODE_CENTER && cluster->getPriority() > en_cluster_importance)
//	//		{
//	//			en_cluster_importance = cluster->getPriority();
//	//			cluster_type = CLUSTERTYPE::NODE_CENTER;
//	//		}
//
//	//		else if (cluster->getClusterType() == CLUSTERTYPE::POLY_CENTER && cluster->getPriority() > re_cluster_importance)
//	//		{
//	//			re_cluster_importance = cluster->getPriority();
//	//			cluster_type = CLUSTERTYPE::POLY_CENTER;
//	//		}
//	//	}
//	//}
//
//	//double en_importance = std::max(avg_en_importance, en_cluster_importance);
//	//double re_importance = std::max(avg_re_importance, re_cluster_importance);
//	//double weight = std::max(re1->getWeight(), re2->getWeight());
//
//	//double preserve_important_entities = max_en_importance + min_en_importance - (en_importance * m_alpha);
//	//double preserve_important_relationships = max_re_importance + min_re_importance - (re_importance * m_beta);
//	//double preserve_average_elements = (en_importance + re_importance) * m_gamma;
//	//double preserve_by_weight = max_en_weight + min_en_weight - (weight * m_other);
//
//	//return 0;
//	//
//	//if (cluster_type == CLUSTERTYPE::POLY_CENTER)
//	//{
//	//	return preserve_important_entities + preserve_important_relationships + preserve_average_elements + preserve_by_weight;
//	//}
//	//else
//	//{
//	//	return preserve_important_entities + preserve_important_relationships + preserve_average_elements + preserve_by_weight - 1.0;
//	//}
//
//	return 1.0 / (double)(m_ERData->getRelationshipEccentricity(re1->getOpidx()) + m_ERData->getRelationshipEccentricity(re2->getOpidx()));
//}