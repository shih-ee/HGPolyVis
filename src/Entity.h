#pragma once
#include <string>
#include <vector>
#include "JPMath/Vector3.h"
#include "JPMath/Vector4.h"
#include "Element.h"
#include "globalVariables.h"

class Relationship;

class Entity : public Element
{
protected:
	vector<Entity*> m_adjacentEntities;
	vector<Relationship*> m_incidentRelationships;
	Vec3 m_Location;
	/*attributes for data*/
	vector<string> attributes; 
	Relationship *dualElement = NULL;

public:
	Entity();
    Entity(const Entity &src);
	Entity(Vec3 location);
	~Entity();

	void setLabelWithIndex() override;

	/*dual related*/
	void removeDualElement();
	void setDualElement(Relationship *r);
	Relationship *getDualRelationship();

	/*connected with this entity by a relationship*/
	bool addAdjacentEntity(Entity* e, bool symmetric = true);
	bool addIncidentRelationship(Relationship * r, bool set_adjacency = false, bool symmetric = true);
	bool removeAdjacentEntity(Entity *e, bool symmetric = true);
	bool removeIncidentRelationship(Relationship *r, bool symmetric = true);
	void finalizeAdjacency();
    
    void copyToIncidentRelationshipList(const vector<Relationship *>& rList);
    void copyToAdjacentEntityList(const vector<Entity *>& elist);

	bool isConnectToReByBiRe(Relationship* target, Relationship * &binaryRe);

	inline const vector<Entity*>& getAdjacentEntities() const;
	inline vector<Entity*>& getAdjacentEntities();
	inline const vector<Relationship*>& getIncidentRelationships() const;
	inline vector<Relationship*>& getIncidentRelationships();
	void getIncidentRelationshipIds(vector<int>& ids, int offset = 0);
	Entity* getAdjacentEntity(int index);
	Relationship* getIncidentRelationship(int index);
	Relationship* getIncidentRelationship(const vector<Entity*>& otherEntities);
	
	void clearAdjacentEntities();
	void clearIncidentRelationships();
	
	int getCardinality() const;
	int getDegree() const;

	bool checkIncident(Relationship* re);
	bool checkAdjacent(Entity* en);

	// Solving Barycentric Position in Graph
	bool m_IsSearched = false;

	/*swap operation*/
	bool m_SelectedForSwap = false;
	void setSwapFlag(bool res);
	bool getSwapFlag();
	bool isDoneWithSwapping = false;

	/*location*/
	void setLocation(double x, double y, double z = 0);
	void setLocationZ(double z);
	void setLocation(Vec3 v);
	Vec3 getLocation();

	void updateRelatedMonogons(); //update all monogons incident to adjacent vertices
	bool distributeMonogons();


	void updateIncidentRes();
    void starrizeIncidentRes();
	void createIncidentRe2DScaffolds();
	void moveToLocation(double x, double y, STARRIZATIONR_FLAG flag1 = STARRIZATIONR_FLAG::STARRIZE, UPDATESCAFFOLD_FLAG flag2 = UPDATESCAFFOLD_FLAG::UPDATESCAFFOLD);
    void translate(double dx, double dy, STARRIZATIONR_FLAG flag1 = STARRIZATIONR_FLAG::STARRIZE, UPDATESCAFFOLD_FLAG flag2 = UPDATESCAFFOLD_FLAG::UPDATESCAFFOLD);
    void translate(Vec3 d, STARRIZATIONR_FLAG flag1 = STARRIZATIONR_FLAG::STARRIZE, UPDATESCAFFOLD_FLAG flag2 = UPDATESCAFFOLD_FLAG::UPDATESCAFFOLD);

    //location backup
    //Vec3 backup_loc;
    //void backupLocation();
    //void resumeLocation();

	//optimization related operations
    bool flag_forcedMoved = false;
    bool flag_Optimizable = true;
	//bool flag_Locked = false;

    //principle direction
    Vec3 adjacentVerticesMassCenter;
    Vec3 vprincipleDir1;
    Vec3 vprincipleDir2;
	double vprincipleDir1Project_max = 0;
	double vprincipleDir1Project_min = 0;
	double vprincipleDir2Project_max = 0;
	double vprincipleDir2Project_min = 0;

	/*attributes*/
	void setAttributes(vector<string> attri);
	vector<string> getAttributes();
	void clearAttributes();
	
	double m_Size;
};

inline const vector<Entity*>& Entity::getAdjacentEntities() const 
{
	return m_adjacentEntities;
}

inline vector<Entity*>& Entity::getAdjacentEntities() 
{
	return m_adjacentEntities;
}

inline const vector<Relationship*>& Entity::getIncidentRelationships() const 
{
	return m_incidentRelationships;
}

inline vector<Relationship*>& Entity::getIncidentRelationships()
{
	return m_incidentRelationships;
}

typedef Entity EN;
typedef vector<Entity*> ENList;