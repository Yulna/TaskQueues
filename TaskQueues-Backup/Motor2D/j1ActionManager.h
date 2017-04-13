#ifndef __J1ACTION_MANAGER_H__
#define __J1ACTION_MANAGER_H__

#include "j1Module.h"
#include <queue>
#include "p2Point.h"
#include "j1Timer.h"

class Entity;
class Unit;
class Building;
class ProductiveBuilding;
class Villager;

class MoveUnitAction;


enum UNIT_TYPE;
enum BUILDING_TYPE;
enum RESOURCE_TYPE;
enum DIPLOMACY;


enum TASK_TYPE
{
	TASK_NONE = 0,
	TASK_U_MOVE,
	TASK_U_ATTACK_U,
	TASK_U_ATTACK_B,
	TASK_U_DIE,
	TASK_B_DIE,
	TASK_U_STUN,
	TASK_U_RECOLLECT,
	TASK_U_SAVE_RESOURCES,
	TASK_U_SCANN,
	TASK_B_SPAWN_UNITS
};


/// Class Action --------------------------------
//Action virtual class (are commands but not for the console)
class Action
{
public:

	//Set all the Actions to a list on the Entities manager to clean them up at closing the app.
	Action(Entity* actor, TASK_TYPE type);
	~Action();

protected:

	Entity*					actor = nullptr;
	TASK_TYPE				type = TASK_NONE;

public:

	//This function defines the action taking place
	//Returns false if Action was unable to initialize
	virtual bool Activation() { return true; }
	//Returns TRUE when execute is finished
	virtual bool Execute()	{ return true; }
	///Each different action inheriting from this class should have custom
	///properties to do its actions.

	//Get methods -----------
	TASK_TYPE GetType();

};
/// ---------------------------------------------


///Action Worker---------------------------------
class ActionWorker
{
public:

	ActionWorker();
	~ActionWorker();

private:

	std::list<Action*> action_queue;
	std::list<Action*> secondary_action_queue;
	std::list<Action*> passive_action_queue;
	Action* current_passive_action = nullptr;
	Action* current_secondary_action = nullptr;
	Action* current_action = nullptr;
	bool paused = false;

private:
	j1Timer refresh_timer;
	uint	refresh_rate = 0;

public:
	//Updates every list
	void Update();

	void AddAction(Action* action);
	void AddPassiveAction(Action* action);
	void AddSecondaryAction(Action* action);
	void AddPriorizedAction(Action* action);

	//Clean all actionss of the worker
	void Reset();
	void ResetActive();

	TASK_TYPE GetCurrentActionType() const;
	Action* GetCurrentAction() const;

	void Pause();
	void Restart();

private:

	///These are called by the public functions
	//Makes the Actions do their Execute and Activation
	//Returns true if the current action has been executed correctly
	bool DoWork(std::list<Action*>* queue, Action** current);

	//Resets a list and their current  action
	void ResetQueue(std::list<Action*>* queue, Action** current);


};
///----------------------------------------------

/// Action Manager ------------------------------
//Action Manager Module
class j1ActionManager : public j1Module
{
public:

	j1ActionManager();
	~j1ActionManager();

	// Called before quitting
	bool CleanUp();

public:

	//Action Calls --------------------
	//Move Functions
	MoveUnitAction*				MoveAction(Unit* actor, const iPoint& destination);

private:

	//List of all action JUST for CLEANUP 
	std::list<Action*> all_actions;

};
/// ---------------------------------------------
#endif // __ACTION_MANAGER_H__
