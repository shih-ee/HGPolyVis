#pragma once

#include <map>
#include "Element.h"
#include "Entity.h"
#include "Relationship.h"
#include "globalVariables.h"

enum BIPART_OP_TYPE
{
	EDGE_CUT,
	K22_COLLAPSE,
	EDGE_PRUNE,
	CHAIN_MERGE,
};

class Element;

class BipartOperation
{
private:

	BIPART_OP_TYPE m_type;
	ELEMENTTYPE m_elemtype;
	std::vector<std::pair<Entity*, Relationship*>> m_edges;
	
	Entity* m_enresult = NULL;
	Relationship* m_reresult = NULL;

public:

	BipartOperation(std::vector<std::pair<Entity*,Relationship*>> edges, BIPART_OP_TYPE type, ELEMENTTYPE elemtype);
	~BipartOperation();
	void deleteResult();
	void deleteOld();

	BIPART_OP_TYPE getType();
	ELEMENTTYPE getElemType();
	std::vector<std::pair<Entity*, Relationship*>> getEdges();
	void getElementIndices(std::vector<int>* enlist, std::vector<int>* relist);

	void setEnResult(Entity* en);
	Entity* getEnResult();
	void setReResult(Relationship* re);
	Relationship* getReResult();
	Element* getResult();

	void incrementSimpFactors();
	void decrementSimpFactors();
};