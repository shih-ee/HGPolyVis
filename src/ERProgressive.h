#pragma once

#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include "ERData.h"
#include "OperationRecord.h"

class ERProgressive
{
private:

	ERData* m_ERData;
	ERData* m_dualERData;
	std::vector<OperationRecord*> m_opqueue;
	std::stack<OperationRecord*> m_applied;
	std::stack<OperationRecord*> m_inverted;
	bool m_erem = false;
	bool m_emerge = false;
	bool m_nrem = false;
	bool m_nmerge = false;

	double m_alpha = 1.0;
	double m_beta = 1.0;
	double m_gamma = 1.0;
	double m_other = 1.0;

	int m_share = 1;

public:

	ERProgressive();
	~ERProgressive();

	ERData* getERData();
	ERData* getDualData();

	void setSimilarityCoefficients(double alpha, double beta, double gamma);
	void setOps(bool erem, bool emerge, bool nrem, bool nmerge);

	bool initialize_operation_queue(ERData* erdata);
	bool apply_operation(int max_ops, double priority_thresh=-DBL_MAX, int min_edges = 1, int min_nodes = 1, OperationRecord* OR = NULL);
	bool invert_operation(std::vector<int>* enlist, std::vector<int>* relist);

	int getAppliedOperationNum();

private:

	void clean_operations();
	void clear_opqueue();
	void updateOperationCost(OperationRecord* op);
	
	void updateQueueEdgeMerge(int removed, int retained, vector<int> adjacent, vector<int> incident);
	void updateQueueEdgeDelete(int removed, vector<int> adjacent, vector<int> incident);
	void updateQueueNodeMerge(int removed, int retained, vector<int> adjacent, vector<int> incident);
	void updateQueueNodeDelete(int removed, vector<int> adjacent, vector<int> incident);

	void removeIllegalOperations();

	
	// depricated
//public:
//	bool generate_operations(ERData* erdata, int max_emerge_size = 10);
//	bool apply_next_operation(TERM_CRITERIA term_criteria, int min_edges = 1, int min_nodes = 1, int max_emerge_size = 10, bool preserve_topology = false, bool prevent_orphans = false);
//	bool invert_last_operation();
//private:
//	bool generate_operations_edge_delete();
//	bool generate_operations_edge_merge(int max_emerge_size = 10);
//	bool generate_operations_node_delete();
//	bool generate_operations_node_merge();

//	double getEnRemPriority(Entity* en);
//	double getEnMergePriority(Entity* en1, Entity* en2);
//	double getReRemPriority(Relationship* re);
//	double getReMergePriority(Relationship* re1, Relationship* re2);
};

