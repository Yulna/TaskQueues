#define _CRT_SECURE_NO_WARNINGS

#include "j1EntitiesManager.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Animator.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "p2Log.h"

//Testing purposes only should be erased
#include "j1Scene.h"

//Test Action Manager
#include "j1ActionManager.h"

//Constructors ========================
j1EntitiesManager::j1EntitiesManager()
{
	name = "entities_manager";
}

//Destructors =========================
j1EntitiesManager::~j1EntitiesManager()
{

}

//Game Loop ===========================
bool j1EntitiesManager::Awake(pugi::xml_node & config_node)
{
	return true;
}

void j1EntitiesManager::Disable()
{
	active = false;

	//Clean Up Units List
	std::list<Unit*>::iterator units_item = units.begin();
	while (units_item != units.end())
	{
		RELEASE(units_item._Ptr->_Myval);
		units_item++;
	}
	
}

bool j1EntitiesManager::Start()
{
	//Load Civilization Test
	bool ret = LoadCivilization("Teutons.xml");

	return ret;
}

bool j1EntitiesManager::Update(float dt)
{
	//Update all the units
	std::list<Unit*>::const_iterator item = units.begin();
	bool ret = true;

	while (item != units.end())
	{
		ret = item._Ptr->_Myval->Update();
		item++;
	}


	ret = true;

	return ret;
}

bool j1EntitiesManager::PostUpdate()
{
	//If there's no entities to delete returns
	if (wasted_units.empty())return true;

	//Clean all the wasted entities
	uint size = wasted_units.size();
	for (uint k = 0; k < size; k++)
	{
		//Get the entity type
		ENTITY_TYPE type = wasted_units[k]->GetEntityType();

		//Remove the entity from the correct list
		if (type == UNIT)
		{
			units.remove((Unit*)wasted_units[k]);
		}

		RELEASE(wasted_units[k]);
	}
	wasted_units.clear();

	return true;
}

bool j1EntitiesManager::Draw() const
{
	bool ret = true;

	//Calculate the current camera viewport to collect all the entities inside
	SDL_Rect viewport = { - (App->render->camera.x + 50) - App->map->data.tile_width, -App->render->camera.y, App->render->camera.w + 100 + App->map->data.tile_width * 2, App->render->camera.h - App->map->data.tile_height * 1 };

	//Draw all units

	std::list<Unit*>::const_iterator units_it = units.begin();

	while (units_it != units.end())
	{
		units_it._Ptr->_Myval->Draw(App->debug_mode);
		units_it++;
	}


	//Draw all buildings
	/*size = buildings.size();
	for (uint k = 0; k < size; k++)
	{
		buildings_vec[k]->Draw(App->debug_mode);
	}*/

	return ret;
}

bool j1EntitiesManager::CleanUp()
{
	//Clean Up Units List
	std::list<Unit*>::iterator units_item = units.begin();
	while (units_item != units.end())
	{
		RELEASE(units_item._Ptr->_Myval);
		units_item++;
	}
	units.clear();


	//Clean Up units_defs vector
	uint size = units_defs.size();
	for (uint k = 0; k < size; k++)
	{
		RELEASE(units_defs[k]);
	}
	units_defs.clear();


	return true;
}



//Methods to add entities definitions
bool j1EntitiesManager::AddUnitDefinition(const pugi::xml_node* unit_node)
{
	if (unit_node == nullptr)return false;

	//Generate a new unit definition from the node & check if is a special unit to allocate extra stats
	Unit* new_def = nullptr;
	UNIT_TYPE unit_type = App->animator->StrToUnitEnum(unit_node->attribute("unit_type").as_string());

	if (unit_type == ARCHER_CHMP)
	{

	}
	else if (unit_type == WIZARD_CHMP)
	{

	}
	else
	{
		new_def = new Unit();
	}

	//Unit ID ---------------
	/*Name*/			new_def->SetName(unit_node->attribute("name").as_string());
	/*Entity Type*/		new_def->SetEntityType(UNIT);
	/*Unit Type*/		new_def->SetUnitType(unit_type);
	//Unit Primitives -------
	/*Mark*/			Circle vision;
	/*Mark Radius*/		vision.SetRad(unit_node->attribute("vision_rad").as_uint());
	/*Mark Color*/		vision.SetColor({ 0,255,255,255 });
						new_def->SetVision(vision);
	/*Mark*/			Circle mark;
	/*Mark Radius*/		mark.SetRad(unit_node->attribute("mark_rad").as_uint());
	/*Mark Color*/		mark.SetColor({ 255,255,255,255 });
						new_def->SetMark(mark);
	/*Soft.C*/			Circle soft_collider;
	/*Soft.C Radius*/	soft_collider.SetRad(unit_node->attribute("soft_rad").as_uint());
	/*Soft.C Color*/	soft_collider.SetColor({ 255,0,255,255 });
						new_def->SetSoftCollider(soft_collider);
	/*Hard.C*/			Circle hard_collider;
	/*Hard.C Radius*/	hard_collider.SetRad(unit_node->attribute("hard_rad").as_uint());
	/*Hard.C Color*/	hard_collider.SetColor({ 255,0,0,255 });
						new_def->SetHardCollider(hard_collider);
	/*Selection Rect*/	SDL_Rect selection_rect;
	/*S.Rect X*/		selection_rect.x = unit_node->attribute("selection_x").as_int();
	/*S.Rect Y*/		selection_rect.y = unit_node->attribute("selection_y").as_int();
	/*S.Rect W*/		selection_rect.w = unit_node->attribute("selection_w").as_int();
	/*S.Rect H*/		selection_rect.h = unit_node->attribute("selection_h").as_int();
						new_def->SetSelectionRect(selection_rect);
	/*Icon Rect*/		SDL_Rect icon_rect;
	/*I.Rect X*/		icon_rect.x = unit_node->attribute("icon_x").as_int();
	/*I.Rect Y*/		icon_rect.y = unit_node->attribute("icon_y").as_int();
	/*I.Rect W*/		icon_rect.w = unit_node->attribute("icon_w").as_int();
	/*I.Rect H*/		icon_rect.h = unit_node->attribute("icon_h").as_int();
						new_def->SetIcon(icon_rect);

	//Unit Metrics ----------
	/*Max Life*/		new_def->SetMaxLife(unit_node->attribute("max_life").as_uint());
	/*Life*/			new_def->SetLife(new_def->GetMaxLife());
	/*View Area*/		new_def->SetViewArea(unit_node->attribute("view_area").as_uint());
	/*Speed*/			new_def->SetSpeed(unit_node->attribute("speed").as_float());
	/*Train Time*/		new_def->SetTrainTime(unit_node->attribute("train_time").as_uint());


	//Fill the extra allocated special unit stats
	bool chmp = false;
	
	if (unit_type == ARCHER_CHMP)
	{

	}
	else if (unit_type == WIZARD_CHMP)
	{

	}


	//Add the generated unit in the units definitions entities manager array
	units_defs.push_back(new_def);

	LOG("%s definition built!", new_def->GetName());
	
	return true;
}



//Check if the entity civilizations string contains the chosen one
bool j1EntitiesManager::CivilizationCheck(char * civs_str, const char * chosen_civ)
{
	if (civs_str == nullptr || chosen_civ == nullptr)return false;

	char* civ = strtok(civs_str, "/");
	while (civ != NULL)
	{

		if (strcmp(civ, chosen_civ) == 0)return true;
		civ = strtok(NULL, "/");

	}
	return false;
}

//Functionality =======================
bool j1EntitiesManager::LoadCivilization(const char * folder)
{
	j1Timer time;
	time.Start();

	//Load Civilization Animations --------------
	//Load civilization data from loaded folder
	LOG("---- Loading %s...", folder);
	std::string load_folder = name + "/" + folder;
	pugi::xml_document civilization_data;
	if (!App->fs->LoadXML(load_folder.c_str(), &civilization_data))
	{
		LOG("Civilization animations load error!");
		return false;
	}

	//Get Civilization name
	std::string civ_name = civilization_data.first_child().first_attribute().as_string();
	//Boolean to check the correct file loads
	bool ret = true;
	//Load civilization units list
	pugi::xml_node unit_node = civilization_data.first_child().child("units").first_child();
	while (unit_node != NULL)
	{
		if (!ret)break;
		ret = App->animator->LoadUnitBlock(unit_node.attribute("xml").as_string());
		unit_node = unit_node.next_sibling();
	}
	//Load civilization buildings list
	pugi::xml_node building_node = civilization_data.child("data").child("buildings").first_child();
	while (building_node != NULL)
	{
		if (!ret)break;
		ret = App->animator->LoadBuildingBlock(building_node.attribute("xml").as_string());
		building_node = building_node.next_sibling();
	}
	//Load civilization resources
	pugi::xml_node resource_node = civilization_data.first_child().child("resources").first_child();
	if(resource_node != NULL)ret = App->animator->LoadResourceBlock(resource_node.attribute("xml").as_string());
	else LOG("Error loading civilization Resources");
	// ------------------------------------------

	//Load Civilization Stats -------------------
	//Load stats document
	civilization_data.reset();
	load_folder.clear();
	load_folder = name + "/" + "StatsData.xml";
	if (!App->fs->LoadXML(load_folder.c_str(), &civilization_data))
	{
		LOG("Civilization stats load error!");
		return false;
	}

	//Focus first entity node
	pugi::xml_node entity_node = civilization_data.child("data").first_child();

	while (entity_node != NULL)
	{
		//Check if the entity is in the chosen civilization
		if (!CivilizationCheck((char*)entity_node.attribute("civ").as_string(), civ_name.c_str()))
		{
			entity_node = entity_node.next_sibling();
			continue;
		}

		//If the entity is in the civilization the entity definition is loaded
		switch (App->animator->StrToEntityEnum(entity_node.attribute("id").as_string()))
		{
		case UNIT:		AddUnitDefinition(&entity_node.first_child());		break;

		default:
			//Entity ID error case
			LOG("Error loading entity %s", entity_node.first_child().attribute("name").as_string());
			break;
		}

		//Focus the next entity node
		entity_node = entity_node.next_sibling();
	}
	// ------------------------------------------

	//Load xml sounds
	load_folder.clear();

	load_folder = name + "/" + "SoundsData.xml";

	// ------------------------------------------

	//Clean loaded xml
	civilization_data.reset();

	LOG("---- %s loaded in %.3f", folder, time.ReadSec());

	return ret;
}

Unit* j1EntitiesManager::GenerateUnit(UNIT_TYPE type, DIPLOMACY diplomacy, bool push_in_list)
{
	Unit* new_unit = nullptr;

	uint def_num = units_defs.size();
	for (uint k = 0; k < def_num; k++)
	{
		if (units_defs[k]->GetUnitType() == type)
		{
			{
				//Build unit
				new_unit = new Unit(*units_defs[k]);
			}

			//Set generated unit diplomacy
			new_unit->SetDiplomacy(diplomacy);

			//Set new unit myself pointer
			new_unit->myself = new_unit;

			//Set unit animation
			App->animator->UnitPlay(new_unit);
			
			//Add the new unit at the units manage list
			if (push_in_list)
			{
				units.push_back(new_unit);
			}

			return new_unit;
		}
	}
	return nullptr;
}



const std::list<Unit*>* j1EntitiesManager::UnitsList() const
{
	return &units;
}



bool j1EntitiesManager::DeleteEntity(Entity * entity)
{
	//Check if the entity is really defined
	if (entity == nullptr)
	{
		return false;
	}
	
	//Add the entity at the wasted entities list 
	wasted_units.push_back(entity);

	return true;
}

bool j1EntitiesManager::SetUnitPath(Unit* target, const iPoint& goal)
{
	fPoint target_pos = target->GetPosition().Round();
	iPoint int_target_pos(target_pos.x, target_pos.y);

	//Check if the position and the goal are the same
	if (goal == int_target_pos)return false;

	//Create path from unit position to the goal
	std::vector<iPoint>* path = App->pathfinding->SimpleAstar(int_target_pos, goal);
	
	if (path == nullptr)return false;

	//Set target path
	if (target->GetAction() != WALK)
	{
		target->SetAction(WALK);
		target->Focus(path->front());
	}

	//Play move fx

	
	return true;
}


void j1EntitiesManager::AddUnit(Unit* unit)
{
	units.push_back((Unit*)unit);
}

Unit * j1EntitiesManager::PopUnit(const Unit * unit)
{
	units.remove((Unit*)unit);
	return (Unit*)unit;
}


std::priority_queue<Unit*, std::vector<Unit*>, LessDistance > j1EntitiesManager::OrganizeByNearest(std::vector<Unit*>& vec, Circle& target)
{
	std::priority_queue<Unit*, std::vector<Unit*>, LessDistance  > organized;
	uint size = vec.size();
	iPoint pos = target.GetPosition();
	fPoint perf_pos(pos.x, pos.y);

	for (uint k = 0; k < size; k++)
	{
		fPoint dist = vec[k]->GetPosition();
		dist.x -= pos.x;
		dist.y -= pos.y;
		float perf_dist = sqrt(dist.x*dist.x + dist.y * dist.y);
		vec[k]->distance_to_target = perf_dist;
		organized.push(vec[k]);
	}

	vec.clear();

	while (!organized.empty())
	{
		Unit* unit = organized.top();
		organized.pop();
		vec.push_back(unit);
	}



	return organized;
}
