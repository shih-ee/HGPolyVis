#pragma once

#include "ERData.h"
#include "Energy.h"
#include <adept.h>
#include <string>
#include <omp.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


enum ENERGYTYPE {
	SUBTRACTION,
	DIVISION,
	LOG
};

enum OVERLAPTYPE {
	COUNT,
	AREA
};

// utilities

bool list_index_to_vertex_index(int xi, const vector<int>&fixedCompIndices, int &vi);
int vertex_index_x_to_list_index(int vi, const vector<int>&fixedCompIndices);
int vertex_index_y_to_list_index(int vi, const vector<int>&fixedCompIndices);
int vertex_comp_index_to_list_index(int vci, vector<int>fixedCompIndices);
int list_index_to_vertex_comp_index(int xi, vector<int>fixedCompIndices);
int monogon_index_to_list_index(int mi, int nv, const vector<int>& fixedCompIndices);
int monogon_comp_index_to_list_index(int mci, vector<int> fixedCompIndices);
double calc_expected_L(const ERData *erdata, int re_i);
double calc_expected_L(int card, int degree, double min_degree, double max_degree);
vector<int> getCommonIndices(const vector<int> &s1, const vector<int> &s2);
vector<int> getGapSeries(const vector<int> &sub, const vector<int> &sup);
typedef vector<int> GapVerts;
vector<GapVerts> getGapVertSeries(const vector<int> &sub, const vector<int> &sup);
void showEnergyInfo(LayoutEnergy<double> m_energy, bool simpleFormat = false);

template<class mytype>
mytype get_vi_x_value(int vi, const ERData *erdata, const mytype *x);
template<class mytype>
mytype get_vi_y_value(int vi, const ERData *erdata, const mytype *x);
template<class mytype>
mytype get_mi_rot_value(int mi, int nv, const ERData* erdata, const mytype* x);
template<class mytype>
vector<int> getRelationshipVertIndices(const ERData *erdata, int reIndex, const int N, const mytype * x);
template<class mytype>
vector<int> getRelationshipVertIndices(const ERData *erdata, Relationship *re, const int N, const mytype * x);
vector<Vec3> getRelationshipLayout(const ERData* erdata, const Relationship* re, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, OVERLAPTYPE type);
template<class mytype>
mytype calc_PolygonAspectRatio(const ERData *erdata, const int N, const mytype * x, const vector<int> &entityindices);
adept::adouble test_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, const vector<adept::adouble>& radii, const Relationship* poly1, const Relationship* poly2);
adept::adouble calc_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, const vector<adept::adouble>& radii, const Relationship* poly1, const Relationship* poly2);
double calc_PolygonOverlap(Relationship* poly1, Relationship* poly2);
double calc_PolygonAspectRatio(Relationship* poly);

// energy terms

template<class mytype>
mytype EnergyEval_AspectRatio(const ERData *erdata, const int N, const mytype * x, ENERGYTYPE type = ENERGYTYPE::SUBTRACTION);
template<class mytype>
mytype EnergyEval_AreaDiff(const ERData *erdata, const int N, const mytype * x, ENERGYTYPE type = ENERGYTYPE::SUBTRACTION);
template<class mytype>
mytype EnergyEval_LengthDiff(const ERData *erdata, const int N, const mytype * x);
template<class mytype>
mytype EnergyEval_PolygonSeparation(const ERData *erdata, int N, const mytype * x);
template<class mytype>
mytype EnergyEval_VertexSeparation(const ERData *erdata, const int N, const mytype * x);
template<class mytype>
mytype EnergyEval_PolygonDualDist(ERData *erdata, ERData *dualerdata, const int N1, const mytype * x, const int N2, const mytype * x_dual);
template<class mytype>
mytype EnergyEval_DualDist(ERData* erdata, ERData* dualerdata, const int N1, const mytype* x, const int N2, const mytype* x_dual);
template<class mytype>
mytype EnergyEval_ContinousTopology(ERData *erdata, const int N, const mytype * x);
double evaluate_toplogy_energy(ERData *erdata, int N, const double* x);
template<class mytype>
mytype EnergyEval_IntersectionAspectRatio(ERData *erdata, const int N, const mytype * x);
template<class mytype>
mytype EnergyEval_Separation(const ERData* erdata, const int NV, const int NM, const mytype* x);
template<class mytype>
mytype EnergyEval_Unravel(const ERData* erdata, const int NV, const int NM, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_ContinousTopology(ERData* erdata, const std::vector<int> relist, const int N, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_IntersectionAspectRatio(ERData* erdata, const std::vector<int> relist, const int N, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_AspectRatio(const ERData* erdata, const std::vector<int> relist, const int N, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_LengthDiff(const ERData* erdata, const std::vector<int> relist, const int N, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_Separation_Local(const ERData* erdata, const std::vector<int> relist, const int NV, const int NM, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_Separation_Global(const ERData* erdata, const std::vector<int> relist, const int NV, const int NM, const mytype* x);
template<class mytype>
mytype EnergyEval_Subgraph_DualDist(ERData* pdata, ERData* ddata, const int pNV, const mytype* px, const int dNV, const mytype* dx);

adept::adouble Evaluate_PolygonRegularity(const ERData* erdata, const int NV, const adept::adouble* x);
adept::adouble Evaluate_PolygonSideLength(const ERData* erdata, const int NV, const adept::adouble* x);
adept::adouble Evaluate_PolygonSeparation(const ERData* erdata, const int NV, const adept::adouble* x);
adept::adouble Evaluate_IntersectionRegularity(ERData* erdata, const int NV, const adept::adouble* x);
adept::adouble Evaluate_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, OVERLAPTYPE type);

// Energy Evaluations

LayoutEnergy<double> evaluate_energy(ERData* erdata, int NV, int NM, const double* x, double* g);
LayoutEnergy<double> dual_evaluate_energy(ERData* erdata, ERData* dualerdata, int pNV, int pNM, int dNV, int dNM, const double* x, double* g);
LayoutEnergy<double> evaluate_polygon_energy(ERData *erdata, int N, const double* x, double* g);
LayoutEnergy<double> dual_evaluate_polygon_energy(ERData *m_ERData, ERData *m_dualERData, int N1, int N2, const double* x, double* g);
LayoutEnergy<double> evaluate_subgraph_energy(ERData* erdata, int NV, int NM, const double* x, double* g);
LayoutEnergy<double> dual_evaluate_subgraph_energy(ERData* pdata, ERData* ddata, int pNV, int pNM, int dNV, int dNM, const double* x, double* g);










