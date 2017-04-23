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
	ResetQueue(&action_queue, &current_action);
}

//Worker methods
void ActionWorker::Update()
{
	
	if (current_action != nullptr)
	{
		//TODO 7: Execute the action
		//If the action ends (execute() == true) erase it and set it to null
		if (current_action->Execute())
		{
			delete current_action;
			current_action = nullptr;
			return;
		}
		return;
	}
	//If the worker has any waiting actions mark the first one as current
	else if (!action_queue.empty())
	{
		//TODO 6: Pick the next action from the queue and Activate it
		//The first action form the list should be now the current_action and be removed from the list
		//Don't foget to delete it if the Activation() fails

		current_action = action_queue.front();
		action_queue.pop_front();

		//Delete action if it can't be activated
		if (!current_action->Activation())
		{
			delete current_action;
			current_action = nullptr;
		}
	}
	return;



	//Fancy ways of doing things
	//DoWork(&action_queue, &current_action);
}

bool ActionWorker::DoWork(std::list<Action*>* queue, Action ** current)
{
	//If the worker has a current action execute it
	if (*current != nullptr)
	{
		//If the action ends (execute() == true) erase it
		if ((*current)->Execute())
		{
			delete (*current);
			(*current) = nullptr;
			return true;
		}
		return false;
	}
	//If the worker has any waiting actions mark the first one as current
	if (!queue->empty() && *current == nullptr)
	{
		(*current) = queue->front();
		queue->pop_front();

		//Delete action if it can't be activated
		if (!(*current)->Activation())
		{
			delete (*current);
			(*current) = nullptr;
		}
	}
	return false;
}


void ActionWorker::AddAction(Action * action)
{
	//TODO 5: Add the new action to the list
	action_queue.push_back(action);
}


void ActionWorker::Reset()
{
	//TODO 8: Free all actions on the worker
	delete current_action;
	current_action = nullptr;

	Action* to_erase;
	while (!action_queue.empty())
	{
		to_erase = action_queue.front();
		action_queue.pop_front();
		RELEASE(to_erase);
	}
	action_queue.clear();
	//---


	//Fancy way of doing it
	//ResetQueue(&action_queue, &current_action);
}


void ActionWorker::ResetQueue(std::list<Action*>* queue, Action ** current)
{
	delete (*current);
	(*current) = nullptr;

	Action* to_erase;

	while (!queue->empty())
	{
		to_erase = queue->front();
		queue->pop_front();
		RELEASE(to_erase);
	}

	queue->clear();
}


///----------------------------------------------