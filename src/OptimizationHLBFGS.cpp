#pragma once

#include "OptimizationHLBFGS.h"
#include <set>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include "ElementUtils.h"
#include <algorithm>
#include "mainwindow.h"
#include "LogConsole.h"
#include <QCoreApplication>
#include "OptimizationOrder.h"
#include <adept_arrays.h> 
#include "HLBFGS.h"
#include <type_traits>
#include "string.h"
#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>

bool OptimizationHLBFGS::optimizeMonogonsDual = false;
ERData* OptimizationHLBFGS::m_ERData = NULL;
ERData* OptimizationHLBFGS::m_dualERData = NULL;
LayoutEnergy<double>  OptimizationHLBFGS::m_energy;
vector<IterLayoutEnergy<double>> OptimizationHLBFGS::m_optEnergyRecords;
int OptimizationHLBFGS::HLBFGSIterations = 0;

using adept::adouble;

void OptimizationHLBFGS::init()
{
	m_ERData = NULL;
	m_dualERData = NULL;
}

OptimizationHLBFGS::OptimizationHLBFGS()
{
	init();
}

OptimizationHLBFGS::~OptimizationHLBFGS()
{
	m_ERData = NULL;
	m_dualERData = NULL;
}

void OptimizationHLBFGS::init_HLBFGS_Settings(double parameter[20], int info[20])
{
	//initialize
	INIT_HLBFGS(parameter, info);
	//parameter[0] = 1e-8;
	parameter[0] = gmainwindow->HLBFGS_linesearch_ftol;		//function tolerance used in line-search, 1.0e-4
	parameter[1] = gmainwindow->HLBFGS_linesearch_xtol;		//variable tolerance used in line-search, 1.0e-16
	parameter[2] = gmainwindow->HLBFGS_linesearch_gtol;		//gradient tolerance used in line-search, 0.9
	parameter[3] = gmainwindow->HLBFGS_linesearch_stepmin;	//stpmin used in line-search, 1.0e-20
	parameter[4] = gmainwindow->HLBFGS_linesearch_stepmax;	//stpmax used in line-search, 1.0e+20
	parameter[5] = gmainwindow->GNormDivisionThreshold;		//：	1.0e-9 the stop criterion ( ||G||/max(1,||X||) < PARAMETER[5] )
	parameter[6] = gmainwindow->GNormThreshold;				//the stop criterion(|| G || < PARAMETER[6])

	info[0] = gmainwindow->HLBFGS_linesearch_IterMax;		//20, the max number of evaluation in line-search
	//info[1] = 0;						//0, the total number of evalfunc calls
	//info[2] = 0;						//0, the current number of iterations
	info[3] = 0;						//0, The lbfgs strategy. 0: standard, 1: M1QN3 strategy(recommended).
	info[4] = gmainwindow->ITERMAX;		//100000, the max number of iterations
	//info[5] = 1						//1, 1: print message, 0: do nothing
	//info[6] = 0;						//10, T: the update interval of Hessian. (typical choices: 0-200)
	info[7] = 0;						//0, 0: without hessian, 1: with accurate hessian
	//info[8] = 15;						//15, icfs parameter
	//info[9] = 0;						//0, 0: classical line-search; 1: modified line-search (it is not useful in practice)
	info[10] = 0;						//0, 0: Disable preconditioned CG; 1: Enable preconditioned CG
	//info[11] = 1;						//1, 0 or 1 defines different methods for choosing beta in CG.
	//info[12] = 1;						//1, internal usage. 0: only update the diag in USER_DEFINED_HLBFGS_UPDATE_H; 1: default.
	//info[13] = 0;						//0, 0: standard lbfgs update, 1 : Biggs's update, 2: Yuan's update; 3: Zhang and Xu's update

										//Gradient Decent,		M = 0, INFO[7] = 0, INFO[10] = 0
										//Conjugate Gradient,	M = 0, INFO[7] = 0, INFO[10] = 1
										//Newton's method,		M = 0, INFO[6] = 0, INFO[7] = 1, INFO[10] = 0 
										//L - BFGS,				M >= 1,INFO[3] = 0, INFO[7] = 0, INFO[10] = 0
}


//void NEWITERATION(int ITER, int CALL_ITER, double *X, double* F, double *G, double* GNORM). 
//is used to collect internal information after each iteration, ITER stores the current number of iterations, 
//CALL_ITER stores the number of EVALFUNC calls from the beginning, GNORM stores the Euclidean norm of the gradient.

bool OptimizationHLBFGS::optimize_HLBFGS(ERData* mdata)
{
	if (mdata == NULL)
		return false;

	m_ERData = mdata;
	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;
	
	int NV = m_ERData->getEntityNum() * 2;
	REList monogons = m_ERData->getMonogons();
	int NM = monogons.size();
	if (NV == 0) 
	{
		LogConsole::showError("No data to optimize");
		return false;
	}

	double* x = new double[NV + NM];
	double* s = new double[NV + NM];
	for (int i = 0; i < m_ERData->getEntityNum(); i++)
	{
		assert(m_ERData->getEntity(i)->getIndex() == i);
		x[i * 2] = m_ERData->getEntity(i)->getLocation().x;
		x[i * 2 + 1] = m_ERData->getEntity(i)->getLocation().y;
	}
	for (int i = 0; i < monogons.size(); i++)
	{
		x[NV + i] = monogons[i]->getMonogonRotation();
	}

	int M = gmainwindow->HLBFGS_UpdateNum;
	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	/*LogConsole::writeConsole("M %d, ITERMAX:%d, min|G|:%.3e", M, info[4], parameter[6]);
	LogConsole::writeConsole("LSearch IterMax:%d", info[0]);
	LogConsole::writeConsole("LSearch ftol:%.4e", parameter[0]);
	LogConsole::writeConsole("LSearch xtol:%.4e", parameter[1]);
	LogConsole::writeConsole("LSearch gtol:%.4e", parameter[2]);
	LogConsole::writeConsole("LSearch stepMin:%.4e", parameter[3]);
	LogConsole::writeConsole("LSearch stepMax:%.4e", parameter[4]);*/

	//if (flag_plotOptimizationEnergy)
	//	HLBFGS_iteration(0, 0, x.value(), NULL, NULL, NULL, NULL);

	int maxIterationNum = gmainwindow->ITERMAX;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif

	// continue optimization until layout cannot be optimized anymore
	double step;
	bool convergence;
	while (true)
	{
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

			convergence = HLBFGS((NV + NM), M, x, this->energy_function, 0, HLBFGS_UPDATE_Hessian, this->HLBFGS_iteration, parameter, info, s, &step);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		 HLBFGSEvaluations++;
		 HLBFGSIterations += info[2];
		 if (gmainwindow->flag_stopOptimization)
		 {
		 	LogConsole::writeConsole("Optimization stopped");
		 	break;
		 }
		 if (HLBFGSIterations >= maxIterationNum)
		 {
		 	LogConsole::writeConsole("Exceed Iter Max %d", maxIterationNum);
		 	break;
		 }

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering) 
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			OptimizationOrder optsolver;
			swapNum = optsolver.reordering(m_ERData, NV, NM, x, false);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addSwapEvalTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

			plot_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
			LogConsole::writeConsole("Swap (%d)", swapNum);
		}

		if (gmainwindow->flag_HLBFGS_stopWhenConverged)
		{
			if (info[2] == 1 && HLBFGSIterations != 1 && swapNum == 0)
			{
				LogConsole::writeConsole("Converged");
				break;
			}
		}
		else if (swapNum == 0)
		{
			break;
		}
	}

	m_ERData->updateAllLocations(NV, NM, x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();
	
	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);

	delete[] x;
	delete[] s;
	m_ERData = NULL;

	return convergence;
}

void OptimizationHLBFGS::energy_function(int N, double* x, double* prev_x, double* f, double* g)
{
	if (gmainwindow->flag_stopOptimization)
		return;

	int NV = m_ERData->getEntityNum() * 2;
	int NM = m_ERData->getMonogons().size();
	assert((NV + NM) == N);

	OptimizationHLBFGS::m_energy = evaluate_energy(m_ERData, NV, NM, x, g);
	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
}

bool OptimizationHLBFGS::HLBFGS_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	int iterIndex = HLBFGSIterations + iter;
	return plot_iteration(iterIndex, call_iter, x, f, g, gnorm, searchdir);
}

bool OptimizationHLBFGS::plot_iteration(int iterIndex, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	// if (flag_showOptimizationInfo && f && gnorm)
	// 	LogConsole::writeConsole("iter:%d:%d\nenergy:%.8f\nnormOfGradient:%.8f", iterIndex, call_iter, *f, *gnorm);

	if (gmainwindow->flag_animateOptimization)
	{
		QCoreApplication::processEvents();

		int NV = m_ERData->getEntityNum() * 2;
		int NM = m_ERData->getMonogons().size();

		m_ERData->updateAllLocations(NV, NM, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}


void OptimizationHLBFGS::dual_optimize_HLBFGS(ERData* mdata, ERData* mdualdata, bool unravel)
{
	if (mdata == NULL || mdualdata == NULL)
		return;

	m_ERData = mdata;
	m_dualERData = mdualdata;
	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;

	int pNV = m_ERData->getEntityNum() * 2;
	int dNV = m_dualERData->getEntityNum() * 2;
	REList monogons = m_ERData->getMonogons();
	REList dual_monogons = m_dualERData->getMonogons();
	int pNM = monogons.size();
	int dNM = dual_monogons.size();

	if (pNV == 0)
	{
		LogConsole::showError("No data to optimize");
		return;
	}

	if (dNV == 0)
	{
		LogConsole::showError("No Dual Data to optimize");
		return;
	}

	double* x = new double[pNV + pNM + dNV + dNM];
	double* s = new double[pNV + pNM + dNV + dNM];
	for (int i = 0; i < m_ERData->getEntityNum(); i++)
	{
		assert(m_ERData->getEntity(i)->getIndex() == i);
		x[i * 2] = m_ERData->getEntity(i)->getLocation().x;
		x[i * 2 + 1] = m_ERData->getEntity(i)->getLocation().y;
	}
	for (int i = 0; i < pNM; i++)
	{
		x[pNV + i] = monogons[i]->getMonogonRotation();
	}
	for (int i = 0; i < m_dualERData->getEntityNum(); i++)
	{
		x[pNV + pNM + i * 2] = m_dualERData->getEntity(i)->getLocation().x;
		x[pNV + pNM + i * 2 + 1] = m_dualERData->getEntity(i)->getLocation().y;
	}
	for (int i = 0; i < dNM; i++)
	{
		x[pNV + pNM + dNV + i] = dual_monogons[i]->getMonogonRotation();
	}

	int M = gmainwindow->HLBFGS_UpdateNum;
	int maxIterationNum = gmainwindow->ITERMAX;
	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	//LogConsole::writeConsole("M %d, ITERMAX:%d, min|G|:%.3e", M, info[4], parameter[6]);
	//LogConsole::writeConsole("LSearch IterMax:%d", info[0]);
	//LogConsole::writeConsole("LSearch ftol:%.4e", parameter[0]);
	//LogConsole::writeConsole("LSearch xtol:%.4e", parameter[1]);
	//LogConsole::writeConsole("LSearch gtol:%.4e", parameter[2]);
	//LogConsole::writeConsole("LSearch stepMin:%.4e", parameter[3]);
	//LogConsole::writeConsole("LSearch stepMax:%.4e", parameter[4]);

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	// continue optimization until layout cannot be optimized anymore
	while (true)
	{
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		if (unravel)
			HLBFGS((pNV + pNM + dNV + dNM), M, x, this->dual_energy_function, 0, HLBFGS_UPDATE_Hessian, this->dual_HLBFGS_iteration, parameter, info, s);
		else
			HLBFGS((pNV + pNM + dNV + dNM), M, x, this->dual_energy_function, 0, HLBFGS_UPDATE_Hessian, this->dual_HLBFGS_iteration, parameter, info, s);
		
// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		HLBFGSEvaluations++;
		HLBFGSIterations += info[2];

		if (gmainwindow->flag_stopOptimization)
		{
			LogConsole::writeConsole("Optimization stopped");
			break;
		}
		if (HLBFGSIterations >= maxIterationNum)
		{
			LogConsole::writeConsole("Exceed Iter Max %d", maxIterationNum);
			break;
		}

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			OptimizationOrder optSolver;
			swapNum = optSolver.dual_reordering(m_ERData, m_dualERData, pNV, pNM, dNV, dNM, x, false);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addSwapEvalTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

			dual_plot_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
			LogConsole::writeConsole("Swap (%d)", swapNum);
		}

		if (gmainwindow->flag_HLBFGS_stopWhenConverged) {
			if (info[2] == 1 && HLBFGSIterations != 1 && swapNum == 0)
			{
				LogConsole::writeConsole("Converged");
				break;
			}
		}
		else if (swapNum == 0)
			break;
	}

	m_ERData->updateAllLocations(pNV, pNM, x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();
	//m_ERData->update2D();

	m_dualERData->updateAllLocations(dNV, dNM, x, pNV + pNM);
	m_dualERData->starrizeRelationships();
	m_dualERData->create2DScaffolds();
	//m_dualERData->update2D();

	
	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);

	delete[] x;
	delete[] s;
	m_ERData = NULL;
	m_dualERData = NULL;
}

void OptimizationHLBFGS::dual_energy_function(int N, double* x, double* prev_x, double* f, double* g)
{
	if (gmainwindow->flag_stopOptimization)
		return;

	int pNV = m_ERData->getEntityNum() * 2;
	int dNV = m_dualERData->getEntityNum() * 2;
	int pNM = m_ERData->getMonogons().size();
	int dNM = m_dualERData->getMonogons().size();
	assert((pNV + dNV + pNM + dNM) == N);

	OptimizationHLBFGS::m_energy = dual_evaluate_energy(m_ERData, m_dualERData, pNV, pNM, dNV, dNM, x, g);
	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
}

bool OptimizationHLBFGS::dual_HLBFGS_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	int iterIndex = HLBFGSIterations + iter;
	return dual_plot_iteration(iterIndex, call_iter, x, f, g, gnorm, searchdir);
}

bool OptimizationHLBFGS::dual_plot_iteration(int iterIndex, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	if (gmainwindow->flag_animateOptimization)
	{
		QCoreApplication::processEvents();

		int pNV = m_ERData->getEntityNum() * 2;
		int dNV = m_dualERData->getEntityNum() * 2;
		int pNM = m_ERData->getMonogons().size();
		int dNM = m_dualERData->getMonogons().size();

		m_ERData->updateAllLocations(pNV, pNM, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		m_dualERData->updateAllLocations(dNV, dNM, x, pNV + pNM);
		m_dualERData->starrizeRelationships();
		m_dualERData->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
		{
			gmainwindow->reset_sceneToContent(gmainwindow->primeVis);
			gmainwindow->reset_sceneToContent(gmainwindow->dualVis);
		}

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}


void OptimizationHLBFGS::optimize_polygons_HLBFGS(ERData *mdata)
{
	if (mdata == NULL)
		return;

	m_ERData = mdata;
	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;

	if (m_ERData->getEntities().size() == 0) {
		LogConsole::showError("No data to optimize");
		return;
	}

	double energy;

	int N = m_ERData->getFreeVertCompNum();;
	double* x = new double[N];

	for (int i = 0; i < m_ERData->getEntityNum(); i++) {
		assert(m_ERData->getEntity(i)->getIndex() == i);

		int vi = m_ERData->getEntity(i)->getIndex();
		int listindex_x = vertex_index_x_to_list_index(vi, m_ERData->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(vi, m_ERData->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1) {
			continue;
		}

		x[listindex_x] = m_ERData->getEntity(i)->getLocation().x;
		x[listindex_y] = m_ERData->getEntity(i)->getLocation().y;
	}

	int M = gmainwindow->HLBFGS_UpdateNum;	//M is the number of LBFGS updates (typical choices: 3-20)
	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	//LogConsole::writeConsole("M %d, ITERMAX:%d, min|G|:%.3e", M, info[4], parameter[6]);
	//LogConsole::writeConsole("LSearch IterMax:%d", info[0]);
	//LogConsole::writeConsole("LSearch ftol:%.4e", parameter[0]);
	//LogConsole::writeConsole("LSearch xtol:%.4e", parameter[1]);
	//LogConsole::writeConsole("LSearch gtol:%.4e", parameter[2]);
	//LogConsole::writeConsole("LSearch stepMin:%.4e", parameter[3]);
	//LogConsole::writeConsole("LSearch stepMax:%.4e", parameter[4]);

	//if(flag_plotOptimizationEnergy)
	//	polygon_HLBFGS_iteration(0, 0, x.value(), NULL, NULL, NULL, NULL);

	int maxIterationNum = gmainwindow->ITERMAX;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP
	
	while (true) {
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		HLBFGS(N, M, x, this->polygon_energy_function, 0, HLBFGS_UPDATE_Hessian, this->polygon_HLBFGS_iteration, parameter, info);
		
// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		HLBFGSEvaluations++;
		HLBFGSIterations += info[2];

		if (gmainwindow->flag_stopOptimization)
		{
			LogConsole::writeConsole("Optimization stopped");
			break;
		}
		if (HLBFGSIterations >= maxIterationNum) {
			LogConsole::writeConsole("Exceed Iter Max %d", maxIterationNum);
			break;
		}

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			OptimizationOrder optsolver;
			swapNum = optsolver.reordering_polygons(m_ERData, N, x, false);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addSwapEvalTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

			plot_polygon_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
			LogConsole::writeConsole("Swap (%d)", swapNum);
		}

		if (gmainwindow->flag_HLBFGS_stopWhenConverged) {
			if (info[2] == 1 && HLBFGSIterations != 1 && swapNum == 0)
			{
				LogConsole::writeConsole("Converged");
				break;
			}
		}
		else if (swapNum == 0) {
			break;
		}
	}
	
	m_ERData->updateFreeEntityLocs(N,x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();

	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);

	delete[] x;
	m_ERData = NULL;
}

void OptimizationHLBFGS::polygon_energy_function(int N, double * x, double * prev_x, double * f, double * g)
{
	if (gmainwindow->flag_stopOptimization)
		return;

	OptimizationHLBFGS::m_energy = evaluate_polygon_energy(m_ERData, N, x, g);

	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo) {
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
	}
}

bool OptimizationHLBFGS::polygon_HLBFGS_iteration(int iter, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir)
{
	int iterIndex = HLBFGSIterations + iter;
	return plot_polygon_iteration(iterIndex, call_iter, x, f, g, gnorm, searchdir);
}

bool OptimizationHLBFGS::plot_polygon_iteration(int iterIndex, int call_iter, double * x, double * f, double * g, double * gnorm, double* searchdir)
{
	int N = m_ERData->getFreeVertCompNum();

	if (gmainwindow->flag_animateOptimization) {

		QCoreApplication::processEvents();

		if (gmainwindow->flag_showLSearchDirs && searchdir!= NULL) {
			int N = m_ERData->getFreeVertCompNum();
			gmainwindow->getGradientDirections(m_ERData, N, searchdir);
		}
		if (gmainwindow->flag_showGradientDirs) {
			int N = m_ERData->getFreeVertCompNum();
			gmainwindow->getGradientDirections(m_ERData, N, g);
		}

		int N = m_ERData->getFreeVertCompNum();
		m_ERData->updateFreeEntityLocs(N, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}


void OptimizationHLBFGS::dual_optimize_polygons_HLBFGS(ERData * mdata, ERData * mdualdata)
{
	if (mdata == NULL)
		return;

	m_ERData = mdata;
	m_dualERData = mdualdata;

	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;

	int N1 = m_ERData->getFreeVertCompNum();
	int N2 = m_dualERData->getEntities().size() * 2;

	if (N1 == 0) {
		LogConsole::showError("No data to optimize");
		return;
	}

	if (N2 == 0) {
		LogConsole::showError("No Dual Data to optimize");
		return;
	}

	double* x = new double[N1 + N2];
	for (int i = 0; i < m_ERData->getEntityNum(); i++) {
		assert(m_ERData->getEntity(i)->getIndex() == i);

		int vi = m_ERData->getEntity(i)->getIndex();
		int listindex_x = vertex_index_x_to_list_index(vi, m_ERData->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(vi, m_ERData->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1) {
			continue;
		}

		x[listindex_x] = m_ERData->getEntity(i)->getLocation().x;
		x[listindex_y] = m_ERData->getEntity(i)->getLocation().y;
	}

	for (int i = 0; i <m_dualERData->getEntities().size(); i++) {
		x[N1 + 2 * i] = m_dualERData->getEntity(i)->getLocation().x;
		x[N1 + 2 * i + 1] = m_dualERData->getEntity(i)->getLocation().y;
	}

	int M = gmainwindow->HLBFGS_UpdateNum;

	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	//LogConsole::writeConsole("M %d, ITERMAX:%d, min|G|:%.3e", M, info[4], parameter[6]);
	//LogConsole::writeConsole("LSearch IterMax:%d", info[0]);
	//LogConsole::writeConsole("LSearch ftol:%.4e", parameter[0]);
	//LogConsole::writeConsole("LSearch xtol:%.4e", parameter[1]);
	//LogConsole::writeConsole("LSearch gtol:%.4e", parameter[2]);
	//LogConsole::writeConsole("LSearch stepMin:%.4e", parameter[3]);
	//LogConsole::writeConsole("LSearch stepMax:%.4e", parameter[4]);

	//if (flag_plotOptimizationEnergy)
	//	dual_polygon_HLBFGS_iteration(0, 0, x.value(), NULL, NULL, NULL, NULL);

	int maxIterationNum = gmainwindow->ITERMAX;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	/*continue optimization until layout cannot be optimized anymore*/
	while (true) {
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		HLBFGS(N1 + N2, M, x, this->dual_polygon_energy_function, 0, HLBFGS_UPDATE_Hessian, this->dual_polygon_HLBFGS_iteration, parameter, info);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		HLBFGSEvaluations++;
		HLBFGSIterations += info[2];
		if (gmainwindow->flag_stopOptimization)
		{
			LogConsole::writeConsole("Optimization stopped");
			break;
		}
		if (HLBFGSIterations >= maxIterationNum) {
			LogConsole::writeConsole("Exceed Itermax)");
			break;
		}

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			OptimizationOrder optsolver;
			swapNum = optsolver.dual_reordering_polygons(m_ERData, m_dualERData, N1, N2, x, false);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addSwapEvalTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

			dual_plot_polygon_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
			LogConsole::writeConsole("Swap (%d)", swapNum);
		}

		if (gmainwindow->flag_HLBFGS_stopWhenConverged) {
			if (info[2] == 1 && HLBFGSIterations != 1 && swapNum == 0)
			{
				LogConsole::writeConsole("Converged)");
				break;
			}
		}
		else if (swapNum == 0) {
			break;
		}
	}

	// update primal ERData
	{
		m_ERData->updateFreeEntityLocs(N1, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();
		m_ERData->update2D();
	}

	//update dual ERData
	{
		vector<Vec3> locList2;
		for (int i = 0; i < m_dualERData->getEntities().size(); i++) {
			Vec3 newLoc(x[N1 + 2 * i], x[N1 + 2 * i + 1], 0);
			locList2.push_back(newLoc);
		}
		m_dualERData->updateEntitiesLocation(locList2);
		m_dualERData->starrizeRelationships();
		m_dualERData->create2DScaffolds();
		m_dualERData->update2D();
	}

	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);
	
	m_ERData = NULL;
	m_dualERData = NULL;
}

void OptimizationHLBFGS::dual_polygon_energy_function(int N, double* x, double *prev_x, double* f, double* g) {

	if (gmainwindow->flag_stopOptimization)
		return;

	int N1 = m_ERData->getFreeVertCompNum();
	int N2 = m_dualERData->getEntities().size() * 2;

	assert(N1 + N2 == N);

	OptimizationHLBFGS::m_energy = dual_evaluate_polygon_energy(m_ERData, m_dualERData, N1, N2, x, g);
	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo) {
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
	}
}

bool OptimizationHLBFGS::dual_polygon_HLBFGS_iteration(int iter, int call_iter, double * x, double * f, double * g, double * gnorm, double* searchdir)
{

	int iterIndex = HLBFGSIterations + iter;
	return dual_plot_polygon_iteration(iterIndex, call_iter, x, f, g, gnorm, searchdir);
}

bool OptimizationHLBFGS::dual_plot_polygon_iteration(int iterIndex, int call_iter, double * x, double * f, double * g, double * gnorm, double * searchdir)
{

	int N1 = m_ERData->getFreeVertCompNum();
	int N2 = m_dualERData->getEntities().size() * 2;

	if (gmainwindow->flag_animateOptimization) {

		QCoreApplication::processEvents();

		m_ERData->updateFreeEntityLocs(N1, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		vector<Vec3> locList2;
		double XNORM2 = 0;
		for (int i = 0; i < m_dualERData->getEntities().size(); i++) {
			Vec3 newLoc(x[N1 + 2 * i], x[N1 + 2 * i + 1], 0);
			locList2.push_back(newLoc);
			XNORM2 += x[i] * x[i];
		}

		m_dualERData->updateEntitiesLocation(locList2);
		m_dualERData->starrizeRelationships();
		m_dualERData->create2DScaffolds();


		if (gmainwindow->flag_autoAdjustViewpoint) {
			gmainwindow->reset_sceneToContent(gmainwindow->primeVis);
			gmainwindow->reset_sceneToContent(gmainwindow->dualVis);
		}

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}


void OptimizationHLBFGS::optimize_monogons_HLBFGS(ERData * mdata, bool optdual)
{
	if (mdata == NULL)
		return;

	optimizeMonogonsDual = optdual;
	m_ERData = mdata;

	if (m_ERData->getMonogons().size() == 0) {
		LogConsole::showError("No monogon to optimize");
		return;
	}

	int N = m_ERData->getMonogons().size();
	std::vector<double> x(N);
	int count = 0;
	for (int i = 0; i <m_ERData->getRelationshipNum(); i++) {
		if (m_ERData->getRelationship(i)->getCardinality() != 1)
		{
			continue;
		}
		x[count] = m_ERData->getRelationship(i)->getMonogonRotation();

		if (optimizeMonogonsDual) {
			if (m_ERData->getRelationship(i)->getDualEntity() == NULL) {
				LogConsole::showError("monogon %d has no dual element", i);
				return;
			}
		}
		count++;
	}

	int M = gmainwindow->HLBFGS_UpdateNum;

	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);

	LogConsole::writeConsole("Max iter:%d, min|G|:%f", info[4], parameter[6]);
	LogConsole::writeConsole("LSearch IterMax:%d", info[0]);
	LogConsole::writeConsole("LSearch ftol:%.4e", parameter[0]);
	LogConsole::writeConsole("LSearch xtol:%.4e", parameter[1]);
	LogConsole::writeConsole("LSearch gtol:%.4e", parameter[2]);
	LogConsole::writeConsole("LSearch stepMin:%.4e", parameter[3]);
	LogConsole::writeConsole("LSearch stepMax:%.4e", parameter[4]);

	HLBFGS(N, M, &x[0], this->monogon_energy_function, 0, HLBFGS_UPDATE_Hessian, this->monogon_HLBFGS_iteration, parameter, info);

	{
		m_ERData->updateMonogonRotations(x);
	}

	LogConsole::writeConsole("----Energy After OPT");
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();
	m_ERData->update2D();
	m_ERData = NULL;
}

void OptimizationHLBFGS::monogon_energy_function(int N, double* x, double *prev_x, double* f, double* g)
{
	if (gmainwindow->flag_stopOptimization)
		return;
	
	gmainwindow->increaseEnergyEvalCount();

	using adept::adouble;						// Import adouble from adept
	adept::Stack stack;							// Where the derivative information is stored

	adouble *x_input = new adouble[N];
	for (int i = 0; i < N; i++) {
		x_input[i] = x[i];						// Initialize active input variables
		if (std::isnan(x[i]))
		{
			LogConsole::writeConsole("[NOTE]nan x value");
			return;
		}
	}

	// Start recording
	stack.new_recording();	
	adouble y = 0.0;                                 // Call version overloaded for adouble args
	
	if (optimizeMonogonsDual) {
		y = calcMonogonSeparationDualEnergy(m_ERData, N, x_input);
	}
	else {
		y = calcMonogonSeparationEnergy(m_ERData, N, x_input);
	}

	if (gmainwindow->flag_showOptimizationInfo)
		LogConsole::writeConsole("[Energy]%.8e", y.value());

	y.set_gradient(1.0);						// Defines y as the objective function
	stack.compute_adjoint();					// Run the adjoint algorithm

	for (int i = 0; i < N; i++) {
		g[i] = x_input[i].get_gradient();
	}

	delete[] x_input;

	*f = y.value();								// Return the result of the simple computation
}

bool OptimizationHLBFGS::monogon_HLBFGS_iteration(int iter, int call_iter, double *x, double* f, double *g, double* gnorm, double* searchdir)
{
	if (gmainwindow->flag_animateOptimization) {
		
		/*update monogon rotations*/
		{
			int i = 0;
			for (auto &re : OptimizationHLBFGS::m_ERData->getRelationships()) {
				if (re->getCardinality() != 1) {
					continue;
				}
				re->setMonogonRotation(x[i]);
				re->create2DScaffolds();
				i++;
			}
		}

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}


void OptimizationHLBFGS::optimize_subgraph_HLBFGS(ERData* mdata)
{
	if (mdata == NULL)
		return;

	m_ERData = mdata;
	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;

	int NV = m_ERData->getFreeVertCompNum();
	int NM = m_ERData->getFreeMonoCompNum();
	REList monogons = m_ERData->getMonogons();
	double* x = new double[NV + NM];
	for (int i = 0; i < m_ERData->getEntityNum(); i++)
	{
		Entity* en = m_ERData->getEntity(i);
		assert(en->getIndex() == i);

		int listindex_x = vertex_index_x_to_list_index(i, m_ERData->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(i, m_ERData->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1)
			continue;

		x[listindex_x] = en->getLocation().x;
		x[listindex_y] = en->getLocation().y;
	}
	for (int i = 0; i < monogons.size(); i++)
	{
		Relationship* re = monogons.at(i);
		int listindex = monogon_index_to_list_index(i, NV, m_ERData->getSortedFixedMonoIndices());

		if (listindex == -1)
			continue;

		x[listindex] = re->getMonogonRotation();
	}

	int M = gmainwindow->HLBFGS_UpdateNum;
	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	int maxIterationNum = gmainwindow->ITERMAX;
	std::vector<std::pair<int, int>> swaps;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	while (true)
	{
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		HLBFGS(NV + NM, M, x, this->subgraph_energy_function, 0, HLBFGS_UPDATE_Hessian, this->HLBFGS_subgraph_iteration, parameter, info);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		HLBFGSIterations += info[2];
		HLBFGSEvaluations++;
		if (gmainwindow->flag_stopOptimization)
		{
			LogConsole::writeConsole("Optimization stopped");
			break;
		}
		if (HLBFGSIterations >= maxIterationNum)
		{
			LogConsole::writeConsole("Exceed Iter Max %d", maxIterationNum);
			break;
		}

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering && NV > 4)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			OptimizationOrder optsolver;
			swapNum = optsolver.subgraph_reordering(m_ERData, NV, NM, x, swaps);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addSwapEvalTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

			HLBFGS_subgraph_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
		}

		if (swapNum == 0)
			break;
	}

	m_ERData->updateAllFreeLocations(NV, NM, x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();

	
	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);

	delete[] x;
	m_ERData = NULL;
}

void OptimizationHLBFGS::subgraph_energy_function(int N, double* x, double* prev_x, double* f, double* g)
{
	if (gmainwindow->flag_stopOptimization)
		return;
	
	int NV = m_ERData->getFreeVertCompNum();
	int NM = m_ERData->getFreeMonoCompNum();
	assert((NV + NM) == N);

	OptimizationHLBFGS::m_energy = evaluate_subgraph_energy(m_ERData, NV, NM, x, g);
	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
}

bool OptimizationHLBFGS::HLBFGS_subgraph_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	// if (flag_showOptimizationInfo && f && gnorm)
	// 	LogConsole::writeConsole("iter:%d:%d\nenergy:%.8f\nnormOfGradient:%.8f", iter, call_iter, *f, *gnorm);

	if (gmainwindow->flag_animateOptimization)
	{
		int NV = m_ERData->getFreeVertCompNum();
		int NM = m_ERData->getFreeMonoCompNum();
		m_ERData->updateAllFreeLocations(NV, NM, x);

		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}

void OptimizationHLBFGS::dual_optimize_subgraph_HLBFGS(ERData* pdata, ERData* ddata)
{
	if (pdata == NULL || ddata == NULL)
		return;

	m_ERData = pdata;
	m_dualERData = ddata;
	HLBFGSIterations = 0;
	HLBFGSEvaluations = 0;

	int pNV = m_ERData->getFreeVertCompNum();
	int dNV = m_dualERData->getFreeVertCompNum();
	int pNM = m_ERData->getFreeMonoCompNum();
	int dNM = m_dualERData->getFreeMonoCompNum();
	REList pMonogons = m_ERData->getMonogons();
	REList dMonogons = m_dualERData->getMonogons();
	double* x = new double[pNV + pNM + dNV + dNM];
	for (int i = 0; i < m_ERData->getEntityNum(); i++)
	{
		Entity* en = m_ERData->getEntity(i);
		assert(en->getIndex() == i);

		int listindex_x = vertex_index_x_to_list_index(i, m_ERData->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(i, m_ERData->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1)
			continue;

		x[listindex_x] = en->getLocation().x;
		x[listindex_y] = en->getLocation().y;
	}
	for (int i = 0; i < pMonogons.size(); i++)
	{
		Relationship* re = pMonogons.at(i);
		int listindex = monogon_index_to_list_index(i, pNV, m_ERData->getSortedFixedMonoIndices());

		if (listindex == -1)
			continue;

		x[listindex] = re->getMonogonRotation();
	}
	for (int i = 0; i < m_dualERData->getEntityNum(); i++)
	{
		Entity* en = m_dualERData->getEntity(i);
		assert(en->getIndex() == i);

		int listindex_x = vertex_index_x_to_list_index(i, m_dualERData->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(i, m_dualERData->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1)
			continue;

		listindex_x = listindex_x + pNV + pNM;
		listindex_y = listindex_y + pNV + pNM;

		x[listindex_x] = en->getLocation().x;
		x[listindex_y] = en->getLocation().y;
	}
	for (int i = 0; i < dMonogons.size(); i++)
	{
		Relationship* re = dMonogons.at(i);
		int listindex = monogon_index_to_list_index(i, dNV , m_dualERData->getSortedFixedMonoIndices());

		if (listindex == -1)
			continue;
		
		listindex = listindex + pNV + pNM;

		x[listindex] = re->getMonogonRotation();
	}

	int M = gmainwindow->HLBFGS_UpdateNum;
	double parameter[20];
	int info[20];
	init_HLBFGS_Settings(parameter, info);
	int maxIterationNum = gmainwindow->ITERMAX;
	std::vector<std::pair<int, int>> swaps;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	while (true)
	{
		info[4] = maxIterationNum - HLBFGSIterations;

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		HLBFGS((pNV+pNM+dNV+dNM), M, x, this->dual_subgraph_energy_function, 0, HLBFGS_UPDATE_Hessian, this->dual_HLBFGS_subgraph_iteration, parameter, info);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addHLBFGSEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addHLBFGSEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		HLBFGSIterations += info[2];
		HLBFGSEvaluations++;
		if (gmainwindow->flag_stopOptimization)
		{
			LogConsole::writeConsole("Optimization stopped");
			break;
		}
		if (HLBFGSIterations >= maxIterationNum)
		{
			LogConsole::writeConsole("Exceed Iter Max %d", maxIterationNum);
			break;
		}

		int swapNum = 0;
		if (gmainwindow->flag_optimizeWithReordering && pNV > 4 || dNV > 4)
		{

////#ifdef USE_OPENMP
////			time1 = omp_get_wtime();
////#else
////			time1 = clock();
////#endif
//
//			OptimizationOrder optsolver;
//			swapNum = optsolver.dual_subgraph_reordering(m_ERData, m_dualERData, pNV, pNM, dNV, dNM, x, swaps);
//
////#ifdef USE_OPENMP
////			double time2 = omp_get_wtime();
////			gmainwindow->addSwapEvalTime(time2 - time1);
////#else
////			clock_t time2 = clock();
////			gmainwindow->addSwapEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
////#endif
//
//			HLBFGS_subgraph_iteration(++HLBFGSIterations, 0, x, NULL, NULL, NULL, NULL);
		}

		if (swapNum == 0)
			break;
	}

	m_ERData->updateAllFreeLocations(pNV, pNM, x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();

	m_dualERData->updateAllFreeLocations(dNV, dNM, x, pNV + pNM);
	m_dualERData->starrizeRelationships();
	m_dualERData->create2DScaffolds();

	// LogConsole::writeConsole("----Energy After Optimization (HLBFGS evaluated %d times, %d total iterations)", HLBFGSEvaluations, HLBFGSIterations);
	// showEnergyInfo(OptimizationHLBFGS::m_energy);
	

	delete[] x;
	m_ERData = NULL;
	m_dualERData = NULL;
}

void OptimizationHLBFGS::dual_subgraph_energy_function(int N, double* x, double* prev_x, double* f, double* g)
{
	if (gmainwindow->flag_stopOptimization)
		return;

	int pNV = m_ERData->getFreeVertCompNum();
	int pNM = m_ERData->getFreeMonoCompNum();
	int dNV = m_dualERData->getFreeVertCompNum();
	int dNM = m_dualERData->getFreeMonoCompNum();
	assert((pNV + pNM + dNV + dNM) == N);

	OptimizationHLBFGS::m_energy = dual_evaluate_subgraph_energy(m_ERData, m_dualERData, pNV, pNM, dNV, dNM, x, g);
	*f = m_energy.total;

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationHLBFGS::m_energy,true);
}

bool OptimizationHLBFGS::dual_HLBFGS_subgraph_iteration(int iter, int call_iter, double* x, double* f, double* g, double* gnorm, double* searchdir)
{
	// if (flag_showOptimizationInfo && f && gnorm)
	// 	LogConsole::writeConsole("iter:%d:%d\nenergy:%.8f\nnormOfGradient:%.8f", iter, call_iter, *f, *gnorm);

	if (gmainwindow->flag_animateOptimization)
	{
		int pNV = m_ERData->getFreeVertCompNum();
		int pNM = m_ERData->getFreeMonoCompNum();
		int dNV = m_dualERData->getFreeVertCompNum();
		int dNM = m_dualERData->getFreeMonoCompNum();
		
		m_ERData->updateAllFreeLocations(pNV, pNM, x);
		m_ERData->starrizeRelationships();
		m_ERData->create2DScaffolds();

		m_dualERData->updateAllFreeLocations(dNV, dNM, x, pNV + pNM);
		m_dualERData->starrizeRelationships();
		m_dualERData->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
		{

			gmainwindow->reset_sceneToContent(gmainwindow->primeVis);
			gmainwindow->reset_sceneToContent(gmainwindow->dualVis);
		}

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}




adept::adouble OptimizationHLBFGS::calcMonogonSeparationEnergy(ERData * erdata, const int N, const adept::adouble * rotations)
{
	using adept::adouble;
	adouble y = 0;

	vector<pair<adouble, adouble>> centers;

	double b = Element::m_MonogonSize;
	double a = Element::m_MonogonLength;

	/*get polygon centers*/
	for (int j = 0; j < erdata->getRelationshipNum(); j++) {
		Relationship *r = erdata->getRelationship(j);

		vector<int> entityindices = r->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;

		if (entityindices.size() == 1) {
			//use observer of mongon as center

			int re1MonogonIndex = erdata->getMonogonIndex(r);
			adouble inclinedAngle = rotations[re1MonogonIndex];

			adouble x0 = r->getIncidentEntity(0)->getLocation().x;
			adouble y0 = r->getIncidentEntity(0)->getLocation().y;

			cx = x0 + a*cos(inclinedAngle);
			cy = y0 + a*sin(inclinedAngle);

		}
		else {
			for (int i = 0; i < entityindices.size(); i++) {
				int indexXi = 2 * entityindices[i];
				int indexYi = 2 * entityindices[i] + 1;

				adouble vindexXi = r->getIncidentEntity(i)->getLocation().x;
				adouble vindexYi = r->getIncidentEntity(i)->getLocation().y;

				cx += vindexXi;
				cy += vindexYi;
			}

			cx /= num;
			cy /= num;
		}
		centers.push_back(pair<adouble, adouble>(cx, cy));
	}

	/*accumulate the separation energies among monogon and its adjacent polygons*/
	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		Relationship *re1 = erdata->getRelationship(i);

		if (re1->getCardinality() != 1)
			continue;

		for (int j = 0; j < re1->getAdjacentRelationships().size(); j++)
		{
			Relationship *adj = re1->getAdjacentRelationship(j);

			if (re1 == adj)
				continue;

			int adj_index = adj->getIndex();

			adouble dir_x = centers[adj_index].first - centers[i].first;
			adouble dir_y = centers[adj_index].second - centers[i].second;
			adouble scale = 1.0;

			if (adj->getCardinality() == 1)
				scale = 0.1;

			adouble dist_sq = (dir_x*dir_x + dir_y*dir_y);
			y += scale / dist_sq;
		}
	}

	return y;
}

adept::adouble OptimizationHLBFGS::calcMonogonSeparationDualEnergy(ERData * erdata, const int N, const adept::adouble * rotations)
{
	using adept::adouble;
	adouble y = 0;

	vector<pair<adouble, adouble>> centers;

	double b = Element::m_MonogonSize;
	double a = Element::m_MonogonLength;

	/*get polygon centers*/
	for (int j = 0; j < erdata->getRelationshipNum(); j++) {
		Relationship *r = erdata->getRelationship(j);

		vector<int> entityindices = r->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;

		if (entityindices.size() == 1) {
			
            //use observer of mongon as center
			int re1MonogonIndex = erdata->getMonogonIndex(r);
			adouble inclinedAngle = rotations[re1MonogonIndex];

			adouble x0 = r->getIncidentEntity(0)->getLocation().x;
			adouble y0 = r->getIncidentEntity(0)->getLocation().y;

			cx = x0 + a*cos(inclinedAngle);
			cy = y0 + a*sin(inclinedAngle);
		}
		else {
			for (int i = 0; i < entityindices.size(); i++) {
				int indexXi = 2 * entityindices[i];
				int indexYi = 2 * entityindices[i] + 1;

				adouble vindexXi = r->getIncidentEntity(i)->getLocation().x;
				adouble vindexYi = r->getIncidentEntity(i)->getLocation().y;

				cx += vindexXi;
				cy += vindexYi;
			}

			cx /= num;
			cy /= num;
		}
		centers.push_back(pair<adouble, adouble>(cx, cy));
	}

	/*minimiz the distance between monogons and its dual node*/
	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		Relationship *re1 = erdata->getRelationship(i);

		if (re1->getCardinality() != 1)
			continue;

		Entity *dualEn = re1->getDualEntity();
		double dualx = dualEn->getLocation().x;
		double dualy = dualEn->getLocation().y;

		adouble dir_x = centers[i].first - dualx;
		adouble dir_y = centers[i].second - dualy;

		adouble dist_sq = (dir_x*dir_x + dir_y*dir_y);

		y += dist_sq;
	}

	return y;
}

// energy evaluation outside of optimization
void OptimizationHLBFGS::evaluate_layout_energy(ERData* mdata, bool polyReg, bool sideLen, bool polySep, bool interReg, bool overArea, bool overCount)
{
	if (mdata == NULL)
		return;
	m_ERData = mdata;

	int NV = m_ERData->getEntityNum() * 2;
	REList monogons = m_ERData->getMonogons();
	int NM = monogons.size();
	if (NV == 0)
	{
		LogConsole::showError("No data to evaluate");
		return;
	}

	using adept::adouble;
	adept::Stack stack;
	adouble* x = new adouble[NV + NM];
	for (int i = 0; i < m_ERData->getEntityNum(); i++)
	{
		x[i * 2] = m_ERData->getEntity(i)->getLocation().x;
		x[i * 2 + 1] = m_ERData->getEntity(i)->getLocation().y;
	}
	for (int i = 0; i < monogons.size(); i++)
	{
		x[NV + i] = monogons[i]->getMonogonRotation();
	}

	
	adouble PR = 0, SL = 0, PS = 0, IR = 0, OA = 0, OC = 0;
	if (polyReg)
	{
		PR += Evaluate_PolygonRegularity(m_ERData, NV, x);
		LogConsole::writeConsole("Polygon Regularity Energy: %.4e", PR.value());
		std::cout << "," << PR.value();
	}
	if (sideLen)
	{
		SL += Evaluate_PolygonSideLength(m_ERData, NV, x);
		LogConsole::writeConsole("Side Length Energy: %.4e", SL.value());
		std::cout << "," << SL.value();
	}
	if (polySep)
	{
		PS += Evaluate_PolygonSeparation(m_ERData, NV, x);
		LogConsole::writeConsole("Polygon Separation Energy: %.4e", PS.value());
		std::cout << "," << PS.value();
	}
	if (interReg)
	{
		IR += Evaluate_IntersectionRegularity(m_ERData, NV, x);
		LogConsole::writeConsole("Intersection Regularity Energy: %.4e", IR.value());
		std::cout << "," << IR.value();
	}
	if (overArea)
	{
		OA += Evaluate_PolygonOverlap(m_ERData, NV, NM, x, OVERLAPTYPE::AREA);
		LogConsole::writeConsole("Total Overlap Area: %.4e", OA.value());
		std::cout << "," << OA.value();
	}
	if (overCount)
	{
		OC += Evaluate_PolygonOverlap(m_ERData, NV, NM, x, OVERLAPTYPE::COUNT);
		LogConsole::writeConsole("Pairwise Overlap Count: %.4e", OC.value());
		std::cout << "," << OC.value();
	}

	//LogConsole::writeConsole("%.4e,%.4e,%.4e,%.4e,%.4e,%.4e,%.4e", PR.value(), SL.value(), PS.value(), IR.value(), OA.value(), OC.value(), gmainwindow->expectedLength);
}