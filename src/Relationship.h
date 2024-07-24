#pragma once
#include "globalVariables.h"
#include "Element.h"
#include "JPMath/Vector4.h"
#include "JPMath/Vector3.h"
#include <string>
#include <vector>
#include "Pipe.h"

class Entity;

/*curved edge*/
struct Scaffold {
    bool m_Curved = false;
    Vec3 ControlPoint;
    vector<Vec3> m_CurvedLocation;
	
};
typedef Scaffold CurvedEdge;

struct BackupLayout {
    vector<Scaffold> scaffolds;
    vector<pair<int, Vec3>> locList;
    Vec3 center;
    Vec3 observer;
};

typedef vector<vector<Vec3>> TriangleMesh;

class Relationship : public Element
{

protected:
	
	vector<Entity*> m_incidentEntities;/*contectivity data*/
	vector<Relationship*> m_adjacentRelationships;
	vector<string> attribute;	/*attributes from data*/
	Entity *dualElement = NULL;

	double m_MonogonSize;
	double m_MonogonLength;
	PipeMesh::Pipe *mMonogonPipe = NULL;
	float m_MonogonRotation = 0.0;

	Vec3 m_center;
	//no need to be the center, the observer is just used to
	//reorder the entities, and to draw the polygons. It can 
	//be any locations inside convex hull of all entities.
	//Different of location of the observer will make different 
	//order of entities
	Vec3 m_observer;

	vector<Vec3>convexHull;
	vector<Scaffold> m_scaffolds;
	
	//create curved bondary edges for polygons
	void createPolygonScaffold();
	
	/*create curved bondary edges for binary relationships*/
	void createDigonScaffold();
	double getDigonLength() const;
	double getDigonRadius() const;
	double getDigonWidth() const;

public:
	Relationship();
	Relationship(Entity* en1, Entity* en2, bool init_connections = true);
	Relationship(const vector<Entity *>& entityList, bool init_connections = true);
	Relationship(const vector<Relationship*> &reList, bool init_connections = true);
	Relationship(const Relationship &r);
	Relationship(const vector<int> entityIncidese);
	void initialize(const vector<Entity *>& entityList, bool init_connections = true);
	void initializeScaffold();
	~Relationship();

	void setLabelWithIndex() override;

	/*construct adjacency*/
	void initalizeAdjacency(const vector<Entity *>& entityList);
	void finalizeAdjacency();

	void copyToIncidentEntityList(const vector<Entity *>& entityList);
	void copyToAdjacentRelationshipList(const vector<Relationship *>& relist);

	bool addIncidentEntity(Entity *tar, bool set_adjacency = true, bool symmetric = true);
	bool removeIncidentEntity(Entity *tar, bool symmetric = true);
	bool addAdjacentRelationship(Relationship *r, bool symmetric = true);
	bool removeAjacentRelationship(Relationship *r, bool symmetric = true);

	vector<Relationship*> getDuplicateRelationships();
	vector<int> getDuplicateRelationshipIndices();
	bool isFirstOfDuplicates();

	void addIncidentEntityList(vector<Entity*> entityList);
	void removeIncidentEntityList(vector<Entity*> entityList);
	bool isTwoEntityConsecutive(Entity *en1, Entity *en2) const;

	/*dual related*/
	void removeDualElement();
	void setDualElement(Entity *r);
	Entity *getDualEntity();

	bool isSameToRelationship(Relationship *target);
	bool isSameToRelationship(const vector<Entity*> &entityList);
	bool hasMultipleRelationship();

	int getCardinality() const;
	int getDegree() const;
	int getDegreeInRelationshipList(const vector<Relationship*> &re_list);
	int getModularityDegree(const vector<Relationship*> &re_list);
	int getSumEntityDegree();
    int getEntityInsidePolyIndex(Entity *en);
	vector<Entity*> getEntitiesByDegree(int degree);
	const vector<Entity*>& getIncidentEntities() const;
	void getIncidentEntityIds(vector<int>& ids, int offset = 0);
	vector<Relationship*> getAdjacentRelationships();
	const vector<Relationship*>& getAdjacentRelationships() const;
	Entity *getIncidentEntity(int i);
	Entity *getIncidentEntityByIndex(int index);
	Relationship *getAdjacentRelationship(int i);
	const Relationship* getAdjacentRelationship(int i) const;
	int getIncidentColorNum();


	/*get entities shared by other polygons*/
	vector<Entity*> getShardEntities() const;

	/*get edgely adjacent polygons which are incident to e*/
	struct EdgelyAdjacentRelationship {
		Entity * e1;
		Entity * e2;
		Relationship *r1;
	};
	vector<EdgelyAdjacentRelationship> getEdgelyAdjacentRelationships(Entity *e);
	vector<EdgelyAdjacentRelationship> getEdgelyAdjacentRelationships(Entity *e1, Entity *e2);
	vector<EdgelyAdjacentRelationship> getEdgelyAdjacentRelationships();
	vector<Relationship*>getRelationshipsSharingEntities(vector<Entity*>elist);
	vector<Scaffold>* getScaffoldList();
	const Scaffold& getScaffold(int index);
	inline int getScaffoldNum() { return m_scaffolds.size(); };
	Scaffold* getScaffoldPointer(int index);

	void clearIncidentEntities();
	void clearAdjacentRelationships();

	/*Operations to change the shape of polygons*/
	double getRadiusByDegree(double minL);
	double getRadiusByCardinality(double minL);

	void regularizePolygon(double length = 0, bool starrize_flag = true, bool update_scaffold = true);
	void DecreasePolygon(bool starrize_flag = true, bool update_scaffold = true);
	void IncreasePolygon(bool starrize_flag = true, bool update_scaffold = true);
	void ScalePolygon(double k, bool starrize_flag = true, bool update_scaffold = true);
	void ScalePolygonWithoutUpdateAdjcent(double k);

	//reorder vertices to make sure the adjacent polygon which sharing two common vertices are sharing a common edge
	void improveContinuity();
	int hasEdgeAdjLoop(vector<Entity*> &loopVerts, vector<Relationship*>&loopRes);

	bool swapEntities(vector<Entity*> selectedEntities, bool starrize_flag = true, bool update_scaffold = true);
	void swapEntities(int i, int j, bool starrize_flag = true, bool update_scaffold = true);
	void swapTwoEntitiesWithoutUpdate(int ith_incident, int jth_incident);

	void PermutePolygonEntities(bool starrize_flag = true, bool update_scaffold = true);
	void rotateCWPolygon(double rotateAngle, bool starrize_flag = true, bool update_scaffold = true);
	void rotatePolygon(double rotateAngle, Vec3 pivot, bool starrize_flag = true, bool update_scaffold = true);

	void flipPolygonAcrossTwoVertices(Vec3 v1, Vec3 v2, bool starrize_flag = true, bool update_scaffold = true);
	void flipPolygonAcrossTwoVertices(int ith_incident, int jth_incident, bool starrize_flag = true, bool update_scaffold = true);

	void translateLocation(double dx, double dy, bool starrize_flag = true, bool update_scaffold = true);
	void moveToLocation(double tx, double ty, bool starrize_flag = true, bool update_scaffold = true);

	void randomeCreateNewObserver();

	/*2D Visual Components Related Functions*/
	void create2DScaffolds();
	void createAdjacentRelationships2DScaffolds();
	void update2D(bool starrize_flag = true, bool update_scaffold = true, bool cardinality_changed = false);

	void starrize();
	void starrizeAdjacentPolygons();
	void starrize(Vec3 observer);
	vector<int> getEntityIndices() const;
	
	template<class mytype = double>
	void reorderEntityIndices(const mytype *x);        //use the new locations to reorder the entities
	template<class mytype>
	vector<int> getEntityIndices(const mytype *x) const;
	vector<int> getEntityIndicesFromIncidentLocs(const vector<Vec3> &locList) const;
	static vector<int> getSortedVerticesFromLocs(vector<int>indices, const vector<Vec3> &locList);
	static vector<int> getSortedVerticesFromLocs(Vec3 pivot, vector<int>indices, const vector<Vec3> &locList);
	vector<Vec3> getSortedLayout();

	int getNextEntityIndex(Entity *e);
	int getPreviousEntityIndex(Entity *e);

	double targetInteriorAngle();
	double area_beforeOptimization;
	double area() const;	//directly calculate the area
	double area2();     //trianglulate the polygon and sum up the area of each polygon
	double perimeter() const;
	double aspectRatio() const;
	double edgeLength_beforeOptimization;
	bool isEntityInInterior(Entity* en);

	/*optimization related operations, Energy*/
	double energy_AspectRatio = 0;
	double energy_Area = 0;
	bool flag_Optimizable = true;
	//bool flag_Locked = false;

	double calcRegularityEnergy_edgeVariance();
	double calcRegularityEnergy_angleVariance();
	double calcUniformAreaEnergy(double expectedArea);
	void showGeometryInfo();

	/*location backup*/
	//BackupLayout m_backupLayout;
	//void backupLocation();
	//void resumeLocation();
	//void backupLocation(vector<pair<int, Vec3>> &locList);
	//void resumeLocation(const vector<pair<int, Vec3>> locList);
	//void backupLayout();	/*Layout (Location and incident entity order)*/
	//void resumeLayout();


	/*Convex Hull*/
	void calcConvexHull();
	vector<Vec3> getConvexHullVertices();
	void clearConvexHullVertices();

	/*center*/
	Vec3 getCenter(double r);
	Vec3 getConstCenter() const;
	void setCenter(Vec3 m_center);

	/*Observer*/
	Vec3 getObserver();
	void setObserver(Vec3 loc);

	/*All Entities in 3D*/
	vector<Vec3> getAllEntititiesLocations();

	float getDiagonInnerCircle();

	/*Monogon*/
	double getMonogonInclinedAngle() const;

	void setMonogonRotation(double angle);
	double getMonogonRotation();
	double getMonogonSize();
	void setMonogonSize(float size);
	double getMonogonLength();
	void setMonogonLength(float size);
	void createMonogonScaffold();	/*create monogon edge which is a circle incident to the entity*/
	void initializeMonogonOrientation();
	void updateRelatedMonogons();
	vector<Relationship*> getRelatedMonogons();
	void createMonogonPipe(double width);
	PipeMesh::Pipe *getMonogonPipe();
	bool monogonPipeReconstructed = true;

	/*Bounding box*/
	float minX, maxX, minY, maxY;
	void updateBoundingBox();
	double calcCircumscribedCircleRadius();

	bool flag_drawAsGraph = false;
	double entangleCoefficient = 0;//Entanglement Coefficient
	double tempK = 0;		//default is 0; should be -1 or 1 if visited
	double tempA = -1;		//default is -1; should be a positive value
};

typedef Relationship RE;
typedef vector<Relationship*> REList;


