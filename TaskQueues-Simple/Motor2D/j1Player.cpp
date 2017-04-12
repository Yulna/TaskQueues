#include "j1Player.h"

#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Animator.h"

#include "j1App.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1EntitiesManager.h"

#include "UI_Element.h"
#include "UI_Image.h"
#include "UI_String.h"

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
	return true;
}

bool j1Player::PreUpdate()
{

	int x, y;
	App->input->GetMousePosition(x, y);


	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{

	}
	
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{

	}



	if (App->debug_mode)
	{
		//Generate a town center in the mouse coordinates
		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			Building* center = App->entities_manager->GenerateBuilding(BUILDING_TYPE::BARRACK, ALLY);
			center->SetPosition(x - App->render->camera.x, y - App->render->camera.y);
			center->SetDiplomacy(ALLY);

		}
		//Generate a Militia unit in the mouse coordinates
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			Unit* new_unit = App->entities_manager->GenerateUnit(MILITIA, ENEMY);
			new_unit->SetPosition(x - App->render->camera.x, y - App->render->camera.y);

			//game_panel->IncressPopulation(1, false);
		}
		//Generate a Arbalest unit in the mouse coordinates
		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN /*&& game_panel->CheckPopulation()*/)
		{
			Unit* new_unit = App->entities_manager->GenerateUnit(ARBALEST, ALLY);
			new_unit->SetPosition(x - App->render->camera.x, y - App->render->camera.y);
			new_unit->AddPasiveAction(App->action_manager->ScanAction(new_unit));
		}

	}
	
	return true;
}

bool j1Player::PostUpdate()
{

	//Draw Mouse Last one
	if (SDL_ShowCursor(-1) == 0) App->gui->DrawMouseTexture();

	return true;
}

bool j1Player::CleanUp()
{
	return true;
}

void j1Player::GUI_Input(UI_Element* target, GUI_INPUT input)
{


	switch (input)
	{
	case UP_ARROW:
		break;
	case DOWN_ARROW:
		break;
	case LEFT_ARROW:
		break;
	case RIGHT_ARROW:
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
		break;
	case MOUSE_LEFT_BUTTON_REPEAT:
		break;
	case MOUSE_LEFT_BUTTON_UP:
		break;
	case MOUSE_RIGHT_BUTTON:
		break;
	case MOUSE_IN: 
		break;
	case MOUSE_OUT:  
		break;
	case SUPR:
		break;
	case BACKSPACE:
		break;
	case ENTER:
		break;
	case TAB:
		break;
	default:
		break;
	}
}
