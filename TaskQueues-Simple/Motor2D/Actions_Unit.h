#ifndef __ACTION_UNIT_H__
#define __ACTION_UNIT_H__

#include "j1ActionManager.h"
#include "BaseEntities.h"
#include "j1App.h"
#include "j1EntitiesManager.h"

//Needed to have the MoveUnitAction with the path
#include "j1Pathfinding.h"

/// Move Unit Action ----------------------------
//Move the character
class MoveUnitAction : public Action
{
public:
	//Constructor -----------
	MoveUnitAction(Unit* actor, const iPoint& destination) : Action(actor, TASK_U_MOVE), destination(destination)
	{
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
		//TODO 3: Fill the activation method---------------------------------------
		//We need to store the actual position of the entity as origin and calculate a path to the destination
		///App->pathfinding->SimpleAstar(...) will return us an iPoint vector to use as path
		iPoint origin(actor->GetPosition().x, actor->GetPosition().y);
		path = App->pathfinding->SimpleAstar(origin, destination);

		//If we don't get a valid path return false to indicate that activation has failed
		if (path == nullptr)return false;
		
		//Animation methods uncomment it when Activation is done
		((Unit*)actor)->SetAction(WALK);
		((Unit*)actor)->Focus(path->back(),true);
		return true;

	}

	bool Execute()
	{
		//TODO 4: Fill the Execute() method
		//Easy one, just call the Move method in the Unit class, the Move method already returns true when it ends the path
		return ((Unit*)actor)->Move(path);
	}

	//Returns the path 
	std::vector<iPoint>* GetPath() const
	{
		return path;
	};

private:

	std::vector<iPoint>* path = nullptr;

	iPoint destination;
};
/// ---------------------------------------------


#endif // !__ACTION_UNIT_H__

