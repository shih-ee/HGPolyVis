#pragma once

#include "ERData.h"
#include <adept.h>
#include "Energy.h"
#include "EnergyEvaluation.h"

class OptimizationHLBFGS
{
public:

	void init();
	void init_HLBFGS_Settings(double parameters[20], int info[20]);
	OptimizationHLBFGS();
	~OptimizationHLBFGS();

	// optimization of focused vis
	bool optimize_HLBFGS(ERData* mdata);
	static void energy_function(int N, double* x, double* prev_x, double* f, double* g);
	static bool HLBFGS_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);
	static bool plot_iteration(int iterIndex, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);

	// optimization of primal and dual vis simultaneously
	void dual_optimize_HLBFGS(ERData* mdata, ERData* mdualdata, bool unravel = false);
	static void dual_energy_function(int N, double* x, double* prev_x, double* f, double* g);
	static bool dual_HLBFGS_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);
	static bool dual_plot_iteration(int iterIndex, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);

	// optimization of focused vis excluding monogons
	void optimize_polygons_HLBFGS(ERData *mdata);
	static void polygon_energy_function(int N, double* x, double *prev_x, double* f, double* g);		//has to be static to be able to be load by HLBFGS function
	static bool polygon_HLBFGS_iteration(int iter, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir);
	static bool plot_polygon_iteration(int iterIndex, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir);

	// Optimization of primal and dual vis simultaneously excluding monogons
	void dual_optimize_polygons_HLBFGS(ERData *mdata, ERData *mdualdata);
	static void dual_polygon_energy_function(int N, double* x, double *prev_x, double* f, double* g);		//has to be static to be able to be load by HLBFGS function
	static bool dual_polygon_HLBFGS_iteration(int iter, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir);
	static bool dual_plot_polygon_iteration(int iterIndex, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir);

	// monogon optimization
	static bool optimizeMonogonsDual;
	void optimize_monogons_HLBFGS(ERData *mdata, bool optdual = false);
	static void monogon_energy_function(int N, double* x, double *prev_x, double* f, double* g);		//has to be static to be able to be load by HLBFGS function
	static bool monogon_HLBFGS_iteration(int iter, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir);

	// Optimization of subgraph
	void optimize_subgraph_HLBFGS(ERData* mdata);
	static void subgraph_energy_function(int N, double* x, double* prev_x, double* f, double* g);
	static bool HLBFGS_subgraph_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);

	// dual subgraph optimization
	void dual_optimize_subgraph_HLBFGS(ERData* pdata, ERData* ddata);
	static void dual_subgraph_energy_function(int N, double* x, double* prev_x, double* f, double* g);
	static bool dual_HLBFGS_subgraph_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir);

	/*the usage of the const adept::adouble follow the example in Adept document*/
	static adept::adouble calcMonogonSeparationEnergy(ERData *erdata, const int N, const adept::adouble * rotations);
	static adept::adouble calcMonogonSeparationDualEnergy(ERData *erdata, const int N, const adept::adouble * rotations);


	// energy evaluation outside of optimization
	void evaluate_layout_energy(ERData* mdata, bool polyReg, bool sideLen, bool polySep, bool interReg, bool overArea, bool overCount);

public:

	/*energy recording for optimization plot*/
	static LayoutEnergy<double> m_energy;
	static vector<IterLayoutEnergy<double>> m_optEnergyRecords;
	static int HLBFGSIterations;
	int HLBFGSEvaluations = 0;

private:
	static ERData *m_ERData;
	static ERData *m_dualERData;

};



