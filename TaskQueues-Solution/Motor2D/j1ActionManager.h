#ifndef __J1ACTION_MANAGER_H__
#define __J1ACTION_MANAGER_H__

#include "j1Module.h"
#include <queue>
#include "p2Point.h"
#include "j1Timer.h"

class Entity;
class Unit;

class MoveUnitAction;

enum UNIT_TYPE;




/// Class Action --------------------------------
//Action virtual class
class Action
{
public:

	//Set all the Actions to a list on the Entities manager to clean them up at closing the app.
	Action(Entity* actor);
	~Action();

protected:
	//Since this class Action will be used purely for the AI on entities
	//we store a pointer to the entity in the base class
	Entity*					actor = nullptr;

public:
	//Returns false if the Action couldn't be activated
	virtual bool Activation() { return true; }

	//See that this time the Execute() returns a bool to allow us to know
	//if the action is in progress or it has ended
	///Returns TRUE when the action finishes
	virtual bool Execute()	{ return true; }

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
	Action* current_action = nullptr;

public:
	//Updates every list
	void Update();

	void AddAction(Action* action);

	//Clean all actionss of the worker
	void Reset();


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

