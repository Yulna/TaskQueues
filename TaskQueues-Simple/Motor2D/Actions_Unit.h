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
	MoveUnitAction(Unit* actor, const iPoint& destination, const iPoint& target) : Action(actor, TASK_U_MOVE), destination(destination), target(target)
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
		
		//Calculate the path
		iPoint origin(actor->GetPosition().x, actor->GetPosition().y);
		path = App->pathfinding->SimpleAstar(origin, destination);
		if (path == nullptr)return false;
		
		//Animation methods uncomment it when Activation is done
		((Unit*)actor)->SetAction(WALK);
		((Unit*)actor)->Focus(path->back(),true);

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


#endif // !__ACTION_UNIT_H__

