#include "BipartOperation.h"
#include "LogConsole.h"

///////////////////////////////////////////////////////////////
// BipartOperation Class
///////////////////////////////////////////////////////////////

BipartOperation::BipartOperation(std::vector<std::pair<Entity*, Relationship*>> edges, BIPART_OP_TYPE type, ELEMENTTYPE elemtype)
{
	m_type = type;
	m_elemtype = elemtype;
	m_edges = edges;

	if (m_type == BIPART_OP_TYPE::K22_COLLAPSE)
	{
		if (m_elemtype == ELEMENTTYPE::ENTITY)
			m_enresult = new Entity();
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
			m_reresult = new Relationship();
	}
}

BipartOperation::~BipartOperation()
{
	/*if (m_enresult)
		delete m_enresult;
	if (m_reresult)
		delete m_reresult;*/
}

void BipartOperation::deleteResult()
{
	if (m_enresult)
		delete m_enresult;
	m_enresult = NULL;

	if (m_reresult)
		delete m_reresult;
	m_reresult = NULL;
}

void BipartOperation::deleteOld()
{
	switch (m_type)
	{
	case BIPART_OP_TYPE::EDGE_PRUNE:
		if (m_elemtype == ELEMENTTYPE::ENTITY)
			delete m_edges.at(0).first;
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
			delete m_edges.at(0).second;
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		if (m_elemtype == ELEMENTTYPE::ENTITY)
		{
			delete m_edges.at(0).first;
			delete m_edges.at(3).first;
		}
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
		{
			delete m_edges.at(0).second;
			delete m_edges.at(3).second;
		}
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (m_elemtype == ELEMENTTYPE::ENTITY)
		{
			delete m_edges.at(0).first;
			delete m_edges.at(1).first;
			delete m_edges.at(0).second;
		}
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
		{
			delete m_edges.at(0).second;
			delete m_edges.at(1).second;
			delete m_edges.at(0).first;
		}
		break;
	default:
		break;
	}
}



BIPART_OP_TYPE BipartOperation::getType()
{
	return m_type;
}

std::vector<std::pair<Entity*, Relationship*>> BipartOperation::getEdges()
{
	return m_edges;
}

ELEMENTTYPE BipartOperation::getElemType()
{
	return m_elemtype;
}

void BipartOperation::setEnResult(Entity* en)
{
	m_enresult = en;
	Relationship* re = en->getDualRelationship();
	if (re)
		m_reresult = re;
}

Entity* BipartOperation::getEnResult()
{
	return m_enresult;
}

void BipartOperation::setReResult(Relationship* re)
{
	m_reresult = re;
	Entity* en = re->getDualEntity();
	if (en)
		m_enresult = en;
}

Relationship* BipartOperation::getReResult()
{
	return m_reresult;
}

Element* BipartOperation::getResult()
{
	if (m_elemtype == ELEMENTTYPE::ENTITY)
		return m_enresult;
	else
		return m_reresult;
}

void BipartOperation::getElementIndices(std::vector<int>* enlist, std::vector<int>* relist)
{
	switch (m_type)
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		enlist->push_back(m_edges[0].first->getIndex());
		relist->push_back(m_edges[0].second->getIndex());
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		enlist->push_back(m_edges[0].first->getIndex());
		relist->push_back(m_edges[0].second->getIndex());
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		enlist->push_back(m_edges[0].first->getIndex());
		relist->push_back(m_edges[0].second->getIndex());
		enlist->push_back(m_edges[3].first->getIndex());
		relist->push_back(m_edges[3].second->getIndex());
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		enlist->push_back(m_edges[0].first->getIndex());
		relist->push_back(m_edges[0].second->getIndex());
		if (m_elemtype == ELEMENTTYPE::ENTITY)
			enlist->push_back(m_edges[1].first->getIndex());
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
			relist->push_back(m_edges[1].second->getIndex());
		break;
	default:
		break;
	}
}

void BipartOperation::incrementSimpFactors()
{
	switch (m_type)
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		m_edges[0].first->incrementSimpFactor();
		m_edges[0].second->incrementSimpFactor();
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		m_edges[0].first->incrementSimpFactor();
		m_edges[0].second->incrementSimpFactor();
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		m_edges[0].first->incrementSimpFactor();
		m_edges[0].second->incrementSimpFactor();
		m_edges[3].first->incrementSimpFactor();
		m_edges[3].second->incrementSimpFactor();
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (m_elemtype == ELEMENTTYPE::ENTITY)
		{
			m_edges[0].first->incrementSimpFactor();
			m_edges[0].second->incrementSimpFactor();
			m_edges[1].first->incrementSimpFactor();
		}
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
		{
			m_edges[0].first->incrementSimpFactor();
			m_edges[0].second->incrementSimpFactor();
			m_edges[1].second->incrementSimpFactor();
		}
		break;
	default:
		break;
	}
}

void BipartOperation::decrementSimpFactors()
{
	switch (m_type)
	{
	case BIPART_OP_TYPE::EDGE_CUT:
		m_edges[0].first->decrementSimpFactor();
		m_edges[0].second->decrementSimpFactor();
		break;
	case BIPART_OP_TYPE::EDGE_PRUNE:
		m_edges[0].first->decrementSimpFactor();
		m_edges[0].second->decrementSimpFactor();
		break;
	case BIPART_OP_TYPE::K22_COLLAPSE:
		m_edges[0].first->decrementSimpFactor();
		m_edges[0].second->decrementSimpFactor();
		m_edges[3].first->decrementSimpFactor();
		m_edges[3].second->decrementSimpFactor();
		break;
	case BIPART_OP_TYPE::CHAIN_MERGE:
		if (m_elemtype == ELEMENTTYPE::ENTITY)
		{
			m_edges[0].first->decrementSimpFactor();
			m_edges[0].second->decrementSimpFactor();
			m_edges[1].first->decrementSimpFactor();
		}
		else if (m_elemtype == ELEMENTTYPE::RELATIONSHIP)
		{
			m_edges[0].first->decrementSimpFactor();
			m_edges[0].second->decrementSimpFactor();
			m_edges[1].second->decrementSimpFactor();
		}
		break;
	default:
		break;
	}
}