/*
	N-ary relationship data 
	Which consists of Entities and N-ary relationships
*/
#pragma once

#include "Entity.h"
#include "Relationship.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "globalVariables.h"
#include <cfloat>
#include <limits>
#include "ColorGradient.h"
#include "JPMath/Vector3.h"
#include <Eigen/Sparse>
#include <ogdf/basic/GraphAttributes.h>

using namespace std;

class ERData
{
public:
	ERData();
    ERData(const ERData &src);
	~ERData();

	double maxX, maxY, minX, minY;
	double midX, midY;

	int maxEntityDegree = INT_MIN;
	int minEntityDegree = INT_MAX;
	int maxRelationshipDegree = INT_MIN;
	int minRelationshipDegree = INT_MAX;
	int maxEntityCardinality = INT_MIN;
	int minEntityCardinality = INT_MAX;
	int maxRelationshipCardinality = INT_MIN;
	int minRelationshipCardinality = INT_MAX;

	Vec3 BC_center;
	double BC_radius;

	Vec3 massCenter;
	Vec3 principleDir1;
	Vec3 principleDir2;
	double principleDir1Project_max;
	double principleDir1Project_min;
	double principleDir2Project_max;
	double principleDir2Project_min;

	int getMaxDegree(ELEMENTTYPE elementType) const;
	int getMinDegree(ELEMENTTYPE elementType) const;
	void calculateEntityDegree();
	void calculateRelationshipDegree();
	void calculateEntityCard();
	void calculateRelationshipCard();

	int getEntityNum() const;
	int getRelationshipNum() const;
	double getEntityX(int index) const;
	double getEntityY(int index) const;
	double getMonogonRotation(int index) const;
	double getEntityRadius(int index) const;
	int getRelationshipCard(int index) const;
	int getRelationshipDegree(int index) const;
	bool isRelationshipOptimizable(int index) const;
	int getEntityDegree(int index) const; 
	bool isTwoResSame(int indexi, int indexj) const;
	bool isTwoResBBIntersect(int indexi, int indexj) const;
	double getAverageEdgeLenth() const;
	vector<int> getRelationshipVertIndices(int ri) const;
	int getSharedEntityNum(int re_i, int re_j) const;
	vector<int> getSharedEntityIndices(int re_i, int re_j) const;
	bool entityHasEnoughAngularSpace(Entity *e, double buffer) const;
	bool entityHasEnoughAngularSpace(int enId, double buffer) const;
	double entityMaxInternalAngle(int enId) const;
	bool hasEnoughAngularSpaceAtCommonVert(int re_i, int re_j, double bufferangle) const;
	int getSharedEntityNum(RE*re1, RE*re2) const;
	int getSharedRelationshipNum(int en_i, int en_j) const;
	int getSharedRelationshipNum(EN* a, EN* b);
	bool isTwoEntityAdjacent(int en_i, int en_j) const;
	template<class mytype>
	vector<int> getRelationshipVertIndices(int ri, const mytype *x) const;
	vector<int> getEntityIndicesFromIncidentLocs(int ri, const vector<Vec3> &locList) const;
	Entity* getEntity(int index);
	Relationship* getRelationship(int index);
	const Entity* getConstEntity(int index) const;
	const Relationship* getConstRelationship(int index) const;
	vector<Entity*> getEntities();
	vector<Entity*> getFreeEntities();
	vector<Relationship*> getRelationships();
	vector<Relationship*> getFreeRelationships();
	Entity* getEntity(string label);
	Relationship* getRelationship(string label);
	vector<Relationship*> getRelationships(vector<string>labels);
	vector<int> getEntityIndicesOfRelationships(vector<int>list);
	vector<Relationship*> getEntityEnclosingRelationships(vector<Entity*> elist);
	vector<Relationship*> getEntityIncidentRelationships(const int vi);
	Relationship* getRelationshipByOpidx(int opidx);
	Entity* getEntityByOpidx(int opidx);

	/*Relationships Operations*/
	void addEntity(Entity *en, bool set_opid = true);
	Entity* addEntity(Vec3 loc);
	Entity* addEntity(string label);
	Entity* addEntityUnique(string label);
	void addEntities(int Num, string label = "NULL");
	void insertEntity(Entity* en);
	void addRelationship(Relationship *r, bool set_opid = true);
	bool addRelationship(vector<Entity*> entitylist);
	Relationship* addRelationship(int Card, string label = "NULL", double radius = 0.5);
	void insertRelationship(Relationship* re);
	bool deleteEntity(Entity* en, bool delete_relationships = true);
	bool deleteEntities(vector<Entity*> entitylist, bool delete_relationships = true);
	bool deleteRelationship(Relationship* relationship, bool delete_entities = false);
	bool deleteRelationships(vector<Relationship*> relationshiplist, bool delete_entities = false);
	void calculateBoundingCirle(bool resetZ = false, bool includeMonogon = true);
	void jitterLocations();
	void setOptimizableVerticesAndPolygons(vector<Entity*> elist = vector<Entity*>(), vector<Relationship*> rlist = vector<Relationship*>(), bool optimizable = true);
	void rescaleBoundingCirle();

	// Check Duplications
	void CheckCollocatedRelationShips();

	void assignIndices();
    void updateEntityIndices(int i = 0);                        //update the indices of entities after i
    void updateRelationshipIndices(int i = 0);                  //update the indices of relationship after i

	void updateEntityLabelVisibilities(vector<Entity*>vis_en_list = vector<Entity*>());
	void updateRelationshipsLabelVisibilities(vector<RE*>vis_en_list = vector<RE*>());

	void assignEntityLabelsWithIndices();
	void assignRelationshipLabelsWithIndices();
	void bringRelationshipForward(Relationship *re);
	void bringRelationshipBackward(Relationship *re);
	void sendRelationshipsToBack(REList relationships);
	void sendRelationshipsToFront(REList relationships);


	bool separateRelationship(Relationship *r);													//the relationships enclosed in r will be separated
	Relationship* mergeToRelationship(vector<Entity*> entityList, vector<Relationship*> relationshipList, string name = "", Entity* dual = NULL);
	Entity* collapseToEntity(vector<Entity*> entityList, string name = "", Relationship* dual = NULL);
	Relationship* mergeRelationships(vector<Relationship*> relist, string name);
	Entity* mergeEntities(vector<Entity*> enlist, string name, Relationship* dual = NULL);
	
	bool addEntityToRelationship(Entity* en, Relationship* r);
	bool removeEntityFromRelationship(Entity *en, Relationship *r);		//remove the entity from the relationships, the left entities of the relationship will sill construct a relationship
	bool removeEntityFromIncidents(Entity* en);				//remove the entities from its incident relationships, the left entities of each relationship will sill construct a relationship
	bool removeEntitiesFromRelationship(vector<Entity*> entityList, Relationship * r);			//remove the entities from the current relationship, the left entities sill construct a relationship
	bool divideEntitiesFromRelationship(vector<Entity*> entityList, Relationship * r);			//remove entities from the current relationship and create a new relationship among them
	bool removeEnclosingRelationships(Relationship *r);											//remove enclosing relationships of r

	void generateRandomRelationship(int card, int num);
	bool isInsideRelationshipList(vector<Entity*> entityList);

    void checkData();
    bool checkData(const ERData &d2);

	/*Data Process*/
	int removeBinaryEnclosedByRelationships();		//check every binary relationship, if it is contained inside a Nary relationship, remove it
	int removeMultipleRelationships();
	int removeNonMultipleRelationships();
	void removeUnvisibleElements(int &removedEntities, int &removedRelationships);
	void removePolygonsByCard(int card);
	void removeDagglingRelationships();
	void removeDagglingEntities();
	void removeMonogons();
	//the entities whose dual relationship is deleted with be deleted, the rest entities inside the relationship will still construct a relationship
	void removeEntitiesWithoutDualElements();		
	void convertToLinear(int mergeThreshold = 1);
	void convertToSimple();
	vector<int> convertMonogonsToDigons();
	void convertDigonsToMonogons(vector<int> indices);

	/*Data Convert*/
	bool convertToGraphData();						// add edges to each polygon
	bool convertToBipartiteGraphData();				// Konig graph
	bool convertToERData(ERData * &dst);			// find cliques
	int findCliques(vector<Entity*> P);
	bool getEntityLocationsFromGraphData(ERData * dst);
	bool findClique(vector<Entity*> enlist);
	bool writeERSubsets(const char* dst);

	/*dual data*/
	bool writeDualERData(vector<Entity*>enlist, vector<Relationship*> relist, const char* dst);
	vector<Entity*> getEntitiesDegreeNotOne();
	vector<Relationship*> getMonogons();
	int getMonogonNum() const;
	int getMonogonIndex(Relationship *re) const;
	int getMonogonIndex(const Relationship* re) const;
	ERData* convertToDualERData();
	ERData* getDualERData();
	void syncDualLayout();

	/*bipartite related*/
	ERData* getBipartiteGraph();
	ERData* getSubHypergraph(vector<int>* enlist);
	ERData* getPartialHypergraph(vector<int>* relist);
	
	/*create scaffold for each digon to shown as link*/
	void createEdgesScaffolds();

	/*finding cliques, Bron-Kerbosch algorithm*/
	void BronKerbosh(vector<Entity*> &Rset, vector<Entity*> &Pset, vector<Entity*> &Xset, vector<vector<Entity*>> &cliques);
	void BronKerbosh(set<Entity*> Rset, set<Entity*> Pset, set<Entity*> Xset, vector<vector<Entity*>> &cliques);

	/*attributes*/
	void retrieveAttributes(vector<vector<string>> attributes);
	string FILECOMMENTS;
	void appendFileComment(string comment);
	void clearComment();

	/*2D layout design*/	
	void scaleEntities(Vec3 scale, Vec3 m_center = Vec3(0, 0, 0), ENList* enlist = NULL);
	void rotateEntities(double angle, Vec3 m_center = Vec3(0, 0, 0), ENList* enlist = NULL);
	void rotateMonogons(double angle, REList* relist = NULL);
	void moveEntities(double dx, double dy, ENList* enlist = NULL);
	void reflectEntities(double angles, Vec3 center, ENList* enlist = NULL);
	void reflectLayout(Vec3 v1, Vec3 v2, ENList* enlist = NULL);

	void forceMove(Entity *e_dst, Entity *e_source, Vec3 force, double stepT = 0.1);
	void forceMove(Entity *e_source, Vec3 force, double stepT = 0.1);
	vector<pair<int, int>>forcePair;

	vector<vector<Entity*>> FindGraphs(vector<Entity*> &entityList);
	vector<Entity*> FindAllLeaves(vector<Entity*> &entityList);
	vector<Entity*> FindAllNonLeaves(vector<Entity*> &entityList);
	vector<Relationship*> FindAllIncidentRelationships(vector<Entity*> entityList);

	void designLayout_GridSeparatedTree(int col = -1, bool sort = true);
	void designLayout_Grid(double sidelength = 1.9);
	void designLayout_GridCircle(double Radius, double fixedDisp);
	void designLayout_RectGrid(int dimension, bool col_wise, float start_x, float start_y, float col_sep, float row_sep, ENList enlist = ENList());
	void ERData::designLayout_CircGrid(float center_x, float center_y, float radius, float start, ENList enlist = ENList(), ELEMENT_STATISTIC sort = ELEM_NONE, bool ascending = true);
	void designLayout_linear(double space);
	void designLayout_GridEisenstein();
	void designLayout_RandomShuffle(unsigned rseed = 10000);
	void designLayout_Random(double radius = 1.0);
	void designLayout_RandomRectangular(double min_x, double min_y, double width, double height, ENList enlist = ENList());
	void designLayout_RandomCircular(double x, double y, double rad_min, double rad_max, ENList enlist = ENList());
	void designLayout_Random3D();
	void designLayout_colocated(Vec3 vec);

	double updateStep(double step, double curr_energy, double last_energy, int &progress);
	Vec3 getAttractiveForce(Entity *en, /*vector<Entity*> enList,*/ double edge_length);
	Vec3 getRepulsiveForce(Entity *en, vector<Entity*> enList, double edge_length, double relative_coef);
	Vec3 getRepulsiveForce2(Entity *en, vector<Entity*> enList, double edge_length, double relative_coef, double div_pow);
	void designLayout_ForceDirected(vector<Entity*> enList, double step, double edge_length, double relative_coef, double div_pow, int times, double tol, bool showAnimation = true);
	void designLayout_FDMaxent_FixedVerts(vector<Entity*> enList, vector<Entity*> fixedEnList, double step, double edge_length, double relative_coef, double div_pow, int times, double tol, bool showAnimation = true);

	/*update the visual components of entities and relationships*/
	void setEntitiesSize(double size);
	void setMonogonsSize(double size);
	void setMonogonLength(double size);
	void updateMonogons2D();
	void resetMonogonAngles(vector<Entity*> enlist);
	void distributeMonogonAngles(vector<Entity*> enlist);
	void calculateRelationshipsConvexHull();
	void update2D();
	void initializeMonogonOrientations(vector<RE*>mlist = vector<RE*>());

	void updateRelatedMonogons(Entity *en);			//update all monogons incident to en's adjacent vertices
	void updateRelatedMonogons(Relationship* re);	//update all monogons adjacent to re's adjacent polygons

	void create2DScaffolds(int ReCard = -1);
	void starrizeRelationships();
	void updateRelationshipLocation(int index, vector<Vec3> loc);
	bool updateEntitiesLocation(vector<Vec3> loc);
	bool updateEntitiesLocation(int N, const double *xinput);
	bool updateFreeEntityLocs(int N, const double *xinput);
	void updateRelationshipLocation(vector<Vec3> centers);
	void updateMonogonRotations(vector<double> orients);
	void updateMonogonRotations(int N, const double *orients);
	bool updateAllLocations(int NV, int NM, const double* x, int offset = 0);
	bool updateAllFreeLocations(int NV, int NM, const double* x, int offset = 0);

	struct ReInfo{
		int card = 0;
		//double reInclinedAngle = 0;
		int monogonIndex = 0;
		vector<bool> sameReList;
	};
	vector<ReInfo> reInfos;
	void constructRelationshipInfos();
	int getNumberOfAjacentMonogon(int vindex) const;

public:

	//Note fixed vert has two components, each component is indixed as 2i and 2i+1*/
	vector<int>fixedVertComp;
	//first find one digon, or find first two vertices of a triangle, or find one polygon two consecutive degree 1 verts, then find first poly's first two 
	vector<int>getTwosVertsToBeFixed();
	vector<int>getSortedFixedCompIndices() const;
	bool isVertFree(int vertIndex);
	int getFreeVertCompNum() const;

	// monogon rotation component indexed as 2(N) * (monogon_index)
	vector<int> fixedMonoComp;
	vector<int> getSortedFixedMonoIndices() const;
	bool isMonogonFixed(int monoIndex);
	int getFreeMonoCompNum() const;


	enum ColorVariationType
	{
		ColorVariationType_HUE,
		ColorVariationType_ALPHA,
	};
	double maxColorVariable, minColorVariable;
	void getRelationshipColorVariableRange(ELEMENT_COLORSCHEME et);
	void assignRelationshipColors(ELEMENT_COLORSCHEME et, ColorGradient colorGradient);
	void assignRelationshipColors(ELEMENT_COLORSCHEME colorScheme);
	void assignRelationshipColors();
	void assignRelationshipColors(vector<Vec4>colors);
	void assignRelationshipColorsBySetIntersections();
	void variateRelationshipColors(ColorVariationType type, double ratio = 0.05);
	void assignRelationshipColorsBySubstring(vector<std::string> subs);
	
	void assignEntitiesColors(Vec4 color);
	void assignEntitiesColors(ELEMENT_COLORSCHEME et, ColorGradient colorGradient);
	void assignEntitiesColors(ELEMENT_COLORSCHEME colorScheme);
	void assignEntitiesColorsDual();
	void assignEntitiesColorsBySubstring(vector<std::string> subs);
    
	void syncColorsWithDualElements(bool withAlpha = false);

	double calculateGeometryInfo();			//check variance of length of edges
	double calculateUniformAreaEnergy(double e_area);


	bool isBoundingBoxIntersect(const Relationship *re1, const Relationship *re2) const;
	void updateRelationshipsBoundingBox();
	bool checkEnclosing(RE *re1, RE *re2);
	
	/*Layout backup and redo*/
private:
	int current_layout_index = -1;
	typedef vector<Vec3> BackupLayout;
	vector<BackupLayout> m_backupLayouts;
public:
	int getCurrentLayoutIndex() const;
	int getSavedLayoutNum() const;
	int saveCurrentLayout();
	bool resumeLayout(int index);
	void clearBackupLayouts();
	int undoLayout();
	int redoLayout();

	/*Greedy merging to find hub*/
	vector<Relationship*> findPolygonsToMergeGreedy();
	int findPolygonContainUnavoidableLoop(vector<Entity*> &enlist, vector<Relationship*>&relist);
	bool FindReWithEdgelyAdjacentNodeOverlapping(vector<Entity*> &enlist, vector<Relationship*>&relist);

	void writeEulerCharacteristics(string vis = "");
	int getEdgeNum();
	int getTotalCard();
	int getTotalDeg();


	// Multiscale related

	void generateAtomicOperations(bool nrem, bool nmerge, bool erem, bool emerge, double alpha, double beta, double gamma);
	void deleteAtomicOperations();
	void updateOperationPriority(OperationRecord* op, double alpha, double beta, double gamma);
	void calcOperationPriority(OperationRecord* op, double alpha, double beta, double gamma);
	bool isConnectedWithoutRelationship(Relationship* remove);
	bool isConnectedWithoutEntity(Entity* remove);
	bool isLinear();
	bool isSubgraphLinear(Relationship* re);
	bool isSubgraphLinear(Entity* en);
	bool deleteCreatesOrphans(Relationship* remove);
	bool deleteCreatesZerogons(Entity* remove);
	bool deleteBreaksEnConnection(Relationship* remove);
	bool deleteBreaksReConnection(Entity* remove);
	bool enIsBranchEnd(Entity* remove);
	bool reIsBranchEnd(Relationship* remove);
	bool hasRemovableRelationships();
	bool hasRemovableEntities();
	int getEnShareNum(Entity* en);
	int getReShareNum(Relationship* re);
	//void lockCurrentVerts();
	int getEntityOpidxById(int id) const;
	int getRelationshipOpidxById(int id) const;
	void findCycleElementsPeel(int* enlist, int* relist);
	void findCycleElementsDFS(int* enlist, int* relist);

	bool applyRelationshipOperation(Relationship* remove, Relationship* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain);
	bool applyEntityOperation(Entity* remove, Entity* retain, vector<pair<int,std::string>>* incidentRemove, vector<int>* incidentRetain);
	REList invertRelationshipOperation(std::pair<int,std::string> remove, Relationship* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain, bool is_primal);
	ENList invertEntityOperation(std::pair<int, std::string> remove, Entity* retain, vector<pair<int, std::string>>* incidentRemove, vector<int>* incidentRetain, Relationship* dual_remove, Relationship* dual_retain);
	// Vector3 findBestEnAddLocation(Relationship* re);
	void getEnAddLocations(Relationship* re, vector<Vec2> &locs);
	void findBestEnLocation(Entity* en, vector<Vec2>& locs);


	// Hypergraph statistic functions

	void calcAllStatsLocally();

	void calcEntityCentralities(float* bc, float* ec, float* cc);
	void calcRelationshipCentralities(float* bc, float* ecc, float* cc);
	void calcCentralities(float* en_bc, float* en_ec, float* en_cc, float* re_bc, float* re_ecc, float* re_cc);
	void calcStatistics(int* en_in, int* re_in, int* en_ad, int* re_ad, int* en_af, int* re_af, bool local = false);
	void calcEffectiveResistances(float* en_ef, float* re_ef);

	int getEntityIncidence(Entity* en) const;
	int getEntityIncidence(const Entity* en) const;
	int getEntityAdjacency(Entity* en) const;
	int getEntityAdjacencyFactor(Entity* en) const;
	float getEntityBetweennessCentrality(Entity* en) const;
	float getEntityEccentricity(Entity* en) const;
	float getEntityClosenessCentrality(Entity* en) const;
	float getEntityEffectiveResistance(Entity* en) const;
	
	int getRelationshipIncidence(Relationship* re) const;
	int getRelationshipIncidence(const Relationship* re) const;
	int getRelationshipAdjacency(Relationship* re) const;
	int getRelationshipAdjacencyFactor(Relationship* re) const;
	float getRelationshipBetweennessCentrality(Relationship* re) const;
	float getRelationshipEccentricity(Relationship* re) const;
	float getRelationshipClosenessCentrality(Relationship* re) const;
	float getRelationshipEffectiveResistance(Relationship* re) const;

	int getMinIncidence() const;
	int getMaxIncidence() const;
	int getMinAdjacency() const;
	int getMaxAdjacency() const;
	int getMinAdjacencyFactor() const;
	int getMaxAdjacencyFactor() const;
	float getMinBetweennessCentrality() const;
	float getMaxBetweennessCentrality() const;
	float getMinEccentricity() const;
	float getMaxEccentricity() const;
	float getMinClosenessCentrality() const;
	float getMaxClosenessCentrality() const;
	float getMinEffectiveResistance() const;
	float getMaxEffectiveResistance() const;

	bool areEntitiesAdjacent(const Entity* eni, const Entity* enj) const;
	bool areRelationshipsAdjacent(const Relationship* rei, const Relationship* rej) const;
	bool areEntityRelationshipIncident(const Entity* en, const Relationship* re) const;


	// Matrix related

	Eigen::SparseMatrix<int> m_IncidenceMatrix;
	Eigen::SparseMatrix<int> m_enAdjMatrix;
	Eigen::SparseMatrix<int> m_reAdjMatrix;
	void constructIncidenceMatrix();
	Eigen::VectorXd getIncidenceEigenvals();
	void constructEnAdjMatrix();
	void constructReAdjMatrix();
	Eigen::VectorXd getEnAdjEigenvals();
	Eigen::VectorXd getReAdjEigenvals();

	int countNAdjacentClusters();


	// Deterministic Layout Algorithms
	bool constructKonigGraph();
	bool isZykovPlanar();
	bool buildKonigLayout(double base_ratio, bool size_optim, bool side_optim);
	bool applyKonigLayout(double base_ratio, bool size_optim, bool side_optim, double scale = 1.0);
	bool overlapAdjustKonigLayout();
	ogdf::Graph* getKonigGraph();
	ogdf::GraphAttributes* getKonigGraphLayout();
	void reCenterScaleData(double scale, Vec3 center);


	// Bipartite Simplification Operations
	bool removeEntity(Entity* en);
	bool removeRelationship(Relationship* re);


	/*Variables*/
private:
	vector<Entity*> m_entityList;
	vector<Relationship*> m_relationshipList;
	ERData *m_dualERData = NULL;
	ogdf::Graph* m_KonigGraph = NULL;
	ogdf::GraphAttributes* m_KonigGraphLayout = NULL;

	int m_entityArraySize = 0;
	int m_relationshipArraySize = 0;
	int m_entityLifetime = 0;
	int m_relationshipLifetime = 0;
	
	int* m_entityIncidence = NULL;
	int* m_entityAdjacency = NULL;
	int* m_entityAdjacencyFactor = NULL;
	float* m_entityBetweennessCentrality = NULL;
	float* m_entityEccentricity = NULL;
	float* m_entityClosenessCentrality = NULL;
	float* m_entityEffectiveResistance = NULL;

	int* m_relationshipIncidence = NULL;
	int* m_relationshipAdjacency = NULL;
	int* m_relationshipAdjacencyFactor = NULL;
	float* m_relationshipBetweennessCentrality = NULL;
	float* m_relationshipEccentricity = NULL;
	float* m_relationshipClosenessCentrality = NULL;
	float* m_relationshipEffectiveResistance = NULL;

	int** m_incidenceMatrix = NULL;
	int** m_entityAdjacencyMatrix = NULL;
	int** m_relationshipAdjacencyMatrix = NULL;

	int* m_entitiesOnCycles = NULL;
	int* m_relationshipsOnCycles = NULL;
};

template<class mytype>
inline vector<int> ERData::getRelationshipVertIndices(int ri, const mytype * x) const
{
	return m_relationshipList[ri]->getEntityIndices<mytype>(x);
}