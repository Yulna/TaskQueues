#include "j1ActionManager.h"
#include "Actions_Unit.h"


///Class Action ---------------------------------
//Base class to build action definitions
//Constructors ========================
Action::Action(Entity* actor, TASK_TYPE type) : actor(actor), type (type)
{

}

//Destructors =========================
Action::~Action()
{
	actor = nullptr;
}

TASK_TYPE Action::GetType()
{
	return type;
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
ActionWorker::ActionWorker() : refresh_rate(500)
{
}

//Destructor
ActionWorker::~ActionWorker()
{
	ResetQueue(&action_queue, &current_action);
	ResetQueue(&passive_action_queue, &current_passive_action);
	ResetQueue(&secondary_action_queue, &current_secondary_action);
}

//Worker methods
void ActionWorker::Update()
{
	//Don't update if the worker is paused
	if (paused)
		return;
		
	//If the worker has a current action execute it
	if (DoWork(&action_queue, &current_action))
	{

		//Reeactive all actions form passive flows when any active action finshes
		if(current_secondary_action != nullptr)	current_secondary_action->Activation();
		if(current_passive_action != nullptr)	current_passive_action->Activation();
	}

	//Secondary and passive actions take place when there is no active_action
	if (current_action == nullptr)
	{
		DoWork(&secondary_action_queue, &current_secondary_action);

		//Make the passive_action logic only work on a refresh rate
		if (refresh_timer.Read() > refresh_rate)
		{
			DoWork(&passive_action_queue, &current_passive_action);
			refresh_timer.Start();
		}
	}
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
	action_queue.push_back(action);
}

void ActionWorker::AddPassiveAction(Action* action)
{
	passive_action_queue.push_back(action);
}

void ActionWorker::AddSecondaryAction(Action * action)
{
	secondary_action_queue.push_back(action);
}

void ActionWorker::AddPriorizedAction(Action * action)
{
	if (current_action != nullptr)
	{
		action_queue.push_front(current_action);
		current_action = action;
		current_action->Activation();
	}
	else
	{
		current_action = action;
		current_action->Activation();
	}
}


void ActionWorker::Reset()
{
	ResetQueue(&action_queue, &current_action);
	ResetQueue(&passive_action_queue, &current_passive_action);

	paused = false;
}

void ActionWorker::ResetActive()
{
	ResetQueue(&action_queue, &current_action);
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

TASK_TYPE ActionWorker::GetCurrentActionType() const
{
	if (current_action != nullptr)
	{
		return current_action->GetType();
	}

	return TASK_TYPE::TASK_NONE;
}

Action * ActionWorker::GetCurrentAction() const
{
	return current_action;
}

void ActionWorker::Pause()
{
	paused = true;
}
void ActionWorker::Restart()
{
	paused = false;
}
///----------------------------------------------