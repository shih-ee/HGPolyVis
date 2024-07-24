#pragma once

#include "ERData.h"
#include "BipartOperation.h"
#include <stack>
#include <queue>
#include <qstringlist.h>

class BipartOperation;

class BipartOpManager
{
private:

	ERData* m_erdata;
	
	std::queue<BipartOperation*> m_opqueue;
	std::vector<BipartOperation*> m_opapplied;
	std::vector<BipartOperation*> m_opinverted;
	std::queue<QStringList> m_readqueue;

public:

	BipartOpManager(ERData* erdata);
	~BipartOpManager();

	ERData* getERData();
	std::vector<BipartOperation*> getAppliedOps();
	int getTotalOpNum();

	void queueOperation(BipartOperation* op);
	bool createOperation(std::vector<Entity*> ens, std::vector<Relationship*> res, BIPART_OP_TYPE type);
	BipartOperation* readOperation(QStringList list);
	bool applyOperation(BipartOperation* op);
	bool invertOperation(BipartOperation* op);
	bool applyNextOperation();
	BipartOperation* invertLastOperation();

	bool cutBipartEdge(Entity* en, Relationship* re, ERData* erdata, bool dual_call = false);
	bool pruneBipartEdge(Entity* en, ERData* erdata, bool dual_call = false);
	bool pruneBipartEdge(Relationship* re, ERData* erdata, bool dual_call = false);
	Entity* collapseBipartK22(Entity* en1, Entity* en2, Entity* en3, ERData* erdata, bool dual_call = false);
	Relationship* collapseBipartK22(Relationship* re1, Relationship* re2, Relationship* re3, ERData* erdata, bool dual_call = false);
	Entity* mergeBipartChain(Entity* en1, Relationship* mi, Entity* en2, Entity* en3, ERData* erdata, bool dual_call = false);
	Relationship* mergeBipartChain(Relationship* re1, Entity* mi, Relationship* re2, Relationship* re3, ERData* erdata, bool dual_call = false);

	bool mendBipartEdge(Entity* en, Relationship* re, bool dual_call = false);
	bool growBipartEdge(Entity* en, ERData* erdata, bool dual_call = false);
	bool growBipartEdge(Relationship* re, ERData* erdata, bool dual_call = false);
	bool expandBipartK22(Entity* en1, Entity* en2, ERData* erdata, bool dual_call = false);
	bool expandBipartK22(Relationship* re1, Relationship* re2, ERData* erdata, bool dual_call = false);
	bool splitBipartChain(Entity* en1, Relationship* mi, Entity* en2, ERData* erdata, bool dual_call = false);
	bool splitBipartChain(Relationship* re1, Entity* mi, Relationship* re2, ERData* erdata, bool dual_call = false);

	int countCombinedCardinality(Relationship* re1, Relationship* re2);
	int countCombinedDegree(Entity* en1, Entity* en2);
	void addReadItem(QStringList list);

};