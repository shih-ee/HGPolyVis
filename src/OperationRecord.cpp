#include "OperationRecord.h"

OperationRecord::OperationRecord(Element* remove, OPERATION_TYPE type, Element* retain, double importance, double cost)
{
	int removed_index = remove->getOpidx();
	string removed_label = remove->getLabel();
	m_removed = std::make_pair(removed_index, removed_label);

	if (retain)
		m_retained = retain->getOpidx();
	else
		m_retained = -1;

	m_type = type;
	m_importance = importance;
	m_cost = cost;
}

OperationRecord::~OperationRecord()
{
}


double OperationRecord::getImportance() const
{
	return m_importance;
}

void OperationRecord::setImportance(double importance)
{
	m_importance = importance;
}

double OperationRecord::getCost() const
{
	return m_cost;
}

void OperationRecord::setCost(double cost)
{
	m_cost = cost;
}

double OperationRecord::getPriority() const
{
	return m_importance - m_cost;
}

int OperationRecord::getRemovedIndex() const
{
	return m_removed.first;
}

void OperationRecord::setRemovedIndex(int index)
{
	m_removed.first = index;
}

std::string OperationRecord::getRemovedLabel()
{
	return m_removed.second;
}

void OperationRecord::setRemovedLabel(std::string label)
{
	m_removed.second = label;
}

std::pair<int, std::string> OperationRecord::getRemoved()
{
	return m_removed;
}

void OperationRecord::setRemoved(std::pair<int, std::string> removed)
{
	m_removed = removed;
}

int OperationRecord::getRetainedIndex()
{
	return m_retained;
}

void OperationRecord::setRetainedIndex(int index)
{
	m_retained = index;
}

std::vector<std::pair<int, std::string>> OperationRecord::getIncidentRemoved()
{
	return m_incidentRemoved;
}

void OperationRecord::setIncidentRemoved(std::vector<std::pair<int, std::string>> incident_list)
{
	m_incidentRemoved = incident_list;
}

std::vector<int> OperationRecord::getIncidentRetained()
{
	return m_incidentRetained;
}

void OperationRecord::setIncidentRetained(std::vector<int> incident_list)
{
	m_incidentRetained = incident_list;
}

OPERATION_TYPE OperationRecord::getType()
{
	return m_type;
}

void OperationRecord::setType(OPERATION_TYPE type)
{
	m_type = type;
}

bool OperationRecord::isLegal()
{
	return m_legal;
}

void OperationRecord::setLegal(bool legal)
{
	m_legal = legal;
}

bool OperationRecord::isApplied()
{
	return m_applied;
}

void OperationRecord::setApplied(bool applied)
{
	m_applied = applied;
}


bool mergesIdentical(OperationRecord* m1, OperationRecord* m2)
{
	return ((m1->getRemovedIndex() == m2->getRemovedIndex() && m1->getRetainedIndex() == m2->getRetainedIndex()) ||
			(m1->getRemovedIndex() == m2->getRetainedIndex() && m1->getRetainedIndex() == m2->getRemovedIndex())) &&
			(m1->getType() == m2->getType());
}