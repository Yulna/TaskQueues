#ifndef _ENTITIES_MANAGER_
#define _ENTITIES_MANAGER_

#include "j1Module.h"
#include "Quadtree.h"

#include "BaseEntities.h"
#include "Buildings.h"

class Action;
class ActionManager;

struct LessDistance
{
	bool operator()(const Unit* l, const Unit* r)
	{
		return l->distance_to_target >= r->distance_to_target;
	}
};

class j1EntitiesManager : public j1Module
{
public:

	j1EntitiesManager();
	~j1EntitiesManager();

public:

	// Called before render is available
	bool Awake(pugi::xml_node& config_node);

	void Disable();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate();

	//Draw entities
	bool Draw()const;

	// Called before quitting
	bool CleanUp();



private:

	//Lists of current game entities
	std::list<Unit*>		units;
	std::list<Building*>	buildings;


	//Vector where all the killed/destroyed entities are placed
	std::vector<Entity*>	wasted_units;

	//Vector of predefined units
	std::vector<Unit*>		units_defs;
	std::vector<Building*>	buildings_defs;


	//Methods to add entities definitions
	bool		AddUnitDefinition(const pugi::xml_node* unit_node);
	bool		AddBuildingDefinition(const pugi::xml_node* building_node);

	//Check if the entity civilizations string contains the chosen one
	bool		CivilizationCheck(char* civs_str, const char* chosen_civ);

public:

	//Entities Quad Trees to help other modules work with enetities
	m_QuadTree<Unit*>			units_quadtree;
	m_QuadTree<Building*>		buildings_quadtree;

	//Functionality -------------------
	//Load all chosen civilization data
	bool		LoadCivilization(const char* folder);
	//Factory Methods -------
	Unit*		GenerateUnit(UNIT_TYPE type, DIPLOMACY diplomacy, bool push_in_list = true);
	Building*	GenerateBuilding(BUILDING_TYPE type, DIPLOMACY diplomacy, bool push_in_list = true);

	//Get Methods -----------
	const std::list<Unit*>*		UnitsList()const;
	const std::list<Building*>*	BuildingList()const;

	//Delete Methods --------
	bool		DeleteEntity(Entity* entity);

	//Entities Management Methods
	bool		SetUnitPath(Unit* target, const iPoint& goal);
	bool		SetGroupPath(const std::vector<Unit*>& targets, const iPoint& goal);

	//Units lists movement units used in buildings cover
	void		AddUnit(Unit* unit);
	Unit*		PopUnit(const Unit* unit);

	//Get the nearest resource save point from the coordinate
	Building* SearchNearestSavePoint(const iPoint& point);

	//Organize unit vector
	std::priority_queue<Unit*, std::vector<Unit*>, LessDistance > OrganizeByNearest(std::vector<Unit*>& vec, Circle& target);
	

};
#endif // _ENTITIES_MANAGER_
