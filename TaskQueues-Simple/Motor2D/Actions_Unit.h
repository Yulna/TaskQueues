#ifndef __ACTION_UNIT_H__
#define __ACTION_UNIT_H__

#include "j1ActionManager.h"
#include "BaseEntities.h"
#include "j1App.h"
#include "j1EntitiesManager.h"
#include "j1SoundManager.h"

//Needed to have the MoveUnitAction with the path
#include "j1Pathfinding.h"

/// Move Unit Action ----------------------------
//Move the character
class MoveUnitAction : public Action
{
public:
	//Constructor -----------
	MoveUnitAction(Unit* actor, const iPoint& destination, const iPoint& target) : Action(actor, TASK_U_MOVE), destination(destination), target(target)
	{
	}

	MoveUnitAction(Unit* actor, std::vector<iPoint>* path, const iPoint& target) : Action(actor, TASK_U_MOVE), path(path),target(target)
	{
		destination = path->front();
	}

	//Destructor ------------
	~MoveUnitAction()
	{
		this->actor = nullptr;
		delete [] path;
	}

	//Functionality ---------
	bool Activation()
	{
		
		//Calculate the path
		iPoint origin(actor->GetPosition().x, actor->GetPosition().y);
		path = App->pathfinding->SimpleAstar(origin, destination);
		if (path == nullptr)return false;
		
		((Unit*)actor)->SetAction(WALK);
		((Unit*)actor)->Focus(path->back(),true);
		if (path->size() > 2)	((Unit*)actor)->SetFutureAction(*(path->rbegin() + 1));
		else					((Unit*)actor)->SetFutureAction(iPoint(-1, -1));

		return true;
	}

	bool Execute()
	{
		return ((Unit*)actor)->Move(path,target);
	}

	//Returns the path 
	std::vector<iPoint>* GetPath() const
	{
		return path;
	};

private:

	std::vector<iPoint>* path = nullptr;

	iPoint destination;
	iPoint target;
};
/// ---------------------------------------------

/// Attack Unit Action ----------------------------
//Move the character
class AttackUnitAction : public Action
{
public:

	//Constructor -----------
	AttackUnitAction(Unit* actor,Unit** target) : Action(actor, TASK_U_ATTACK_U), target(target)
	{
		//Set actor interaction target
	//	((Unit*)actor)->SetInteractionTarget(target);
	}

	//Destructor ------------
	~AttackUnitAction()
	{
		this->actor = nullptr;
		this->target = nullptr;
	}

	//Functionality ---------
	bool Execute()
	{
		if ((*target) == nullptr) return true;

		//Actor attack the target
		return ((Unit*)actor)->AttackUnit(target);
	}

private:

	Unit** target = nullptr;
};
/// ---------------------------------------------

/// Attack Building Action ----------------------------
//Move the character
class AttackBuildingAction : public Action
{
public:

	//Constructor -----------
	AttackBuildingAction(Unit* actor, Building** target) : Action(actor, TASK_U_ATTACK_B), target(target)
	{

	}

	//Destructor ------------
	~AttackBuildingAction()
	{
		this->actor = nullptr;
		this->target = nullptr;
	}

	//Functionality ---------
	bool Execute()
	{
		//Actor attack the target
		return ((Unit*)actor)->AttackBuilding(target);
	}

private:

	Building** target = nullptr;
};
/// ---------------------------------------------

/// Die Action ----------------------------
//Kill the character
class DieUnitAction : public Action
{
public:

	//Constructor -----------
	DieUnitAction(Unit* actor) : Action(actor, TASK_U_DIE)
	{
	}

	//Functionality ---------
	bool Activation() 
	{
		App->sound->PlayFXAudio(DEATH_SOUND);
		return true;
	}

	bool Execute()
	{
		//Actor attack the target
		return ((Unit*)actor)->Die();
	}
};
/// ---------------------------------------------



///Scann for units action------------------------
class ScannAction : public Action
{
public:

	//Constructor -------------
	ScannAction(Unit* actor) : Action(actor, TASK_U_SCANN)
	{

	}

	//Destructor ------------
	~ScannAction()
	{
		this->actor = nullptr;
		surrounding_units.clear();
		surrounding_buildings.clear();
	}

public:

	bool Execute()
	{
		if (actor->GetWorker()->GetCurrentActionType() == TASK_U_DIE) return true;

		surrounding_units.clear();
		surrounding_buildings.clear();

		App->entities_manager->units_quadtree.CollectCandidates(surrounding_units, actor->GetVision());
		App->entities_manager->OrganizeByNearest(surrounding_units, actor->GetVision());

		App->entities_manager->buildings_quadtree.CollectCandidates(surrounding_buildings, actor->GetVision());


		uint size = surrounding_units.size();

			for (uint i = 0; i < size; i++)
			{
				if (actor->GetDiplomacy() != surrounding_units[i]->GetDiplomacy() && surrounding_units[i]->GetAction() != (DIE || DISAPPEAR))
				{
					actor->AddPriorizedAction(App->action_manager->AttackToUnitAction((Unit*)actor, (Unit**)surrounding_units[i]->GetMe()));
					return false;
				}
			};

			uint sizeb = surrounding_buildings.size();

			for (uint k = 0; k < sizeb; k++)
			{
				if (actor->GetDiplomacy() != surrounding_buildings[k]->GetDiplomacy())
				{
					actor->AddPriorizedAction(App->action_manager->AttackToBuildingAction((Unit*)actor, (Building**)surrounding_buildings[k]->GetMe()));
					return false;
				}
			};

		return false;
	};


private:

	std::vector<Unit*> surrounding_units;
	std::vector<Building*> surrounding_buildings;
};




///----------------------------------------------
#endif // !__ACTION_UNIT_H__

