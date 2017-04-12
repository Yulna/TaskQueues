#include "j1Scene.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Map.h"

#include "j1EntitiesManager.h"
#include "j1Animator.h"
#include "j1Pathfinding.h"
#include "j1FileSystem.h"
#include "j1Player.h"


j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{

}

void j1Scene::Enable()
{
	active = true;
	
}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	for (pugi::xml_node map_tmx = config.child("map_folder"); map_tmx; map_tmx = map_tmx.next_sibling("map_folder")) {

		map_folder.push_back(std::string(map_tmx.child_value()));

	}



	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Map build -------------------------------------------
	Load_Current_Map();

	//Load Logic Map --------------------------------------
	uint width = 0;
	uint height = 0;
	uchar* logic_map = nullptr;
	j1Timer ptimer;
	App->map->CreateWalkabilityMap(width, height);
	App->pathfinding->SetMap(width, height);
	// -----------------------------------------
	
	//Load the camera position
	iPoint camera_pos = App->map->MapToWorld(104, 90);
	App->render->camera.x = camera_pos.x;
	App->render->camera.y = -camera_pos.y;
	App->map->CalculateTilesInView();


	//load troops
	blue_unit = App->entities_manager->GenerateUnit(MILITIA, ALLY, true);
	iPoint blue_pos = App->map->MapToWorld(103, 103);
	blue_unit->SetPosition(blue_pos.x, blue_pos.y);

	red_unit = App->entities_manager->GenerateUnit(MILITIA, ENEMY, true);
	iPoint red_pos = App->map->MapToWorld(109, 109);
	red_unit->SetPosition(red_pos.x, red_pos.y);



	App->gui->ChangeMouseTexture(DEFAULT);
	App->player->Enable();
	App->entities_manager->Enable();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	//MAP MOVEMENT-----------------------------------------
	bool moved = false;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += SDL_ceil(1500 * dt);
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= SDL_ceil(1500 * dt);
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += SDL_ceil(1500 * dt);
		moved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= SDL_ceil(1500 * dt);
		moved = true;
	}

	if(moved)App->map->CalculateTilesInView();
	
	// ------------------------------------------

	App->map->Draw(App->map_debug_mode);
	App->entities_manager->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->scene->Disable();
		App->player->Disable();
		App->entities_manager->Disable();
		App->SetQuit();

	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->win->fullscreen)
		{
			SDL_SetWindowFullscreen(App->win->window, NULL);
			App->win->fullscreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN);
			App->win->fullscreen = true;
		}
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::Change_Map()
{
	if (current_map < map_folder.size() - 1)current_map++;
	else current_map = 0;
}

bool j1Scene::Load_Current_Map()
{
	bool ret = false;

	ret = App->map->Load(map_folder.at(current_map).c_str());

	return ret;
}

