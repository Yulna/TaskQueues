#ifndef _BASE_ENTITIES_
#define _BASE_ENTITIES_

#include <list>
#include <vector>
#include <queue>
#include "p2Defs.h"
#include "p2Point.h"
#include "Iso_Primitives.h"
#include "SDL/include/SDL_rect.h"
#include "j1Timer.h"
#include "j1Animator.h"
#include "j1ActionManager.h"

struct	SDL_Texture;
struct	PathNode;
enum	CURSOR_TARGET;

#define FOCUS_MARGIN 12

// ENTITIES ENUMS -----------
enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	UNIT,
	RESOURCE,
	BUILDING
};
enum UNIT_TYPE
{
	NO_UNIT = 0,
	MILITIA,
	ARBALEST,
	ARCHER,
	CAVALIER,
	CAVALRY_ARCHER,
	ELITE_SKIRMISHER,
	HEAVY_CAVALRY_ARCHER,
	KNIGHT,
	MONK,
	PALADIN,
	PIKEMAN,
	SPEARMAN,
	VILLAGER,
	WARRIOR_CHMP,
	ARCHER_CHMP,
	WIZARD_CHMP
};
enum UNIT_CLASS
{
	CIVILIAN = 0,
	INFANTRY,
	ARCHERY,
	CAVALRY,
	SIEGE
};
enum BUILDING_TYPE
{
	NO_BUILDING = 0,
	FARM,
	TOWN_CENTER,
	BARRACK,
	RUBBLE_THREE,
	RUBBLE_FOUR
};
enum ACTION_TYPE
{
	NO_ACTION = 0,
	ATTATCK,
	DIE,
	DISAPPEAR,
	IDLE,
	WALK
};
enum DIRECTION_TYPE
{
	NO_DIRECTION = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};
enum DIPLOMACY
{
	NEUTRAL,
	ALLY,
	ENEMY
};

// --------------------------

///Class Entity ---------------------------------
//Base Class that define the general attributes of all kind of entities
class Entity
{
public:

	Entity();
	Entity(const Entity& copy);

	~Entity();

protected:

	//State ------------
	bool			selected = false;
	std::string		name;
	fPoint			position = { 0,0 };
	ENTITY_TYPE		entity_type = NO_ENTITY;
	DIPLOMACY		entity_diplomacy = NEUTRAL;
	Circle			vision;
	//Life -------------
	uint			max_life = 0;
	int				life = 0;
	//Visual -----------
	Animation*		current_animation = nullptr;
	bool			flip_sprite = false;
	SDL_Rect		selection_rect = { 0,0,0,0 };
	SDL_Rect		icon_rect = { 0,0,0,0 };
	//Worker -----------
	ActionWorker	action_worker;

public:

	Entity*			myself = nullptr;

public:

	//Functionality -------------------
	//Select/Deselect ------
	void			Select();
	void			Deselect();

	//Operators -------------
	bool operator == (const Entity& tar);

	//Update ----------------
	virtual bool	Update();
	virtual bool	Die();

	//Draw ------------------
	virtual bool	Draw(bool debug);
	void			CleanAnimation();
	virtual void	CleanMapLogic();
	//Add Action ------------
	void			AddAction(Action* action);
	void			AddPriorizedAction(Action* action);

	//Set Methods -----------
	void			SetName(const char* name_str);
	virtual void	SetPosition(float x, float y, bool insert = true);
	void			OnlySetPosition(float x, float y);
	void			SetEntityType(ENTITY_TYPE type);
	void			SetDiplomacy(DIPLOMACY new_diplomacy);
	void			SetVision(const Circle& new_mark);
	void			SetMaxLife(uint full_life_val);
	void			SetLife(uint life_val);
	void			SetAnimation(Animation* anim);
	void			SetFlipSprite(bool flip);
	void			SetSelectionRect(const SDL_Rect& rect);
	void			SetIcon(const SDL_Rect& icon);

	//Get Methods -----------
	const char*		GetName()const;
	const fPoint&	GetPosition()const;
	iPoint			GetPositionRounded()const;
	ENTITY_TYPE		GetEntityType()const;
	DIPLOMACY		GetDiplomacy()const;
	Circle			GetVision()const;
	uint			GetMaxLife()const;
	int				GetLife()const;
	Animation*		GetAnimation();
	bool			GetFlipSprite()const;
	const SDL_Rect*	GetSelectionRect()const;
	const SDL_Rect&	GetIcon()const;
	ActionWorker*	GetWorker();
	Entity**		GetMe();
};
/// ---------------------------------------------

///Class Unit -----------------------------------
//Base class that define the general attributes for all units
class Unit : public Entity
{
public:

	Unit();
	Unit(const Unit& copy);

	~Unit();

protected:

	//Stats ----------------------
	UNIT_TYPE		unit_type = NO_UNIT;

	Circle			mark;
	Circle			soft_collider;
	Circle			hard_collider;
	Entity*			interaction_target = nullptr;

	//Movement ---------
	uint			view_area = 0;
	float			speed = 0.0f;
	float			mutable_speed = 0.0f;
	ACTION_TYPE		action_type = IDLE;
	DIRECTION_TYPE	direction_type = SOUTH;
	iPoint			future_position = { 0,0 };
	//Resources --------
	uint			train_time = 0;
	uint			exp = 0;
	
	// Attack area
	Circle	attack_area;

public:

	float distance_to_target = 0;

public:

	//Functionality -------------------
	//Operator --------------
	bool operator<(const Unit& target) const;

	//Draw ------------------
	bool	Draw(bool debug);

	//Actions ---------------
	bool					Move(std::vector<iPoint>*& path);
	void					Repath(std::vector<iPoint>*& path,const iPoint & destination);
	void					CorrectPath(std::vector<iPoint>*& path);
	iPoint					NextGoal(std::vector<iPoint>* path);
	void					NewPosition(const iPoint& goal,float& position_x, float& position_y);
	iPoint					FindWalkableCell(const iPoint& center);
	bool					UnitHere(std::vector<Unit*> other_units,const iPoint& destination);
	bool					UnitHere(const iPoint& destination, int radius);
	iPoint					FindWalkableAdjacent(const iPoint& center);
	Unit**					FindNewTarget();
	void					Focus(const iPoint& target, bool play = true);
	DIRECTION_TYPE			LookDirection(const iPoint& from, const iPoint& to);


	//Set Methods -----------
	void	SetPosition(float x, float y, bool insert = true);
	void	SetFutureAction(const iPoint& future_position);
	void	SetUnitType(UNIT_TYPE type);
	void	SetInteractionTarget(const Entity* target);

	void	SetMark(const Circle& new_mark);
	void	SetSoftCollider(const Circle& new_soft_collider);
	void	SetHardCollider(const Circle& new_hard_collider);
	void	SetViewArea(uint area_val);
	void	SetSpeed(float speed_val);
	void	SetAction(ACTION_TYPE action_val);
	void	SetDirection(DIRECTION_TYPE direction_val);
	void	SetTrainTime(uint train_time_val);
	void	SetExp(uint experience);

	//Get Methods -----------
	UNIT_TYPE		GetUnitType()const;
	const Circle&	GetMark()const;
	const Circle&	GetSoftCollider()const;
	const Circle&	GetHardCollider()const;

	const Entity*	GetInteractionTarget();
	uint			GetViewArea()const;
	float			GetSpeed()const;
	ACTION_TYPE		GetAction()const;
	DIRECTION_TYPE	GetDirection()const;
	uint			GetAttackRange()const;
	const Circle*	GetAttackArea()const;
	uint			GetTrainTime()const;
	uint			GetExp()const;
	std::vector<iPoint>* GetPath() const;

};
/// ---------------------------------------------

#endif // _ENTITY_
