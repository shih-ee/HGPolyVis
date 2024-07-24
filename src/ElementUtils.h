#pragma once
#include "Entity.h"
#include "Relationship.h"
#include <algorithm>
#include <vector>
#include <stack>
#include <set>
#include <float.h>


#define CURVEFACTOR 0.6
#define CURVESCALEFACTOR 0.22
#define TRANSLATIONX 0.0
#define TRANSLATIONY 0.0
#define ROTATIONANGLE 245

template <class myType>
int isInsideList(const myType *en, const vector<myType *>& el1) {

    if (el1.size() == 0)
        return -1;

	auto it = std::find(el1.begin(), el1.end(), en);
	if (it == el1.end())
		return -1;
	else
		return it - el1.begin();
}

template <class myType>
bool uniqueInsert(myType *en, vector<myType *>& el1) {

	int k = isInsideList<myType>(en, el1);
	if (k == -1) {
		el1.push_back(en);
		return true;
	}
	else {
		return false;
	}
}

template <class myType>
vector<int>getIndices(const vector<myType*>&elelist)
{
	vector<int> res;
	for (int i = 0; i < elelist.size(); i++) {
		res.push_back(elelist[i]->getIndex());
	}
	return res;
}

/*check whether two relationships have same set of entities*/
bool isTwoRelationshipSame(const Relationship *re1, const Relationship *re2);

//check if relationship b is inside the target relationship t
bool isRelationshipInsideRelationship(Relationship *b, Relationship *t);

/*check whether two set of entities are same*/
bool isTwoEntityListSame(const vector<Entity *>& el1, const vector<Entity *>& el2);

bool isEntityInsideEntityList(Entity *en, const vector<Entity *>& el1);

//check if list 1 is inside list 2
bool isEntityListInsideEntityList(const vector<Entity*>& enlist1,  const vector<Entity*>& enlist2);

bool isRelationshipInsideRelationshipList(Relationship *re, const vector<Relationship *>& relist);

vector<Entity*> getUniqueIncidentEntities(const vector<Relationship *>& relist);

/*check whether entity connect relationship by a binary relationship*/
bool checkIfVertexConnectPolyByDigon(Entity *entity, Relationship* relationship);

bool checkIfEntityConnectRelationship(Entity *entity, Relationship* relationship);

bool checkIfTwoEntityAdjacent(Entity *entity1, Entity* entity2, Relationship* remove = NULL);

//check if relationship b is inside the target relationship t
bool checkIfTwoRelationAdjacent(Relationship *b, Relationship *t, Entity* remove = NULL);

vector<Entity*> getSharedEntities(Relationship *a, Relationship *b);
int getSharedEntitiesNum(Relationship* a, Relationship* b);
vector<Relationship*> getSharedRelationships(Entity* a, Entity* b);
int getSharedRelationshipsNum(Entity* a, Entity* b);

/*get entities shared by polygons a and all polygons incident to entity e*/
vector<pair<Entity*, Entity*>> getConnectedSharedEntities(Relationship *a, Entity *e);

/*Get all a's edgely adjacent polygons which is incident to entity e*/
vector<Relationship*> getEdgelyAdjacentRelationshipIncidentToEntity(Relationship *a, Entity *e);

int getNumberOfAjacentRelationships(Entity *v1, Entity *v2);

vector<Relationship*> getRelationshipWithCardinality(const vector<Relationship*> &res, int i);

/*Except the digons and monogons*/
int getMinPolygonCardinality(const vector<Relationship*> &res);

/*Except the digons and monogons*/
int getMaxPolygonCardinality(const vector<Relationship*> &res);

vector<Relationship*> getRelationshipsSharingEntities(vector<Entity*> elist);

//get all relationships incident to both v1 and v2

int getNumberOfAjacentRelationships(Entity * v1, Entity * v2, vector<Relationship*> &sharedRelationships);

int getNumberOfAjacentDigons(Entity * v1, Entity * v2, vector<Relationship*> &sharedRelationships);

int getNumberOfAjacentMonogon(Entity * v1, vector<Relationship*> &sharedRelationships);

int getOrderOfRelationship(const Relationship *r, vector<Relationship*> RelationshipList);

int getOrderOfRelationshipWithoutDigon(Relationship *r, vector<Relationship*> RelationshipList);

template<typename T, typename Enable = std::enable_if<std::is_base_of<T, Entity>::value>>
Vec3 calculateCenter(const vector<T*> &enlist){

	Vec3 sum = Vec3(0.0);

	for (int i = 0; i < enlist.size(); i++) {
		sum += enlist[i]->getLocation();
	}
	sum = sum / double(enlist.size());

	return sum;
}

Vec3 calculateCenter(const vector<Vec3> &loclist);

float calculateMinimumDis(Vec3 center, vector<Entity*> enlist);

vector<Vec3> getCubicBezierCurve(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);

vector<Vec3> getArc(double cx, double cy, double r, double start_angle, double arc_angle, double num_segments);

void UpdateBezierLocation(vector<Vec3> &currentListToAdd);

/*
	check if two line segment intersect
	return 0: there is no intersection
	return 1: there is one not end intersection
	return 2: there is one end intersection
*/
int get_line_intersection(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3);

bool isPointInsidePolygon(Vec3 v, vector<Vec3>vlist);

vector<vector<Vec3>> createTriangleMesh(Vec3 point0, Vec3 point1, Vec3 point2, double thickNess);

Vec3 CalculateNormal(Vec3 &p0, Vec3 &p1, Vec3 &p2);

bool isEntitiesClique(vector<Entity*> enlist);

double ccwAngle(Vec3 v1, Vec3 v2);

vector<Vec3> getConvexHull(vector<Vec3>points);

bool getConvexHull(vector<Entity*> entityList, vector<Vec3>&convexHull);

Vec3 generateRandomLocInPolygon(vector<Vec3>vertexList);

void quickSort(vector<pair<double, Entity*>> &arr, int left, int right);

double CalcTriangleArea(Vec3 a, Vec3 b, Vec3 c);

Vec3 rotate(Vec3 dir, double angle);

void scalePoints(vector<Vec3> &locs, Vec3 center, double scale);


bool isRelationshipEncloseEntities(Relationship *r, vector<Entity*> elist);
bool isRelationshipEnclosedByEntities(Relationship *r, vector<Entity*> elist);

float calculateDisPointToLine(Vec2 p1, Vec2 p2, Vec2 p0);
float calculateDisPointToLineSeg(Vec2 p1, Vec2 p2, Vec2 p0);

bool isBoundingBoxIntersect(const Relationship * re1, const Relationship * re2);


Vec3 findLargestEdgeMidpoint(Relationship* re);

double findLargestGapMidAngle(Entity* en);

double getAvgIncidentDist(Entity* en);

double getAvgIncidentDist(Relationship* re);


// classes for comparing elements
class EntityCompare
{
private:
	ELEMENT_STATISTIC m_sort = ELEM_INDEX;
	bool m_ascending = true;
	Vec3 m_observer = Vec3(0, 0, 0);
public:
	EntityCompare(ELEMENT_STATISTIC sort, bool ascending, Vec3 observer = Vec3(0,0,0));
	bool operator()(Entity* en1, Entity* en2);
};

class RelationshipCompare
{
private:
	ELEMENT_STATISTIC m_sort = ELEM_INDEX;
	bool m_ascending = true;
public:
	RelationshipCompare(ELEMENT_STATISTIC sort, bool ascending);
	bool operator()(Relationship* re1, Relationship* re2);
};