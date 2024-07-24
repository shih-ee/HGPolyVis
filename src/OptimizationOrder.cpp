#pragma once

#include "OptimizationOrder.h"
#include "LogConsole.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <algorithm>
#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>

ERData* OptimizationOrder::m_ERData = NULL;
ERData* OptimizationOrder::m_dualERData = NULL;
LayoutEnergy<double>  OptimizationOrder::m_energy;
vector<IterLayoutEnergy<double>> OptimizationOrder::m_optEnergyRecords;

double OptimizationOrder::evaluate_Topology_energy(ERData * erdata)
{
	if (erdata == NULL)
		return 0.0;

	m_ERData = erdata;

	int N = erdata->getFreeVertCompNum();;
	double* g = new double[N];
	double* x = new double[N];
	int evaltime = 0;

	for (int i = 0; i < erdata->getEntityNum(); i++) {
		assert(erdata->getEntity(i)->getIndex() == i);

		int vi = erdata->getEntity(i)->getIndex();
		int listindex_x = vertex_index_x_to_list_index(vi, erdata->getSortedFixedCompIndices());
		int listindex_y = vertex_index_y_to_list_index(vi, erdata->getSortedFixedCompIndices());

		if (listindex_x == -1 || listindex_y == -1) {
			continue;
		}

		x[listindex_x] = erdata->getEntity(i)->getLocation().x;
		x[listindex_y] = erdata->getEntity(i)->getLocation().y;
	}

	evaluate_ordering_energy_polygons(erdata, N, x);

	delete[] g;
	delete[] x;

	return OptimizationOrder::m_energy.TO;
}

double OptimizationOrder::evaluate_ordering_energy_polygons(ERData * erdata, int N, const double * x)
{
	OptimizationOrder::m_energy = evaluate_polygon_energy(erdata, N, x, NULL);

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationOrder::m_energy,true);

	return m_energy.total;
}

double OptimizationOrder::dual_evaluate_ordering_energy_polygons(ERData * erdata, ERData * dualerdata, int N1, int N2, const double * x)
{
	OptimizationOrder::m_energy = dual_evaluate_polygon_energy(erdata, dualerdata, N1, N2, x, NULL);

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationOrder::m_energy,true);

	return m_energy.total;
}

double OptimizationOrder::evaluate_ordering_energy(ERData* erdata, int NV, int NM, const double* x)
{
	OptimizationOrder::m_energy = evaluate_energy(erdata, NV, NM, x, NULL);

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationOrder::m_energy,true);

	return m_energy.total;
}

double OptimizationOrder::dual_evaluate_ordering_energy(ERData* erdata, ERData* dualerdata, int pNV, int pNM, int dNV, int dNM, const double* x)
{
	OptimizationOrder::m_energy = dual_evaluate_energy(erdata, dualerdata, pNV, pNM, dNV, dNM, x, NULL);

	if (gmainwindow->flag_showOptimizationInfo)
		showEnergyInfo(OptimizationOrder::m_energy,true);
	
	return m_energy.total;
}

bool OptimizationOrder::iteration(ERData *erdata, int iter, int acceptIter, double *x, double f)
{
	if (gmainwindow->flag_showOptimizationInfo)
		LogConsole::writeConsole("iter:%d accept(%d)\nenergy:%.8f", iter, acceptIter, f);

	if (gmainwindow->flag_animateOptimization) {

		QCoreApplication::processEvents();
		
		int N = erdata->getFreeVertCompNum();
		erdata->updateFreeEntityLocs(N, x);
		erdata->starrizeRelationships();
		erdata->create2DScaffolds();

		if (gmainwindow->flag_autoAdjustViewpoint)
			gmainwindow->reset_sceneToContent();

		gmainwindow->refreshOpenGLWindow();
	}

	if (gmainwindow->flag_stopOptimization)
		return false;
	else
		return true;
}

bool OptimizationOrder::dualiteration(ERData * erdata, ERData * dualERData, int N1, int N2, double * xprime, double * xdual, int iter, int acceptIter, double f)
{
	if (gmainwindow->flag_showOptimizationInfo)
		LogConsole::writeConsole("iter:%d accept(%d)\nenergy:%.8f", iter, acceptIter, f);

	if (gmainwindow->flag_animateOptimization) {

		QCoreApplication::processEvents();

		int N = erdata->getFreeVertCompNum();
		erdata->updateFreeEntityLocs(N1, xprime);
		erdata->starrizeRelationships();
		erdata->create2DScaffolds();

		dualERData->updateEntitiesLocation(N2, xdual);
		dualERData->starrizeRelationships();
		dualERData->create2DScaffolds();

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


int OptimizationOrder::reordering_polygons(ERData * mdata)
{
	if (mdata == NULL)
		return 0;

	m_ERData = mdata;

	if (m_ERData->getEntities().size() == 0) {
		LogConsole::showError("No data to optimize");
		return 0;
	}

	int N = m_ERData->getFreeVertCompNum();;
	double* g = new double[N];
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

	int acceptedcount = reordering_polygons(m_ERData, N, x, true);
	LogConsole::writeConsole("Swap %d pairs", acceptedcount);

	m_ERData->updateFreeEntityLocs(N, x);
	m_ERData->starrizeRelationships();
	m_ERData->create2DScaffolds();

	delete[] x;
	delete[] g;
	m_ERData = NULL;

	return acceptedcount;
}

int OptimizationOrder::reordering_polygons(ERData * mdata, int N, double * x, bool update)
{
	int swapSum = 0;
	int iternum = 0;

	while (true) {

		/*improve the ordering for each polygon*/
		double energy = evaluate_ordering_energy_polygons(mdata, N, x);
		int acceptedcount = 0;
		for (int i = 0; i < mdata->getRelationshipNum(); i++)
		{
			RE *re = mdata->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(mdata, re, N, x);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = mdata->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = mdata->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(mdata, index_m, index_n, N, x, newlayout);
					double newenergy = evaluate_ordering_energy_polygons(mdata, N, newlayout);

					if (energy - newenergy > 1e-8)
					{
						for (int ii = 0; ii < N; ii++) x[ii] = newlayout[ii];
						acceptedcount++;

                        LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							iteration(mdata, iternum, acceptedcount, x, newenergy);

						energy = newenergy;
					}

					iternum++;
					delete[] newlayout;

					if (gmainwindow->flag_stopOptimization)
						break;

				}

				if (gmainwindow->flag_stopOptimization)
					break;

			}

			if (gmainwindow->flag_stopOptimization)
				break;

		}

		evaluate_ordering_energy_polygons(mdata, N, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;

		if (gmainwindow->flag_stopOptimization)
			break;
	}

	return swapSum;
}

int OptimizationOrder::dual_reordering_polygons(ERData * mdata, ERData * dualERdata)
{
	if (mdata == NULL)
		return 0;

	m_ERData = mdata;
	m_dualERData = dualERdata;

	if (m_ERData->getEntities().size() == 0) {
		LogConsole::showError("No data to optimize");
		return 0;
	}

	int N1 = m_ERData->getFreeVertCompNum();
	int N2 = m_dualERData->getEntities().size() * 2;

	if (N1 == 0) {
		LogConsole::showError("No data to optimize");
		return 0;
	}

	if (N2 == 0) {
		LogConsole::showError("No Dual Data to optimize");
		return 0;
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

	int acceptedcount = dual_reordering_polygons(m_ERData, m_dualERData, N1, N2, x);
	LogConsole::writeConsole("Swap %d pairs", acceptedcount);

	//update prime ERData
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

	delete[] x;
	m_ERData = NULL;
	m_dualERData = NULL;

	return acceptedcount;
}

int OptimizationOrder::dual_reordering_polygons(ERData * mdata, ERData * dualERdata, int N1, int N2, double * x, bool update)
{
	int swapSum = 0;
	int iternum = 0;

	double *x_input = new double[N1];
	for (int i = 0; i < N1; i++) {
		x_input[i] = x[i];						// Initialize active input variables
		if (std::isnan(x[i]))
		{
			{
				LogConsole::showError("[NOTE]nan x value");
				return 0;
			}

		}
	}

	double *x_input_dual = new double[N2];
	for (int i = 0; i < N2; i++) {
		x_input_dual[i] = x[N1 + i];						// Initialize active input variables
		if (std::isnan(x[N1 + i]))
		{
			{
				LogConsole::showError("[NOTE]nan x value");
				return 0;
			}
		}
	}

	auto updateVariableList = [](double *target, int N1, int N2, double *prim, double *dual) {
		for (int i = 0; i < N1; i++) {
			target[i] = prim[i];						// Initialize active input variables
		}
		for (int i = 0; i < N2; i++) {
			target[N1 + i] = dual[i];						// Initialize active input variables
		}
	};

	while (true) {
		/*improve the ordering for each polygon*/
		double energy = dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, x);

		int acceptedcount = 0;
		for (int i = 0; i < mdata->getRelationshipNum(); i++)
		{
			RE *re = mdata->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(mdata, re, N1, x_input);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = mdata->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = mdata->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(mdata, index_m, index_n, N1, x_input, newlayout);
					double* tempLayout = new double[N1 + N2];
					updateVariableList(tempLayout, N1, N2, newlayout, x_input_dual);

					double newenergy = dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, tempLayout);

					if (energy - newenergy > 1e-8)
					{
						for (int ii = 0; ii < N1+N2; ii++) x[ii] = tempLayout[ii];
						for (int ii = 0; ii < N1; ii++) x_input[ii] = newlayout[ii];

						acceptedcount++;

						if (gmainwindow->flag_showOptimizationInfo)
							LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							dualiteration(mdata, dualERdata, N1, N2, x_input, x_input_dual, iternum, acceptedcount, newenergy);

						energy = newenergy;
					}

					iternum++;
					delete[] newlayout;
					delete[] tempLayout;

					if (gmainwindow->flag_stopOptimization)
						break;

				}

				if (gmainwindow->flag_stopOptimization)
					break;
			}

			if (gmainwindow->flag_stopOptimization)
				break;

		}

		dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;

		if (gmainwindow->flag_stopOptimization)
			break;
	}

	/*sort dual*/
	while (true) {
		/*improve the ordering for each polygon*/
		double energy = dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, x);
		
		int acceptedcount = 0;
		for (int i = 0; i < dualERdata->getRelationshipNum(); i++)
		{
			RE *re = dualERdata->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(dualERdata, re, N2, x_input_dual);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, dualERdata->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, dualERdata->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = dualERdata->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, dualERdata->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, dualERdata->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = dualERdata->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(dualERdata, index_m, index_n, N2, x_input_dual, newlayout);
					double* tempLayout = new double[N1 + N2];
					updateVariableList(tempLayout, N1, N2, x_input, newlayout);

					double newenergy = dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, tempLayout);

					if (newenergy < energy)
					{
						for (int ii = 0; ii < N1 + N2; ii++) x[ii] = tempLayout[ii];
						for (int ii = 0; ii < N2; ii++) x_input_dual[ii] = newlayout[ii];

						acceptedcount++;

						if (gmainwindow->flag_showOptimizationInfo)
							LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							dualiteration(mdata, dualERdata, N1, N2, x_input, x_input_dual, iternum, acceptedcount, newenergy);

						energy = newenergy;
					}

					delete[] newlayout;
					delete[] tempLayout;
					iternum++;
				}
			}
		}

		dual_evaluate_ordering_energy_polygons(mdata, dualERdata, N1, N2, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;
	}

	delete[] x_input;
	delete[] x_input_dual;
	return swapSum;
}


int OptimizationOrder::reordering(ERData* mdata, int NV, int NM, double* x, bool update)
{
	int swapSum = 0;
	int iternum = 0;

	while (true) {

		double energy;
		energy = evaluate_ordering_energy(mdata, NV, NM, x);

		int acceptedcount = 0;
		for (int i = 0; i < mdata->getRelationshipNum(); i++)
		{
			RE* re = mdata->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(mdata, re, NV, x);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = mdata->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = mdata->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(mdata, index_m, index_n, NV, x, newlayout);
					double newenergy;
					newenergy = evaluate_ordering_energy(mdata, NV, NM, x);

					if (energy - newenergy > 1e-8)
					{
						for (int ii = 0; ii < NV; ii++) x[ii] = newlayout[ii];
						acceptedcount++;

						LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							iteration(mdata, iternum, acceptedcount, x, newenergy);

						energy = newenergy;
					}

					iternum++;
					delete[] newlayout;

					if (gmainwindow->flag_stopOptimization)
						break;

				}

				if (gmainwindow->flag_stopOptimization)
					break;

			}

			if (gmainwindow->flag_stopOptimization)
				break;

		}

		//evaluate_ordering_energy(mdata, NV, NM, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;

		if (gmainwindow->flag_stopOptimization)
			break;
	}

	return swapSum;
}

int OptimizationOrder::dual_reordering(ERData* mdata, ERData* dualERData, int pNV, int pNM, int dNV, int dNM, double* x, bool update)
{
	int swapSum = 0;
	int iternum = 0;

	double* x_input = new double[pNV+pNM];
	for (int i = 0; i < pNV+pNM; i++) {
		x_input[i] = x[i];						// Initialize active input variables
		if (std::isnan(x[i]))
		{
				LogConsole::showError("[NOTE]nan x value");
				return 0;
		}
	}

	double* x_input_dual = new double[dNV+dNM];
	for (int i = 0; i < dNV+dNM; i++) {
		x_input_dual[i] = x[pNV + pNM + i];		// Initialize active input variables
		if (std::isnan(x[pNV + pNM + i]))
		{
				LogConsole::showError("[NOTE]nan x value");
				return 0;
		}
	}

	auto updateVariableList = [](double* target, int N1, int N2, double* prim, double* dual) {
		for (int i = 0; i < N1; i++) {
			target[i] = prim[i];						// Initialize active input variables
		}
		for (int i = 0; i < N2; i++) {
			target[N1 + i] = dual[i];						// Initialize active input variables
		}
	};

	while (true)
	{
		
		double energy;
		energy = dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);
		
		int acceptedcount = 0;
		for (int i = 0; i < mdata->getRelationshipNum(); i++)
		{
			RE* re = mdata->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(mdata, re, pNV, x_input);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, mdata->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = mdata->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, mdata->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = mdata->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(mdata, index_m, index_n, pNV, x_input, newlayout);
					double* tempLayout = new double[pNV + pNM + dNV + dNM];
					updateVariableList(tempLayout, pNV + pNM, dNV + dNM, newlayout, x_input_dual);

					double newenergy;
					newenergy = dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);

					if (energy - newenergy > 1e-8)
					{
						for (int ii = 0; ii < pNV + pNM + dNV + dNM; ii++) x[ii] = tempLayout[ii];
						for (int ii = 0; ii < pNV + pNM; ii++) x_input[ii] = newlayout[ii];

						acceptedcount++;

						if (gmainwindow->flag_showOptimizationInfo)
							LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							dualiteration(mdata, dualERData, pNV, pNM, x_input, x_input_dual, iternum, acceptedcount, newenergy);

						energy = newenergy;
					}
					
					iternum++;
					delete[] newlayout;
					delete[] tempLayout;

					if (gmainwindow->flag_stopOptimization)
						break;

				}

				if (gmainwindow->flag_stopOptimization)
					break;
			}

			if (gmainwindow->flag_stopOptimization)
				break;
		}

		//dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;

		if (gmainwindow->flag_stopOptimization)
			break;
	}



	while (true) {

		double energy;
		energy = dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);

		int acceptedcount = 0;
		for (int i = 0; i < dualERData->getRelationshipNum(); i++)
		{
			RE* re = dualERData->getRelationship(i);
			int card = re->getCardinality();

			vector<int> vindices_i = getRelationshipVertIndices<double>(dualERData, re, dNV, x_input_dual);

			if (vindices_i.size() == 1)
				continue;

			for (int m = 0; m < card; m++)
			{
				int index_m = vindices_i[m];

				int listindex_x = vertex_index_x_to_list_index(index_m, dualERData->getSortedFixedCompIndices());
				int listindex_y = vertex_index_y_to_list_index(index_m, dualERData->getSortedFixedCompIndices());
				if (listindex_x == -1 || listindex_y == -1) {
					continue;
				}

				int degree_m = dualERData->getEntityDegree(index_m);

				for (int n = m + 1; n < card; n++) {

					int index_n = vindices_i[n];

					int listindex_nx = vertex_index_x_to_list_index(index_n, dualERData->getSortedFixedCompIndices());
					int listindex_ny = vertex_index_y_to_list_index(index_n, dualERData->getSortedFixedCompIndices());
					if (listindex_nx == -1 || listindex_ny == -1) {
						continue;
					}

					int degree_n = dualERData->getEntityDegree(index_n);

					if (degree_m == 1 && degree_n == 1)
						continue;

					double* newlayout;
					swapVerts(dualERData, index_m, index_n, dNV, x_input_dual, newlayout);
					double* tempLayout = new double[pNV + pNM + dNV + dNM];
					updateVariableList(tempLayout, pNV + pNM, dNV + dNM, x_input, newlayout);

					double newenergy;
					newenergy = dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);

					if (energy - newenergy > 1e-8)
					{
						for (int ii = 0; ii < pNV + pNM + dNV + dNM; ii++) x[ii] = tempLayout[ii];
						for (int ii = 0; ii < dNV + dNM; ii++) x_input_dual[ii] = newlayout[ii];

						acceptedcount++;

						if (gmainwindow->flag_showOptimizationInfo)
							LogConsole::writeConsole("swap %d, %d", index_m, index_n);

						if (update)
							dualiteration(mdata, dualERData, pNV, pNM, x_input, x_input_dual, iternum, acceptedcount, newenergy);

						energy = newenergy;
					}

					iternum++;
					delete[] newlayout;
					delete[] tempLayout;
				}
			}
		}

		//dual_evaluate_ordering_energy(mdata, dualERData, pNV, pNM, dNV, dNM, x);

		swapSum += acceptedcount;
		if (acceptedcount == 0)
			break;
	}

	delete[] x_input;
	delete[] x_input_dual;
	return swapSum;
}


int OptimizationOrder::subgraph_reordering(ERData* mdata, int NV, int NM, double* x, std::vector<std::pair<int, int>> &swaps)
{
	double energy = evaluate_subgraph_ordering_energy(mdata, NV, NM, x);
	const vector<int> fixedCompIndices = mdata->getSortedFixedCompIndices();
	std::vector<std::pair<int, int>> new_swaps;
	for (int i = 0; i < NV; i+=2)
	{
		int idxi;
		list_index_to_vertex_index(i, fixedCompIndices, idxi);
		const Entity* eni = mdata->getConstEntity(idxi);

		for (int j = i + 2; j < NV; j += 2)
		{
			int idxj;
			list_index_to_vertex_index(j, fixedCompIndices, idxj);
			const Entity* enj = mdata->getConstEntity(idxj);

			if (mdata->areEntitiesAdjacent(eni, enj))
			{
				// check if swap was done last time
				bool repeat = false;
				for (pair<int, int> swap : swaps)
				{
					if ((eni->getOpidx() == swap.first && enj->getOpidx() == swap.second) ||
						(enj->getOpidx() == swap.first && eni->getOpidx() == swap.second))
						repeat = true;
				}

				if (!repeat)
				{
					// try the swap
					swapVertsInPlace(i, j, x);
					double new_energy = evaluate_subgraph_ordering_energy(mdata, NV, NM, x);

					if (energy - new_energy > 1.0e-8)
					{
						new_swaps.push_back(std::pair<int,int>(eni->getOpidx(), enj->getOpidx()));
						energy = new_energy;
					}
					else
					{
						swapVertsInPlace(i, j, x);
					}
				}
			}
		}
	}

	swaps.clear();
	swaps = new_swaps;
	return swaps.size();
}

double OptimizationOrder::evaluate_subgraph_ordering_energy(ERData* erdata, int NV, int NM, const double* x)
{
	OptimizationOrder::m_energy = evaluate_subgraph_energy(erdata, NV, NM, x, NULL);
	return m_energy.total;
}

void OptimizationOrder::swapVerts(ERData * erdata, int vi, int vj, int N, const double * layout, double* newlayout)
{
	newlayout = new double[N];
	for (int ii = 0; ii < N; ii++) newlayout[ii] = layout[ii];

	int indexXi = 2 * vi;
	int li_x = vertex_comp_index_to_list_index(indexXi, erdata->getSortedFixedCompIndices());
	int indexYi = 2 * vi + 1;
	int li_y = vertex_comp_index_to_list_index(indexYi, erdata->getSortedFixedCompIndices());


	int indexXj = 2 * vj;
	int lj_x = vertex_comp_index_to_list_index(indexXj, erdata->getSortedFixedCompIndices());
	int indexYj = 2 * vj + 1;
	int lj_y = vertex_comp_index_to_list_index(indexYj, erdata->getSortedFixedCompIndices());

	newlayout[li_x] = layout[lj_x];
	newlayout[lj_x] = layout[li_x];
	newlayout[li_y] = layout[lj_y];
	newlayout[lj_y] = layout[li_y];
}

Entity * OptimizationOrder::getEntity(ERData *erdata, int N, const double * x, Relationship * re, int ith_incident)
{
	vector<int> entityindices = getRelationshipVertIndices<double>(erdata, re, N, x);
	int index = entityindices[ith_incident];
	Entity*en = erdata->getEntity(index);
	return en;
}

void OptimizationOrder::swapVertsInPlace(int i, int j, double* x)
{
	double temp_x = x[j];
	double temp_y = x[j + 1];
	x[j] = x[i];
	x[j + 1] = x[i + 1];
	x[i] = temp_x;
	x[i + 1] = temp_y;
}