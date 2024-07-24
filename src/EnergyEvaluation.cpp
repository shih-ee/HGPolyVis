#include "EnergyEvaluation.h"
#include "mainwindow.h"
#include "LogConsole.h"

string L_SCALE_SCHEME_NAMES[5] = {
	"UNIFORM_L_WITH_CUTOFF_MAXAREA",
	"UNIFORM_L_WITH_CUTOFF_MAXCCR",
	"UNIFORM_CCR",
	"SCALE_BY_CARD",
	"SCALE_BY_DEGREE"
};

// utilities

void showEnergyInfo(LayoutEnergy<double> m_energy, bool simpleFormat)
{
	double wAR = m_energy.wAR;
	double wAD = m_energy.wAD;
	double wOC = m_energy.wOC;
	double wPS = m_energy.wPS;
	double wDD = m_energy.wDD;
	double wNS = m_energy.wNS;
	double wTO = m_energy.wTO;
	
	double y = m_energy.total;

	if (simpleFormat) {
		LogConsole::writeConsole("%4e,%4e,%4e,%4e,%4e",wAR*m_energy.AR,wAD*m_energy.AD,wPS*m_energy.PS,wTO*m_energy.TO,wDD*m_energy.DD);
	}
	else {
		if (wAR > 0) LogConsole::writeConsole("AR:[%f]*[%.8f]=%.8e", wAR, m_energy.AR, wAR*m_energy.AR);
		if (wAD > 0) LogConsole::writeConsole("LD:[%f]*[%.8f]=%.8e", wAD, m_energy.AD, wAD*m_energy.AD);
		if (wPS > 0) LogConsole::writeConsole("PS:[%f]*[%.8f]=%.8e", wPS, m_energy.PS, wPS*m_energy.PS);
		if (wTO > 0) LogConsole::writeConsole("TO:[%f]*[%.8f]=%.8e", wTO, m_energy.TO, wTO*m_energy.TO);
		if (wDD > 0) LogConsole::writeConsole("DD:[%f]*[%.8f]=%.8e", wDD, m_energy.DD, wDD*m_energy.DD);
	}

	//LogConsole::writeConsole("[Energy] %.8e", y);
	//LogConsole::writeConsole("[GeoEny] %.8e", m_energy.geometry);
}

/*return: true (x component), false (y component), vi is the vertex index*/
bool list_index_to_vertex_index(int xi, const vector<int>& fixedCompIndices, int & vi)
{
	if (xi < 0)
	{
		vi = -1;
		return true;
	}

	int vci = list_index_to_vertex_comp_index(xi, fixedCompIndices);

	vi = vci / 2;
	int mod = vci % 2;
	if (mod == 0)
		return true;
	else
		return false;
}

/*vertex vi's x component to list index, return -1 if vi's x is fixed*/
int vertex_index_x_to_list_index(int vi, const vector<int>&fixedCompIndices)
{
	int vci = 2 * vi;

	if (fixedCompIndices.size() == 0)
		return vci;

	int li = vertex_comp_index_to_list_index(vci, fixedCompIndices);
	return li;
}

/*vertex vi's y component to list index, return -1 if vi's y is fixed*/
int vertex_index_y_to_list_index(int vi, const vector<int>&fixedCompIndices)
{
	int vci = 2 * vi + 1;

	if (fixedCompIndices.size() == 0)
		return vci;

	int li = vertex_comp_index_to_list_index(vci, fixedCompIndices);
	return li;
}

/*if vertex index is i, vertex comp index is 2i and 2i+1 for x and y component
expand the 2D array into 1D array list with skipping the fixed vertices, return the element index,
fixed vci return to -1
*/
int vertex_comp_index_to_list_index(int vci, vector<int>fixedCompIndices) {

	if (fixedCompIndices.size() == 0)
		return vci;

	std::sort(fixedCompIndices.begin(), fixedCompIndices.end(), [](int x, int y) { return x < y; });

	if (find(fixedCompIndices.begin(), fixedCompIndices.end(), vci) != fixedCompIndices.end()) {
		return -1;
	}

	int dec = 0;
	for (int i = 0; i < fixedCompIndices.size(); i++) {
		if (vci >= fixedCompIndices[i])
			dec++;
	}

	return vci - dec;
}

/*List:[v[0].x, v[0].y, v[1].x, v[1].y, ... v[i].x, v[i].y] without fixed vertices
given a list element index, return the k = 2i or 2i+1
*/
int list_index_to_vertex_comp_index(int xi, vector<int>fixedCompIndices)
{
	if (fixedCompIndices.size() == 0)
		return xi;

	std::sort(fixedCompIndices.begin(), fixedCompIndices.end(), [](int x, int y) { return x < y; });

	int inc = 0;
	for (int i = 0; i < fixedCompIndices.size(); i++) {
		if (xi + inc >= fixedCompIndices[i])
			inc++;
	}

	return xi + inc;
}

// monogon mi's rotation component in the free component list, return -1 if it is fixed
int monogon_index_to_list_index(int mi, int nv, const vector<int>& fixedCompIndices)
{
	int mci = nv + mi;
	if (fixedCompIndices.size() == 0)
		return mci;
	else
		return monogon_comp_index_to_list_index(mci, fixedCompIndices);
}

int monogon_comp_index_to_list_index(int mci, vector<int> fixedCompIndices)
{
	if (fixedCompIndices.size() == 0)
		return mci;

	
	if (find(fixedCompIndices.begin(), fixedCompIndices.end(), mci) != fixedCompIndices.end())
		return -1;

	int dec = 0;
	for (int i = 0; i < fixedCompIndices.size(); i++)
	{
		if (mci >= fixedCompIndices[i])
			dec++;
	}

	return mci - dec;
}

/*get the edge length L based on different scaling scheme*/
double calc_expected_L(const ERData *erdata, int re_i)
{
	int card = erdata->getRelationshipCard(re_i);
	int degree = erdata->getRelationshipDegree(re_i);
	double min_degree = erdata->getMinDegree(ELEMENTTYPE::RELATIONSHIP);
	double max_degree = erdata->getMaxDegree(ELEMENTTYPE::RELATIONSHIP);

	return calc_expected_L(card, degree, min_degree, max_degree);
}

double calc_expected_L(int card, int degree, double min_degree, double max_degree)
{
	double L = gmainwindow->expectedLength;
	int card_max = gmainwindow->PA_max_card;
	double L_scaled = L;

	if (card == 1)
	{
		double b = Element::m_MonogonSize;
		double a = Element::m_MonogonLength;

		return (a + b);
	}
	else if (card == 2) {
		double digonRatio = gmainwindow->AD_Digon_Len_scale;
		L_scaled = digonRatio*L_scaled;
	}
	else if (card > card_max)
	{
		// uniform L, cut off for card bigger than card_max to get uniform area
		double area_max = double(1.0 / 4.0)*card_max*L*L / tan(M_PI / double(card_max));
		double L_s_sqrt = area_max * 4.0*tan(M_PI / double(card)) / card;
		L_scaled = sqrt(L_s_sqrt);
	}

	return L_scaled;
}

vector<int> getCommonIndices(const vector<int>& s1, const vector<int>& s2)
{
	vector<int> common;
	for (int i = 0; i < s1.size(); i++) {
		int index = s1[i];

		auto res = std::find(s2.begin(), s2.end(), index);
		if (res != s2.end()) {
			common.push_back(index);
		}
	}

	return common;
}

/*the sub should be a sub-set of the sup*/
vector<int> getGapSeries(const vector<int>& sub, const vector<int>& sup)
{
	vector<int> gapseries;
	vector<int> order_in_sup;
	/*verify the correctness of the sub and sup*/
	for (int i = 0; i<sub.size(); i++) {
		int index = sub[i];
		auto res = std::find(sup.begin(), sup.end(), index);
		if (res == sup.end()) {
			LogConsole::showError("wrong gap series input");
			return gapseries;
		}

		order_in_sup.push_back(res - sup.begin());
	}

	/*make sub and sup in the same orientation*/
	assert(sub.size() >= 2);

	int card = sup.size();
	int start = order_in_sup[0];
	int ci = 1;
	int gap = 0;
	/*find the gap number*/
	for (int i = 0; i<card; i++)
	{
		start = (start + 1) % card;
		if (start == order_in_sup[ci]) {
			gapseries.push_back(gap);
			gap = 0;
			ci = (ci + 1) % sub.size();
		}
		else {
			gap++;
		}
	}
	assert(gapseries.size() == sub.size());
	return gapseries;
}

template<class mytype>
mytype get_vi_x_value(int vi, const ERData *erdata, const mytype *x) {

	int indexXi = 2 * vi;
	int li_x = vertex_comp_index_to_list_index(indexXi, erdata->getSortedFixedCompIndices());
	mytype vindexXi;
	if (li_x == -1) {
		vindexXi = erdata->getEntityX(vi);
	}
	else {
		vindexXi = x[li_x];
	}

	return vindexXi;
}

template<class mytype>
mytype get_vi_y_value(int vi, const ERData *erdata, const mytype *x) {

	int indexYi = 2 * vi + 1;
	int li_y = vertex_comp_index_to_list_index(indexYi, erdata->getSortedFixedCompIndices());
	mytype vindexYi;
	if (li_y == -1) {
		vindexYi = erdata->getEntityY(vi);
	}
	else {
		vindexYi = x[li_y];
	}

	return vindexYi;
}

template<class mytype>
mytype get_mi_rot_value(int mi, int nv, const ERData* erdata, const mytype* x)
{

	int mli = monogon_index_to_list_index(mi, nv, erdata->getSortedFixedMonoIndices());
	mytype mi_rot;
	if (mli == -1) {
		mi_rot = erdata->getMonogonRotation(mi);
	}
	else{
		mi_rot = x[mli];
	}

	return mi_rot;
}

/*get the relationship layout from x, the layout of the free verts*/
template<class mytype>
vector<int> getRelationshipVertIndices(const ERData *erdata, int reIndex, const int N, const mytype * x){

	if (N != erdata->getFreeVertCompNum()) {
		LogConsole::showError("wrong input x");
		return vector<int>();
	}

	vector<int> entityindices = erdata->getRelationshipVertIndices(reIndex);

	if (entityindices.size() < 3)
		return entityindices;

	vector<Vec3> relayout(entityindices.size());

	for (int i = 0; i < entityindices.size(); i++) {
		int vi = entityindices[i];
		mytype vi_x, vi_y;
		vi_x = get_vi_x_value(vi, erdata, x);
		vi_y = get_vi_y_value(vi, erdata, x);
		relayout[i] = Vec3(vi_x, vi_y, 0);
	}

	return erdata->getEntityIndicesFromIncidentLocs(reIndex, relayout);
}

/*get the relationship layout from x, the layout of the free verts*/
template<class mytype>
vector<int> getRelationshipVertIndices(const ERData *erdata, Relationship *re, const int N, const mytype * x) {

	if (N != erdata->getFreeVertCompNum()) {
		LogConsole::showError("wrong input x");
		return vector<int>();
	}

	vector<int> entityindices = re->getEntityIndices();
	vector<Vec3> relayout(entityindices.size());

	for (int i = 0; i < entityindices.size(); i++) {
		int vi = entityindices[i];
		mytype vi_x, vi_y;
		vi_x = get_vi_x_value(vi, erdata, x);
		vi_y = get_vi_y_value(vi, erdata, x);
		relayout[i] = Vec3(vi_x, vi_y, 0);
	}

	return re->getEntityIndicesFromIncidentLocs(relayout);
}

/*get the relationship layout from x, including monogon angles*/
vector<Vec3> getRelationshipLayout(const ERData* erdata, const Relationship* re, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, OVERLAPTYPE type)
{
	using adept::adouble;

	vector<int> indices = re->getEntityIndices();
	vector<Vec3> layout;
	Vec3 pivot;

	if (indices.size() == 1)
	{
		int vi = indices[0];
		int mi = erdata->getMonogonIndex(re);
		double a = x[mi + NV].value();
		double b = Element::m_MonogonLength;
		double c = Element::m_MonogonSize;
		Vec3 vpos(x[vi * 2], x[vi * 2 + 1], 0);
		Vec3 boff(cos(a), sin(a));
		Vec3 coff(sin(a), -cos(a));
		pivot = vpos + b * boff;

		if (type == OVERLAPTYPE::AREA)
		{
			layout.push_back(vpos);
			layout.push_back(vpos + b * boff + c * coff);
			layout.push_back(vpos + b * boff + c * boff);
			layout.push_back(vpos + b * boff - c * coff);
		}
		else if (type == OVERLAPTYPE::COUNT)
		{
			layout.push_back(vpos);
			layout.push_back(vpos + b * boff);
			return layout;
		}
	}
	else if (indices.size() == 2)
	{
		int vi = indices[0]; int vj = indices[1];
		
		pivot = Vec3(centers[re->getIndex()], 0.0);
		Vec3 vipos(x[vi * 2], x[vi * 2 + 1], 0);
		Vec3 vjpos(x[vj * 2], x[vj * 2 + 1], 0);

		if (type == OVERLAPTYPE::AREA)
		{
			Vec3 vdiff = (vjpos - vipos) * 0.5;
			Vec3 voff(vdiff.y, -vdiff.x, 0); normalize(voff);
			double a = Element::m_DigonLensAngle * M_PI / 360.0;
			double b = length(vdiff);
			double c = tan(a) * b;

			layout.push_back(vipos);
			layout.push_back(vipos + vdiff + c * voff);
			layout.push_back(vjpos);
			layout.push_back(vipos + vdiff - c * voff);
		}
		else if (type == OVERLAPTYPE::COUNT)
		{
			layout.push_back(vipos);
			layout.push_back(vjpos);
			return layout;
		}
	}
	else
	{
		pivot = Vec3(centers[re->getIndex()],0.0);
		for (int vi : indices)
		{
			Vec3 xyai(x[vi * 2], x[vi * 2 + 1], 0);
			layout.push_back(xyai);
		}
	}

	for (Vec3& point : layout)
	{
		Vec3 dir = point - pivot;
		point.z = atan2(dir.y, dir.x);
	}
	
	std::sort(layout.begin(), layout.end(),
			[](Vec3 v1, Vec3 v2) {return v1.z < v2.z;});

	return layout;
}

/*get the relationship layout from x, the layout of the free verts*/
//template<class mytype>
//vector<int> getRelationshipVertIndices(const ERData* erdata, int reIndex) {
//
//	vector<int> entityindices = erdata->getRelationshipVertIndices(reIndex);
//	vector<Vector3> relayout(entityindices.size());
//
//	for (int i = 0; i < entityindices.size(); i++) {
//		int vi = entityindices[i];
//		mytype vi_x, vi_y;
//		vi_x = erdata->getEntityX(vi); // get_vi_x_value(vi, erdata, x);
//		vi_y = erdata->getEntityX(vi); //get_vi_y_value(vi, erdata, x);
//		relayout[i] = Vector3(vi_x, vi_y, 0);
//	}
//
//	return erdata->getEntityIndicesFromIncidentLocs(reIndex, relayout);
//}

/*get the relationship layout from x, the layout of the free verts*/
//template<class mytype>
//vector<int> getRelationshipVertIndices(const ERData* erdata, Relationship* re) {
//
//	vector<int> entityindices = re->getEntityIndices();
//	vector<Vector3> relayout(entityindices.size());
//
//	for (int i = 0; i < entityindices.size(); i++) {
//		int vi = entityindices[i];
//		mytype vi_x, vi_y;
//		vi_x = erdata->getEntityX(vi); // get_vi_x_value(vi, erdata, x);
//		vi_y = erdata->getEntityX(vi); //get_vi_y_value(vi, erdata, x);
//		relayout[i] = Vector3(vi_x, vi_y, 0);
//	}
//
//	return re->getEntityIndicesFromIncidentLocs(relayout);
//}

/*
VertSeq[0] and VertSeq[size - 1] are the end points of two consecutive common verts
VertSeq[1] ... VertSeq[size - 2] are the gap verts of sup
*/
vector<GapVerts> getGapVertSeries(const vector<int>& sub, const vector<int>& sup)
{
	vector<GapVerts> gvs;

	vector<int> gapseries;
	vector<int> order_in_sup;
	/*verify the correctness of the sub and sup*/
	for (int i = 0; i<sub.size(); i++) {
		int index = sub[i];
		auto res = std::find(sup.begin(), sup.end(), index);
		if (res == sup.end()) {
			LogConsole::showError("wrong gap series input");
			return gvs;
		}

		order_in_sup.push_back(res - sup.begin());
	}

	/*make sub and sup in the same orientation*/
	assert(sub.size() >= 2);

	int card = sup.size();
	int supVertID = order_in_sup[0];
	int ci = 1;
	int gap = 0;
	GapVerts gapverts;
	gapverts.push_back(sup[supVertID]);
	/*find the gap number*/
	for (int i = 0; i<card; i++)
	{
		supVertID = (supVertID + 1) % card;
		if (supVertID == order_in_sup[ci]) {
			gapseries.push_back(gap);
			gapverts.push_back(sup[supVertID]);
			gvs.push_back(gapverts);
			assert(gap == gapverts.size() - 2);
			gap = 0;
			gapverts.clear();
			gapverts.push_back(sup[supVertID]);
			ci = (ci + 1) % sub.size();
		}
		else {
			gap++;
			gapverts.push_back(sup[supVertID]);
		}
	}
	assert(gapseries.size() == sub.size());
	assert(gapseries.size() == gvs.size());
	

	return gvs;
}

template<class mytype>
mytype calc_PolygonAspectRatio(const ERData *erdata, const int N, const mytype * x, const vector<int> &entityindices) {

	mytype M_area = 0;
	int num = entityindices.size();
	mytype perimeter = 0;
	//get the perimeter P of the polygon
	for (int i = 0; i < entityindices.size(); i++) {

		int vi = entityindices[i];
		int vi_n = entityindices[(i + 1) % num];

		mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
		mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
		mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
		mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

		mytype ux = vindexXi_n - vindexXi;
		mytype uy = vindexYi_n - vindexYi;

		mytype dist_sq = ux * ux + uy *uy;

		//for gradient calculation
		if (dist_sq < gmainwindow->endiv_threshold)
			dist_sq = gmainwindow->endiv_threshold;

		perimeter += sqrt(dist_sq);

		//get the current area A of the polygon
		M_area += vindexXi * vindexYi_n - vindexYi * vindexXi_n;
	}

	M_area = M_area*0.5;

	double C = 4.0 * (double)num*tan(M_PI / (double)num);

    mytype area = abs(M_area);
    mytype energy = perimeter*perimeter - C*area;
    assert(energy >= -1e-8);

	return energy;
}

adept::adouble test_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, const vector<adept::adouble>& radii, const Relationship* poly1, const Relationship* poly2)
{
	using adept::adouble;

	int p1 = poly1->getIndex();
	int p2 = poly2->getIndex();
	Vec2 pdir = centers[p2] - centers[p1];
	adouble pdist = pdir.length();
	adouble mindist = radii[p1] + radii[p2];

	if (pdist >= mindist)
		return 0.0;

	vector<Vec3> layout1 = getRelationshipLayout(erdata, poly1, NV, NM, x, centers, OVERLAPTYPE::COUNT);
	vector<Vec3> layout2 = getRelationshipLayout(erdata, poly2, NV, NM, x, centers, OVERLAPTYPE::COUNT);

	// if both polygons are digons or monogons, check for line-line intersections
	if (layout1.size() < 3 && layout2.size() < 3)
	{
		Vec3 v1 = layout1[0]; Vec3 v2 = layout1[1];
		Vec3 v3 = layout2[0]; Vec3 v4 = layout2[1];

		// check for duplicates
		if ((v1 == v3 && v2 == v4) || (v1 == v4 && v2 == v3))
			return 1.0;

		double cx = (v1.x * v2.y - v1.y * v2.x) * (v3.x - v4.x) - (v1.x - v2.x) * (v3.x * v4.y - v3.y * v4.x);
		double cy = (v1.x * v2.y - v1.y * v2.x) * (v3.y - v4.y) - (v1.y - v2.y) * (v3.x * v4.y - v3.y * v4.x);
		double dn = (v1.x - v2.x) * (v3.y - v4.y) - (v1.y - v2.y) * (v3.x - v4.x);
		
		// lines are parallel
		if (dn == 0)
		{
			Vec3 v12 = v1 - v2;
			Vec3 v13 = v1 - v3;
			Vec3 v14 = v1 - v4;
			// check for collinearity
			if ((v12.x * v13.y - v12.y * v13.x == 0) &&
				(v12.x * v14.y - v12.y * v14.x == 0))
				return ((v1.x > v3.x && v1.x < v4.x) || (v1.x > v4.x && v1.x < v3.x) ||
						(v1.y > v3.y && v1.y < v4.y) || (v1.y > v4.y && v1.y < v3.y) ||
						(v2.x > v3.x && v2.x < v4.x) || (v2.x > v4.x && v2.x < v3.x) ||
						(v2.y > v3.y && v2.y < v4.y) || (v2.y > v4.y && v2.y < v3.y) ||
						(v3.x > v1.x && v3.x < v2.x) || (v3.x > v2.x && v3.x < v1.x) ||
						(v3.y > v1.y && v3.y < v2.y) || (v3.y > v2.y && v3.y < v1.y) ||
						(v4.x > v1.x && v4.x < v2.x) || (v4.x > v2.x && v4.x < v1.x) ||
						(v4.y > v1.y && v4.y < v2.y) || (v4.y > v2.y && v4.y < v1.y));
			else
				return 0.0;
		}

		// lines are not parallel, check if intersection is in range
		cx /= dn; cy /= dn;
		return (((cx > v1.x && cx < v2.x) || (cx > v2.x && cx < v1.x)) &&
				((cy > v1.y && cy < v2.y) || (cy > v2.y && cy < v1.y)) &&
				((cx > v3.x && cx < v4.x) || (cx > v4.x && cy < v3.x)) &&
				((cy > v3.y && cy < v4.y) || (cy > v4.y && cy < v3.y)));
	}

	// check polygon 1 edges for a separating line
	if (layout1.size() > 2)
	{
		for (int i = 0; i < layout1.size(); i++)
		{
			bool separator = true;
			Vec2 v1(layout1[i]);
			Vec2 v2(layout1[(i + 1) % layout1.size()]);
			Vec2 dir = v1 - v2;
			Vec2 perp(dir.y, -dir.x);
			adouble center = perp.dot(centers[p1] - v2) / perp.dot(perp);
			for (int j = 0; j < layout2.size(); j++)
			{
				Vec2 v3 = Vec2(layout2[j]);
				if (v3 == v1 || v3 == v2)
					continue;
				adouble proj = perp.dot(v3 - v2) / perp.dot(perp);
				if (center * proj > 0)
				{
					separator = false;
					break;
				}
			}
			if (separator)
				return 0.0;
		}
	}

	// check polygon 2 edges for a separating line
	if (layout2.size() > 2)
	{
		for (int i = 0; i < layout2.size(); i++)
		{
			bool separator = true;
			Vec2 v1(layout2[i]);
			Vec2 v2(layout2[(i + 1) % layout2.size()]);
			Vec2 dir = v1 - v2;
			Vec2 perp(dir.y, -dir.x);
			adouble center = perp.dot(centers[p2] - v2) / perp.dot(perp);
			for (int j = 0; j < layout1.size(); j++)
			{
				Vec2 v3 = Vec2(layout1[j]);
				if (v3 == v1 || v3 == v2)
					continue;
				adouble proj = perp.dot(v3 - v2) / perp.dot(perp);
				if (center * proj > 0)
				{
					separator = false;
					break;
				}
			}
			if (separator)
				return 0.0;
		}
	}

	return 1.0;
}


adept::adouble calc_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, const vector<adept::adouble>& radii, const Relationship* poly1, const Relationship* poly2)
{
	typedef boost::geometry::model::d2::point_xy<double> boost_point;
	typedef boost::geometry::model::polygon<boost_point, false> boost_polygon;

	using adept::adouble;
	adouble area = 0.0;
	
	adouble overlap = test_PolygonOverlap(erdata, NV, NM, x, centers, radii, poly1, poly2);
	if (overlap.value() == 0)
		return 0.0;
	
	vector<Vec3> layout1 = getRelationshipLayout(erdata, poly1, NV, NM, x, centers, OVERLAPTYPE::AREA);
	vector<Vec3> layout2 = getRelationshipLayout(erdata, poly2, NV, NM, x, centers, OVERLAPTYPE::AREA);

	boost_polygon p1, p2;
	for (Vec3 const& vert : layout1)
		boost::geometry::append(p1.outer(), boost_point(vert.x, vert.y));
	boost::geometry::append(p1.outer(), boost_point(layout1[0].x, layout1[0].y));
	for (Vec3 const& vert : layout2)
		boost::geometry::append(p2.outer(), boost_point(vert.x, vert.y));
	boost::geometry::append(p2.outer(), boost_point(layout2[0].x, layout2[0].y));

	vector<boost_polygon> intersection;
	boost::geometry::intersection(p1, p2, intersection);
	for (boost_polygon const& p : intersection)
		area += boost::geometry::area(p);

	return area;
}

//typedef long long int hp;
//typedef struct { long int x; long int y; } ipoint;
//typedef struct { long int min; long int max; } range;
//typedef struct { ipoint ip; range rx; range ry; short int in; } vertex;
//adept::adouble calc_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, const vector<Vec2>& centers, const vector<adept::adouble>& radii, const Relationship* poly1, const Relationship* poly2)
//{
//	using adept::adouble;
//
//	adouble overlap = test_PolygonOverlap(erdata, NV, NM, x, centers, radii, poly1, poly2);
//	if (overlap.value() == 0)
//		return 0.0;
//
//	vector<Vec3> layout1 = getRelationshipLayout(erdata, poly1, NV, NM, x, centers, OVERLAPTYPE::AREA);
//	vector<Vec3> layout2 = getRelationshipLayout(erdata, poly2, NV, NM, x, centers, OVERLAPTYPE::AREA);
//
//	// get ranges
//	Vec2 min_xy = Vec2(DBL_MAX, DBL_MAX);
//	Vec2 max_xy = Vec2(-DBL_MAX, -DBL_MAX);
//	for (Vec3 v : layout1)
//	{
//		min_xy.x = v.x < min_xy.x ? v.x : min_xy.x;
//		min_xy.y = v.y < min_xy.y ? v.y : min_xy.y;
//		max_xy.x = v.x > max_xy.x ? v.x : max_xy.x;
//		max_xy.y = v.y > max_xy.y ? v.y : max_xy.y;
//	}
//	for (Vec3 v : layout2)
//	{
//		min_xy.x = v.x < min_xy.x ? v.x : min_xy.x;
//		min_xy.y = v.y < min_xy.y ? v.y : min_xy.y;
//		max_xy.x = v.x > max_xy.x ? v.x : max_xy.x;
//		max_xy.y = v.y > max_xy.y ? v.y : max_xy.y;
//	}
//	const double gamut = 500000000.0, mid = gamut / 2.0;
//	double range_x = max_xy.x - min_xy.x, scale_x = gamut / range_x;
//	double range_y = max_xy.y - min_xy.y, scale_y = gamut / range_y;
//	double ascale = scale_x * scale_y;
//
//	// fit points into ranges
//	int len1 = layout1.size();
//	int len2 = layout2.size();
//	vector<vertex> ipoints1(len1 + 1);
//	vector<vertex> ipoints2(len2 + 1);
//	{
//		int c = len1;
//		while (c--)
//		{
//			ipoints1[c].ip.x = ((long)((layout1[c].x - min_xy.x) * scale_x - mid) & ~7) | 0 | (c & 1);
//			ipoints1[c].ip.y = ((long)((layout1[c].y - min_xy.y) * scale_y - mid) & ~7) | 0;
//		}
//	}
//	ipoints1[0].ip.y += len1 & 1;
//	ipoints1[len1] = ipoints1[0];
//	{
//		int c = len1;
//		while (c--)
//		{		
//			ipoints1[c].rx = ipoints1[c].ip.x < ipoints1[c + 1].ip.x ?
//				range{ ipoints1[c].ip.x, ipoints1[c + 1].ip.x } : range{ ipoints1[c + 1].ip.x, ipoints1[c].ip.x };
//			ipoints1[c].ry = ipoints1[c].ip.y < ipoints1[c + 1].ip.y ?
//				range{ ipoints1[c].ip.y, ipoints1[c + 1].ip.y } : range{ ipoints1[c + 1].ip.y, ipoints1[c].ip.y };
//			ipoints1[c].in = 0;
//		}
//	}
//	{
//		int c = len2;
//		while (c--)
//		{
//			ipoints2[c].ip.x = (long)((layout2[c].x - min_xy.x) * scale_x - mid) & ~7 | 2 | c & 1;
//			ipoints2[c].ip.y = (long)((layout2[c].y - min_xy.y) * scale_y - mid) & ~7 | 2;
//		}
//	}
//	ipoints2[0].ip.y += len2 & 1;
//	ipoints2[len2] = ipoints2[0];
//	{
//		int c = len2;
//		while (c--)
//		{
//			range rx, ry;
//			ipoints2[c].rx = ipoints2[c].ip.x < ipoints2[c + 1].ip.x ?
//				range{ ipoints2[c].ip.x, ipoints2[c + 1].ip.x } : range{ ipoints2[c + 1].ip.x, ipoints2[c].ip.x };
//			ipoints2[c].ry = ipoints2[c].ip.y < ipoints2[c + 1].ip.y ?
//				range{ ipoints2[c].ip.y, ipoints2[c + 1].ip.y } : range{ ipoints2[c + 1].ip.y, ipoints2[c].ip.y };
//			ipoints2[c].in = 0;
//		}
//	}
//
//	// calculate area
//	hp s = 0;
//	for (int j = 0; j < len1; j++)
//	{
//		for (int k = 0; k < len2; k++)
//		{
//			if ((ipoints1[j].rx.min < ipoints2[k].rx.max && ipoints2[k].rx.min < ipoints1[j].rx.max) &&
//				(ipoints1[j].ry.min < ipoints2[k].ry.max && ipoints2[k].ry.min < ipoints1[j].ry.max))
//			{
//				hp a1 = -((hp)ipoints2[k].ip.x * ipoints2[k + 1].ip.y - (hp)ipoints2[k].ip.y * ipoints2[k + 1].ip.x +
//					(hp)ipoints1[j].ip.x * (ipoints2[k].ip.y - ipoints2[k + 1].ip.y) + (hp)ipoints1[j].ip.y * (ipoints2[k + 1].ip.x - ipoints2[k].ip.x));
//				hp a2 = (hp)ipoints2[k].ip.x * ipoints2[k + 1].ip.y - (hp)ipoints2[k].ip.y * ipoints2[k + 1].ip.x +
//					(hp)ipoints1[j + 1].ip.x * (ipoints2[k].ip.y - ipoints2[k + 1].ip.y) + (hp)ipoints1[j + 1].ip.y * (ipoints2[k + 1].ip.x - ipoints2[k].ip.x);
//				int o = a1 < 0;
//				if (o == a2 < 0)
//				{
//					hp a3 = (hp)ipoints1[j].ip.x * ipoints1[j + 1].ip.y - (hp)ipoints1[j].ip.y * ipoints1[j + 1].ip.x +
//						(hp)ipoints2[k].ip.x * (ipoints1[j].ip.y - ipoints1[j + 1].ip.y) + (hp)ipoints2[k].ip.y * (ipoints1[j + 1].ip.x - ipoints1[j].ip.x);
//					hp a4 = -((hp)ipoints1[j].ip.x * ipoints1[j + 1].ip.y - (hp)ipoints1[j].ip.y * ipoints1[j + 1].ip.x +
//						(hp)ipoints2[k + 1].ip.x * (ipoints1[j].ip.y - ipoints1[j + 1].ip.y) + (hp)ipoints2[k + 1].ip.y * (ipoints1[j + 1].ip.x - ipoints1[j].ip.x));
//					if (o)
//					{
//						double r1 = a1 / (a1 + a2);
//						double r2 = a3 / (a3 + a4);
//						ipoint f, t;
//						f.x = (ipoints1[j].ip.x + r1 * (ipoints1[j + 1].ip.x - ipoints1[j].ip.x));
//						f.y = (ipoints1[j].ip.y + r1 * (ipoints1[j + 1].ip.y - ipoints1[j].ip.y));
//						t = ipoints1[j + 1].ip;
//						s += (hp)1 * (t.x - f.x) * (t.y + f.y) / 2;
//
//						t.x = (ipoints2[k].ip.x + r2 * (ipoints2[k + 1].ip.x - ipoints2[k].ip.x));
//						t.y = (ipoints2[k].ip.y + r2 * (ipoints2[k + 1].ip.y - ipoints2[k].ip.y));
//						f = ipoints2[k + 1].ip;
//						s += (hp)1 * (t.x - f.x) * (t.y + f.y) / 2;
//						
//						++ipoints1[j].in; 
//						--ipoints2[k].in;
//					}
//					else
//					{
//						double r1 = a3 / (a3 + a4);
//						double r2 = a1 / (a1 + a2);
//						ipoint f, t;
//						f.x = (ipoints2[k].ip.x + r1 * (ipoints2[k+1].ip.x - ipoints2[k].ip.x));
//						f.y = (ipoints2[k].ip.y + r1 * (ipoints2[k+1].ip.y - ipoints2[k].ip.y));
//						t = ipoints2[k+1].ip;
//						s += (hp)1 * (t.x - f.x) * (t.y + f.y) / 2;
//
//						t.x = (ipoints1[j].ip.x + r2 * (ipoints1[j+1].ip.x - ipoints1[j].ip.x));
//						t.y = (ipoints1[j].ip.y + r2 * (ipoints1[j+1].ip.y - ipoints1[j].ip.y));
//						f = ipoints1[j+1].ip;
//						s += (hp)1 * (t.x - f.x) * (t.y + f.y) / 2;
//
//						++ipoints2[k].in;
//						--ipoints1[j].in;
//					}
//				}
//			}
//		}
//	}
//
//	return abs((s / ascale) / 2.0);
//}

double calc_PolygonOverlap(Relationship* poly1, Relationship* poly2)
{
	typedef boost::geometry::model::d2::point_xy<double> boost_point;
	typedef boost::geometry::model::polygon<boost_point, false> boost_polygon;

	vector<Vec3> layout1 = poly1->getSortedLayout();
	vector<Vec3> layout2 = poly2->getSortedLayout();
	

	boost_polygon p1, p2;
	for (Vec3 const& vert: layout1)
		boost::geometry::append(p1.outer(), boost_point(vert.x, vert.y));
	boost::geometry::append(p1.outer(), boost_point(layout1[0].x, layout1[0].y));
	for (Vec3 const& vert : layout2)
		boost::geometry::append(p2.outer(), boost_point(vert.x, vert.y));
	boost::geometry::append(p2.outer(), boost_point(layout2[0].x, layout2[0].y));

	vector<boost_polygon> intersection12;
	boost::geometry::intersection(p1, p2, intersection12);
	
	double overlap_area = 0.0;
	for (boost_polygon const& p : intersection12)
		overlap_area += boost::geometry::area(p);
	double poly1_area = boost::geometry::area(p1);

	return overlap_area / poly1_area;
}

double calc_PolygonAspectRatio(Relationship* poly)
{
	double area = 0;
	double perim = 0;
	vector<Vec3> layout = poly->getSortedLayout();
	for (int i = 0; i < layout.size(); i++)
	{
		int j = (i + 1) % layout.size();
		Vec3 diff = layout[j] - layout[i];
		perim += sqrt(diff.x * diff.x + diff.y * diff.y);
		area += layout[i].x * layout[j].y - layout[i].y * layout[j].x;
	}
	area = abs(area * 0.5);
	double c = 4.0 * (double)layout.size() * tan(M_PI / (double)layout.size());
	return (perim * perim - c * area) / (perim * perim);
}



// Energy Terms

template<class mytype>
mytype EnergyEval_AspectRatio(const ERData *erdata, const int N, const mytype * x, ENERGYTYPE type) {

	using adept::adouble;

	mytype y = 0;

	assert(N == erdata->getFreeVertCompNum());

	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		if (erdata->getRelationshipCard(j) < 3) {
			continue;
		}

		/*for regularize operation*/
		if (!erdata->isRelationshipOptimizable(j))
			continue;

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<mytype>(erdata, j, N, x);

		mytype energy = calc_PolygonAspectRatio<mytype>(erdata, N, x, entityindices);

		y += energy;

	}

	return y;
}

template<class mytype>
mytype EnergyEval_AreaDiff(const ERData *erdata, const int N, const mytype * x, ENERGYTYPE type)
{
	using adept::adouble;
	mytype y = 0;
	assert(N == erdata->getFreeVertCompNum());
    
	double theta = (Element::m_DigonLensAngle) / 180.0*M_PI;


	for (int j = 0; j < erdata->getRelationshipNum(); j++) {

		const int rcard = erdata->getRelationshipCard(j);

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<mytype>(erdata, j, N, x);

		int num = entityindices.size();

		double targetArea = 0;
		mytype M_a = 0;

		if (rcard == 2) 
		{
			int vi = entityindices[0];
			int vi_n = entityindices[1];

			mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
			mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
			mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
			mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

			mytype ux = vindexXi - vindexXi_n;
			mytype uy = vindexYi - vindexYi_n;

			mytype sq_dist = (ux)*(ux)+(uy)*(uy);

			M_a = ((theta - sin(theta)) * sq_dist) / (4.0*(sin(theta / 2.0)*sin(theta / 2.0)));

			double tar_L = calc_expected_L(erdata, j);
			targetArea = ((theta - sin(theta)) *tar_L*tar_L) / (4.0*(sin(theta / 2.0)*sin(theta / 2.0)));

		}
		else if (rcard > 2) {
			M_a = 0.0;

			double tar_L = calc_expected_L(erdata, j);
			targetArea = double(1.0 / 4.0) * num*tar_L*tar_L / tan(double(M_PI) / num);

			for (int i = 0; i < entityindices.size(); i++) {

				int vi = entityindices[i];
				int vi_n = entityindices[(i + 1) % num];

				mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
				mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
				mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
				mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

				M_a += vindexXi * vindexYi_n - vindexYi * vindexXi_n;
			}
			M_a = M_a*0.5;

		}
		else {
			continue;
		}

		mytype energy = 0;
		if (type == ENERGYTYPE::SUBTRACTION) 
		{
			LogConsole::showError("L^2 AD only applied to LD");
		}
		else if (type == ENERGYTYPE::DIVISION)
		{
			mytype area = abs(M_a);
			mytype d = area / targetArea - 1.0;
			energy = d*d;
		}
		else if (type == ENERGYTYPE::LOG) {

			mytype area = abs(M_a);
			mytype d = log(area / targetArea);
			energy = d*d;
		}

		if (rcard == 2) {
			energy *= gmainwindow->weight_DL*energy;
		}

		y += energy;
	}

	return y;
}

template<class mytype>
mytype EnergyEval_LengthDiff(const ERData *erdata, const int N, const mytype * x)
{
	using adept::adouble;
	mytype y = 0;
	assert(N == erdata->getFreeVertCompNum());
	
	for (int j = 0; j < erdata->getRelationshipNum(); j++) {

		const int rcard = erdata->getRelationshipCard(j);

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<mytype>(erdata, j, N, x);

		int num = entityindices.size();

		mytype lenDiff = 0;

		if (rcard >= 2) 
		{
			for (int i = 0; i < entityindices.size(); i++) {

				int vi = entityindices[i];
				int vi_n = entityindices[(i + 1) % num];

				mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
				mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
				mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
				mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

				mytype ux = vindexXi - vindexXi_n;
				mytype uy = vindexYi - vindexYi_n;

				mytype dist_sq = (ux)*(ux)+(uy)*(uy);

				double L = calc_expected_L(erdata, j);

				mytype dist = sqrt(dist_sq);
				lenDiff += (dist - L)*(dist - L);
			}
		}
		else {
			continue;
		}

		if (rcard == 2) {
			lenDiff *= gmainwindow->weight_DL*lenDiff;
		}

		y += lenDiff;
	}

	return y;
}

template<class mytype>
mytype EnergyEval_PolygonSeparation(const ERData *erdata, int N, const mytype * x) {
	
	assert(N == erdata->getFreeVertCompNum());
	using adept::adouble;
	mytype y = 0;

	int recount = erdata->getRelationshipNum();
	vector<pair<mytype, mytype>> centers;
	centers.reserve(recount);

	for (int j = 0; j < erdata->getRelationshipNum(); j++) {

		vector<int> entityindices;
		entityindices = erdata->getRelationshipVertIndices(j);

		double num = entityindices.size();

		mytype cx = 0;
		mytype cy = 0;

		for (int i = 0; i < entityindices.size(); i++) {

			int vi = entityindices[i];

			mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
			mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);

			cx += vindexXi;
			cy += vindexYi;
		}

		cx /= num;
		cy /= num;

		centers.push_back(pair<mytype, mytype>(cx, cy));
	}

	/*calculate the minimum distance between polygon center and the digon line segment*/
	auto calcDigonPolyDistSq = [&](int digonv1, int digonv2, int polyi) {
		mytype v1_X = get_vi_x_value<mytype>(digonv1, erdata, x);
		mytype v1_Y = get_vi_y_value<mytype>(digonv1, erdata, x);
		mytype dist1_x = v1_X - centers[polyi].first;
		mytype dist1_y = v1_Y - centers[polyi].second;
		mytype dist1_sq = (dist1_x*dist1_x + dist1_y*dist1_y);

		mytype v2_X = get_vi_x_value<mytype>(digonv2, erdata, x);
		mytype v2_Y = get_vi_y_value<mytype>(digonv2, erdata, x);
		mytype dist2_x = v2_X - centers[polyi].first;
		mytype dist2_y = v2_Y - centers[polyi].second;
		mytype dist2_sq = (dist2_x*dist2_x + dist2_y*dist2_y);

		mytype dist_v1_v2_sq = (v2_X - v1_X)*(v2_X - v1_X) + (v2_Y - v1_Y)*(v2_Y - v1_Y);

		mytype dist_min = 0.0;

		if (dist1_sq <= dist2_sq)
			dist_min = dist1_sq;
		else
			dist_min = dist2_sq;

		/*project (Pi - V1) on vector (V2 - V1)*/
		mytype dotp = (centers[polyi].first - v1_X)*(v2_X - v1_X) + (centers[polyi].second - v1_Y)*(v2_Y - v1_Y);
		mytype t = dotp / dist_v1_v2_sq;
		if (t < 1 && t >0) {
			mytype dist3_sq = ((v2_X - v1_X)*(v1_Y - centers[polyi].second) - (v1_X - centers[polyi].first)*(v2_Y - v1_Y))*
				((v2_X - v1_X)*(v1_Y - centers[polyi].second) - (v1_X - centers[polyi].first)*(v2_Y - v1_Y)) / dist_v1_v2_sq;
			dist_min = dist3_sq;
		}

		return dist_min;
	};

	for (int j = 0; j < erdata->getRelationshipNum(); j++) {

		if (erdata->getRelationshipCard(j) == 1)
			continue;

		for (int i = j+1; i < erdata->getRelationshipNum(); i++)
		{

			if (i == j)
				continue;

			if (erdata->getRelationshipCard(i) == 1)
				continue;

			if (erdata->isTwoResSame(j, i)) {
				continue;
			}


			mytype dir_x = centers[j].first - centers[i].first;
			mytype dir_y = centers[j].second - centers[i].second;

			mytype dist_sq = (dir_x*dir_x + dir_y*dir_y);

			//for gradient calculation
			if (dist_sq < gmainwindow->endiv_threshold)
				dist_sq = gmainwindow->endiv_threshold;

            {
				/*use radius of bounding circle of regular polygon with expected area*/
				int cardj = erdata->getRelationshipCard(j);
				int cardi = erdata->getRelationshipCard(i);

				double Lj = calc_expected_L(erdata, j);
				double Li = calc_expected_L(erdata, i);

				double Rj = 0.5 * Lj / sin(M_PI / cardj);
				double Ri = 0.5 * Li / sin(M_PI / cardi);
				double scale = gmainwindow->PS_distance_buffer;
				
				vector<int> sharedVertsId = erdata->getSharedEntityIndices(j, i);
				int common_vert_num = sharedVertsId.size();

				mytype energy = 0.0;
				mytype sq_of_diff = 0.0;

				if (common_vert_num == 0) {

					double expectdist = Rj + Ri + scale*0.5*(Li + Lj);

					/*special handler when on polygon is a digon*/
					if (cardj == 2) {
						expectdist = Ri + scale*Li;
						vector<int>digon_indices = erdata->getRelationshipVertIndices(j);
						std::sort(digon_indices.begin(), digon_indices.end());
						assert(digon_indices.size() == 2);

						dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], i);
						assert(dist_sq.value() != 0.0);
					}
					else if (cardi == 2) {
						expectdist = Rj + scale*Lj;
						vector<int>digon_indices = erdata->getRelationshipVertIndices(i);
						std::sort(digon_indices.begin(), digon_indices.end());
						assert(digon_indices.size() == 2);

						dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], j);
						assert(dist_sq.value() != 0.0);
					}


					if (dist_sq < expectdist*expectdist)
						sq_of_diff = expectdist*expectdist + dist_sq - 2 * expectdist*sqrt(dist_sq);
					else
						sq_of_diff = 0.0;
				}
				
				if (common_vert_num == 1) {

					double anglei = (cardi - 2)*M_PI / cardi;
					double anglej = (cardj - 2)*M_PI / cardj;
					double gamma_scale = gmainwindow->PS_angle_buffer;

					double gamma_scale_rad = gamma_scale / 180.0 * M_PI;
					double gamma_rad = gamma_scale_rad + (anglei + anglej)*0.5;

					int sharedNodeID = sharedVertsId[0];

					mytype vx = get_vi_x_value<mytype>(sharedNodeID, erdata, x);
					mytype vy = get_vi_y_value<mytype>(sharedNodeID, erdata, x);

					mytype dir_j_x = centers[j].first - vx;
					mytype dir_j_y = centers[j].second - vy;

					mytype dj_sq = (dir_j_x*dir_j_x + dir_j_y*dir_j_y);
					mytype dj = sqrt(dj_sq);

					mytype dir_i_x = centers[i].first - vx;
					mytype dir_i_y = centers[i].second - vy;

					mytype di_sq = (dir_i_x*dir_i_x + dir_i_y*dir_i_y);
					mytype di = sqrt(di_sq);

					mytype mindistsq = dj*dj + di*di - 2 * dj*di*cos(gamma_rad);
						
					if(dist_sq < mindistsq)
						sq_of_diff = dist_sq + mindistsq - 2 * sqrt(mindistsq) * sqrt(dist_sq);
					else
						sq_of_diff = 0.0;
				}

				if (common_vert_num == 2) {

					if (cardj == 2 || cardi == 2) {
						continue;
					}

					double L = 0.5*(Li + Lj);
					double width_lens = (Rj - sqrt(Rj*Rj - 0.25*L*L)) + (Ri - sqrt(Ri*Ri - 0.25*L*L));
					double expectdist = Rj + Ri - width_lens;

                    if (dist_sq < expectdist*expectdist)
                        sq_of_diff = expectdist*expectdist + dist_sq - 2 * expectdist*sqrt(dist_sq);
                    else
                        sq_of_diff = 0.0;
				}
				
				energy = sq_of_diff;

				if (std::isinf(energy.value()) || energy.value() != energy.value()) {
					gmainwindow->flag_stopOptimization = true;
					LogConsole::writeError("energy invalid");
				}

				y += energy;

			}
		}
	}

	return y;
}

template<class mytype>
mytype EnergyEval_VertexSeparation(const ERData *erdata, const int N, const mytype * x) {
	
	mytype y = 0;

	double L = gmainwindow->expectedLength;
	double buffer = gmainwindow->VertexSepBuffer;

	//calculate energy for all vertices
	for (int i = 0; i < erdata->getEntityNum(); i++) {

		//calculate repulsive forces
		for (int j = i + 1; j < erdata->getEntityNum(); j++) {

			if (i == j)
				continue;

			mytype vindexXi = get_vi_x_value<mytype>(i, erdata, x);
			mytype vindexYi = get_vi_y_value<mytype>(i, erdata, x);
			mytype vindexXj = get_vi_x_value<mytype>(j, erdata, x);
			mytype vindexYj = get_vi_y_value<mytype>(j, erdata, x);

			mytype dir_x = vindexXj - vindexXi;
			mytype dir_y = vindexYj - vindexYi;

			mytype dist_sq = dir_x*dir_x + dir_y*dir_y;

			if (dist_sq < gmainwindow->endiv_threshold)
				dist_sq = gmainwindow->endiv_threshold;


            double r1 = erdata->getEntityRadius(i);
            double r2 = erdata->getEntityRadius(j);
            double exp = (r1 + r2)*(buffer + 1.0);

            if (dist_sq < exp*exp) {
                y += (dist_sq + exp*exp - 2 * sqrt(dist_sq)*exp);
            }

		}

	}

	return y;
}

template<class mytype>
mytype EnergyEval_PolygonDualDist(ERData *erdata, ERData *dualerdata, const int N1, const mytype * x, const int N2, const mytype * x_dual)
{
	mytype y = 0;

	/*calc the distance between each polygon's center and its dual node*/
	for (int j = 0; j < dualerdata->getRelationshipNum(); j++) {

		Entity* en = erdata->getEntity(j);
		Relationship* re = en->getDualRelationship();

		vector<int> entityindices = re->getEntityIndices();
		mytype num = re->getCardinality();

		if (num == 1)
			continue;

		mytype ex = get_vi_x_value<mytype>(j, erdata, x);
		mytype ey = get_vi_y_value<mytype>(j, erdata, x);

		/*calc the center of poly*/
		mytype cx = 0;
		mytype cy = 0;
		for (int k = 0; k < entityindices.size(); k++) {

			int index = entityindices[k];

			mytype dualx = get_vi_x_value<mytype>(index, dualerdata, x_dual);
			mytype dualy = get_vi_y_value<mytype>(index, dualerdata, x_dual);

			cx += dualx;
			cy += dualy;
		}

		cx /= num;
		cy /= num;

		y += (ex - cx)*(ex - cx) + (ey - cy)*(ey - cy);
	}


	for (int j = 0; j < erdata->getRelationshipNum(); j++) {

		Entity* en = dualerdata->getEntity(j);
		Relationship* re = en->getDualRelationship();

		vector<int> entityindices = re->getEntityIndices();
		mytype num = re->getCardinality();

		if (num == 1)
			continue;

		mytype ex = get_vi_x_value<mytype>(j, dualerdata, x_dual);
		mytype ey = get_vi_y_value<mytype>(j, dualerdata, x_dual);

		/*calc the center of poly*/
		mytype cx = 0;
		mytype cy = 0;
		for (int k = 0; k < entityindices.size(); k++) {

			int index = entityindices[k];

			mytype dualx = get_vi_x_value<mytype>(index, erdata, x);
			mytype dualy = get_vi_y_value<mytype>(index, erdata, x);

			cx += dualx;
			cy += dualy;
		}

		cx /= num;
		cy /= num;

		y += (ex - cx)*(ex - cx) + (ey - cy)*(ey - cy);
	}

	return y;
}

template<class mytype>
mytype EnergyEval_ContinousTopology(ERData *erdata, const int N, const mytype * x)
{
	using adept::adouble;
	mytype energy = 0;
	assert(N == erdata->getFreeVertCompNum());

	auto calcGapVertsDistSq = [&](GapVerts gapVertSeries) {
		mytype dist = 0;
		int num = gapVertSeries.size();
		for (int i = 0; i < num - 1; i++) {

			int vi = gapVertSeries[i];
			int vi_n = gapVertSeries[i + 1];

			mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
			mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
			mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
			mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

			mytype ux = vindexXi - vindexXi_n;
			mytype uy = vindexYi - vindexYi_n;

			mytype dist_sq = (ux)*(ux)+(uy)*(uy);

			dist += sqrt(dist_sq);
		}
		return dist;
	};

	auto calcGapVertSeriesDist = [calcGapVertsDistSq](vector<GapVerts> gapVertSeries) {
		vector<mytype> gapVertSeriesDist;
		int num = gapVertSeries.size();
		for (int i = 0; i < num; i++) {
			mytype dist = calcGapVertsDistSq(gapVertSeries[i]);
			gapVertSeriesDist.push_back(dist);
		}
		return gapVertSeriesDist;
	};

	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		RE *rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<mytype>(erdata, rei, N, x);
		int adjNum = rei->getAdjacentRelationships().size();
		int cardi = vindices_i.size();

		for (int j = 0; j < adjNum; j++)
		{
			RE *rei_adj = rei->getAdjacentRelationship(j);
			int rei_adj_index = rei_adj->getIndex();
			vector<int> vindices_j = getRelationshipVertIndices<mytype>(erdata, rei_adj, N, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			assert(common_vert_num > 0);

			if (common_vert_num < 2)
				continue;

			vector<GapVerts> gapVertsList = getGapVertSeries(commonIndices, vindices_i);
			vector<mytype> gapVertSeriesDist = calcGapVertSeriesDist(gapVertsList);

			if (common_vert_num == 2)
			{
				double expGapCount = cardi - 2;
				double L = calc_expected_L(erdata, rei_adj_index);
				double expectedDist = L*(expGapCount + 1);

				assert(gapVertSeriesDist.size() == 2);

				mytype min = gapVertSeriesDist[0];
				mytype max = gapVertSeriesDist[1];
				if (min > max)
					std::swap(min, max);

				energy += (max - expectedDist)*(max - expectedDist) + (min - L)*(min - L);

			}
			else if (common_vert_num > 2)
			{
				double L = calc_expected_L(erdata, rei_adj_index);
				double expectedDist = L*double(cardi)/ (common_vert_num);

				for (int k = 0; k < gapVertSeriesDist.size(); k++) {
					energy += (expectedDist - gapVertSeriesDist[k])*(expectedDist - gapVertSeriesDist[k]);
				}
			}
		}
	}
	return energy;
}

double evaluate_toplogy_energy(ERData * erdata, int N, const double * x)
{
	assert(N == erdata->getFreeVertCompNum());

	double energy = 0;

	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		RE *rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<double>(erdata, rei, N, x);
		int adjNum = rei->getAdjacentRelationships().size();
		int cardi = vindices_i.size();

		for (int j = 0; j < adjNum; j++)
		{
			RE *rei_adj = rei->getAdjacentRelationship(j);
			vector<int> vindices_j = getRelationshipVertIndices<double>(erdata, rei_adj, N, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			assert(common_vert_num > 0);

			if (common_vert_num < 2)
				continue;

			vector<int> gapSeries = getGapSeries(commonIndices, vindices_i);

			if (common_vert_num == 2)
			{
				double expGapCount = cardi - 2;

				assert(gapSeries.size() == 2);
				int min = gapSeries[0];
				int max = gapSeries[1];
				if (min > max)
					std::swap(min, max);

				energy += (max - expGapCount)*(max - expGapCount) + min*min;

			}
			else if (common_vert_num > 2)
			{
				double expGapCount = double(cardi - common_vert_num) / common_vert_num;
				for (int k = 0; k < gapSeries.size(); k++) {
					energy += (expGapCount - gapSeries[k])*(expGapCount - gapSeries[k]);
				}
			}
		}
	}
	return energy;
}

template<class mytype>
mytype EnergyEval_IntersectionAspectRatio(ERData *erdata, const int N, const mytype * x)
{
	using adept::adouble;
	mytype y = 0;
	assert(N == erdata->getFreeVertCompNum());

	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		RE *rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<mytype>(erdata, rei, N, x);
		int adjNum = rei->getAdjacentRelationships().size();
		int cardi = vindices_i.size();

		for (int j = 0; j < adjNum; j++)
		{
			RE *rei_adj = rei->getAdjacentRelationship(j);
			vector<int> vindices_j = getRelationshipVertIndices<mytype>(erdata, rei_adj, N, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			assert(common_vert_num > 0);

			if (common_vert_num <= 2)
				continue;

			{
				vector<Vec3> intersectlayout(commonIndices.size());
				for (int i = 0; i < commonIndices.size(); i++) {
					int vi = commonIndices[i];
					mytype vi_x, vi_y;
					vi_x = get_vi_x_value(vi, erdata, x);
					vi_y = get_vi_y_value(vi, erdata, x);
					intersectlayout[i] = Vec3(vi_x, vi_y, 0);
				}

				commonIndices = Relationship::getSortedVerticesFromLocs(commonIndices, intersectlayout);
			}

			mytype energy = calc_PolygonAspectRatio<mytype>(erdata, N, x, commonIndices);

			y += energy;
		}
	}

	return y;
}

template<class mytype>
mytype EnergyEval_Separation(const ERData* erdata, const int NV, const int NM, const mytype* x)
{
	using adept::adouble;
	mytype y = 0;

	int recount = erdata->getRelationshipNum();
	vector<pair<mytype, mytype>> centers;
	centers.reserve(recount);
	double monogon_length = Element::m_MonogonLength;

	// get polygon centers
	for (int j = 0; j < recount; j++)
	{
		const Relationship* r = erdata->getConstRelationship(j);
		vector<int> entityindices = r->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;
		adouble max_radius = 0.0;

		if (entityindices.size() == 1)
		{
			int mon_index = erdata->getMonogonIndex(r);
			int ent_index_x = 2 * entityindices[0];
			int ent_index_y = ent_index_x + 1;
			
			adouble angle = x[NV + mon_index];
			adouble x0 = x[ent_index_x];
			adouble y0 = x[ent_index_y];

			cx = x0 + (monogon_length / 2.0) * cos(angle);
			cy = y0 + (monogon_length / 2.0) * sin(angle);
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				int ent_index_x = 2 * entityindices[i];
				int ent_index_y = ent_index_x + 1;

				adouble x0 = x[ent_index_x];
				adouble y0 = x[ent_index_y];

				cx += x0;
				cy += y0;
			}

			cx /= num;
			cy /= num;
		}
		
		centers.push_back(pair<adouble, adouble>(cx, cy));
	}

	/*calculate the minimum distance between polygon center and the digon line segment*/
	auto calcDigonPolyDistSq = [&](int digonv1, int digonv2, int polyi) {
		mytype v1_X = get_vi_x_value<mytype>(digonv1, erdata, x);
		mytype v1_Y = get_vi_y_value<mytype>(digonv1, erdata, x);
		mytype dist1_x = v1_X - centers[polyi].first;
		mytype dist1_y = v1_Y - centers[polyi].second;
		mytype dist1_sq = (dist1_x * dist1_x + dist1_y * dist1_y);

		mytype v2_X = get_vi_x_value<mytype>(digonv2, erdata, x);
		mytype v2_Y = get_vi_y_value<mytype>(digonv2, erdata, x);
		mytype dist2_x = v2_X - centers[polyi].first;
		mytype dist2_y = v2_Y - centers[polyi].second;
		mytype dist2_sq = (dist2_x * dist2_x + dist2_y * dist2_y);

		mytype dist_v1_v2_sq = (v2_X - v1_X) * (v2_X - v1_X) + (v2_Y - v1_Y) * (v2_Y - v1_Y);

		mytype dist_min = 0.0;

		if (dist1_sq <= dist2_sq)
			dist_min = dist1_sq;
		else
			dist_min = dist2_sq;

		/*project (Pi - V1) on vector (V2 - V1)*/
		mytype dotp = (centers[polyi].first - v1_X) * (v2_X - v1_X) + (centers[polyi].second - v1_Y) * (v2_Y - v1_Y);
		mytype t = dotp / dist_v1_v2_sq;
		if (t < 1 && t >0) {
			mytype dist3_sq = ((v2_X - v1_X) * (v1_Y - centers[polyi].second) - (v1_X - centers[polyi].first) * (v2_Y - v1_Y)) *
				((v2_X - v1_X) * (v1_Y - centers[polyi].second) - (v1_X - centers[polyi].first) * (v2_Y - v1_Y)) / dist_v1_v2_sq;
			dist_min = dist3_sq;
		}

		return dist_min;
	};


	// accumulate separation values
	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		for (int i = j + 1; i < erdata->getRelationshipNum(); i++)
		{
			if (erdata->isTwoResSame(j, i))
				continue;

			mytype dir_x = centers[j].first - centers[i].first;
			mytype dir_y = centers[j].second - centers[i].second;
			mytype dist_sq = (dir_x * dir_x + dir_y * dir_y);

			//for gradient calculation
			if (dist_sq < gmainwindow->endiv_threshold)
				dist_sq = gmainwindow->endiv_threshold;

			int cardj = erdata->getRelationshipCard(j);
			int cardi = erdata->getRelationshipCard(i);

			double scale = gmainwindow->PS_distance_buffer;

			vector<int> sharedVertsId = erdata->getSharedEntityIndices(j, i);
			int common_vert_num = sharedVertsId.size();

			mytype energy = 0.0;
			//mytype diff_of_sq = 0.0;
			mytype sq_of_diff = 0.0;
			//double scalarK = gmainwindow->PS_exponent_scalar;
			
			mytype Rj, Ri, Li, Lj;

			if (cardj == 1)
			{
				Lj = calc_expected_L(erdata, j);
				Rj = monogon_length / 2.0;
			}
			else
			{
				Lj = calc_expected_L(erdata, j);
				Rj = 0.5 * Lj / sin(M_PI / cardj);
			}

			if (cardi == 1)
			{
				Li = calc_expected_L(erdata, i);
				Ri = monogon_length / 2.0;
			}
			else
			{
				Li = calc_expected_L(erdata, i);
				Ri = 0.5 * Li / sin(M_PI / cardi);
			}


			if (common_vert_num == 0)
			{
				mytype expectdist = Rj + Ri + scale * 0.5 * (Li + Lj);

				/*special handler when on polygon is a digon*/
				if (cardj == 2) {
					expectdist = Ri + scale * Li;
					vector<int>digon_indices = erdata->getRelationshipVertIndices(j);
					std::sort(digon_indices.begin(), digon_indices.end());
					assert(digon_indices.size() == 2);

					dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], i);
					assert(dist_sq.value() != 0.0);
				}
				else if (cardi == 2) {
					expectdist = Rj + scale * Lj;
					vector<int>digon_indices = erdata->getRelationshipVertIndices(i);
					std::sort(digon_indices.begin(), digon_indices.end());
					assert(digon_indices.size() == 2);

					dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], j);
					assert(dist_sq.value() != 0.0);
				}

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 1) 
			{
				double anglei = 0;
				double anglej = 0;
				if (cardi > 2)
					anglei = (cardi - 2) * M_PI / cardi;
				if (cardj > 2)
					anglej = (cardj - 2) * M_PI / cardj;
				double gamma_scale = gmainwindow->PS_angle_buffer;

				double gamma_scale_rad = gamma_scale / 180.0 * M_PI;
				double gamma_rad = gamma_scale_rad + (anglei + anglej) * 0.5;

				int sharedNodeID = sharedVertsId[0];

				mytype vx = get_vi_x_value<mytype>(sharedNodeID, erdata, x);
				mytype vy = get_vi_y_value<mytype>(sharedNodeID, erdata, x);

				mytype dir_j_x = centers[j].first - vx;
				mytype dir_j_y = centers[j].second - vy;

				mytype dj_sq = (dir_j_x * dir_j_x + dir_j_y * dir_j_y);
				mytype dj = sqrt(dj_sq);

				mytype dir_i_x = centers[i].first - vx;
				mytype dir_i_y = centers[i].second - vy;

				mytype di_sq = (dir_i_x * dir_i_x + dir_i_y * dir_i_y);
				mytype di = sqrt(di_sq);

				mytype mindistsq = dj * dj + di * di - 2 * dj * di * cos(gamma_rad);

				if (dist_sq < mindistsq)
					sq_of_diff = dist_sq + mindistsq - 2 * sqrt(mindistsq) * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 2) {

				if (cardj == 2 || cardi == 2)
					continue;

				mytype L = 0.5 * (Li + Lj);
				mytype width_lens = (Rj - sqrt(Rj * Rj - 0.25 * L * L)) + (Ri - sqrt(Ri * Ri - 0.25 * L * L));
				mytype expectdist = Rj + Ri - width_lens;

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			///////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////

			energy = sq_of_diff;


			if (std::isinf(energy.value()) || energy.value() != energy.value()) {
				gmainwindow->flag_stopOptimization = true;
				LogConsole::writeError("energy invalid");
			}

			y += energy;
		}
	}

	return y;
}

//template<class mytype>
//mytype EnergyEval_Unravel(const ERData* erdata, const int NV, const int NM, const mytype* x)
//{
//	using adept::adouble;
//	mytype y = 0;
//
//	int recount = erdata->getRelationshipNum();
//	vector<pair<mytype, mytype>> centers;
//	centers.reserve(recount);
//	vector<mytype> radii;
//	radii.reserve(recount);
//	double b = Element::m_MonogonSize;
//	double a = Element::m_MonogonLength;
//
//	// get polygon centers
//	for (int j = 0; j < erdata->getRelationshipNum(); j++)
//	{
//		const Relationship* r = erdata->getConstRelationship(j);
//		vector<int> entityindices = r->getEntityIndices();
//		double num = entityindices.size();
//
//		adouble cx = 0;
//		adouble cy = 0;
//		adouble max_radius = 0.0;
//
//		if (entityindices.size() == 1)
//		{
//			int mon_index = erdata->getMonogonIndex(r);
//			int ent_index_x = 2 * entityindices[0];
//			int ent_index_y = ent_index_x + 1;
//
//			adouble angle = x[NV + mon_index];
//			adouble x0 = x[ent_index_x];
//			adouble y0 = x[ent_index_y];
//
//			cx = x0 + a * cos(angle);
//			cy = y0 + a * sin(angle);
//
//			max_radius = (cx - x0) * (cx - x0) + (cy - y0) * (cy - y0);
//		}
//		else
//		{
//			for (int i = 0; i < num; i++)
//			{
//				int ent_index_x = 2 * entityindices[i];
//				int ent_index_y = ent_index_x + 1;
//
//				adouble x0 = x[ent_index_x];
//				adouble y0 = x[ent_index_y];
//
//				cx += x0;
//				cy += y0;
//			}
//
//			cx /= num;
//			cy /= num;
//
//			for (int i = 0; i < num; i++)
//			{
//				int ent_index_x = 2 * entityindices[i];
//				int ent_index_y = ent_index_x + 1;
//
//				adouble x0 = x[ent_index_x];
//				adouble y0 = x[ent_index_y];
//
//				adouble radius = (cx - x0) * (cx - x0) + (cy - y0) * (cy - y0);
//				if (radius > max_radius)
//					max_radius = radius;
//			}
//		}
//
//		centers.push_back(pair<adouble, adouble>(cx, cy));
//		radii.push_back(max_radius);
//	}
//
//	// accumulate separation values
//	for (int j = 0; j < erdata->getRelationshipNum(); j++)
//	{
//		for (int i = j + 1; i < erdata->getRelationshipNum(); i++)
//		{
//			if (erdata->isTwoResSame(j, i))
//				continue;
//
//			mytype dir_x = centers[j].first - centers[i].first;
//			mytype dir_y = centers[j].second - centers[i].second;
//			mytype dist_sq = (dir_x * dir_x + dir_y * dir_y);
//
//			//for gradient calculation
//			if (dist_sq < gmainwindow->endiv_threshold)
//				dist_sq = gmainwindow->endiv_threshold;
//
//			int cardj = erdata->getRelationshipCard(j);
//			int cardi = erdata->getRelationshipCard(i);
//
//			double scale = gmainwindow->PS_distance_buffer;
//
//			vector<int> sharedVertsId = erdata->getSharedEntityIndices(j, i);
//			int common_vert_num = sharedVertsId.size();
//
//			mytype energy = 0.0;
//			mytype diff_of_sq = 0.0;
//			mytype sq_of_diff = 0.0;
//			double scalarK = gmainwindow->PS_exponent_scalar;
//
//			mytype Rj, Ri, Li, Lj;
//
//			Rj = radii[j];
//			Ri = radii[i];
//			Li = calc_expected_L(erdata, i);
//			Lj = calc_expected_L(erdata, j);
//
//			mytype expectdist = Rj + Ri + scale * 0.5 * (Lj + Li);
//
//			if (dist_sq < expectdist * expectdist) {
//				diff_of_sq = expectdist * expectdist - dist_sq;
//				sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
//			}
//			else {
//				diff_of_sq = 0.0;
//				sq_of_diff = 0.0;
//			}
//
//			if (flag_PSType == PSTYPE::PS_Expect_Exponential_DiffOfSq)
//				energy += exp(scalarK * diff_of_sq) - 1.0;
//			else if (flag_PSType == PSTYPE::PS_Expect_DiffOfSq)
//				energy = diff_of_sq;
//			else if (flag_PSType == PSTYPE::PS_Expect_Exponential_SqOfDiff)
//				energy += exp(scalarK * sq_of_diff) - 1.0;
//			else if (flag_PSType == PSTYPE::PS_Expect_SqOfDiff)
//				energy = sq_of_diff;
//
//
//			if (std::isinf(energy.value()) || energy.value() != energy.value()) {
//				gmainwindow->flag_stopOptimization = true;
//				LogConsole::writeError("energy invalid");
//			}
//
//			y += energy;
//		}
//	}
//
//	return y;
//}

template<class mytype>
mytype EnergyEval_Subgraph_ContinousTopology(ERData* erdata, const std::vector<int> relist, const int N, const mytype* x)
{
	using adept::adouble;
	mytype energy = 0;

	auto calcGapVertsDistSq = [&](GapVerts gapVertSeries) 
	{
		mytype dist = 0;
		int num = gapVertSeries.size();
		for (int i = 0; i < num - 1; i++) {

			int vi = gapVertSeries[i];
			int vi_n = gapVertSeries[i + 1];

			mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
			mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
			mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
			mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

			mytype ux = vindexXi - vindexXi_n;
			mytype uy = vindexYi - vindexYi_n;

			mytype dist_sq = (ux) * (ux)+(uy) * (uy);

			dist += sqrt(dist_sq);
		}
		return dist;
	};

	auto calcGapVertSeriesDist = [calcGapVertsDistSq](vector<GapVerts> gapVertSeries) {
		vector<mytype> gapVertSeriesDist;
		int num = gapVertSeries.size();
		for (int i = 0; i < num; i++) {
			mytype dist = calcGapVertsDistSq(gapVertSeries[i]);
			gapVertSeriesDist.push_back(dist);
		}
		return gapVertSeriesDist;
	};

	for (int i : relist)
	{
		if (erdata->getRelationshipCard(i) == 1)
			continue;

		RE* rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<mytype>(erdata, rei, N, x);
		int cardi = vindices_i.size();
		int sizei = erdata->getRelationshipIncidence(rei);
		int adjNum = rei->getAdjacentRelationships().size();
		double L = gmainwindow->expectedLength * sin(M_PI / cardi) / sin(M_PI / sizei);

		for (int j = 0; j < adjNum; j++)
		{
			RE* rei_adj = rei->getAdjacentRelationship(j);
			int rei_adj_index = rei_adj->getIndex();
			vector<int> vindices_j = getRelationshipVertIndices<mytype>(erdata, rei_adj, N, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			if (common_vert_num < 2)
				continue;

			vector<GapVerts> gapVertsList = getGapVertSeries(commonIndices, vindices_i);
			vector<mytype> gapVertSeriesDist = calcGapVertSeriesDist(gapVertsList);

			if (common_vert_num == 2)
			{
				double expGapCount = (double)(cardi - 2);
				double expectedDist = L * (expGapCount + 1);

				assert(gapVertSeriesDist.size() == 2);

				mytype min = gapVertSeriesDist[0];
				mytype max = gapVertSeriesDist[1];
				if (min > max)
					std::swap(min, max);

				energy += (max - expectedDist) * (max - expectedDist) + (min - L) * (min - L);

			}
			else if (common_vert_num > 2)
			{
				double expectedDist = L * double(cardi / common_vert_num);

				for (int k = 0; k < gapVertSeriesDist.size(); k++) {
					energy += (expectedDist - gapVertSeriesDist[k]) * (expectedDist - gapVertSeriesDist[k]);
				}
			}
		}
	}
	return energy;
}

template<class mytype>
mytype EnergyEval_Subgraph_IntersectionAspectRatio(ERData* erdata, const std::vector<int> relist, const int N, const mytype* x)
{
	using adept::adouble;
	mytype y = 0;

	for (int i : relist)
	{
		if (erdata->getRelationshipCard(i) == 1)
			continue;

		RE* rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<mytype>(erdata, rei, N, x);
		int adjNum = rei->getAdjacentRelationships().size();

		for (int j = 0; j < adjNum; j++)
		{
			RE* rei_adj = rei->getAdjacentRelationship(j);
			vector<int> vindices_j = getRelationshipVertIndices<mytype>(erdata, rei_adj, N, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			assert(common_vert_num > 0);

			if (common_vert_num <= 2)
				continue;

			{
				vector<Vec3> intersectlayout(commonIndices.size());
				for (int i = 0; i < commonIndices.size(); i++) {
					int vi = commonIndices[i];
					mytype vi_x, vi_y;
					vi_x = get_vi_x_value(vi, erdata, x);
					vi_y = get_vi_y_value(vi, erdata, x);
					intersectlayout[i] = Vec3(vi_x, vi_y, 0);
				}

				commonIndices = Relationship::getSortedVerticesFromLocs(commonIndices, intersectlayout);
			}

			mytype energy = calc_PolygonAspectRatio<mytype>(erdata, N, x, commonIndices);

			y += energy;
		}
	}

	return y;
}

template<class mytype>
mytype EnergyEval_Subgraph_AspectRatio(const ERData* erdata, const std::vector<int> relist, const int N, const mytype* x)
{
	using adept::adouble;
	mytype y = 0;

	for (int j : relist)
	{
		if (erdata->getRelationshipCard(j) < 3)
			continue;

		/*for regularize operation*/
		if (!erdata->isRelationshipOptimizable(j))
			continue;

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<mytype>(erdata, j, N, x);

		mytype energy = calc_PolygonAspectRatio<mytype>(erdata, N, x, entityindices);

		y += energy;
	}

	return y;
}

template<class mytype>
mytype EnergyEval_Subgraph_LengthDiff(const ERData* erdata, const std::vector<int> relist, const int N, const mytype* x)
{
	using adept::adouble;
	mytype y = 0;

	for (int j : relist)
	{
		if (erdata->getRelationshipCard(j) == 1)
			continue;

		const Relationship* re = erdata->getConstRelationship(j);
		int rcard = erdata->getRelationshipIncidence(re);

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<mytype>(erdata, j, N, x);
		int num = entityindices.size();

		double L = gmainwindow->expectedLength * sin(M_PI / num) / sin(M_PI / rcard);

		mytype lenDiff = 0;

		if (rcard > 1)
		{
			for (int i = 0; i < entityindices.size(); i++) {

				int vi = entityindices[i];
				int vi_n = entityindices[(i + 1) % num];

				mytype vindexXi = get_vi_x_value<mytype>(vi, erdata, x);
				mytype vindexXi_n = get_vi_x_value<mytype>(vi_n, erdata, x);
				mytype vindexYi = get_vi_y_value<mytype>(vi, erdata, x);
				mytype vindexYi_n = get_vi_y_value<mytype>(vi_n, erdata, x);

				mytype ux = vindexXi - vindexXi_n;
				mytype uy = vindexYi - vindexYi_n;

				mytype dist_sq = (ux) * (ux)+(uy) * (uy);

				mytype dist = sqrt(dist_sq);
				lenDiff += (dist - L) * (dist - L);
			}
		}
		else
		{
			continue;
		}

		if (rcard == 2)
			lenDiff *= gmainwindow->weight_DL * lenDiff;

		y += lenDiff;
	}

	return y;
}

template<class mytype>
mytype EnergyEval_Subgraph_Separation_Local(const ERData* erdata, const std::vector<int> relist, const int NV, const int NM, const mytype* x)
{
	mytype y = 0;

	int setcount = relist.size();
	vector<pair<mytype, mytype>> centers;
	centers.reserve(setcount);

	// get polygon centers
	for (int idx : relist)
	{
		const Relationship* re = erdata->getConstRelationship(idx);
		vector<int> entityindices = re->getEntityIndices();
		double num = entityindices.size();

		mytype cx = 0;
		mytype cy = 0;

		if (num == 1)
		{
			int mi = erdata->getMonogonIndex(re);
			int vi = entityindices[0];

			mytype rot = get_mi_rot_value<mytype>(mi, NV, erdata, x);
			mytype x0 = get_vi_x_value<mytype>(vi, erdata, x);
			mytype y0 = get_vi_y_value<mytype>(vi, erdata, x);

			int card = erdata->getRelationshipIncidence(re);
			double r = 0.5 * gmainwindow->expectedLength;
			if (card > 2)
				r /= sin(M_PI / card);

			cx = x0 + r * cos(rot);
			cy = y0 + r * sin(rot);
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				int vi = entityindices[i];

				mytype x0 = get_vi_x_value<mytype>(vi, erdata, x);
				mytype y0 = get_vi_y_value<mytype>(vi, erdata, x);

				cx += x0;
				cy += y0;
			}
			cx /= num;
			cy /= num;
		}

		centers.push_back(pair<mytype, mytype>(cx, cy));
	}

	// accumulate separation values
	for (int j = 0; j < setcount; j++)
	{
		int idxj = relist[j];
		const Relationship* rej = erdata->getConstRelationship(idxj);
		int adjNum = rej->getAdjacentRelationships().size();

		for (int i = 0; i < adjNum; i++)
		{
			const Relationship* rei = rej->getAdjacentRelationship(i);
			int idxi = rei->getIndex();
			int idxi_in_list = std::distance(relist.begin(), std::find(relist.begin(), relist.end(), idxi));
			Vec3 centeri = rei->getConstCenter();

			mytype dir_x, dir_y;
			if (idxi_in_list == relist.size())
			{
				dir_x = centers[j].first - centeri.x;
				dir_y = centers[j].second - centeri.y;
			}
			else
			{
				dir_x = centers[j].first - centers[idxi_in_list].first;
				dir_y = centers[j].second - centers[idxi_in_list].second;
			}
			
			mytype dist_sq = (dir_x * dir_x + dir_y * dir_y);
			if (dist_sq < gmainwindow->endiv_threshold) //for gradient calculation
				dist_sq = gmainwindow->endiv_threshold;

			int cardj = erdata->getRelationshipIncidence(rej);
			int cardi = erdata->getRelationshipIncidence(rei);

			double scale = gmainwindow->PS_distance_buffer;

			vector<int> sharedVertsId = erdata->getSharedEntityIndices(idxj,idxi);
			int common_vert_num = sharedVertsId.size();

			mytype energy = 0.0;
			mytype diff_of_sq = 0.0;
			mytype sq_of_diff = 0.0;
			double scalarK = gmainwindow->PS_exponent_scalar;

			double Rj, Ri, L;
			L = gmainwindow->expectedLength;
			Rj = 0.5 * L;
			Ri = 0.5 * L;
			if (cardj > 2)
				Rj /= sin(M_PI / cardj);
			if (cardi > 2)
				Ri /= sin(M_PI / cardi);

			if (common_vert_num == 0)
			{
				mytype expectdist = Rj + Ri + scale * L;

				if (dist_sq < expectdist * expectdist) {
					diff_of_sq = expectdist * expectdist - dist_sq;
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				}
				else {
					diff_of_sq = 0.0;
					sq_of_diff = 0.0;
				}
			}

			if (common_vert_num == 1)
			{
				double anglei = 0;
				double anglej = 0;
				if (cardi > 2)
					anglei = (cardi - 2) * M_PI / cardi;
				if (cardj > 2)
					anglej = (cardj - 2) * M_PI / cardj;
				double gamma_scale = gmainwindow->PS_angle_buffer;

				double gamma_scale_rad = gamma_scale / 180.0 * M_PI;
				double gamma_rad = gamma_scale_rad + (anglei + anglej) * 0.5;

				int sharedNodeID = sharedVertsId[0];

				mytype vx = get_vi_x_value<mytype>(sharedNodeID, erdata, x);
				mytype vy = get_vi_y_value<mytype>(sharedNodeID, erdata, x);

				mytype dir_j_x = centers[j].first - vx;
				mytype dir_j_y = centers[j].second - vy;

				mytype dj_sq = (dir_j_x * dir_j_x + dir_j_y * dir_j_y);
				mytype dj = (dj_sq == 0.0) ? (mytype)0.0 : (mytype)sqrt(dj_sq);

				mytype dir_i_x, dir_i_y;
				if (idxi_in_list == relist.size())
				{
					dir_i_x = centeri.x - vx;
					dir_i_y = centeri.y - vy;
				}
				else
				{
					dir_i_x = centers[idxi_in_list].first - vx;
					dir_i_y = centers[idxi_in_list].second - vy;
				}

				mytype di_sq = (dir_i_x * dir_i_x + dir_i_y * dir_i_y);
				mytype di = (di_sq == 0.0) ? (mytype)0.0 : (mytype)sqrt(di_sq);

				mytype mindistsq = dj * dj + di * di - 2 * dj * di * cos(gamma_rad);

				if (dist_sq < mindistsq) {
					diff_of_sq = mindistsq - dist_sq;
					sq_of_diff = dist_sq + mindistsq - 2 * sqrt(mindistsq) * sqrt(dist_sq);
				}
				else {
					diff_of_sq = 0.0;
					sq_of_diff = 0.0;
				}
			}

			if (common_vert_num == 2)
			{
				mytype ri, rj;	// inscribed radius
				if (cardi == 2)
					ri = 0.0;
				else
					ri = sqrt(Ri * Ri - 0.25 * L * L);
				if (cardj == 2)
					rj = 0.0;
				else
					rj = sqrt(Rj * Rj - 0.25 * L * L);

				mytype expectdist = ri + rj;

				if (dist_sq < expectdist * expectdist) {
					diff_of_sq = expectdist * expectdist - dist_sq;
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				}
				else {
					diff_of_sq = 0.0;
					sq_of_diff = 0.0;
				}
			}

			if (flag_PSType == PSTYPE::PS_Expect_Exponential_DiffOfSq)
				energy += exp(scalarK * diff_of_sq) - 1.0;
			else if (flag_PSType == PSTYPE::PS_Expect_DiffOfSq)
				energy = diff_of_sq;
			else if (flag_PSType == PSTYPE::PS_Expect_Exponential_SqOfDiff)
				energy += exp(scalarK * sq_of_diff) - 1.0;
			else if (flag_PSType == PSTYPE::PS_Expect_SqOfDiff)
				energy = sq_of_diff;


			if (std::isinf(energy.value()) || energy.value() != energy.value()) {
				gmainwindow->flag_stopOptimization = true;
				LogConsole::writeError("energy invalid");
			}

			y += energy;
		}
	}

	if (y > 0)
		return y;
	else
		return 0;
}

template<class mytype>
mytype EnergyEval_Subgraph_Separation_Global(const ERData* erdata, const std::vector<int> relist, const int NV, const int NM, const mytype* x)
{
	using adept::adouble;
	mytype y = 0;

	int recount = erdata->getRelationshipNum();
	int setcount = relist.size();
	vector<pair<mytype, mytype>> centers;
	centers.reserve(recount);

	// get polygon centers
	for (int j = 0; j < recount; j++)
	{
		const Relationship* re = erdata->getConstRelationship(j);
		vector<int> entityindices = re->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;

		for (int i = 0; i < num; i++)
		{
			int vi = entityindices[i];

			adouble x0 = get_vi_x_value<mytype>(vi, erdata, x);
			adouble y0 = get_vi_y_value<mytype>(vi, erdata, x);

			cx += x0;
			cy += y0;
		}

		if (num == 1)
		{
			int mi = erdata->getMonogonIndex(re);
			adouble rot = get_mi_rot_value<mytype>(mi, NV, erdata, x);

			int card = erdata->getRelationshipIncidence(re);
			mytype r = 0.5 * gmainwindow->expectedLength;
			if (card > 2)
				r /= sin(M_PI / card);

			cx += r * cos(rot);
			cy += r * sin(rot);
		}
		else
		{
			cx /= num;
			cy /= num;
		}

		centers.push_back(pair<adouble, adouble>(cx, cy));
	}

	// accumulate separation values
	for (int j : relist)
	{
		const Relationship* rej = erdata->getConstRelationship(j);

		for (int i = 0; i < recount; i++)
		{
			if (erdata->isTwoResSame(j, i))
				continue;

			const Relationship* rei = erdata->getConstRelationship(i);

			mytype dir_x = centers[j].first - centers[i].first;
			mytype dir_y = centers[j].second - centers[i].second;
			mytype dist_sq = (dir_x * dir_x + dir_y * dir_y);

			//for gradient calculation
			if (dist_sq < gmainwindow->endiv_threshold)
				dist_sq = gmainwindow->endiv_threshold;

			int cardj = erdata->getRelationshipIncidence(rej);
			int cardi = erdata->getRelationshipIncidence(rei);

			double scale = gmainwindow->PS_distance_buffer;

			vector<int> sharedVertsId = erdata->getSharedEntityIndices(j, i);
			int common_vert_num = sharedVertsId.size();

			mytype energy = 0.0;
			mytype sq_of_diff = 0.0;

			mytype Rj, Ri, L;
			L = gmainwindow->expectedLength;
			if (cardj == 1)
				Rj = Element::m_MonogonLength;
			else
				Rj = 0.5 * L;
			if (cardi == 1)
				Ri = Element::m_MonogonLength;
			else
				Ri = 0.5 * L;
			if (cardj > 2)
				Rj /= sin(M_PI / cardj);
			if (cardi > 2)
				Ri /= sin(M_PI / cardi);

			if (common_vert_num == 0)
			{
				mytype expectdist = Rj + Ri + scale * L;

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 1)
			{
				double anglei, anglej;
				if (cardi == 1)
					anglei = atan2(Element::m_MonogonSize, Element::m_MonogonLength);
				else if (cardi == 2)
					anglei = Element::m_DigonLensAngle / 2.0;
				else
					anglei = (cardi - 2) * M_PI / cardi;
				if (cardj == 1)
					anglej = atan2(Element::m_MonogonSize, Element::m_MonogonLength);
				else if (cardj == 2)
					anglej = Element::m_DigonLensAngle / 2.0;
				else
					anglej = (cardi - 2) * M_PI / cardj;

				double gamma_scale = gmainwindow->PS_angle_buffer;
				double gamma_scale_rad = gamma_scale / 180.0 * M_PI;
				double gamma_rad = gamma_scale_rad + (anglei + anglej) * 0.5;

				int sharedNodeID = sharedVertsId[0];

				mytype vx = get_vi_x_value<mytype>(sharedNodeID, erdata, x);
				mytype vy = get_vi_y_value<mytype>(sharedNodeID, erdata, x);

				mytype dir_j_x = centers[j].first - vx;
				mytype dir_j_y = centers[j].second - vy;

				mytype dj_sq = (dir_j_x * dir_j_x + dir_j_y * dir_j_y);
				mytype dj = (dj_sq == 0.0) ? (mytype)0.0 : (mytype)sqrt(dj_sq);

				mytype dir_i_x = centers[i].first - vx;
				mytype dir_i_y = centers[i].second - vy;

				mytype di_sq = (dir_i_x * dir_i_x + dir_i_y * dir_i_y);
				mytype di = (di_sq == 0.0) ? (mytype)0.0 : (mytype)sqrt(di_sq);

				mytype mindistsq = dj * dj + di * di - 2 * dj * di * cos(gamma_rad);

				if (dist_sq < mindistsq)
					sq_of_diff = dist_sq + mindistsq - 2 * sqrt(mindistsq) * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 2)
			{
				mytype ri, rj;	// inscribed radius
				if (cardi == 2)
					ri = 0.0;
				else
					ri = sqrt(Ri * Ri - 0.25 * L * L);
				if (cardj == 2)
					rj = 0.0;
				else
					rj = sqrt(Rj * Rj - 0.25 * L * L);

				mytype expectdist = ri + rj;

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			energy = sq_of_diff;

			if (std::isinf(energy.value()) || energy.value() != energy.value()) {
				gmainwindow->flag_stopOptimization = true;
				LogConsole::writeError("energy invalid");
			}

			y += energy;
		}
	}

	if (y > 0)
		return y;
	else
		return 0;
}

template<class mytype>
mytype EnergyEval_Subgraph_DualDist(ERData* pdata, ERData* ddata, const int pNV, const mytype* px, const int dNV, const mytype* dx)
{
	mytype y = 0;

	/*calc the distance between polygon centers and dual vertices*/
	for (int i = 0; i < pdata->getEntityNum(); i++)
	{
		Entity* en = pdata->getEntity(i);
		if (!en->flag_Optimizable)
			continue;
		Relationship* re = en->getDualRelationship();

		vector<int> entityindices = re->getEntityIndices();
		mytype num = entityindices.size();

		mytype ex = get_vi_x_value<mytype>(i, pdata, px);
		mytype ey = get_vi_y_value<mytype>(i, pdata, px);

		// calc polygon center
		mytype cx = 0;
		mytype cy = 0;
		
		if (num == 1)
		{
			int mi = ddata->getMonogonIndex(re);
			int vi = entityindices[0];

			mytype rot = get_mi_rot_value<mytype>(mi, dNV, ddata, dx);
			mytype x0 = get_vi_x_value<mytype>(vi, ddata, dx);
			mytype y0 = get_vi_y_value<mytype>(vi, ddata, dx);

			int card = ddata->getRelationshipIncidence(re);
			double r = 0.5 * gmainwindow->expectedLength;
			if (card > 2)
				r /= sin(M_PI / card);

			cx = x0 + r * cos(rot);
			cy = y0 + r * sin(rot);
		}
		else
		{
			for (int k = 0; k < entityindices.size(); k++)
			{
				int idx = entityindices[k];
				mytype dualx = get_vi_x_value<mytype>(idx, ddata, dx);
				mytype dualy = get_vi_y_value<mytype>(idx, ddata, dx);

				cx += dualx;
				cy += dualy;
			}
			cx /= num;
			cy /= num;
		}

		y += (ex - cx) * (ex - cx) + (ey - cy) * (ey - cy);
	}

	for (int i = 0; i < ddata->getEntityNum(); i++)
	{
		Entity* en = ddata->getEntity(i);
		if (!en->flag_Optimizable)
			continue;
		Relationship* re = en->getDualRelationship();

		vector<int> entityindices = re->getEntityIndices();
		mytype num = entityindices.size();

		mytype ex = get_vi_x_value<mytype>(i, ddata, dx);
		mytype ey = get_vi_y_value<mytype>(i, ddata, dx);

		// calc polygon center
		mytype cx = 0;
		mytype cy = 0;

		if (num == 1)
		{
			int mi = pdata->getMonogonIndex(re);
			int vi = entityindices[0];

			mytype rot = get_mi_rot_value<mytype>(mi, pNV, pdata, px);
			mytype x0 = get_vi_x_value<mytype>(vi, pdata, px);
			mytype y0 = get_vi_y_value<mytype>(vi, pdata, px);

			int card = pdata->getRelationshipIncidence(re);
			double r = 0.5 * gmainwindow->expectedLength;
			if (card > 2)
				r /= sin(M_PI / card);

			cx = x0 + r * cos(rot);
			cy = y0 + r * sin(rot);
		}
		else
		{
			for (int k = 0; k < entityindices.size(); k++)
			{
				int idx = entityindices[k];
				mytype dualx = get_vi_x_value<mytype>(idx, pdata, px);
				mytype dualy = get_vi_y_value<mytype>(idx, pdata, px);

				cx += dualx;
				cy += dualy;
			}
			cx /= num;
			cy /= num;
		}

		y += (ex - cx) * (ex - cx) + (ey - cy) * (ey - cy);
	}

	return y;
}




adept::adouble Evaluate_PolygonRegularity(const ERData* erdata, const int NV, const adept::adouble* x)
{
	using adept::adouble;
	adouble y = 0;

	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		if (erdata->getRelationshipCard(j) < 3)
			continue;

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<adouble>(erdata, j, NV, x);

		y += calc_PolygonAspectRatio<adouble>(erdata, NV, x, entityindices);
	}

	return y;
}

adept::adouble Evaluate_PolygonSideLength(const ERData* erdata, const int NV, const adept::adouble* x)
{
	using adept::adouble;
	adouble y = 0;

	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		if (erdata->getRelationshipCard(j) == 1)
			continue;

		vector<int> entityindices;
		entityindices = getRelationshipVertIndices<adouble>(erdata, j, NV, x);
		int card = entityindices.size();

		double L = gmainwindow->expectedLength;
		adouble lendiff = 0;
		for (int i = 0; i < entityindices.size(); i++)
		{
			int vi = entityindices[i];
			int vj = entityindices[(i + 1) % card];

			adouble vix = get_vi_x_value<adouble>(vi, erdata, x);
			adouble vjx = get_vi_x_value<adouble>(vj, erdata, x);
			adouble viy = get_vi_y_value<adouble>(vi, erdata, x);
			adouble vjy = get_vi_y_value<adouble>(vj, erdata, x);

			adouble dx = vix - vjx;
			adouble dy = viy - vjy;

			adouble dist_sq = (dx) * (dx)+(dy) * (dy);
			adouble dist = sqrt(dist_sq);
			lendiff += (dist - L) * (dist - L);
		}
		y += lendiff;
	}
	return y;
}

adept::adouble Evaluate_PolygonSeparation(const ERData* erdata, const int NV, const adept::adouble* x)
{
	using adept::adouble;
	adouble y = 0;

	int recount = erdata->getRelationshipNum();
	vector<Vec2> centers(recount);
	double a = Element::m_MonogonLength;

	// get polygon centers
	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		const Relationship* r = erdata->getConstRelationship(j);
		vector<int> entityindices = r->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;

		if (entityindices.size() == 1)
		{
			int mon_index = erdata->getMonogonIndex(r);
			int ent_index_x = 2 * entityindices[0];
			int ent_index_y = ent_index_x + 1;

			adouble angle = x[NV + mon_index];
			adouble x0 = x[ent_index_x];
			adouble y0 = x[ent_index_y];

			cx = x0 + (a / 2.0) * cos(angle);
			cy = y0 + (a / 2.0) * sin(angle);
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				int ent_index_x = 2 * entityindices[i];
				int ent_index_y = ent_index_x + 1;

				adouble x0 = x[ent_index_x];
				adouble y0 = x[ent_index_y];

				cx += x0;
				cy += y0;
			}

			cx /= num;
			cy /= num;
		}
		centers[j] = Vec2(cx, cy);
	}

	/*calculate the minimum distance between polygon center and the digon line segment*/
	auto calcDigonPolyDistSq = [&](int digonv1, int digonv2, int polyi) {
		adouble v1_X = get_vi_x_value<adouble>(digonv1, erdata, x);
		adouble v1_Y = get_vi_y_value<adouble>(digonv1, erdata, x);
		adouble dist1_x = v1_X - centers[polyi].x;
		adouble dist1_y = v1_Y - centers[polyi].y;
		adouble dist1_sq = (dist1_x * dist1_x + dist1_y * dist1_y);

		adouble v2_X = get_vi_x_value<adouble>(digonv2, erdata, x);
		adouble v2_Y = get_vi_y_value<adouble>(digonv2, erdata, x);
		adouble dist2_x = v2_X - centers[polyi].x;
		adouble dist2_y = v2_Y - centers[polyi].y;
		adouble dist2_sq = (dist2_x * dist2_x + dist2_y * dist2_y);

		adouble dist_v1_v2_sq = (v2_X - v1_X) * (v2_X - v1_X) + (v2_Y - v1_Y) * (v2_Y - v1_Y);

		adouble dist_min = 0.0;

		if (dist1_sq <= dist2_sq)
			dist_min = dist1_sq;
		else
			dist_min = dist2_sq;

		/*project (Pi - V1) on vector (V2 - V1)*/
		adouble dotp = (centers[polyi].x - v1_X) * (v2_X - v1_X) + (centers[polyi].y - v1_Y) * (v2_Y - v1_Y);
		adouble t = dotp / dist_v1_v2_sq;
		if (t < 1 && t >0) {
			adouble dist3_sq = ((v2_X - v1_X) * (v1_Y - centers[polyi].x) - (v1_X - centers[polyi].y) * (v2_Y - v1_Y)) *
				((v2_X - v1_X) * (v1_Y - centers[polyi].x) - (v1_X - centers[polyi].y) * (v2_Y - v1_Y)) / dist_v1_v2_sq;
			dist_min = dist3_sq;
		}

		return dist_min;
	};

	// accumulate separation values
	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		for (int i = j + 1; i < erdata->getRelationshipNum(); i++)
		{
			if (erdata->isTwoResSame(j, i))
				continue;

			adouble dir_x = centers[j].x - centers[i].x;
			adouble dir_y = centers[j].y - centers[i].y;
			adouble dist_sq = (dir_x * dir_x + dir_y * dir_y);

			int cardj = erdata->getRelationshipCard(j);
			int cardi = erdata->getRelationshipCard(i);

			double scale = gmainwindow->PS_distance_buffer;

			vector<int> sharedVertsId = erdata->getSharedEntityIndices(j, i);
			int common_vert_num = sharedVertsId.size();

			adouble energy = 0.0;
			//adouble diff_of_sq = 0.0;
			adouble sq_of_diff = 0.0;
			//double scalarK = gmainwindow->PS_exponent_scalar;

			adouble Rj, Ri, Li, Lj;

			if (cardj == 1)
			{
				Lj = calc_expected_L(erdata, j);
				Rj = a / 2.0;
			}
			else
			{
				Lj = calc_expected_L(erdata, j);
				Rj = 0.5 * Lj / sin(M_PI / cardj);
			}

			if (cardi == 1)
			{
				Li = calc_expected_L(erdata, i);
				Ri = a / 2.0;
			}
			else
			{
				Li = calc_expected_L(erdata, i);
				Ri = 0.5 * Li / sin(M_PI / cardi);
			}


			if (common_vert_num == 0)
			{
				adouble expectdist = Rj + Ri + scale * 0.5 * (Li + Lj);

				/*special handler when on polygon is a digon*/
				if (cardj == 2)
				{
					expectdist = Ri + scale * Li;
					vector<int>digon_indices = erdata->getRelationshipVertIndices(j);
					std::sort(digon_indices.begin(), digon_indices.end());
					assert(digon_indices.size() == 2);

					dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], i);
					assert(dist_sq.value() != 0.0);
				}
				else if (cardi == 2)
				{
					expectdist = Rj + scale * Lj;
					vector<int>digon_indices = erdata->getRelationshipVertIndices(i);
					std::sort(digon_indices.begin(), digon_indices.end());
					assert(digon_indices.size() == 2);

					dist_sq = calcDigonPolyDistSq(digon_indices[0], digon_indices[1], j);
					assert(dist_sq.value() != 0.0);
				}

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 1)
			{
				double anglei = 0;
				double anglej = 0;
				if (cardi > 2)
					anglei = (cardi - 2) * M_PI / cardi;
				if (cardj > 2)
					anglej = (cardj - 2) * M_PI / cardj;
				double gamma_scale = gmainwindow->PS_angle_buffer;

				double gamma_scale_rad = gamma_scale / 180.0 * M_PI;
				double gamma_rad = gamma_scale_rad + (anglei + anglej) * 0.5;

				int sharedNodeID = sharedVertsId[0];

				adouble vx = get_vi_x_value<adouble>(sharedNodeID, erdata, x);
				adouble vy = get_vi_y_value<adouble>(sharedNodeID, erdata, x);

				adouble dir_j_x = centers[j].x - vx;
				adouble dir_j_y = centers[j].y - vy;

				adouble dj_sq = (dir_j_x * dir_j_x + dir_j_y * dir_j_y);
				adouble dj = sqrt(dj_sq);

				adouble dir_i_x = centers[i].x - vx;
				adouble dir_i_y = centers[i].y - vy;

				adouble di_sq = (dir_i_x * dir_i_x + dir_i_y * dir_i_y);
				adouble di = sqrt(di_sq);

				adouble mindistsq = dj * dj + di * di - 2 * dj * di * cos(gamma_rad);

				if (dist_sq < mindistsq)
					sq_of_diff = dist_sq + mindistsq - 2 * sqrt(mindistsq) * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}

			if (common_vert_num == 2) {

				if (cardj == 2 || cardi == 2) {
					continue;
				}

				adouble L = 0.5 * (Li + Lj);
				adouble width_lens = (Rj - sqrt(Rj * Rj - 0.25 * L * L)) + (Ri - sqrt(Ri * Ri - 0.25 * L * L));
				adouble expectdist = Rj + Ri - width_lens;

				if (dist_sq < expectdist * expectdist)
					sq_of_diff = expectdist * expectdist + dist_sq - 2 * expectdist * sqrt(dist_sq);
				else
					sq_of_diff = 0.0;
			}
			
			energy = sq_of_diff;
			y += energy;
		}
	}
	return y;
}

adept::adouble Evaluate_IntersectionRegularity(ERData* erdata, const int NV, const adept::adouble* x)
{
	using adept::adouble;
	adouble y = 0;

	for (int i = 0; i < erdata->getRelationshipNum(); i++)
	{
		Relationship* rei = erdata->getRelationship(i);
		vector<int> vindices_i = getRelationshipVertIndices<adouble>(erdata, rei, NV, x);
		int adjNum = rei->getAdjacentRelationships().size();
		int cardi = vindices_i.size();

		for (int j = 0; j < adjNum; j++)
		{
			RE* rei_adj = rei->getAdjacentRelationship(j);
			vector<int> vindices_j = getRelationshipVertIndices<adouble>(erdata, rei_adj, NV, x);

			vector<int> commonIndices = getCommonIndices(vindices_i, vindices_j);
			int common_vert_num = commonIndices.size();

			if (common_vert_num <= 2)
				continue;

			{
				vector<Vec3> intersectlayout(commonIndices.size());
				for (int i = 0; i < commonIndices.size(); i++) {
					int vi = commonIndices[i];
					adouble vi_x, vi_y;
					vi_x = get_vi_x_value(vi, erdata, x);
					vi_y = get_vi_y_value(vi, erdata, x);
					intersectlayout[i] = Vec3(vi_x, vi_y, 0);
				}

				commonIndices = Relationship::getSortedVerticesFromLocs(commonIndices, intersectlayout);
			}

			adouble energy = calc_PolygonAspectRatio<adouble>(erdata, NV, x, commonIndices);

			y += energy;
		}
	}

	return y;
}

adept::adouble Evaluate_PolygonOverlap(const ERData* erdata, const int NV, const int NM, const adept::adouble* x, OVERLAPTYPE type)
{
	using adept::adouble;
	adouble y = 0;

	int recount = erdata->getRelationshipNum();
	vector<Vec2> centers(recount);
	vector<adouble> radii(recount);
	double b = Element::m_MonogonSize;
	double a = Element::m_MonogonLength;

	// get polygon centers and radii
	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		const Relationship* r = erdata->getConstRelationship(j);
		vector<int> entityindices = r->getEntityIndices();
		double num = entityindices.size();

		adouble cx = 0;
		adouble cy = 0;
		adouble max_radius = 0.0;

		if (entityindices.size() == 1)
		{
			int mon_index = erdata->getMonogonIndex(r);
			int ent_index_x = 2 * entityindices[0];
			int ent_index_y = ent_index_x + 1;

			adouble angle = x[NV + mon_index];
			adouble x0 = x[ent_index_x];
			adouble y0 = x[ent_index_y];

			cx = x0 + (a / 2.0) * cos(angle);
			cy = y0 + (a / 2.0) * sin(angle);
			max_radius = (a / 2.0);
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				int ent_index_x = 2 * entityindices[i];
				int ent_index_y = ent_index_x + 1;

				adouble x0 = x[ent_index_x];
				adouble y0 = x[ent_index_y];

				cx += x0;
				cy += y0;
			}

			cx /= num;
			cy /= num;

			for (int i = 0; i < num; i++)
			{
				int ent_index_x = 2 * entityindices[i];
				int ent_index_y = ent_index_x + 1;

				adouble x0 = x[ent_index_x];
				adouble y0 = x[ent_index_y];

				adouble radius = sqrt((cx - x0) * (cx - x0) + (cy - y0) * (cy - y0));
				if (radius > max_radius)
					max_radius = radius;
			}
		}

		centers[j] = Vec2(cx, cy);
		radii[j] = max_radius;
	}

	// accumulate overlapping polygon pairs
	for (int j = 0; j < erdata->getRelationshipNum(); j++)
	{
		for (int i = j + 1; i < erdata->getRelationshipNum(); i++)
		{
			const Relationship* poly1 = erdata->getConstRelationship(j);
			const Relationship* poly2 = erdata->getConstRelationship(i);
			if (type == OVERLAPTYPE::AREA)
				y += calc_PolygonOverlap(erdata, NV, NM, x, centers, radii, poly1, poly2);
			else if (type == OVERLAPTYPE::COUNT)
				y += test_PolygonOverlap(erdata, NV, NM, x, centers, radii, poly1, poly2);
		}
	}

	return y;
}



// Energy Evaluations

LayoutEnergy<double> evaluate_energy(ERData* erdata, int NV, int NM, const double* x, double* g)
{
	gmainwindow->increaseEnergyEvalCount();

	LayoutEnergy<double> res;
	using adept::adouble;
	adept::Stack stack;

	adouble* x_input = new adouble[NV + NM];
	for (int i = 0; i < NV + NM; i++) 
	{
		x_input[i] = x[i];
		if (std::isnan(x[i]))
		{
			LogConsole::writeConsole("[NOTE]nan x value");
			return res;
		}
	}

	stack.new_recording();	// start recording
	adouble y = 0;

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;

	adouble PS = 0;
	adouble AR = 0;
	adouble AD = 0;
	adouble TO = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP
	
	/*Note that the order of the energy calculation affect the final results*/
	if (wPS > 0) 
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		PS = EnergyEval_Separation(erdata, NV, NM, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}
	
	if (wAR > 0) 
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AR = EnergyEval_AspectRatio<adouble>(erdata, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}
	
	if (wAD > 0) 
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AD = EnergyEval_LengthDiff<adouble>(erdata, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wTO > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		TO = EnergyEval_IntersectionAspectRatio<adouble>(erdata, NV, x_input);
		TO += EnergyEval_ContinousTopology<adouble>(erdata, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	y = wAD * AD + wAR * AR + wPS * PS + wTO * TO;

	// if (flag_showOptimizationInfo)
	// 	LogConsole::writeConsole("[Energy]%.8e", y.value());

	if (g != NULL) 
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function
		try 
		{
			stack.compute_adjoint();					// Run the adjoint algorithm
		}
		catch (exception& e) 
		{
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < (NV+NM); i++) 
		{
			g[i] = x_input[i].get_gradient();
			//LogConsole::writeConsole("index: %d, gradient %.2f", i, g[i]);
			assert(!std::isnan(g[i]));
			assert(!std::isinf(g[i]));
		}
	}

	delete[] x_input;

	res.wAR = wAR;
	res.wPS = wPS;
	res.wAD = wAD;
	res.wTO = wTO;

	res.AR = AR.value();
	res.PS = PS.value();
	res.AD = AD.value();
	res.TO = TO.value();

	res.geometry = res.wPS * res.PS + res.wDD * res.DD + res.wAR * res.AR
		+ res.wAD * res.AD + res.wOC * res.OC + res.wNS * res.NS;

	res.total = res.geometry + res.TO * res.wTO;


	assert(y.value() == y.value());
	assert(!std::isinf(y.value()));

	return res;
}

LayoutEnergy<double> dual_evaluate_energy(ERData* erdata, ERData* dualerdata, int pNV, int pNM, int dNV, int dNM, const double* x, double* g)
{
	gmainwindow->increaseEnergyEvalCount();

	LayoutEnergy<double> res;
	using adept::adouble;
	adept::Stack stack;

	adouble* x_input = new adouble[pNV + pNM];
	for (int i = 0; i < pNV + pNM; i++)
	{
		x_input[i] = x[i];
		if (std::isnan(x[i]))
		{
			LogConsole::writeConsole("[NOTE]nan x value");
			return res;
		}
	}

	adouble* x_input_dual = new adouble[dNV + dNM];
	for (int i = 0; i < dNV + dNM; i++)
	{
		x_input_dual[i] = x[pNV + pNM + i];
		if (std::isnan(x[pNV + pNM + i]))
		{
			LogConsole::writeConsole("[NOTE]nan x value");
			return res;
		}
	}

	stack.new_recording();
	adouble y = 0;

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;
	double wDD = gmainwindow->weight_DD;

	double sumAR = 0;
	double sumAD = 0;
	double sumPS = 0;
	double sumTO = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	{
		adouble AR = 0;
		adouble AD = 0;
		adouble PS = 0;
		adouble TO = 0;

		// Note that the order of the energy calculation affect the final results
		if (wPS > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			PS = EnergyEval_Separation(erdata, pNV, pNM, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAR > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AR = EnergyEval_AspectRatio<adouble>(erdata, pNV, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAD > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AD = EnergyEval_LengthDiff<adouble>(erdata, pNV, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wTO > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			TO = EnergyEval_IntersectionAspectRatio<adouble>(erdata, pNV, x_input);
			TO += EnergyEval_ContinousTopology<adouble>(erdata, pNV, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		y += wAD * AD + wAR * AR + wPS * PS + wTO * TO;

		sumAR += AR.value();
		sumAD += AD.value();
		sumPS += PS.value();
		sumTO += TO.value();
	}

	{
		adouble AR = 0;
		adouble AD = 0;
		adouble PS = 0;
		adouble TO = 0;

		// Note that the order of the energy calculation affect the final results
		if (wPS > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			PS = EnergyEval_Separation(dualerdata, dNV, dNM, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAR > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AR = EnergyEval_AspectRatio<adouble>(dualerdata, dNV, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAD > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AD = EnergyEval_LengthDiff<adouble>(dualerdata, dNV, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wTO > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			TO = EnergyEval_IntersectionAspectRatio<adouble>(dualerdata, dNV, x_input_dual);
			TO += EnergyEval_ContinousTopology<adouble>(dualerdata, dNV, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		y += wAD * AD + wAR * AR + wPS * PS + wTO * TO;

		sumAR += AR.value();
		sumAD += AD.value();
		sumPS += PS.value();
		sumTO += TO.value();
	}

	adouble DD = 0;
	if (wDD > 0)
	{
		DD = EnergyEval_PolygonDualDist<adouble>(erdata, dualerdata, pNV, x_input, dNV, x_input_dual);
		y += wDD * DD;
	}

	if (g != NULL)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function
		try
		{
			stack.compute_adjoint();				// Run the adjoint algorithm
		}
		catch (exception& e)
		{
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < (pNV + pNM); i++)
		{
			g[i] = x_input[i].get_gradient();
			assert(!std::isnan(g[i]));
			assert(!std::isinf(g[i]));
		}

		for (int i = 0; i < (dNV + dNM); i++)
		{
			g[pNV + pNM + i] = x_input_dual[i].get_gradient();
			assert(!std::isnan(g[pNV + pNM + i]));
			assert(!std::isinf(g[pNV + pNM + i]));
		}

		delete[] x_input;
		delete[] x_input_dual;

		res.wAR = wAR;
		res.wPS = wPS;
		res.wAD = wAD;
		res.wDD = wDD;
		res.wTO = wTO;

		res.AR = sumAR;
		res.PS = sumPS;
		res.AD = sumAD;
		res.TO = sumTO;

		res.DD = DD.value();

		res.geometry = res.wPS * res.PS + res.wDD * res.DD + res.wAR * res.AR
			+ res.wAD * res.AD + res.wOC * res.OC + res.wNS * res.NS;

		res.total = res.geometry + res.TO * res.wTO;

		assert(y.value() == y.value());
		assert(!std::isinf(y.value()));

		return res;
	}
	return res;
}

LayoutEnergy<double> evaluate_polygon_energy(ERData *erdata, int N, const double* x, double* g) 
{	
	gmainwindow->increaseEnergyEvalCount();

	LayoutEnergy<double> res;

	using adept::adouble;						// Import adouble from adept
	adept::Stack stack;							// Where the derivative information is stored
	
	adouble *x_input = new adouble[N];
	for (int i = 0; i < N; i++) {
		x_input[i] = x[i];						// Initialize active input variables
		if (std::isnan(x[i]))
		{
			{
				LogConsole::writeConsole("[NOTE]nan x value");
				return res;
			}
		}
	}

	stack.new_recording();						// Start recording

	adouble y;                                 // Call version overloaded for adouble args

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;

	adouble PS = 0;
	adouble AR = 0;
	adouble AD = 0;
	adouble TO = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	/*Note that the order of the energy calculation affect the final results*/
	if (wPS > 0) {

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		PS = EnergyEval_PolygonSeparation<adouble>(erdata, N, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wAR > 0) {

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AR = EnergyEval_AspectRatio<adouble>(erdata, N, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wAD > 0) {

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AD = EnergyEval_LengthDiff<adouble>(erdata, N, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wTO > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		TO = EnergyEval_IntersectionAspectRatio<adouble>(erdata, N, x_input);
		TO += EnergyEval_ContinousTopology<adouble>(erdata, N, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	y = wAD * AD + wAR * AR + wPS * PS + wTO * TO;

	// if (flag_showOptimizationInfo)
	// 	LogConsole::writeConsole("[Energy]%.8e", y.value());

	if (g != NULL) {

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function
		try {
			stack.compute_adjoint();					// Run the adjoint algorithm
		}
		catch (exception& e) {
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < N; i++) {
			g[i] = x_input[i].get_gradient();

			assert(!std::isnan(g[i]));
			assert(!std::isinf(g[i]));

		}
	}
	
	delete[] x_input;

	res.wAR = wAR;
	res.wPS = wPS;
	res.wAD = wAD;
	res.wTO = wTO;

	res.AR = AR.value();
	res.PS = PS.value();
	res.AD = AD.value();
	res.TO = TO.value();

	res.geometry = res.wPS*res.PS + res.wDD*res.DD + res.wAR*res.AR
		+ res.wAD*res.AD + res.wOC*res.OC + res.wNS*res.NS;
	
	res.total = res.geometry + res.TO*res.wTO;


	assert(y.value() == y.value());
	assert(!std::isinf(y.value()));
	
	return res;
}

LayoutEnergy<double> dual_evaluate_polygon_energy(ERData *m_ERData, ERData *m_dualERData, int N1, int N2, const double* x, double* g) {

	LayoutEnergy<double> res;

	gmainwindow->increaseEnergyEvalCount();

	using adept::adouble;						// Import adouble from adept
	adept::Stack stack;							// Where the derivative information is stored

	adouble *x_input = new adouble[N1];
	for (int i = 0; i < N1; i++) {
		x_input[i] = x[i];						// Initialize active input variables
		if (std::isnan(x[i]))
		{
			{
				LogConsole::showError("[NOTE]nan x value");
				return res;
			}

		}
	}

	adouble *x_input_dual = new adouble[N2];
	for (int i = 0; i < N2; i++) {
		x_input_dual[i] = x[N1 + i];
		if (std::isnan(x[N1 + i]))
		{
			{
				LogConsole::showError("[NOTE]nan x value");
				return res;
			}
		}
	}

	stack.new_recording();						// Start recording

	adouble y = 0;                                 // Call version overloaded for adouble args

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;
	double wDD = gmainwindow->weight_DD;

	double sumAR = 0;
	double sumAD = 0;
	double sumPS = 0;
	double sumTO = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	{
		adouble AR = 0;
		adouble AD = 0;
		adouble PS = 0;
		adouble TO = 0;

		/*Note that the order of the energy calculation affect the final results*/
		if (wPS > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			PS = EnergyEval_PolygonSeparation<adouble>(m_ERData, N1, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAR > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AR = EnergyEval_AspectRatio<adouble>(m_ERData, N1, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAD > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AD = EnergyEval_LengthDiff<adouble>(m_ERData, N1, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wTO > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			TO = EnergyEval_IntersectionAspectRatio<adouble>(m_ERData, N1, x_input);
			TO += EnergyEval_ContinousTopology<adouble>(m_ERData, N1, x_input);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		y += wAD * AD + wAR * AR + wPS * PS + wTO * TO;

		sumAR += AR.value();
		sumAD += AD.value();
		sumPS += PS.value();
		sumTO += TO.value();

	}

	{

		adouble AR = 0;
		adouble AD = 0;
		adouble PS = 0;
		adouble TO = 0;
		//adouble OC = 0;
		//adouble NS = 0;

		/*Note that the order of the energy calculation affect the final results*/
		if (wPS > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			PS = EnergyEval_PolygonSeparation<adouble>(m_dualERData, N2, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAR > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AR = EnergyEval_AspectRatio<adouble>(m_dualERData, N2, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wAD > 0) {

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			AD = EnergyEval_LengthDiff<adouble>(m_dualERData, N2, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		if (wTO > 0)
		{

// #ifdef USE_OPENMP
// 			time1 = omp_get_wtime();
// #else
// 			time1 = clock();
// #endif

			TO = EnergyEval_IntersectionAspectRatio<adouble>(m_dualERData, N2, x_input_dual);
			TO += EnergyEval_ContinousTopology<adouble>(m_dualERData, N2, x_input_dual);

// #ifdef USE_OPENMP
// 			double time2 = omp_get_wtime();
// 			gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 			clock_t time2 = clock();
// 			gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		}

		y += wAD * AD + wAR * AR + wPS * PS + wTO * TO;

		sumAR += AR.value();
		sumAD += AD.value();
		sumPS += PS.value();
		sumTO += TO.value();

	}

	adouble DD = 0;
	if (wDD > 0) {
		DD = EnergyEval_PolygonDualDist<adouble>(m_ERData, m_dualERData, N1, x_input, N2, x_input_dual);
		y += wDD * DD;

		if (gmainwindow->flag_showOptimizationInfo)
			LogConsole::writeConsole("DD:[%.3f]*[%.8e]=%.8e", wDD, DD.value(), wDD * DD.value());
	}

	if (g != NULL) {

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function
		try {
			stack.compute_adjoint();					// Run the adjoint algorithm
		}
		catch (exception& e) {
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < N1; i++) {
			g[i] = x_input[i].get_gradient();
		}

		for (int i = 0; i < N2; i++) {
			g[N1 + i] = x_input_dual[i].get_gradient();
		}
	}

	delete[] x_input;
	delete[] x_input_dual;

	assert(y.value() == y.value());
	assert(!std::isinf(y.value()));

	res.wAR = wAR;
	res.wPS = wPS;
	res.wAD = wAD;
	res.wDD = wDD;
	res.wTO = wTO;

	res.AR = sumAR;
	res.PS = sumPS;
	res.AD = sumAD;
	res.TO = sumTO;

	res.DD = DD.value();

	res.geometry = res.wPS*res.PS + res.wDD*res.DD + res.wAR*res.AR
		+ res.wAD*res.AD + res.wOC*res.OC + res.wNS*res.NS;

	res.total = res.geometry + res.TO*res.wTO;

	return res;
}

LayoutEnergy<double> evaluate_subgraph_energy(ERData* erdata, int NV, int NM, const double* x, double* g)
{
	gmainwindow->increaseEnergyEvalCount();

	LayoutEnergy<double> res;
	using adept::adouble;
	adept::Stack stack;

	adouble* x_input = new adouble[NV + NM];
	for (int i = 0; i < NV + NM; i++)
		x_input[i] = x[i];

	std::set<int> relationship_set;
	for (int i = 0; i < erdata->getEntityNum(); i++)
	{
		if (erdata->isVertFree(i))
		{
			Entity* en = erdata->getEntity(i);
			for (Relationship* incident : en->getIncidentRelationships())
				relationship_set.insert(incident->getIndex());
		}
	}
	std::vector<int> relationships(relationship_set.begin(), relationship_set.end());

	stack.new_recording();	// start recording
	adouble y = 0;

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;

	adouble PS = 0;
	adouble AR = 0;
	adouble AD = 0;
	adouble TO = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	if (wPS > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		PS = EnergyEval_Subgraph_Separation_Global<adouble>(erdata, relationships, NV, NM, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wAR > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AR = EnergyEval_Subgraph_AspectRatio<adouble>(erdata, relationships, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wAD > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		AD = EnergyEval_Subgraph_LengthDiff<adouble>(erdata, relationships, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wTO > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		TO = EnergyEval_Subgraph_IntersectionAspectRatio<adouble>(erdata, relationships, NV, x_input);
		TO += EnergyEval_Subgraph_ContinousTopology<adouble>(erdata, relationships, NV, x_input);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	y = wAD * AD + wAR * AR + wPS * PS + wTO * TO;

	// if (flag_showOptimizationInfo)
	// 	LogConsole::writeConsole("[Energy]%.8e", y.value());

	// std::filebuf fb;
	// fb.open("statements.txt", std::ios::out);
	// std::ostream os(&fb);
	// stack.print_statements(os);
	// fb.close();

	if (g != NULL)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function

		try
		{
			stack.compute_adjoint();					// Run the adjoint algorithm
		}
		catch (exception& e)
		{
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < NV+NM; i++)
		{
			g[i] = x_input[i].get_gradient();
			//LogConsole::writeConsole("index: %d, gradient %.2f", i, g[i]);
			assert(!std::isnan(g[i]));
			assert(!std::isinf(g[i]));
		}
	}

	delete[] x_input;

	res.wAR = wAR;
	res.wPS = wPS;
	res.wAD = wAD;
	res.wTO = wTO;

	res.AR = AR.value();
	res.PS = PS.value();
	res.AD = AD.value();
	res.TO = TO.value();

	res.geometry = res.wPS * res.PS + res.wDD * res.DD + res.wAR * res.AR
		+ res.wAD * res.AD + res.wOC * res.OC + res.wNS * res.NS;

	res.total = res.geometry + res.TO * res.wTO;


	assert(y.value() == y.value());
	assert(!std::isinf(y.value()));

	return res;
}

LayoutEnergy<double> dual_evaluate_subgraph_energy(ERData* pdata, ERData* ddata, int pNV, int pNM, int dNV, int dNM, const double* x, double* g)
{
	gmainwindow->increaseEnergyEvalCount();

	LayoutEnergy<double> res;
	using adept::adouble;
	adept::Stack stack;

	adouble* pInput = new adouble[pNV + pNM];
	for (int i = 0; i < pNV + pNM; i++)
	{
		pInput[i] = x[i];
	}

	adouble* dInput = new adouble[dNV + dNM];
	for (int i = 0; i < dNV + dNM; i++)
	{
		dInput[i] = x[pNV + pNM + i];
	}

	std::set<int> pRelationship_set;
	for (int i = 0; i < pdata->getEntityNum(); i++)
	{
		if (pdata->isVertFree(i))
		{
			Entity* en = pdata->getEntity(i);
			for (Relationship* incident : en->getIncidentRelationships())
				pRelationship_set.insert(incident->getIndex());
		}
	}
	std::vector<int> pRelationships(pRelationship_set.begin(), pRelationship_set.end());

	std::set<int> dRelationship_set;
	for (int i = 0; i < ddata->getEntityNum(); i++)
	{
		if (ddata->isVertFree(i))
		{
			Entity* en = ddata->getEntity(i);
			for (Relationship* incident : en->getIncidentRelationships())
				dRelationship_set.insert(incident->getIndex());
		}
	}
	std::vector<int> dRelationships(dRelationship_set.begin(), dRelationship_set.end());

	stack.new_recording();
	adouble y = 0;

	double wAR = gmainwindow->weight_PR;
	double wAD = gmainwindow->weight_PA;
	double wPS = gmainwindow->weight_PS;
	double wTO = gmainwindow->weight_PI;
	double wDD = gmainwindow->weight_DD;

	adouble pPS = 0;
	adouble pAR = 0;
	adouble pAD = 0;
	adouble pTO = 0;

	adouble dPS = 0;
	adouble dAR = 0;
	adouble dAD = 0;
	adouble dTO = 0;

	adouble DD = 0;

// #ifdef USE_OPENMP
// 	double time1, time2;
// #else
// 	clock_t time1, time2;
// #endif // USE_OPENMP

	/*Note that the order of the energy calculation affect the final results*/

	if (wPS > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		pPS = EnergyEval_Subgraph_Separation_Global<adouble>(pdata, pRelationships, pNV, pNM, pInput);
		dPS = EnergyEval_Subgraph_Separation_Global<adouble>(ddata, dRelationships, dNV, dNM, dInput);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolySeparationTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolySeparationTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}


	if (wAR > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		pAR = EnergyEval_Subgraph_AspectRatio<adouble>(pdata, pRelationships, pNV, pInput);
		dAR = EnergyEval_Subgraph_AspectRatio<adouble>(ddata, dRelationships, dNV, dInput);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyRegularityTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyRegularityTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}


	if (wAD > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		pAD = EnergyEval_Subgraph_LengthDiff<adouble>(pdata, pRelationships, pNV, pInput);
		dAD = EnergyEval_Subgraph_LengthDiff<adouble>(ddata, dRelationships, dNV, dInput);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyAreaTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyAreaTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}


	if (wTO > 0)
	{

// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		pTO = EnergyEval_Subgraph_IntersectionAspectRatio<adouble>(pdata, pRelationships, pNV, pInput);
		pTO += EnergyEval_Subgraph_ContinousTopology<adouble>(pdata, pRelationships, pNV, pInput);
		dTO = EnergyEval_Subgraph_IntersectionAspectRatio<adouble>(ddata, dRelationships, dNV, dInput);
		dTO += EnergyEval_Subgraph_ContinousTopology<adouble>(ddata, dRelationships, dNV, dInput);

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addPolyIntersectionTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addPolyIntersectionTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

	}

	if (wDD > 0)
	{
		DD = EnergyEval_Subgraph_DualDist<adouble>(pdata, ddata, pNV, pInput, dNV, dInput);
	}


	y = wPS * (pPS + dPS) + wAD * (pAD + dAD) + wAR * (pAR + dAR) + wTO * (pTO + dTO) + wDD * DD;

	// if (flag_showOptimizationInfo)
	// 	LogConsole::writeBlueConsole("[Energy]%.8e", y.value());

	if (g != NULL)
	{
// #ifdef USE_OPENMP
// 		time1 = omp_get_wtime();
// #else
// 		time1 = clock();
// #endif

		y.set_gradient(1.0);						// Defines y as the objective function

		//std::filebuf fb;
		//fb.open("statements5.txt", std::ios::out);
		//std::ostream os(&fb);
		//stack.print_statements(os);
		//stack.print_gradients(os);
		//fb.close();

		try
		{
			stack.compute_adjoint();					// Run the adjoint algorithm
		}
		catch (exception& e)
		{
			LogConsole::showError("Adept Gradient Calculation Error!: %s", e.what());
		}

// #ifdef USE_OPENMP
// 		double time2 = omp_get_wtime();
// 		gmainwindow->addAdeptEvalTime(time2 - time1);
// #else
// 		clock_t time2 = clock();
// 		gmainwindow->addAdeptEvalTime((double)(time2 - time1) / CLOCKS_PER_SEC);
// #endif

		for (int i = 0; i < (pNV + pNM); i++)
		{
			g[i] = pInput[i].get_gradient();
			assert(!std::isnan(g[i]));
			assert(!std::isinf(g[i]));
		}

		for (int i = 0; i < (dNV + dNM); i++)
		{
			g[pNV + pNM + i] = dInput[i].get_gradient();
			assert(!std::isnan(g[pNV + pNM + i]));
			assert(!std::isinf(g[pNV + pNM + i]));
		}

		delete[] pInput;
		delete[] dInput;

		res.wAR = wAR;
		res.wPS = wPS;
		res.wAD = wAD;
		res.wDD = wDD;
		res.wTO = wTO;

		res.AR = pAR.value() + dAR.value();
		res.PS = pPS.value() + dPS.value();
		res.AD = pAD.value() + dAD.value();
		res.TO = pTO.value() + dTO.value();
		res.DD = DD.value();

		res.geometry = res.wPS * res.PS + res.wDD * res.DD + res.wAR * res.AR
			+ res.wAD * res.AD + res.wOC * res.OC + res.wNS * res.NS;

		res.total = res.geometry + res.TO * res.wTO;

		assert(y.value() == y.value());
		assert(!std::isinf(y.value()));

		return res;
	}
	return res;
}