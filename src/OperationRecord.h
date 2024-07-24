#pragma once

#include <utility>
#include <string>
#include <vector>
#include "Element.h"
#include "globalVariables.h"

class Element;

class OperationRecord
{
private:

	bool m_legal = false;
	bool m_applied = false;
	double m_importance;
	double m_cost;
	std::pair<int, std::string> m_removed;						// removed element opid and label
	int m_retained;												// retained element opid (-1 if removal operation)
	std::vector<std::pair<int, std::string>> m_incidentRemoved;	// opids and labels of elements incident to removed element
	std::vector<int> m_incidentRetained;						// opids of elements incident to removed element
	OPERATION_TYPE m_type;

public:

	OperationRecord(Element* remove, OPERATION_TYPE type, Element* retain = NULL, double importance = 0.0, double cost = 0.0);
	~OperationRecord();

	double getImportance() const;
	void setImportance(double importance);
	double getCost() const;
	void setCost(double cost);
	double getPriority() const;
	int getRemovedIndex() const;
	void setRemovedIndex(int index);
	std::string getRemovedLabel();
	void setRemovedLabel(std::string label);
	std::pair<int, std::string> getRemoved();
	void setRemoved(std::pair<int, std::string> removed);
	int getRetainedIndex();
	void setRetainedIndex(int index);
	std::vector<std::pair<int, std::string>> getIncidentRemoved();
	void setIncidentRemoved(std::vector<std::pair<int, std::string>> incident_list);
	std::vector<int> getIncidentRetained();
	void setIncidentRetained(std::vector<int> incident_list);
	OPERATION_TYPE getType();
	void setType(OPERATION_TYPE type);
	bool isLegal();
	void setLegal(bool legal);
	bool isApplied();
	void setApplied(bool applied);
};

bool mergesIdentical(OperationRecord* m1, OperationRecord* m2);

struct CompareOperationRecords
{
	bool operator()(const OperationRecord* r1, const OperationRecord* r2)
	{
		if (r1->getImportance() == r2->getImportance())
			return r1->getRemovedIndex() < r2->getRemovedIndex();
		else
			return r1->getPriority() < r2->getPriority();
	}
};
