#include "BipartOpManager.h"
#include "LogConsole.h"
#include "ElementUtils.h"
#include "JPMath/JPMath.h"

///////////////////////////////////////////////////////////////
// BipartOpManager Class
///////////////////////////////////////////////////////////////

BipartOpManager::BipartOpManager(ERData* erdata)
{
	m_erdata = erdata;
}

BipartOpManager::~BipartOpManager()
{
	while (!m_opqueue.empty())
	{
		BipartOperation* op = m_opqueue.front();
		m_opqueue.pop();
		delete op;
	}
	for (BipartOperation* op : m_opapplied)
		delete op;
	for (BipartOperation* op : m_opinverted)
		delete op;
	/*while (!m_opapplied.empty())
	{
		BipartOperation* op = m_opapplied.top();
		m_opapplied.pop();
		delete op;
	}
	while (!m_opinverted.empty())
	{
		BipartOperation* op = m_opinverted.top();
		m_opinverted.pop();
		delete op;
	}*/
}


ERData* BipartOpManager::getERData()
{
	return m_erdata;
}

std::vector<BipartOperation*> BipartOpManager::getAppliedOps()
{
	return m_opapplied;
}

int BipartOpManager::getTotalOpNum()
{
	return m_opqueue.size() + m_opapplied.size() + m_opinverted.size() + m_readqueue.size();
}


void BipartOpManager::queueOperation(BipartOperation* op)
{
	m_opqueue.push(op);
}

bool BipartOpManager::createOperation(std::vector<Entity*> ens, std::vector<Relationship*> res, BIPART_OP_TYPE type)
{
	std::vector<std::pair<Entity*, Relationship*>> edges;
	ELEMENTTYPE elemtype;
	switch (type)
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		if (ens.size() != 1 || res.size() != 1)
		{
			LogConsole::writeError("Select only one vertex and one hyperedge.");
			return false;
		}
		if (!ens.at(0)->checkIncident(res.at(0)))
		{
			LogConsole::writeError("Invalid edge cut: elements are not incident.");
			return false;
		}
		edges.push_back(std::make_pair(ens.at(0), res.at(0)));
		elemtype = ELEMENTTYPE::ENTITY;
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		if (ens.size() == 1 && res.size() == 0)
		{
			if (ens.at(0)->getDegree() == 1)
				edges.push_back(std::make_pair(ens.at(0), ens.at(0)->getIncidentRelationship(0)));
			else
			{
				LogConsole::writeError("Invalid edge prune: vertex does not have degree 1.");
				return false;
			}
			elemtype = ELEMENTTYPE::ENTITY;
		}
		else if (ens.size() == 0 && res.size() == 1)
		{
			std::vector<Entity*> incidents = res.at(0)->getIncidentEntities();
			if (res.at(0)->getCardinality() == 1)
				edges.push_back(std::make_pair(res.at(0)->getIncidentEntity(0), res.at(0)));
			else
			{
				LogConsole::writeError("Invalid edge prune: hyperedge does not have cardinality 1.");
				return false;
			}
			elemtype = ELEMENTTYPE::RELATIONSHIP;
		}
		else
		{
			LogConsole::writeError("Select only one vertex or hyperedge.");
			return false;
		}
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		if (ens.size() != 2 || res.size() != 2)
		{
			LogConsole::writeError("Incorrect number of elements selected");
			return false;
		}
		if (!ens.at(0)->checkIncident(res.at(0)) || !ens.at(0)->checkIncident(res.at(1)) ||
			!ens.at(1)->checkIncident(res.at(0)) || !ens.at(1)->checkIncident(res.at(0)))
		{
			LogConsole::writeError("Invalid K22 collapse: elements do not form a cycle");
			return false;
		}
		edges.push_back(std::make_pair(ens.at(0), res.at(0)));
		edges.push_back(std::make_pair(ens.at(0), res.at(1)));
		edges.push_back(std::make_pair(ens.at(1), res.at(0)));
		edges.push_back(std::make_pair(ens.at(1), res.at(1)));
		if (countCombinedDegree(ens.at(0), ens.at(1)) < countCombinedCardinality(res.at(0), res.at(1)))
			elemtype = ELEMENTTYPE::ENTITY;
		else
			elemtype = ELEMENTTYPE::RELATIONSHIP;
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (ens.size() == 2 && res.size() == 1)
		{
			if (!ens.at(0)->checkIncident(res.at(0)) || !ens.at(1)->checkIncident(res.at(0)))
			{
				LogConsole::writeError("Invalid chain merge: elements do not form a chain.");
				return false;
			}
			else if (res.at(0)->getCardinality() > 2)
			{
				LogConsole::writeError("Invalid chain merge: link element does not have degree 2.");
				return false;
			}
			edges.push_back(std::make_pair(ens.at(0), res.at(0)));
			edges.push_back(std::make_pair(ens.at(1), res.at(0)));
			elemtype = ELEMENTTYPE::ENTITY;
		}
		else if (ens.size() == 1 && res.size() == 2)
		{
			if (!ens.at(0)->checkIncident(res.at(0)) || !ens.at(0)->checkIncident(res.at(1)))
			{
				LogConsole::writeError("Invalid chain merge: elements do not form a chain.");
				return false;
			}
			else if (ens.at(0)->getDegree() > 2)
			{
				LogConsole::writeError("Invalid chain merge: link element does not have degree 2.");
				return false;
			}
			edges.push_back(std::make_pair(ens.at(0), res.at(0)));
			edges.push_back(std::make_pair(ens.at(0), res.at(1)));
			elemtype = ELEMENTTYPE::RELATIONSHIP;
		}
		else
		{
			LogConsole::writeError("Incorrect number of elements selected.");
			return false;
		}
		break;
	default:
		LogConsole::writeError("Invalid operation type.");
		return false;
		break;
	}

	BipartOperation* op =  new BipartOperation(edges, type, elemtype);
	m_opqueue.push(op);
	return true;
}

BipartOperation* BipartOpManager::readOperation(QStringList list)
{
	BIPART_OP_TYPE type = BIPART_OP_TYPE(list[0].toInt());
	ELEMENTTYPE elem_type = ELEMENTTYPE(list[1].toInt());
	std::vector<std::pair<Entity*, Relationship*>> edges;

	switch (type)
	{
	case BIPART_OP_TYPE::EDGE_CUT:
	{
		Entity* en = m_erdata->getEntityByOpidx(list[2].toInt());
		Relationship* re = m_erdata->getRelationshipByOpidx(list[3].toInt());
		if (!en->checkIncident(re))
		{
			LogConsole::writeError("Invalid edge cut: elements are not incident.");
			return NULL;
		}
		else
		{
			edges.push_back(std::make_pair(en, re));
		}
	}
	break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
	{
		Entity* en = m_erdata->getEntityByOpidx(list[2].toInt());
		Relationship* re = m_erdata->getRelationshipByOpidx(list[3].toInt());
		if (elem_type == ELEMENTTYPE::ENTITY && en->getDegree() != 1)
		{
			LogConsole::writeError("Invalid edge prune: Vertex does not have degree 1.");
			return NULL;
		}
		else if (elem_type == ELEMENTTYPE::RELATIONSHIP && re->getCardinality() != 1)
		{
			LogConsole::writeError("Invalid edge prune: Hyperedge does not have cardinality 1.");
			return NULL;
		}
		else if (!en->checkIncident(re))
		{
			LogConsole::writeError("Invalid edge prune: elements are not incident.");
			return NULL;
		}
		else
		{
			edges.push_back(std::make_pair(en, re));
		}
	}
	break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
	{
		Entity* en1 = m_erdata->getEntityByOpidx(list[2].toInt());
		Relationship* re1 = m_erdata->getRelationshipByOpidx(list[3].toInt());
		Entity* en2 = m_erdata->getEntityByOpidx(list[8].toInt());
		Relationship* re2 = m_erdata->getRelationshipByOpidx(list[9].toInt());

		if (!en1->checkIncident(re1) || !en1->checkIncident(re2) ||
			!en2->checkIncident(re1) || !en2->checkIncident(re2))
		{
			LogConsole::writeError("Invalid K22 collapse: elements do not form a cycle");
			return NULL;
		}
		else
		{
			edges.push_back(std::make_pair(en1, re1));
			edges.push_back(std::make_pair(en1, re2));
			edges.push_back(std::make_pair(en2, re1));
			edges.push_back(std::make_pair(en2, re2));
		}
	}
	break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
	{
		Entity* en1 = m_erdata->getEntityByOpidx(list[2].toInt());
		Relationship* re1 = m_erdata->getRelationshipByOpidx(list[3].toInt());
		Entity* en2 = m_erdata->getEntityByOpidx(list[4].toInt());
		Relationship* re2 = m_erdata->getRelationshipByOpidx(list[5].toInt());
		if (elem_type == ELEMENTTYPE::ENTITY)
		{
			if (!en1->checkIncident(re1) || !en2->checkIncident(re1))
			{
				LogConsole::writeError("Invalid chain merge: elements do not form a chain.");
				return NULL;
			}
			else if (re1->getCardinality() > 2)
			{
				LogConsole::writeError("Invalid chain merge: link element does not have degree 2.");
				return NULL;
			}
			else
			{
				edges.push_back(std::make_pair(en1, re1));
				edges.push_back(std::make_pair(en2, re1));
			}
		}
		else if (elem_type == ELEMENTTYPE::RELATIONSHIP)
		{
			if (!en1->checkIncident(re1) || !en1->checkIncident(re2))
			{
				LogConsole::writeError("Invalid chain merge: elements do not form a chain.");
				return NULL;
			}
			else if (en1->getDegree() > 2)
			{
				LogConsole::writeError("Invalid chain merge: elements do not form a chain.");
				return NULL;
			}
			else
			{
				edges.push_back(std::make_pair(en1, re1));
				edges.push_back(std::make_pair(en1, re2));
			}
		}
	}
	break;
	default:
		LogConsole::writeError("Unrecognized operation type");
		return NULL;
		break;
	}

	BipartOperation* op = new BipartOperation(edges, type, elem_type);
	return op;
}

bool BipartOpManager::applyOperation(BipartOperation* op)
{
	std::vector<std::pair<Entity*, Relationship*>> edges = op->getEdges();

	bool result = false;
	switch (op->getType())
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		result = cutBipartEdge(edges.at(0).first, edges.at(0).second, m_erdata);
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
			result = pruneBipartEdge(edges.at(0).first, m_erdata);
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
			result = pruneBipartEdge(edges.at(0).second, m_erdata);
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
		{
			Entity* en = collapseBipartK22(edges.at(0).first, edges.at(3).first, op->getEnResult(), m_erdata);
			op->setEnResult(en);
			result = bool(en);
		}
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
		{
			Relationship* re = collapseBipartK22(edges.at(0).second, edges.at(3).second, op->getReResult(), m_erdata);
			op->setReResult(re);
			result = bool(re);
		}
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
		{
			Entity* en = mergeBipartChain(edges.at(0).first, edges.at(0).second, edges.at(1).first, op->getEnResult(), m_erdata);
			op->setEnResult(en);
			result = bool(en);
		}
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
		{
			Relationship* re = mergeBipartChain(edges.at(0).second, edges.at(0).first, edges.at(1).second, op->getReResult(), m_erdata);
			op->setReResult(re);
			result = bool(re);
		}
		break;
	default:
		LogConsole::writeError("Invalid operation type.");
		result = false;
		break;
	}

	if (result)
	{
		m_opapplied.push_back(op);
		op->incrementSimpFactors();
	}
		
	return result;
}

bool BipartOpManager::invertOperation(BipartOperation* op)
{
	std::vector<std::pair<Entity*, Relationship*>> edges = op->getEdges();

	bool result = false;
	switch (op->getType())
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		result = mendBipartEdge(edges.at(0).first, edges.at(0).second);
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
			result = growBipartEdge(edges.at(0).first, m_erdata);
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
			result = growBipartEdge(edges.at(0).second, m_erdata);
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
			result = expandBipartK22(edges.at(0).first, edges.at(3).first, m_erdata);
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
			result = expandBipartK22(edges.at(0).second, edges.at(3).second, m_erdata);
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (op->getElemType() == ELEMENTTYPE::ENTITY)
			result = splitBipartChain(edges.at(0).first, edges.at(0).second, edges.at(1).first, m_erdata);
		else if (op->getElemType() == ELEMENTTYPE::RELATIONSHIP)
			result = splitBipartChain(edges.at(0).second, edges.at(0).first, edges.at(1).second, m_erdata);
		break;
	default:
		LogConsole::writeError("Invalid operation type.");
		result = false;
		break;
	}

	if (result)
	{
		m_opinverted.push_back(op);
		op->decrementSimpFactors();
	}
	return result;
}

bool BipartOpManager::applyNextOperation()
{
	if (m_readqueue.empty() && m_opqueue.empty() && m_opinverted.empty())
	{
		LogConsole::writeError("No bipartite operations to apply.");
		return false;
	}

	BipartOperation* op;
	if (!m_opinverted.empty())
	{
		op = m_opinverted.back();
		m_opinverted.pop_back();
	}
	else if (!m_opqueue.empty())
	{
		op = m_opqueue.front();
		m_opqueue.pop();
	}
	else
	{
		op = readOperation(m_readqueue.front());
		m_readqueue.pop();
	}

	if (op)
		return applyOperation(op);
	else
		return false;
}

BipartOperation* BipartOpManager::invertLastOperation()
{
	if (m_opapplied.empty())
	{
		LogConsole::writeError("No bipartite operations to invert.");
		return false;
	}

	BipartOperation* op = m_opapplied.back();
	m_opapplied.pop_back();
	
	if (invertOperation(op))
		return op;
	else
		return NULL;
}


bool BipartOpManager::cutBipartEdge(Entity* en, Relationship* re, ERData* erdata, bool dual_call)
{
	for (Entity* adj : re->getIncidentEntities())
		en->removeAdjacentEntity(adj, true);
	for (Relationship* adj : en->getIncidentRelationships())
		re->removeAjacentRelationship(adj,true);

	en->removeIncidentRelationship(re,true);
	re->update2D(true, true, true);

	Relationship* red = en->getDualRelationship();
	Entity* end = re->getDualEntity();
	ERData* dual = erdata->getDualERData();
	if (red && end && dual && !dual_call)
		cutBipartEdge(end, red, dual, true);

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Cut link between %s and %s", en->getLabel().c_str(), re->getLabel().c_str());*/
	return true;
}

bool BipartOpManager::pruneBipartEdge(Entity* en, ERData* erdata, bool dual_call)
{
	Relationship* re = en->getIncidentRelationship(0);

	re->removeIncidentEntity(en, false);
	for (Entity* adj : re->getIncidentEntities())
		adj->removeAdjacentEntity(en, false);
	re->update2D(true, true, true);
	erdata->removeEntity(en);

	Relationship* red = en->getDualRelationship();
	ERData* dual = erdata->getDualERData();
	if (red && dual && !dual_call)
		pruneBipartEdge(red, dual, true);

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Pruned link connecting %s", en->getLabel().c_str());*/
	return true;
}

bool BipartOpManager::pruneBipartEdge(Relationship* re, ERData* erdata, bool dual_call)
{
	Entity* en = re->getIncidentEntity(0);

	en->removeIncidentRelationship(re, false);
	for (Relationship* adj : en->getIncidentRelationships())
		adj->removeAjacentRelationship(re);
	erdata->removeRelationship(re);

	Entity* end = re->getDualEntity();
	ERData* dual = erdata->getDualERData();
	if (end && dual && !dual_call)
		pruneBipartEdge(end, dual, true);

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Pruned link connecting %s", re->getLabel().c_str());*/
	return true;
}

Entity* BipartOpManager::collapseBipartK22(Entity* en1, Entity* en2, Entity* en3, ERData* erdata, bool dual_call)
{
	if (!en3) // if there is no prior merge result, creat a new one.
	{
		Vec3 loc = (en1->getLocation() + en2->getLocation()) / 2.0;
		en3 = new Entity(loc);
		en3->setLabel(en1->getLabel() + en2->getLabel());
		en3->setOpidx(en1->getOpidx());
	}
	erdata->addEntity(en3, false);

	for (Entity* old : { en1,en2 })
	{
		for (Relationship* re : old->getIncidentRelationships())
		{
			re->removeIncidentEntity(old, false);
			re->addIncidentEntity(en3, false, true);
			re->update2D(true, true, true);
		}
		for (Entity* en : old->getAdjacentEntities())
		{
			en->removeAdjacentEntity(old, false);
			en->addAdjacentEntity(en3, true);
		}
		erdata->removeEntity(old);
	}

	Relationship* red1 = en1->getDualRelationship();
	Relationship* red2 = en2->getDualRelationship();
	Relationship* red3 = en3->getDualRelationship();
	ERData* dual = erdata->getDualERData();
	if (red1 && red2 && dual && !dual_call)
	{
		red3 = collapseBipartK22(red1, red2, red3, dual, true);
		en3->setDualElement(red3);
		red3->setDualElement(en3);
	}

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Merged cycle between %s and %s", en2->getLabel().c_str(), en1->getLabel().c_str());*/
	return en3;
}

Relationship* BipartOpManager::collapseBipartK22(Relationship* re1, Relationship* re2, Relationship* re3, ERData* erdata, bool dual_call)
{
	if (!re3) // if there is no prior merge result, creat a new one.
	{
		re3 = new Relationship(re1->getIncidentEntities(),false);
		re3->setLabel(re1->getLabel() + re2->getLabel());
		re3->setOpidx(re1->getOpidx());
	}

	erdata->addRelationship(re3, false);

	for (Relationship* old : { re1,re2 })
	{
		for (Entity* en : old->getIncidentEntities())
		{
			en->removeIncidentRelationship(old, false);
			en->addIncidentRelationship(re3, false, true);
		}
		for (Relationship* re : old->getAdjacentRelationships())
		{
			re->removeAjacentRelationship(old, false);
			re->addAdjacentRelationship(re3, true);
		}
		erdata->removeRelationship(old);
	}
	re3->update2D(true, true, true);

	Entity* end1 = re1->getDualEntity();
	Entity* end2 = re2->getDualEntity();
	Entity* end3 = re3->getDualEntity();
	ERData* dual = erdata->getDualERData();
	if (end1 && end2 && dual && !dual_call)
	{
		end3 = collapseBipartK22(end1, end2, end3, dual, true);
		re3->setDualElement(end3);
		end3->setDualElement(re3);
	}
	
	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Merged cycle between %s and %s", re2->getLabel().c_str(), re1->getLabel().c_str());*/
	return re3;
}

Entity* BipartOpManager::mergeBipartChain(Entity* en1, Relationship* mi, Entity* en2, Entity* en3, ERData* erdata, bool dual_call)
{
	if (!en3) // if there is no prior merge result, creat a new one.
	{
		Vec3 loc = (en1->getLocation() + en2->getLocation()) / 2.0;
		en3 = new Entity(loc);
		en3->setLabel(en1->getLabel() + en2->getLabel());
		en3->setOpidx(en1->getOpidx());
	}
	erdata->addEntity(en3, false);

	for (Entity* old : { en1,en2 })
	{
		for (Relationship* re : old->getIncidentRelationships())
		{
			if (re == mi)
				continue;
			re->removeIncidentEntity(old, false);
			re->addIncidentEntity(en3, false, true);
			re->update2D(true, true, true);
		}
		for (Entity* en : old->getAdjacentEntities())
		{
			en->removeAdjacentEntity(old, false);
			en->addAdjacentEntity(en3, true);
		}
		erdata->removeEntity(old);
	}
	for (Relationship* re : mi->getAdjacentRelationships())
		re->removeAjacentRelationship(mi, false);
	erdata->removeRelationship(mi);

	Relationship* red1 = en1->getDualRelationship();
	Entity* mid = mi->getDualEntity();
	Relationship* red2 = en2->getDualRelationship();
	Relationship* red3 = en3->getDualRelationship();
	ERData* dual = erdata->getDualERData();
	if (red1 && mid && red2 && dual && !dual_call)
	{
		red3 = mergeBipartChain(red1, mid, red2, red3, dual, true);
		en3->setDualElement(red3);
		red3->setDualElement(en3);
	}

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Merged %s through %s into %s", en1->getLabel().c_str(), mi->getLabel().c_str(), en2->getLabel().c_str());*/
	return en3;
}

Relationship* BipartOpManager::mergeBipartChain(Relationship* re1, Entity* mi, Relationship* re2, Relationship* re3, ERData* erdata, bool dual_call)
{
	if (!re3) // if there is no prior merge result, creat a new one.
	{
		re3 = new Relationship(re1->getIncidentEntities(), false);
		re3->removeIncidentEntity(mi, true);
		re3->setLabel(re1->getLabel() + re2->getLabel());
		re3->setOpidx(re1->getOpidx());
	}
	erdata->addRelationship(re3, false);

	ENList incident;
	for (Relationship* old : { re1,re2 })
	{
		for (Entity* en : old->getIncidentEntities())
		{
			if (en == mi)
				continue;
			en->removeIncidentRelationship(old, false);
			en->addIncidentRelationship(re3, false, true);
		}
		for (Relationship* re : old->getAdjacentRelationships())
		{
			re->removeAjacentRelationship(old, false);
			re->addAdjacentRelationship(re3, true);
		}
		erdata->removeRelationship(old);
	}
	for (Entity* en : mi->getAdjacentEntities())
		en->removeAdjacentEntity(mi, false);
	erdata->removeEntity(mi);
	re3->update2D(true,true,true);

	Entity* end1 = re1->getDualEntity();
	Relationship* mid = mi->getDualRelationship();
	Entity* end2 = re2->getDualEntity();
	Entity* end3 = re3->getDualEntity();
	ERData* dual = erdata->getDualERData();
	if (end1 && mid && end2 && dual && !dual_call)
	{
		end3 = mergeBipartChain(end1, mid, end2, end3, dual, true);
		re3->setDualElement(end3);
		end3->setDualElement(re3);
	}

	/*if (!dual_call)
		LogConsole::writeColorConsole(0.7, 0.4, 0.0, "Merged %s through %s into %s", re1->getLabel().c_str(), mi->getLabel().c_str(), re2->getLabel().c_str());*/
	return re3;
}


bool BipartOpManager::mendBipartEdge(Entity* en, Relationship* re, bool dual_call)
{
	en->addIncidentRelationship(re, true, true);
	re->update2D(true, true, true);
	
	Relationship* red = en->getDualRelationship();
	Entity* end = re->getDualEntity();
	if (red && end && !dual_call)
		mendBipartEdge(end, red, true);

	if (!dual_call)
		LogConsole::writeGreenConsole("Mended link between %s and %s", en->getLabel().c_str(), re->getLabel().c_str());
	return true;
}

bool BipartOpManager::growBipartEdge(Entity* en, ERData* erdata, bool dual_call)
{
	Relationship* re = en->getIncidentRelationship(0);
	
	if (!dual_call)
	{
		Vec3 loc = findLargestEdgeMidpoint(re);
		en->setLocation(loc);

		Relationship* red = en->getDualRelationship();
		ERData* dual = erdata->getDualERData();
		if (red && dual)
		{
			Vec3 dir = loc - re->getCenter(re->getMonogonLength());
			double a = angle(Vec2(dir));
			red->setMonogonRotation(a);
			growBipartEdge(red, dual, true);
		}
		
		LogConsole::writeGreenConsole("Grew link connecting %s", en->getLabel().c_str());
	}

	erdata->insertEntity(en);
	re->addIncidentEntity(en, true, true);
	re->update2D(true, true, true);
	return true;
}

bool BipartOpManager::growBipartEdge(Relationship* re, ERData* erdata, bool dual_call)
{
	Entity* en = re->getIncidentEntity(0);

	if (!dual_call)
	{
		double a = findLargestGapMidAngle(en);
		re->setMonogonRotation(a);

		Entity* end = re->getDualEntity();
		Relationship* red = en->getDualRelationship();
		ERData* dual = erdata->getDualERData();
		if (end && red && dual)
		{	
			Vec3 dir = Vec3(cos(a), sin(a), 0);
			Vec3 loc = red->getCenter(red->getMonogonLength()) + dir;
			end->setLocation(loc);
			growBipartEdge(end, dual, true);
		}
		
		LogConsole::writeGreenConsole("Grew link connecting %s", re->getLabel().c_str());
	}

	erdata->insertRelationship(re);
	en->addIncidentRelationship(re, true, true);
	re->update2D(true, true, false);
	return true;
}

bool BipartOpManager::expandBipartK22(Entity* en1, Entity* en2, ERData* erdata, bool dual_call)
{
	Entity* en3 = erdata->getEntityByOpidx(en1->getOpidx());

	// get new locations for en1 and en2
	if (!dual_call || (en1->getDegree()==2 && en2->getDegree() == 2)) 
	{
		Vec3 loc = en3->getLocation();
		double dist = getAvgIncidentDist(en3);
		double a = 2.0 * M_PI * (double)rand() / RAND_MAX;
		Vec3 dir = 0.1 * dist * Vec3(cos(a), sin(a), 0);
		Vec3 loc1 = loc + dir; en1->setLocation(loc1);
		Vec3 loc2 = loc - dir; en2->setLocation(loc2);
	}
	
	// add back in old entities, remove merged connections
	for (Entity* old : { en1,en2 }) 
	{
		for (Relationship* re : old->getIncidentRelationships())
		{
			re->addIncidentEntity(old, false, true);
			re->removeIncidentEntity(en3, true);
			re->update2D(true, true, true);
		}
		for (Entity* en : old->getAdjacentEntities())
		{
			en->addAdjacentEntity(old, true);
			en->removeAdjacentEntity(en3, true);
		}
		erdata->addEntity(old, false);
	}

	// remove invalid adjacencies
	REList relist1 = en1->getIncidentRelationships();
	REList relist2 = en2->getIncidentRelationships();
	for (Relationship* re1 : relist1)
		for (Relationship* re2 : relist2)
			if (getSharedEntitiesNum(re1, re2) < 1)
				re1->removeAjacentRelationship(re2, true);

	erdata->removeEntity(en3);

	if (!dual_call)
	{
		Relationship* red1 = en1->getDualRelationship();
		Relationship* red2 = en2->getDualRelationship();
		ERData* dual = erdata->getDualERData();
		if (red1 && red2 && dual)
			expandBipartK22(red1, red2, dual, true);
		
		LogConsole::writeGreenConsole("Expanded cycle between %s and %s", en2->getLabel().c_str(), en1->getLabel().c_str());
	}
		
	return true;
}

bool BipartOpManager::expandBipartK22(Relationship* re1, Relationship* re2, ERData* erdata, bool dual_call)
{
	// add back in old relationships, remove merged connections
	Relationship* re3 = erdata->getRelationshipByOpidx(re1->getOpidx());
	for (Relationship* old : { re1,re2 })
	{
		for (Entity* en : old->getIncidentEntities())
		{
			en->addIncidentRelationship(old,false,true);
			en->removeIncidentRelationship(re3,true);
		}
		for (Relationship* re : old->getAdjacentRelationships())
		{
			re->addAdjacentRelationship(old,true);
			re->removeAjacentRelationship(re3, true);
		}
		erdata->addRelationship(old, false);
		old->update2D(true, true, true);
	}

	// remove invalid adjacencies
	ENList enlist1 = re1->getIncidentEntities();
	ENList enlist2 = re2->getIncidentEntities();
	for (Entity* en1 : enlist1)
		for (Entity* en2 : enlist2)
			if (getSharedRelationshipsNum(en1, en2) < 1)
				en1->removeAdjacentEntity(en2, true);

	erdata->removeRelationship(re3);

	if (!dual_call)
	{
		Entity* end1 = re1->getDualEntity();
		Entity* end2 = re2->getDualEntity();
		ERData* dual = erdata->getDualERData();
		if (end1 && end2 && dual)
		{
			end1->setLocation(re1->getCenter(re1->getMonogonLength()));
			end2->setLocation(re2->getCenter(re1->getMonogonLength()));
			expandBipartK22(end1, end2, dual, true);
		}
			
		LogConsole::writeGreenConsole("Expanded cycle between %s and %s", re2->getLabel().c_str(), re1->getLabel().c_str());
	}

	return true;
}

bool BipartOpManager::splitBipartChain(Entity* en1, Relationship* mi, Entity* en2, ERData* erdata, bool dual_call)
{
	Entity* en3 = erdata->getEntityByOpidx(en1->getOpidx());

	if (!dual_call) // new get locations for en1 and en2
	{
		Vec3 loc = en3->getLocation();
		double dist = getAvgIncidentDist(en3);
		double a = 2.0 * M_PI * (double)rand() / RAND_MAX;
		Vec3 dir = 0.1 * dist * Vec3(cos(a), sin(a), 0);
		Vec3 loc1 = loc + dir; en1->setLocation(loc1);
		Vec3 loc2 = loc - dir; en2->setLocation(loc2);
	}

	for (Entity* old : { en1,en2 })
	{
		for (Relationship* re : old->getIncidentRelationships()) // update incidence
		{
			re->addIncidentEntity(old, false, true);
			re->removeIncidentEntity(en3, true);
			re->update2D(true, true, true);
		}
		for (Entity* en : old->getAdjacentEntities()) // update adjacency
		{
			en->addAdjacentEntity(old, true);
			en->removeAdjacentEntity(en3, true);
		}
		erdata->addEntity(old, false);
	}
	for (Relationship* re : mi->getAdjacentRelationships()) // add back middle element
		re->addAdjacentRelationship(mi, true);
	erdata->addRelationship(mi, false);
	mi->update2D(true, true, false);
	erdata->removeEntity(en3);

	if (!dual_call) // do dual call if using dual
	{
		Relationship* red1 = en1->getDualRelationship();
		Relationship* red2 = en2->getDualRelationship();
		Entity* mid = mi->getDualEntity();
		ERData* dual = erdata->getDualERData();
		if (red1 && red2 && mid && dual)
		{
			splitBipartChain(red1, mid, red2, dual, true);
			mid->setLocation(en3->getLocation());
		}

		LogConsole::writeGreenConsole("split chain between %s and %s", en2->getLabel().c_str(), en1->getLabel().c_str());
	}

	return true;
}

bool BipartOpManager::splitBipartChain(Relationship* re1, Entity* mi, Relationship* re2, ERData* erdata, bool dual_call)
{
	Relationship* re3 = erdata->getRelationshipByOpidx(re1->getOpidx());

	if (!dual_call) // get new location for middle element
	{
		Vec3 loc = re3->getCenter(re3->getMonogonLength());
		mi->setLocation(loc);
	}
	
	for (Relationship* old : { re1,re2 })
	{
		for (Entity* en : old->getIncidentEntities()) // updated incidence
		{
			en->addIncidentRelationship(old, false, true);
			en->removeIncidentRelationship(re3, true);
		}
		for (Relationship* re : old->getAdjacentRelationships()) // update adjacency
		{
			re->addAdjacentRelationship(old, true);
			re->removeAjacentRelationship(re3, true);
		}
		erdata->addRelationship(old, false);
		old->update2D(true, true, true);
	}
	for (Entity* en : mi->getAdjacentEntities()) // add back middle element
		en->addAdjacentEntity(mi, true);
	erdata->addEntity(mi, false);
	erdata->removeRelationship(re3);

	if (!dual_call) // do dual call if using dual
	{
		Entity* end1 = re1->getDualEntity();
		Entity* end2 = re2->getDualEntity();
		Relationship* mid = mi->getDualRelationship();
		ERData* dual = erdata->getDualERData();
		if (end1 && end2 && mid && dual)
		{
			end1->setLocation(re1->getCenter(re1->getMonogonLength()));
			end2->setLocation(re2->getCenter(re2->getMonogonLength()));
			splitBipartChain(end1, mid, end2, dual, true);
		}
		
		LogConsole::writeGreenConsole("Split chain between %s and %s", re2->getLabel().c_str(), re1->getLabel().c_str());
	}
		
	return true;
}


int BipartOpManager::countCombinedCardinality(Relationship* re1, Relationship* re2)
{
	std::unordered_set<Entity*> enset;
	for (Entity* en : re1->getIncidentEntities())
		enset.insert(en);
	for (Entity* en : re2->getIncidentEntities())
		enset.insert(en);
	return enset.size();
}

int BipartOpManager::countCombinedDegree(Entity* en1, Entity* en2)
{
	std::unordered_set<Relationship*> reset;
	for (Relationship* re : en1->getIncidentRelationships())
		reset.insert(re);
	for (Relationship* re : en2->getIncidentRelationships())
		reset.insert(re);
	return reset.size();
}

void BipartOpManager::addReadItem(QStringList list)
{
	m_readqueue.push(list);
}