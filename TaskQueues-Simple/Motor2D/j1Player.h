#ifndef _J1PLAYER_H_
#define _J1PLAYER_H_

#include "j1Module.h"
#include "j1Timer.h"
#include "p2Point.h"
#include "p2Log.h"
#include "SDL\include\SDL_rect.h"


struct SDL_Texture;


class Entity;
class Unit;
class Resource;
class Building;

class Entity_Profile;


class InputTask
{
public:
	InputTask() {};
	~InputTask() {};

	virtual void Execute() {};
};


class MoveBlue : public InputTask
{
public:
	MoveBlue() {};
	~MoveBlue() {};

	void Execute();
};

class MoveRed : public InputTask
{
public:
	MoveRed() {};
	~MoveRed() {};

	void Execute();
};

//Animator Module -------------------------------
class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

public:

	void Init() { active = false; }


	//Enable and disable modules
	virtual void Enable();

	virtual void Disable();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	std::string			civilization;

	InputTask*			right_click = nullptr;
	InputTask*			left_click  = nullptr;

public:
	Unit* blue_unit = nullptr;
	Unit* red_unit = nullptr;

};
#endif // !_J1PLAYER_H_
