#include "BaseEntities.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Animator.h"
#include "j1Pathfinding.h"
#include "j1Fonts.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1EntitiesManager.h"
#include "j1ActionManager.h"
#include "j1Player.h"

#include "Actions_Unit.h"

///Class Entity ---------------------------------
//Constructors ========================
Entity::Entity() :name(""), action_worker()
{
}

Entity::Entity(const Entity& copy) : name(copy.name), position(copy.position), entity_type(copy.entity_type), entity_diplomacy(copy.entity_diplomacy), vision(copy.vision), selection_rect(copy.selection_rect),
icon_rect(copy.icon_rect), max_life(copy.max_life), life(copy.life), current_animation(copy.current_animation), action_worker()
{

}

//Destructors =========================
Entity::~Entity()
{
	if (current_animation != nullptr)
	{
		delete current_animation;
	}
	current_animation = nullptr;
	myself = nullptr;
}


//Functionality =======================
//Select/Deselect -
void Entity::Select()
{
	selected = true;
}

void Entity::Deselect()
{
	selected = false;
}

//Operators -------------
bool Entity::operator == (const Entity& tar)
{
	return (myself == tar.myself && position == tar.position && entity_type == tar.entity_type && tar.life == life);
}

//Update ----------
bool Entity::Update()
{
	action_worker.Update();
	return true;
}

bool Entity::Die()
{
	return true;
}

// Draw -----------
bool Entity::Draw(bool debug)
{
	bool ret = false;

	//Draw Entity Current animation frame
	const Sprite* sprite = current_animation->GetCurrentSprite();
	ret = App->render->CallBlit(current_animation->GetTexture(), position.x, position.y, sprite->GetFrame(), flip_sprite, -position.y - sprite->GetZ_cord(), sprite->GetXpivot(), sprite->GetYpivot());

	return ret;
}

void Entity::CleanAnimation()
{
	if (current_animation != nullptr)
	{
		delete current_animation;
		current_animation = nullptr;
	}
}

void Entity::CleanMapLogic()
{

}

//Add Action ------------
void Entity::AddAction(Action * action)
{
	action_worker.AddAction(action);
}

//Set Methods -----
void Entity::SetName(const char * name_str)
{
	name = name_str;
}

void Entity::SetPosition(float x, float y, bool insert)
{
	position.x = x;
	position.y = y;
}

void Entity::OnlySetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Entity::SetEntityType(ENTITY_TYPE type)
{
	entity_type = type;
}

void Entity::SetDiplomacy(DIPLOMACY new_diplomacy)
{
	entity_diplomacy = new_diplomacy;
}

void Entity::SetVision(const Circle & new_vision)
{
	vision = new_vision;
}

void Entity::SetMaxLife(uint full_life_val)
{
	max_life = full_life_val;
}

void Entity::SetLife(uint life_val)
{
	life = life_val;
}

void Entity::SetAnimation(Animation * anim)
{
	current_animation = anim;
}

void Entity::SetFlipSprite(bool flip)
{
	flip_sprite = flip;
}

void Entity::SetSelectionRect(const SDL_Rect & rect)
{
	selection_rect = rect;
}

void Entity::SetIcon(const SDL_Rect & icon)
{
	icon_rect = icon;
}

// ----------------
//Get Methods -----
const char* Entity::GetName() const
{
	return name.c_str();
}

const fPoint& Entity::GetPosition() const
{
	return position;
}

iPoint Entity::GetPositionRounded() const
{
	return iPoint(position.x, position.y);
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

DIPLOMACY Entity::GetDiplomacy() const
{
	return entity_diplomacy;
}

Circle Entity::GetVision() const
{
	return vision;
}

uint Entity::GetMaxLife() const
{
	return max_life;
}

int Entity::GetLife() const
{
	return life;
}

Animation* Entity::GetAnimation() 
{
	return current_animation;
}

bool Entity::GetFlipSprite() const
{
	return flip_sprite;
}

const SDL_Rect * Entity::GetSelectionRect() const
{
	return &selection_rect;
}

const SDL_Rect& Entity::GetIcon()const
{
	return icon_rect;
}

ActionWorker * Entity::GetWorker()
{
	return &action_worker;
}

Entity ** Entity::GetMe()
{
	return &myself;
}


// ----------------
///----------------------------------------------


///Class Unit -----------------------------------
//Constructors ========================
Unit::Unit() :Entity()
{

}

Unit::Unit(const Unit& copy) : Entity(copy), unit_type(copy.unit_type), mark(copy.mark),soft_collider(copy.soft_collider),hard_collider(copy.hard_collider), view_area(copy.view_area),
speed(copy.speed), action_type(copy.action_type), direction_type(copy.direction_type), train_time(copy.train_time)
{

}

//Destructors =========================
Unit::~Unit()
{

}

bool Unit::operator<(const Unit & target) const
{
	return distance_to_target < target.distance_to_target;
}

//Functionality =======================
//Draw ------------
bool Unit::Draw(bool debug)
{
	bool ret = false;

	//Draw Entity Mark
	if (selected)ret = mark.Draw();
	
	if (debug) {
		if (selected)
			attack_area.Draw();
			soft_collider.Draw();
			hard_collider.Draw();
			vision.Draw();
	}
	

	//Draw Entity Current animation frame
	const Sprite* sprite = current_animation->GetCurrentSprite();
	ret = App->render->CallBlit(((DiplomaticAnimation*)current_animation)->GetTexture(entity_diplomacy), position.x, position.y, sprite->GetFrame(), flip_sprite, -position.y - sprite->GetZ_cord(), sprite->GetOpacity(), sprite->GetXpivot(), sprite->GetYpivot());

	return ret;
}


//Actions ---------
bool Unit::Move(std::vector<iPoint>*& path) ///Returns true when it ends
{
	//Check if the unit have an assigned path
	if (path == nullptr)
	{
		LOG("Error path not found!");
		return true;
	}


	//Build goal path point
	iPoint goal = path->back();
	iPoint location = iPoint(position.x, position.y);
	float x_step = 0.0f;
	float y_step = 0.0f;
	int path_size = path->size();
	//Update goal node and animation direction
	if (location.DistanceTo(goal) < 2)
	{
	
		if (path->size() == 1)
		{
			//Set unit at the goal pixel position
			SetPosition((float)goal.x, (float)goal.y);
			//Stop idle walk animation
			action_type = IDLE;
			App->animator->UnitPlay(this);

			//Delete unit path
			delete path;
			path = nullptr;

			return true;
		}

		//Set the unit next tile goal
		goal = NextGoal(path);

	}

	//Check actor animation
	if (action_type != WALK)
	{
		action_type = WALK;
		Focus(goal);
	}

	//Calculate the X/Y values that the unit have to move 
	//checking the goal location and the unit movement speed
	NewPosition(goal, x_step, y_step);

	//Add the calculated values at the unit & mark position
	SetPosition(x_step, y_step);

	return false;
}


void Unit::Repath(std::vector<iPoint>*& path,const iPoint & destination)
{
	iPoint new_destination = FindWalkableCell(destination);
	std::vector<iPoint>* new_path;
	new_path = App->pathfinding->SimpleAstar(GetPositionRounded(), new_destination);

	if (new_path != nullptr) {

		path->clear();
		delete path;

		path = new_path;
	}
}

void Unit::CorrectPath(std::vector<iPoint>*& path)
{
	std::vector<iPoint>* new_path;
	path->pop_back();
	iPoint next_goal;
	for (int i = path->size() - 1; i >= 0; i--) {
		if (App->pathfinding->IsWalkable(App->map->WorldToMap(path->at(i).x, path->at(i).y))) {
			next_goal = path->at(i);
			break;
		}
		else
			path->pop_back();

	}
	if (path->empty())
	{
		return;
	}
	new_path = App->pathfinding->SimpleAstar(GetPositionRounded(), next_goal);
	if (new_path != nullptr) {
		LOG("PATH SIZE %i", new_path->size());
		path->insert(path->end(), new_path->begin(), new_path->end());
	}
}

iPoint Unit::NextGoal(std::vector<iPoint>* path)
{
	iPoint goal;
	path->pop_back();

	if (path->size() >= 2)	SetFutureAction(*(path->rbegin() + 1));

	else					SetFutureAction(*(path->rbegin()));

	goal = path->back();

	//Focus the unit at the next goal
	Focus(goal);
	return goal;
}

void Unit::NewPosition(const iPoint& goal, float & position_x, float & position_y)
{
	iPoint location = iPoint(position.x, position.y);

	int norm = location.DistanceTo(goal);
	float x_step = GetSpeed() * (App->GetDT() * 100) * (goal.x - location.x) / norm;
	float y_step = GetSpeed() * (App->GetDT() * 100) * (goal.y - location.y) / norm;

	//Add the calculated values at the unit & mark position
	position_x = position.x + x_step;
	position_y = position.y + y_step;

}

iPoint Unit::FindWalkableCell(const iPoint & center)
{
	iPoint cell;
	iPoint pos = App->map->WorldToMap(center.x, center.y);
	std::vector<Unit*> other_units;
	int i = 1;
	while (i<=5) {
		// south
		cell.create(pos.x, pos.y + i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}

		// north
		cell.create(pos.x, pos.y - i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}

		// east
		cell.create(pos.x + i, pos.y);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}

		// west
		cell.create(pos.x - i, pos.y);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}

		// south-east
		cell.create(pos.x + i, pos.y + i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}
		// south-west
		cell.create(pos.x - i, pos.y + i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}
		// north-east
		cell.create(pos.x + i, pos.y - i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}
		// north-west
		cell.create(pos.x - i, pos.y - i);
		if (App->pathfinding->IsWalkable(cell) && UnitHere(other_units, cell))
		{
			return App->map->MapToWorld(cell.x, cell.y);;
		}
		i++;
	}
}

bool Unit::UnitHere(std::vector<Unit*> other_units, const iPoint& cell)
{
	iPoint destination = App->map->MapToWorldCenter(cell.x,cell.y);
	iPoint other_unit_cell;
	for(int i = 0;i<other_units.size();i++)
	{
		
		Unit* other = other_units[i];
		if (this == other)
		{
			continue;
		}
		other_unit_cell = App->map->WorldToMap(other->GetPositionRounded().x, other->GetPositionRounded().y);
		if (other_unit_cell == cell) {
			return false;
		}
		/*
		if (other->action_type == IDLE || other->action_type == ATTATCK)
		{
			if (sqrt((other->GetPositionRounded().x - destination.x) * (other->GetPositionRounded().x - destination.x) + (other->GetPositionRounded().y - destination.y) * (other->GetPositionRounded().y - destination.y)) < (this->hard_collider.GetRad() + other->hard_collider.GetRad()))
			{
				return false;
			}
		}
		else if (other->action_type == WALK) {
			if (sqrt((other->GetPositionRounded().x - destination.x) * (other->GetPositionRounded().x - destination.x) + (other->GetPositionRounded().y - destination.y) * (other->GetPositionRounded().y - destination.y)) < (this->hard_collider.GetRad() + other->hard_collider.GetRad()))
			{
				return false;
			}
		}
		*/
	}
	
	
	return true;
}

bool Unit::UnitHere(const iPoint & destination, int radius)
{


	std::vector<Unit*> other_units;
	Circle area;
	area.SetRad(radius);

	area.SetPosition(destination);

	if (!other_units.empty()) {
		if (other_units.size() == 1 && *(other_units.begin()) == this)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}


iPoint Unit::FindWalkableAdjacent(const iPoint & center)
{
	iPoint cell;
	iPoint pos = App->map->WorldToMap(center.x, center.y);
	iPoint goal;

	int radius = GetSoftCollider().GetRad();
	Circle checker;
	checker.SetRad(radius);

	// south
	cell.create(pos.x, pos.y + 1);
	checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
	goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal,radius))
		{
			return goal;
		}

		// north
		cell.create(pos.x, pos.y - 1);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);

		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}


		// east
		cell.create(pos.x + 1, pos.y);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}


		// west
		cell.create(pos.x - 1, pos.y);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}

		
		// south-east
		cell.create(pos.x + 1, pos.y + 1);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}

		// south-west
		cell.create(pos.x - 1, pos.y + 1);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}

		// north-east
		cell.create(pos.x + 1, pos.y - 1);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);

		
		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}

		// north-west
		cell.create(pos.x - 1, pos.y - 1);
		checker.SetPosition(App->map->MapToWorldCenter(cell.x, cell.y));
		goal = attack_area.NearestPoint(&checker);


		if (App->pathfinding->IsWalkable(cell) && !UnitHere(goal, radius))
		{
			return goal;
		}

	
		return iPoint(-1, -1);
}

Unit** Unit::FindNewTarget()
{
std::vector<Unit*> other_units;

Unit* unit;
iPoint goal;
while (!other_units.empty())
{
	unit = other_units.back();
	other_units.pop_back();

	if(unit->GetDiplomacy()==ALLY||unit->GetAction()==DIE ||unit->GetAction()==DISAPPEAR)
	{
		continue;
	}
	goal = FindWalkableAdjacent(unit->GetPositionRounded());
	if (goal != iPoint(-1, -1)) 
	{

		SetInteractionTarget(unit);
		return (Unit**)unit->GetMe();
	}
}
	return nullptr;
}

void Unit::Focus(const iPoint & target, bool play)
{
	//Calculate the directional vector
	iPoint dir_point = target - iPoint(position.x, position.y);
	bool directed = false;
	//Find the correct direction in relation of the goal and the location
	if (abs(dir_point.x) < FOCUS_MARGIN)
	{
		if (dir_point.y > 0 && direction_type != SOUTH)
		{
			direction_type = DIRECTION_TYPE::SOUTH;
			directed = true;
		}
		else if (dir_point.y <= 0 && direction_type != NORTH)
		{
			direction_type = DIRECTION_TYPE::NORTH;
			directed = true;
		}
	}
	else if (abs(dir_point.y) < FOCUS_MARGIN)
	{
		if (dir_point.x > 0 && direction_type != EAST)
		{
			direction_type = DIRECTION_TYPE::EAST;
			directed = true;
		}
		else if (dir_point.x <= 0 && direction_type != WEST)
		{
			direction_type = DIRECTION_TYPE::WEST;
			directed = true;
		}
	}
	else if (dir_point.x >= 0 && dir_point.y >= 0 && direction_type != SOUTH_EAST)
	{
		direction_type = DIRECTION_TYPE::SOUTH_EAST;
		directed = true;
	}
	else if (dir_point.x <= 0 && dir_point.y >= 0 && direction_type != SOUTH_WEST)
	{
		direction_type = DIRECTION_TYPE::SOUTH_WEST;
		directed = true;
	}
	else if (dir_point.x >= 0 && dir_point.y <= 0 && direction_type != NORTH_EAST)
	{
		direction_type = DIRECTION_TYPE::NORTH_EAST;
		directed = true;
	}
	else if (dir_point.x <= 0 && dir_point.y <= 0 && direction_type != NORTH_WEST)
	{
		direction_type = DIRECTION_TYPE::NORTH_WEST;
		directed = true;
	}

	//Set the unit animation with the new direction
	if (play)
	{
		App->animator->UnitPlay(this);
	}
	else if (directed)
	{
		App->animator->UnitPlay(this);
	}
}

DIRECTION_TYPE Unit::LookDirection(const iPoint & from, const iPoint & to)
{
	//Calculate the directional vector
	iPoint dir_point = from - to;

	//Find the correct direction in relation of the goal and the location
	if (abs(dir_point.x) < FOCUS_MARGIN)
	{
		if (dir_point.y > 0)return SOUTH;
		else return NORTH;
	}
	else if (abs(dir_point.y) < FOCUS_MARGIN)
	{
		if (dir_point.x > 0)return EAST;
		else return WEST;
	}
	else if (dir_point.x >= 0 && dir_point.y >= 0)
	{
		return SOUTH_EAST;
	}
	else if (dir_point.x <= 0 && dir_point.y >= 0)
	{
		return SOUTH_WEST;
	}
	else if (dir_point.x >= 0 && dir_point.y <= 0)
	{
		return NORTH_EAST;
	}
	else if (dir_point.x <= 0 && dir_point.y <= 0)
	{
		return NORTH_WEST;
	}
	else {
		return NO_DIRECTION;
	}
}




//Set Methods -----
void Unit::SetPosition(float x, float y, bool insert)
{
	//Extract the units to push it with the new position later

	//Set unit position
	position.x = x;
	position.y = y;
	iPoint pos(position.x, position.y);
	//Set unit vision position
	vision.SetPosition(pos);
	//Set unit mark position
	mark.SetPosition(pos);
	//Set soft_collider mark position
	soft_collider.SetPosition(pos);
	//Set hard_collider mark position
	hard_collider.SetPosition(pos);
	//Set unit attack area position
	attack_area.SetPosition(pos);

	//Add the unit with the correct position in the correct quad tree
}

void Unit::SetFutureAction(const iPoint & position)
{
	future_position = position;
}

void Unit::SetUnitType(UNIT_TYPE type)
{
	unit_type = type;
}

void Unit::SetInteractionTarget(const Entity * target)
{
	interaction_target = (Entity*)target;
}

void Unit::SetMark(const Circle & new_mark)
{
	mark = new_mark;
}

void Unit::SetSoftCollider(const Circle & new_soft_collider)
{
	soft_collider = new_soft_collider;
}

void Unit::SetHardCollider(const Circle & new_hard_collider)
{
	hard_collider = new_hard_collider;
}

void Unit::SetViewArea(uint area_val)
{
	view_area = area_val;
}

void Unit::SetSpeed(float speed_val)
{
	speed = speed_val;
}

void Unit::SetAction(ACTION_TYPE action_val)
{
	action_type = action_val;
}

void Unit::SetDirection(DIRECTION_TYPE direction_val)
{
	direction_type = direction_val;
}

void Unit::SetTrainTime(uint train_time_val)
{
	train_time = train_time_val;
}
void Unit::SetExp(uint experience)
{
	exp = experience;
}

// ----------------
//Get Methods -----
UNIT_TYPE Unit::GetUnitType()const
{
	return unit_type;
}

const Circle& Unit::GetMark() const
{
	return mark;
}

const Circle & Unit::GetSoftCollider() const
{
	return soft_collider;
}

const Circle & Unit::GetHardCollider() const
{
	return hard_collider;
}

const Entity * Unit::GetInteractionTarget()
{
	return interaction_target;
}

uint Unit::GetViewArea()const
{
	return view_area;
}

float Unit::GetSpeed()const
{
	return speed+mutable_speed;
}

ACTION_TYPE Unit::GetAction()const
{
	return action_type;
}

DIRECTION_TYPE Unit::GetDirection()const
{
	return direction_type;
}

uint Unit::GetAttackRange()const
{
	return attack_area.GetRad();
}

const Circle * Unit::GetAttackArea() const
{
	return &attack_area;
}

uint Unit::GetTrainTime() const
{
	return train_time;
}

uint Unit::GetExp() const
{
	return exp;
}

// ----------------
///----------------------------------------------
