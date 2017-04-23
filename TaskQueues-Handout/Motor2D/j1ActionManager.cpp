#include "j1ActionManager.h"
#include "Actions_Unit.h"


///Class Action ---------------------------------
//Base class to build action definitions
//Constructors ========================
Action::Action(Entity* actor) : actor(actor)
{

}

//Destructors =========================
Action::~Action()
{
	actor = nullptr;
}
/// ---------------------------------------------


/// Action Manager ------------------------------
//Action Manager Module
//Constructors ========================
j1ActionManager::j1ActionManager()
{
	name = "action_manager";
}

//Destructors =========================
j1ActionManager::~j1ActionManager()
{

}

//Game Loop ===========================
bool j1ActionManager::CleanUp()
{
	//Freeing space of all actions created
	all_actions.clear();

	return true;
}

//Action Calls ========================
MoveUnitAction* j1ActionManager::MoveAction(Unit * actor, const iPoint& destination)
{
	//Generate a new move action definition
	MoveUnitAction* action = new MoveUnitAction(actor, destination);

	//Add the new action at the action manager
	all_actions.push_back(action);

	return action;
}
/// ---------------------------------------------



///Action Worker---------------------------------
//Constructor
ActionWorker::ActionWorker()
{
}

//Destructor
ActionWorker::~ActionWorker()
{
	Reset();
}

//Worker methods
void ActionWorker::Update()
{
	
	if (current_action != nullptr)
	{
		//TODO 7: Execute the action
		//If the action ends (execute() == true) erase it and set it to null



		return;
	}
	//If the worker has any waiting actions mark the first one as current
	else if (!action_queue.empty())
	{
		//TODO 6: Pick the next action from the queue and Activate it
		//The first action form the list should be now the current_action and be removed from the list
		//Don't foget to delete it if the Activation() fails


		//Delete action if it can't be activated

		
	}
	return;




}



void ActionWorker::AddAction(Action * action)
{
	//TODO 5: Add the new action to the list

}


void ActionWorker::Reset()
{
	//TODO 8: Free all actions on the worker

	//---

}



///----------------------------------------------