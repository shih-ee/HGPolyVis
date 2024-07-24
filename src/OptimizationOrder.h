#pragma once

#include "ERData.h"
#include <adept.h>
#include "Energy.h"
#include "EnergyEvaluation.h"

class OptimizationOrder
{
public:

	static double evaluate_Topology_energy(ERData *erdata);
	static double evaluate_ordering_energy_polygons(ERData* erdata, int N, const double* x);
	static double dual_evaluate_ordering_energy_polygons(ERData* erdata, ERData* dualerdata, int N1, int N2, const double* x);
	static double evaluate_ordering_energy(ERData* erdata, int NV, int NM, const double* x);
	static double dual_evaluate_ordering_energy(ERData* erdata, ERData* dualerdata, int pNV, int pNM, int dNV, int dNM, const double* x);

	bool iteration(ERData *erdata, int iter, int acceptIter, double *x, double f);
	bool dualiteration(ERData *erdata, ERData *dualERData, int N1, int N2,  double *xprime, double *xdual, int iter, int acceptIter, double f);

	/*change the locs, the order will be automatically corrected by reordering*/
	static void swapVerts(ERData *erdata, int vi, int vj, int N, const double* x, double* newx);
	void swapVertsInPlace(int i, int j, double* x);
	static Entity* getEntity(ERData *erdata, int N, const double * x, Relationship * re, int ith_incident);

	int reordering_polygons(ERData *mdata);
	int reordering_polygons(ERData *mdata, int N, double* x, bool update = true);
	int dual_reordering_polygons(ERData *mdata, ERData *dualERdata);
	int dual_reordering_polygons(ERData * mdata, ERData *dualERdata, int N1, int N2, double * x, bool update = true);

	int reordering(ERData* mdata, int NV, int NM, double* x, bool update = true);
	int dual_reordering(ERData* mdata, ERData* dualERData, int pNV, int pNM, int dNV, int dNM, double* x, bool update = true);

	int subgraph_reordering(ERData* mdata, int NV, int NM, double* x, std::vector<std::pair<int,int>> &swaps);
	double evaluate_subgraph_ordering_energy(ERData* erdata, int NV, int NM, const double* x);

	static LayoutEnergy<double> m_energy;
	static vector<IterLayoutEnergy<double>> m_optEnergyRecords;

private:
	static ERData *m_ERData;
	static ERData *m_dualERData;
};