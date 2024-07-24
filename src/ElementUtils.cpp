#include "ElementUtils.h"
#include "LogConsole.h"
#include <JPMath/JPMath.h>

/*check whether two relationships have same set of entities*/
bool isTwoRelationshipSame(const Relationship *re1, const Relationship *re2) {
	
	int card1 = re1->getCardinality();
	int card2 = re2->getCardinality();

	if (card1 != card2 || (card1 == 1 && card2 == 1))
		return false;

	vector<int> ei1 = re1->getEntityIndices();
	vector<int> ei2 = re2->getEntityIndices();

	/*skip two indentical polygons*/
	std::sort(ei1.begin(), ei1.end());
	std::sort(ei2.begin(), ei2.end());

	if (std::equal(ei1.begin(), ei1.end(), ei2.begin()))
		return true;
	else
		return false;
}

/*check whether two set of entities are same*/
bool isTwoEntityListSame(const vector<Entity *>& el1, const vector<Entity *>& el2) {

	if (el1.size() != el2.size())
		return false;

	for (int i = 0; i < el1.size(); i++) {
		Entity *tempEn = el1[i];

		bool inside = false;
		for (int k = 0; k < el2.size(); k++) {
			if (tempEn == el2[k]) {
				inside = true;
				break;
			}
		}

		if (!inside) {
			return false;
		}
	}

}

//check if relationship b is inside the target relationship t
bool isRelationshipInsideRelationship(Relationship *b, Relationship *t) {

	//if any entity in b is not in t, b is not inside t
	for (int i = 0; i < b->getIncidentEntities().size(); i++) {

		Entity *temp = b->getIncidentEntity(i);
		int inside = isInsideList<Entity>(temp, t->getIncidentEntities());

		if (inside == -1)
			return false;
	}

	return true;
}

bool isEntityInsideEntityList(Entity *en, const vector<Entity *>& el1) {

	for (int i = 0; i < el1.size(); i++) {
		if (en == el1[i])
			return true;
	}

	return false;
}

bool isRelationshipInsideRelationshipList(Relationship *re, const vector<Relationship *>& relist) {

	for (int i = 0; i < relist.size(); i++) {
		if (re == relist[i])
			return true;
	}

	return false;
}

vector<Entity*> getUniqueIncidentEntities(const vector<Relationship*>& relist)
{
	vector<Entity*> incidents;

	for (int i = 0; i < relist.size(); i++) {
		Relationship *re = relist[i];

		for (int j = 0; j < re->getIncidentEntities().size(); j++) {
			Entity* en = re->getIncidentEntity(j);

			if (isInsideList<Entity>(en, incidents) == -1)
				incidents.push_back(en);
		}

	}

	return incidents;
}

/*check whether entity connect relationship by a binary relationship*/
bool checkIfVertexConnectPolyByDigon(Entity *entity, Relationship* relationship) {

	for (int i = 0; i < entity->getIncidentRelationships().size(); i++) {

		Relationship *re = entity->getIncidentRelationship(i);

		if (re->getIncidentEntities().size() > 2)
			continue;

		Entity *e2 = (entity == re->getIncidentEntity(0)) ? re->getIncidentEntity(1) : re->getIncidentEntity(0);

		for (int j = 0; j < e2->getIncidentRelationships().size(); j++) {

			Relationship *cr = e2->getIncidentRelationship(j);

			if (cr == relationship)
				return true;
		}
	}

	return false;

}

/*check whether entity connect to relationship by a relationship*/
bool checkIfEntityConnectRelationship(Entity *entity, Relationship* relationship) {

	for (int i = 0; i < entity->getIncidentRelationships().size(); i++) {

		Relationship *re = entity->getIncidentRelationship(i);

		for (int j = 0; j < re->getIncidentEntities().size(); j++) {

			Entity *e2 = re->getIncidentEntity(j);

			if (e2 == entity)
				continue;

			for (int j = 0; j < e2->getIncidentRelationships().size(); j++) {

				Relationship *cr = e2->getIncidentRelationship(j);

				if (cr == relationship)
					return true;
			}
		}
	}

	return false;

}

bool checkIfTwoEntityAdjacent(Entity *entity1, Entity* entity2, Relationship* remove) {

	for (int i = 0; i < entity2->getIncidentRelationships().size(); i++) {
		Relationship *re = entity2->getIncidentRelationship(i);

		if (re == remove)
			continue;

		int inside = isInsideList<Entity>(entity1, re->getIncidentEntities());

		if (inside != -1) {
			return true;
		}
	}

	return false;
}

bool checkIfTwoRelationAdjacent(Relationship *b, Relationship *t, Entity* remove) {

	for (int i = 0; i < b->getIncidentEntities().size(); i++) {
		
		Entity *e = b->getIncidentEntity(i);

		if (e == remove)
			continue;

		int inside = isInsideList<Entity>(e, t->getIncidentEntities());

		if (inside != -1)
			return true;
	}

	return false;

}

/*get entities shared by polygon a and polygon b*/
vector<Entity*> getSharedEntities(Relationship * a, Relationship * b)
{
    vector<Entity*> elist;
    for (int i = 0; i < a->getIncidentEntities().size(); i++) {
        Entity *e = a->getIncidentEntity(i);
        int inside = isInsideList<Entity>(e, b->getIncidentEntities());
        if (inside != -1)
            elist.push_back(e);
    }

    return elist;
}

/*get number of entities shared by polygon a and polygon b*/
int getSharedEntitiesNum(Relationship* a, Relationship* b)
{
	int share_count = 0;
	for (int i = 0; i < a->getIncidentEntities().size(); i++) {
		Entity* e = a->getIncidentEntity(i);
		int inside = isInsideList<Entity>(e, b->getIncidentEntities());
		if (inside != -1)
			share_count++;
	}

	return share_count;
}

/*get relationships shared by node a and node b*/
vector<Relationship*> getSharedRelationships(Entity* a, Entity* b)
{
	vector<Relationship*> rlist;
	for (int i = 0; i < a->getIncidentRelationships().size(); i++) {
		Relationship* e = a->getIncidentRelationship(i);
		int inside = isInsideList<Relationship>(e, b->getIncidentRelationships());
		if (inside != -1)
			rlist.push_back(e);
	}

	return rlist;
}

/*get entities shared by polygon a and polygon b*/
int getSharedRelationshipsNum(Entity* a, Entity* b)
{
	int share_count = 0;
	for (int i = 0; i < a->getIncidentRelationships().size(); i++) {
		Relationship* e = a->getIncidentRelationship(i);
		int inside = isInsideList<Relationship>(e, b->getIncidentRelationships());
		if (inside != -1)
			share_count++;
	}

	return share_count;
}


/*get entities shared by polygon a and all polygons incident to entity e*/
vector<pair<Entity*, Entity*>> getConnectedSharedEntities(Relationship * a, Entity * e)
{
    int inside = isInsideList<Entity>(e, a->getIncidentEntities());
    if (inside == -1){
        LogConsole::writeConsole("entity is not incident to relationship");
        return vector<pair<Entity*, Entity*>>();
    }


    vector<pair<Entity*, Entity*>> result;

    for (int i = 0; i < e->getIncidentRelationships().size(); i++) {
        Relationship *r = e->getIncidentRelationship(i);
        vector<Entity*> sharedEntities = getSharedEntities(r, a);

        if (sharedEntities.size() != 2)
            continue;
        else{
            int res = isInsideList<Entity>(e, sharedEntities);
            if (res == -1) {
                LogConsole::writeConsole("the shared entities are wrong");
                return vector<pair<Entity*, Entity*>>();
            }
            result.push_back(pair<Entity*, Entity*>(sharedEntities[0], sharedEntities[1]));
        }
    }

    return result;
}

vector<Relationship*> getEdgelyAdjacentRelationshipIncidentToEntity(Relationship * a, Entity * e)
{


    vector<Relationship*> edgelyAdjacentPolys;

    int inside = isInsideList<Entity>(e, a->getIncidentEntities());
    if (inside == -1)
        return edgelyAdjacentPolys;

    set<Relationship*> results;

    for (int i = 0; i < a->getIncidentEntities().size(); i++) {

        Entity *aincident = a->getIncidentEntity(i);

        if (e == aincident)
            continue;

        vector<Entity*>tempelist;
        tempelist.push_back(e);
        tempelist.push_back(aincident);
        vector<Relationship*> temprlist = getRelationshipsSharingEntities(tempelist);

        if (temprlist.size() == 0){
            LogConsole::showError("wrong algorithm");
        }

        if (temprlist.size() == 1)
        {
            if (temprlist[0] != a)
                LogConsole::showError("wrong algorithm");
        }

        else {
            for (int j = 0; j < temprlist.size(); j++) {
                vector<Entity*>elist = getSharedEntities(temprlist[j], a);
                if(elist.size() == 2)
                    results.insert(temprlist[j]);
            }
        }
    }

    for (set<Relationship*>::iterator it = results.begin(); it != results.end(); ++it) {
        edgelyAdjacentPolys.push_back(*it);
    }

    return edgelyAdjacentPolys;
}

int getNumberOfAjacentRelationships(Entity * v1, Entity * v2)
{
	int sum = 0;
	for (int i = 0; i < v1->getIncidentRelationships().size(); i++) {

		Relationship *p1 = v1->getIncidentRelationship(i);

		int inside = isInsideList<Relationship>(p1, v2->getIncidentRelationships());

		if (inside != -1)
			sum++;
	}

	return sum;
}

vector<Relationship*> getRelationshipWithCardinality(const vector<Relationship*> &rlist, int i)
{
	 vector<Relationship*> res;

	 for (auto r : rlist) {
		 if (r->getCardinality() == i)
			 res.push_back(r);
	 }

	 return res;
}

int getMinPolygonCardinality(const vector<Relationship*> &res)
{
	int min = INT_MAX;

	for (auto r : res) {
		int c = r->getCardinality();
		if (c < min && c>2)
			min = c;
	}

	return min;
}

int getMaxPolygonCardinality(const vector<Relationship*> &res)
{
	int max = INT_MIN;

	for (auto r : res) {
		int c = r->getCardinality();
		if (c > max && c>2)
			max = c;
	}

	return max;
}


vector<Relationship*> getRelationshipsSharingEntities(vector<Entity*> elist)
{
    set<Relationship*> rlist;

    for (int i = 0; i < elist.size(); i++) {
        for (int j = 0; j < elist[i]->getIncidentRelationships().size(); j++) {
            Relationship *r = elist[i]->getIncidentRelationship(j);
            rlist.insert(r);
        }
    }
    
    vector<Relationship*>results;
    for (set<Relationship*>::iterator it = rlist.begin(); it != rlist.end(); ++it) {
        Relationship* r = (*it);

        bool res = true;
        for (int i = 0; i < elist.size(); i++) {
            int index = isInsideList<Entity>(elist[i], r->getIncidentEntities());
            if (index == -1) {
                res = false;
                break;
            }
        }

        if (res == true) {
            results.push_back(r);
        }
    }

    return results;

}

int getNumberOfAjacentRelationships(Entity * v1, Entity * v2, vector<Relationship*> &sharedRelationships)
{
    int sum = 0;
    for (int i = 0; i < v1->getIncidentRelationships().size(); i++) {

        Relationship *p1 = v1->getIncidentRelationship(i);

        int inside = isInsideList<Relationship>(p1, v2->getIncidentRelationships());

        if (inside != -1) {

			/*check whether v1 and v2 are consecutive in p1*/
			if (p1->isTwoEntityConsecutive(v1, v2))
			{
				sharedRelationships.push_back(p1);
				sum++;
			}
        }
    }

    return sum;
}


int getNumberOfAjacentMonogon(Entity * v1, vector<Relationship*> &sharedRelationships)
{
	int sum = 0;
	for (int i = 0; i < v1->getIncidentRelationships().size(); i++) {

		Relationship *p1 = v1->getIncidentRelationship(i);

		if (p1->getCardinality() == 1) {
			sharedRelationships.push_back(p1);
			sum++;
		}
	}

	return sum;
}

int getNumberOfAjacentDigons(Entity * v1, Entity * v2, vector<Relationship*>& sharedRelationships)
{
    int sum = 0;
    for (int i = 0; i < v1->getIncidentRelationships().size(); i++) {

        Relationship *p1 = v1->getIncidentRelationship(i);

        if (p1->getCardinality() != 2)
            continue;

        int inside = isInsideList<Relationship>(p1, v2->getIncidentRelationships());

        if (inside != -1) {
            sharedRelationships.push_back(p1);
            sum++;
        }
    }

    return sum;
}

bool compareRelationshipIndex(Relationship *i, Relationship *j) { return (i->getIndex()<j->getIndex()); }

void sortRelationships(vector<Relationship*> &RelationshipList) {
    sort(RelationshipList.begin(), RelationshipList.end(), compareRelationshipIndex);
}

int getOrderOfRelationship(const Relationship* r, vector<Relationship*> RelationshipList)
{
    sortRelationships(RelationshipList);

    int index = isInsideList<Relationship>(r, RelationshipList);

    return index;
}

int getOrderOfRelationshipWithoutDigon(Relationship * r, vector<Relationship*> RelationshipList)
{
	sortRelationships(RelationshipList);

	int index = 0;
	for (auto re : RelationshipList) {
		if (re->getCardinality() == 2)
			continue;
		
		if (re == r) {
			return index;
		}

		index++;
	}

	return index;
}

//check if list 1 is inside list 2
bool isEntityListInsideEntityList(const vector<Entity*>& enlist1, const vector<Entity*>& enlist2) {

	if (enlist1.size() > enlist2.size())
		return false;

	for (int i = 0; i < enlist1.size(); i++) {
		int res = isInsideList<Entity>(enlist1[i], enlist2);
		if (res == -1)
			return false;
	}

	return true;
}

float calculateDisPointToLine(Vec2 p1, Vec2 p2, Vec2 p0) {
	
	//https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	return abs((p2.y - p1.y)*p0.x - (p2.x - p1.x)*p0.y + p2.x*p1.y - p2.y *p1.x) / length(p2 - p1);
}

float calculateDisPointToLineSeg(Vec2 p1, Vec2 p2, Vec2 p0) {

	float dist = calculateDisPointToLine(p1, p2, p0);
	float distp0p1 = length(p0 - p1);
	float distp0p2 = length(p0 - p2);
	float distp1p2 = length(p1 - p2);

	if (distp0p1  > distp1p2) {
		return distp0p2;
	}
	else if (distp0p2 > distp1p2) {
		return distp0p1;
	}
	else {
		return dist;
	}
		
}

float calculateMinimumDis(Vec3 center, vector<Entity*> enlist) {

	float miniumd = 0;

	for (int i = 0; i < enlist.size(); i++) {
		float d = length((enlist[i]->getLocation() - center));
		if (i == 0)
			miniumd = d;
		else if(d < miniumd) {
			miniumd = d;
		}

		float edgeDis = calculateDisPointToLineSeg(enlist[i]->getLocation(), enlist[(i + 1) % enlist.size()]->getLocation(), center);
		if (edgeDis < miniumd)
			miniumd = edgeDis;
	}

	return miniumd;
}

Vec3 calculateCenter(const vector<Vec3> &loclist) {

	Vec3 sum = Vec3(0.0);
	int size = loclist.size();
	
	for (int i = 0; i < size; i++) {
		sum += loclist[i];
	}
	sum = sum / double(size);

	return sum;
}

void UpdateBezierLocation(vector<Vec3> &currentListToAdd)
{
	for (int s = 0; s<5; s++)
	{
		vector<Vec3> localUpdateLocations;

		if (currentListToAdd.size())
		{
			localUpdateLocations.push_back(currentListToAdd[0]);
		}

		for (int i = 0; i<currentListToAdd.size() - 1; i++)
		{
			Vec3 Currentvec = currentListToAdd[i];
			Vec3 Nextvec = currentListToAdd[i + 1];

			Vec3 directionVec(Nextvec - Currentvec);

			Vec3 CurrentUpdate = Currentvec + 0.25*directionVec;
			Vec3 NextUpdate = Currentvec + 0.75*directionVec;

			if (0 == i)
			{
				localUpdateLocations.push_back(NextUpdate);
			}
			else if (i == currentListToAdd.size() - 2)
			{
				localUpdateLocations.push_back(CurrentUpdate);
			}
			else
			{
				localUpdateLocations.push_back(CurrentUpdate);
				localUpdateLocations.push_back(NextUpdate);
			}
		}

		if (currentListToAdd.size())
		{
			localUpdateLocations.push_back(currentListToAdd[currentListToAdd.size() - 1]);
		}
		currentListToAdd = localUpdateLocations;
	}
}

//create a cubic bezier curve
vector<Vec3> getCubicBezierCurve( Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4)
{
	int T = 10;
	double ctrlPoints[4][2] = { { v1.x, v1.y },{ v2.x, v2.y },{ v3.x, v3.y },{ v4.x, v4.y } };

	double fu[4];

	vector<Vec3> res;

	for (int uInt = 0; uInt <= T; uInt++)
	{
		double u = uInt / (double)T;

		fu[0] = (1.0 - u)*(1.0 - u)*(1.0 - u);
		fu[1] = 3.0*(1.0 - u)*(1.0 - u)*u;
		fu[2] = 3.0*(1.0 - u)*u*u;
		fu[3] = u*u*u;

		double x = 0.0;
		double y = 0.0;

		for (int i = 0; i < 4; i++)
		{
			x += fu[i] * ctrlPoints[i][0];
			y += fu[i] * ctrlPoints[i][1];
		}
		Vec3 vtemp = Vec3(x, y, 0);
		res.push_back(vtemp);
	}
	return res;
}

vector<Vec3> getArc(double cx, double cy, double r, double start_angle, double arc_angle, double num_segments) {

	double theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

	double tangetial_factor = tan(theta);

	double radial_factor = cos(theta);


	double x = r * cos(start_angle);//we now start at the start angle
	double y = r * sin(start_angle);

	vector<Vec3> curve;

	for (int ii = 0; ii < num_segments; ii++)
	{
		curve.push_back(Vec3(x + cx, y + cy, 0));

		double tx = -y;
		double ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}

	return curve;
}

//p0 p1 is a line segment, p2 p3 is another line segment
int get_line_intersection(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3)
{

	double p0_x = p0.x;
	double p0_y = p0.y;

	double p1_x = p1.x;
	double p1_y = p1.y;

	double p2_x = p2.x;
	double p2_y = p2.y;

	double p3_x = p3.x;
	double p3_y = p3.y;

	double *i_x;
	double *i_y;

	double s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	//colinear situations
	double s1_cross_s2 = (-s2_x * s1_y + s1_x * s2_y);

	if (fabs(s1_cross_s2) < EPI) {

		double p2p1_cross_s1 = (p2_x - p0_x)*s1_y - (p2_y - p0_y)*s1_x;		//(p2-p1) cross s1

		if (fabs(p2p1_cross_s1) < EPI)
		{
			//colinear
			double t0 = ((p2_x - p0_x)*s1_x + (p2_y - p0_y)*s1_y) / (s1_x * s1_x + s1_y*s1_y);	//to = dot((p2-p0), s1)/dot(s1, s1)
			double t1 = t0 + s2_x*s1_x / (s1_x * s1_x + s1_y*s1_y) + s2_y*s1_y / (s1_x * s1_x + s1_y*s1_y);				//t1 = t0 + dot(s1,s2/dot(s1, s1))

			double lb = min(t0, t1);
			double ub = max(t0, t1);

			if ((lb >= 0 && lb <= 1) || (ub >= 0 && ub <= 1))
				return 1;
			else
				return 0;
		}
		else
		{
			//parallel and non-intersecting
			return 0;
		}

	}


	double s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= -EPI && s <= 1+ EPI && t >= -EPI && t <= 1+ EPI)
	{
		//including collision at the end point
		if (s <= EPI && s >= -EPI)
			return 2;
		if (t <= EPI && t >= -EPI)
			return 2;
		if (s <= 1.0 + EPI && s >= 1.0 - EPI)
			return 2;
		if (t <= 1.0 + EPI && t >= 1.0 - EPI)
			return 2;

		// Collision detected
		//if (i_x != NULL)
		//	*i_x = p0_x + (t * s1_x);
		//if (i_y != NULL)
		//	*i_y = p0_y + (t * s1_y);
		return 1;
	}
	else
		return 0; // No collision
}

bool isEntitiesClique(vector<Entity*> enlist)
{
	for (int i = 0; i < enlist.size(); i++) {

		for (int j = i + 1; j < enlist.size(); j++) {

			int res = isInsideList<Entity>(enlist[i], enlist[j]->getAdjacentEntities());
			if(res== -1)
				return false;
		}
	}
	
	return true;
}

double ccwAngle(Vec3 from, Vec3 to)
{
	double a = atan2(from.x*to.y - from.y*to.x, from.x*to.x + from.y*to.y);
	if (a < 0)
		return a + 2.0*M_PI;
	else
		return a;
}

bool isPointInsidePolygon(Vec3 v, vector<Vec3>vlist) {


	int intersect = 0;

	//assum 1e10 is much bigger than the maximum x coordinates
	Vec3 v2 = Vec3(1e10, v.y, v.z);

	int endIntersectCount = 0;
	for (int i = 0; i < vlist.size(); i++) {

		Vec3 p0 = vlist[i];
		Vec3 p1 = vlist[(i + 1) % vlist.size()];

		int res = get_line_intersection(p0, p1, v, v2);

		if (res == 2)
		{
			//intersect on end point
			endIntersectCount++;
		}

		if (res == 1) {
			intersect++;
		}
	}

	if (endIntersectCount % 2 != 0)
		LogConsole::showError("There is no way we get odd number of end intersections");

	int odd =  (intersect + endIntersectCount/2) % 2;  // Same as (count%2 == 1)

	if (odd == 0)
		return false;
	else
		return true;

}


vector<vector<Vec3>> createTriangleMesh(Vec3 point0, Vec3 point1, Vec3 point2, double thickNess)
{
	vector<vector<Vec3>> triangels;
	Vec3 checkNormal = CalculateNormal(point0, point1, point2);

	// make sure the winding is anticlock wise
	if (dot(checkNormal, Vec3(0, 0, -1)) < 0.0)
	{
		Vec3 tempVec = point1;
		point1 = point2;
		point2 = tempVec;
	}

	for (int j = 0; j < 4; j++)
	{
		Vec3 p0;
		Vec3 p1;
		Vec3 p2;

		if (0 == j)
		{
			p0 = point0;
			p0.z -= thickNess / 2.0;
			p1 = point1;
			p1.z -= thickNess / 2.0;
			p2 = point2;
			p2.z -= thickNess / 2.0;
		}
		else if (1 == j)
		{
			p0 = point1;
			p0.z -= thickNess / 2.0;
			p1 = point2;
			p1.z -= thickNess / 2.0;
			p2 = point1;
			p2.z += thickNess / 2.0;
		}
		else if (2 == j)
		{
			p0 = point2;
			p0.z -= thickNess / 2.0;
			p1 = point2;
			p1.z += thickNess / 2.0;
			p2 = point1;
			p2.z += thickNess / 2.0;
		}
		else if (3 == j)
		{
			p0 = point0;
			p0.z += thickNess / 2.0;
			p1 = point1;
			p1.z += thickNess / 2.0;
			p2 = point2;
			p2.z += thickNess / 2.0;

			// Normal should be oppositive of the normal of initial triangle
			Vec3 tempVec = p1;
			p1 = p2;
			p2 = tempVec;
		}

		vector<Vec3> toAddTriangle;
		toAddTriangle.push_back(p0);
		toAddTriangle.push_back(p1);
		toAddTriangle.push_back(p2);

		Vec3 NormalVec = CalculateNormal(p0, p1, p2);
		toAddTriangle.push_back(NormalVec);

		triangels.push_back(toAddTriangle);

	}
	return triangels;
}


Vec3 CalculateNormal(Vec3 &p0, Vec3 &p1, Vec3 &p2)
{
	Vec3 vec1 = p1 - p0;
	normalize(vec1);

	Vec3 vec2 = p2 - p0;
	normalize(vec2);

	Vec3 NormalVec = cross(vec1, vec2);

	normalize(NormalVec);

	return NormalVec;

}

// A globle point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
Vec3 p0;

// A utility function to find next to top in a stack
Vec3 nextToTop(stack<Vec3> &S)
{
	Vec3 p = S.top();
	S.pop();
	Vec3 res = S.top();
	S.push(p);
	return res;
}

// A utility function to swap two points
void swap(Vec3 &p1, Vec3 &p2)
{
	Vec3 temp = p1;
	p1 = p2;
	p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(Vec3 p1, Vec3 p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Vec3 p, Vec3 q, Vec3 r)
{
	double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
	Vec3 *p1 = (Vec3 *)vp1;
	Vec3 *p2 = (Vec3 *)vp2;

	// Find orientation
	int o = orientation(p0, *p1, *p2);
	if (o == 0)
		return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

	return (o == 2) ? -1 : 1;
}

// Prints convex hull of a set of n points.
vector<Vec3> getConvexHull(vector<Vec3>points)
{
	
	vector<Vec3>convexHull;

	// Find the bottommost point
	double ymin = points[0].y;
	int min = 0;

	for (int i = 1; i < points.size(); i++)
	{
		double y = points[i].y;

		// Pick the bottom-most or chose the left
		// most point in case of tie
		if ((y < ymin) || (ymin == y && points[i].x < points[min].x)){
			ymin = points[i].y;
			min = i;
		}
	}

	// Place the bottom-most point at first position
	swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.
	// A point p1 comes before p2 in sorted ouput if p2
	// has larger polar angle (in counterclockwise
	// direction) than p1
	p0 = points[0];
	qsort(&points[1], points.size() - 1, sizeof(Vec3), compare);

	// If two or more points make same angle with p0,
	// Remove all but the one that is farthest from p0
	// Remember that, in above sorting, our criteria was
	// to keep the farthest point at the end when more than
	// one points have same angle.
	int m = 1; // Initialize size of modified array
	for (int i = 1; i<points.size(); i++)
	{
		// Keep removing i while angle of i and i+1 is same
		// with respect to p0
		while (i < points.size() - 1 && orientation(p0, points[i], points[i + 1]) == 0)
			i++;

		points[m] = points[i];
		m++;  // Update size of modified array
	}


	// Create an empty stack and push first three points
	// to it.
	stack<Vec3> S;

	// If modified array of points has less than 3 points,
	// convex hull is not possible
	if (m < 3){
		return convexHull;
	}

	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by
		// points next-to-top, top, and points[i] makes
		// a non-left turn
		while (orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}

	
	// Now stack has the output points, print contents of stack
	while (!S.empty())
	{
		Vec3 p = S.top();
		//cout << "(" << p.x << ", " << p.y << ")" << endl;
		convexHull.push_back(p);
		S.pop();
	}

	return convexHull;
}

bool getConvexHull(vector<Entity*> entityList, vector<Vec3> &convexHull) {

	if (entityList.size() < 3) {
		return false;
	}

	vector<Vec3>pointlist;
	for (int i = 0; i < entityList.size(); i++) {
		pointlist.push_back(entityList[i]->getLocation());
	}

	convexHull =  getConvexHull(pointlist);

	return true;
}

Vec3 generateRandomLocInRange(double xmin, double xmax, double ymin, double ymax) {
	
	double rx = (double)rand() / RAND_MAX;
	double ry = (double)rand() / RAND_MAX;

	double x = rx*(xmax - xmin) + xmin;
	double y = ry*(ymax - ymin) + ymin;

	Vec3 temp = Vec3(x, y, 0);

	return temp;
}

Vec3 generateRandomLocInPolygon(vector<Vec3> vertexList)
{
	double ymax = DBL_MIN;
	double xmax = DBL_MIN;
	double ymin = DBL_MAX;
	double xmin = DBL_MAX;
	
	for (int i = 0; i < vertexList.size(); i++) {
		if (vertexList[i].x < xmin)
			xmin = vertexList[i].x;
		if (vertexList[i].x > xmax)
			xmax = vertexList[i].x;
		if (vertexList[i].y < ymin)
			ymin = vertexList[i].y;
		if (vertexList[i].y > ymax)
			ymax = vertexList[i].y;
	}
	
	Vec3 temp = generateRandomLocInRange(xmin, xmax, ymin, ymax);

	//int count = 0;
	while (!isPointInsidePolygon(temp, vertexList)) {
		//count++;
		temp = generateRandomLocInRange(xmin, xmax, ymin, ymax);
	}

	return temp;
}

void quickSort(vector<pair<double, Entity*>> &arr, int left, int right) {
    int i = left, j = right;
    pair<double, Entity*> tmp;
    double pivot = arr[(left + right) / 2].first;

    /* partition */
    while (i <= j) {
        while (arr[i].first < pivot)
            i++;
        while (arr[j].first > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

double CalcTriangleArea(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 l1 = a - b;
    Vec3 l2 = a - c;
    Vec3 l3 = b - c;

    double ll1 = length(l1);
    double ll2 = length(l2);
    double ll3 = length(l3);

    double s = (ll1 + ll2 + ll3)*0.5;

    double T = sqrt(s*(s - ll1)*(s - ll2)*(s - ll3));

    return T;
}

Vec3 rotate(Vec3 dir, double angle) {

    double x = dir.x*cos(angle) - dir.y*sin(angle);
    double y = dir.x*sin(angle) + dir.y*cos(angle);

    return Vec3(x, y, 0.0);
}

void scalePoints(vector<Vec3> &locs, Vec3 center, double scale)
{

    if (scale == 1.0)
        return;

    scale = abs(scale);

    int i = 0;
    for (i; i < locs.size(); i++) {
        Vec3 loc = locs[i];
        Vec3 dir = loc - center;
        double dis = length(dir);
        Vec3 newloc = center + dir*scale;
        locs[i] = newloc;
    }
}

bool isRelationshipEncloseEntities(Relationship * r, vector<Entity*> elist)
{
    for (int i = 0; i < elist.size(); i++) {
        int inside = isInsideList<Entity>(elist[i], r->getIncidentEntities());
        if (inside == -1)
        {
            return false;
        }
    }

    return true;
}

bool isRelationshipEnclosedByEntities(Relationship * r, vector<Entity*> elist)
{
    for (int i = 0; i < r->getIncidentEntities().size(); i++) {

        int inside = isInsideList<Entity>(r->getIncidentEntity(i), elist);
        if (inside == -1)
        {
            return false;
        }

    }
    return true;
}

bool isBoundingBoxIntersect(const Relationship * re1, const Relationship * re2)
{
	if (re1->minX > re2->maxX || re2->minX > re1->maxX)
		return false;

	if (re1->minY > re2->maxY || re2->minY > re1->maxY)
		return false;

	return true;
}


Vec3 findLargestEdgeMidpoint(Relationship* re)
{
	ENList enlist = re->getIncidentEntities();
	Vec3 center = re->getCenter(re->getMonogonLength());
	Vec3 midpoint = Vec3(0, 0, 0);
	
	if (enlist.size() == 1)
	{
		double a = re->getMonogonInclinedAngle();
		double len = re->getMonogonLength();
		Vec3 off = Vec3(cos(a), sin(a), 0) * len;
		midpoint = center + off;
	}
	else if (enlist.size() == 2)
	{
		midpoint = center;
	}
	else if (enlist.size() > 2)
	{
		std::sort(enlist.begin(), enlist.end(), EntityCompare(ELEMENT_STATISTIC::ELEM_REL_ANGLE, true, center));
		double max_len = 0.0;
		for (int i = 0; i < enlist.size(); i++)
		{
			int j = (i + 1) % enlist.size();
			Vec3 loc1 = enlist.at(i)->getLocation();
			Vec3 loc2 = enlist.at(j)->getLocation();
			double len = length(loc2 - loc1);
			if (len > max_len)
			{
				midpoint = (loc1 + loc2) / 2.0;
				max_len = len;
			}
		}
	}
	
	return midpoint;
}

double findLargestGapMidAngle(Entity* en)
{
	REList relist = en->getIncidentRelationships();
	Vec3 loc = en->getLocation();
	double mid_angle = 0;

	if (relist.size() == 1)
	{
		Vec3 dir = loc - relist[0]->getCenter(relist[0]->getMonogonLength());
		mid_angle = angle(Vec2(dir));
	}
	else if (relist.size() > 1)
	{
		std::vector<double> angles;
		for (Relationship* re : relist)
		{
			Vec3 dir = re->getCenter(re->getMonogonLength()) - loc;
			angles.push_back(angle(Vec2(dir)));
		}

		std::sort(angles.begin(), angles.end());
		double max_diff = 0;
		for (int i = 0; i < angles.size(); i++)
		{
			int j = (i + 1) % angles.size();
			double diff = angles[j] - angles[i];
			if (diff < 0)
				diff += 2 * M_PI;
			if (diff > max_diff)
			{
				max_diff = diff;
				mid_angle = angles[i] + (diff / 2.0);
			}
		}
	}
	
	return mid_angle;
}

double getAvgIncidentDist(Entity* en)
{
	double dist = 0;
	Vec3 loc = en->getLocation();
	for (Relationship* re : en->getIncidentRelationships())
	{
		Vec3 center = re->getCenter(re->getMonogonLength());
		dist += length(center - loc);
	}
	return dist / float(en->getDegree());
}

double getAvgIncidentDist(Relationship* re)
{
	double dist = 0;
	Vec3 center = re->getCenter(re->getMonogonLength());
	for (Entity* en : re->getIncidentEntities())
	{
		Vec3 loc = en->getLocation();
		dist += length(loc - center);
	}
	return dist / float(re->getCardinality());
}


// comparitor classes

EntityCompare::EntityCompare(ELEMENT_STATISTIC sort, bool ascending, Vec3 observer)
{
	m_sort = sort;
	m_ascending = ascending;
	m_observer = observer;
}

bool EntityCompare::operator()(Entity* en1, Entity* en2)
{
	bool result;
	switch (m_sort)
	{
	case ELEM_INDEX:
		return m_ascending ? (en1->getIndex() < en2->getIndex()) : (en1->getIndex() > en2->getIndex());
		break;
	case ELEM_INCIDENCE:
		if (en1->getDegree() == en2->getDegree())
			return m_ascending ? (en1->getIndex() < en2->getIndex()) : (en1->getIndex() > en2->getIndex());
		else
			return m_ascending ? (en1->getDegree() < en2->getDegree()) : (en1->getDegree() > en2->getDegree());
		break;
	case ELEM_ADJACENCY:
		if (en1->getCardinality() == en2->getCardinality())
			return m_ascending ? (en1->getIndex() < en2->getIndex()) : (en1->getIndex() > en2->getIndex());
		else
			return m_ascending ? (en1->getCardinality() < en2->getCardinality()) : (en1->getCardinality() > en2->getCardinality());
		break;
	case ELEM_REL_ANGLE:
		{
			double a1 = angle(Vec2(en1->getLocation() - m_observer));
			double a2 = angle(Vec2(en2->getLocation() - m_observer));
			return m_ascending ? (a1 < a2) : (a1 > a2);
		}
		break;
	default:
		return m_ascending ? (en1->getIndex() < en2->getIndex()) : (en1->getIndex() > en2->getIndex());
		break;
	}
}

RelationshipCompare::RelationshipCompare(ELEMENT_STATISTIC sort, bool ascending)
{
	m_sort = sort;
	m_ascending = ascending;
}

bool RelationshipCompare::operator()(Relationship* re1, Relationship* re2)
{
	bool result;
	switch (m_sort)
	{
	case ELEM_INDEX:
		return m_ascending ? (re1->getIndex() < re2->getIndex()) : (re1->getIndex() > re2->getIndex());
		break;
	case ELEM_INCIDENCE:
		if (re1->getCardinality() == re2->getCardinality())
			return m_ascending ? (re1->getIndex() < re2->getIndex()) : (re1->getIndex() > re2->getIndex());
		else
			return m_ascending ? (re1->getCardinality() < re2->getCardinality()) : (re1->getCardinality() > re2->getCardinality());
		break;
	case ELEM_ADJACENCY:
		if (re1->getDegree() == re2->getDegree())
			return m_ascending ? (re1->getIndex() < re2->getIndex()) : (re1->getIndex() > re2->getIndex());
		else
			return m_ascending ? (re1->getDegree() < re2->getDegree()) : (re1->getDegree() > re2->getDegree());
		break;
	default:
		return m_ascending ? (re1->getIndex() < re2->getIndex()) : (re1->getIndex() > re2->getIndex());
		break;
	}

	if (m_ascending)
		return result;
	else
		return !result;
}