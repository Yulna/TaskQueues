#include "j1Player.h"

#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Animator.h"
#include "j1Map.h"

#include "j1App.h"
#include "j1Window.h"
#include "j1EntitiesManager.h"

#include "Actions_Unit.h"

//j1Player Constructor ============================================================
j1Player::j1Player()
{
	name = "player";
}

//j1Player Destructor ============================================================
j1Player::~j1Player()
{
}

void j1Player::Enable()
{
	active = true;

}

void j1Player::Disable()
{
	active = false;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;


	return ret;
}

bool j1Player::Start()
{
	left_click = new MoveBlue;
	right_click = new MoveRed;

	return true;
}

bool j1Player::PreUpdate()
{

	int x, y;
	App->input->GetMousePosition(x, y);


	//TODO 1: Execute the left and right click tasks------------------
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(left_click != nullptr) left_click->Execute();
	}
	
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		if (right_click != nullptr)	right_click->Execute();
	}
	//-----------------------------------------------------------------


	//TODO 2:Swap the right and left click functionality
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		InputTask* temp = left_click;
		left_click = right_click;
		right_click = temp;
	}
	//------------------------------------------------------------------
	
	return true;
}


void MoveBlue::Execute()
{
	LOG("Moving blue...");
	
	//TODO 9: Uncomment this when you have finished the AI part

	int x, y;
	App->input->GetMousePosition(x, y);
	
	//Reset the worker so our input overwrithes the previous ones
	//Don't reset if left shift is pressed so we can define a predefined path of movements.
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
		App->player->blue_unit->GetWorker()->Reset();

	App->player->blue_unit->AddAction(App->action_manager->MoveAction(App->player->blue_unit, iPoint(x - App->render->camera.x, y - App->render->camera.y)));
	//------------------------------------------------------------
}

void MoveRed::Execute()
{
	LOG("Moving red...");


	//TODO 9: Uncomment this when you have finished the AI part
	int x, y;
	App->input->GetMousePosition(x, y);

	//Reset the worker so our input overwrithes the previous ones
	//Don't reset if left shift is pressed so we can define a predefined path of movements.
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
		App->player->red_unit->GetWorker()->Reset();

	App->player->red_unit->AddAction(App->action_manager->MoveAction(App->player->red_unit, iPoint(x - App->render->camera.x, y - App->render->camera.y)));
	//----------------------------------------------------------
}



bool j1Player::PostUpdate()
{

	return true;
}

bool j1Player::CleanUp()
{
	return true;
}
