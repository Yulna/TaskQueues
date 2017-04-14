#include "j1Player.h"

#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Animator.h"
#include "j1Map.h"

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
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		InputTask* temp = left_click;
		left_click = right_click;
		right_click = temp;
	}
	//------------------------------------------------------------------
	
	return true;
}

//TODO 8: Fill the execute functions so it move the units---------------
void MoveBlue::Execute()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	LOG("Moving blue...");
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
		App->player->blue_unit->GetWorker()->Reset();

	App->player->blue_unit->MoveNow(iPoint(x - App->render->camera.x, y - App->render->camera.y));
	//App->player->blue_unit->AddAction(App->action_manager->MoveAction(App->player->blue_unit, iPoint(x - App->render->camera.x, y - App->render->camera.y)));
}

void MoveRed::Execute()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	LOG("Moving red...");
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
		App->player->red_unit->GetWorker()->Reset();

	App->player->red_unit->AddAction(App->action_manager->MoveAction(App->player->red_unit, iPoint(x - App->render->camera.x, y - App->render->camera.y)));
}
//-----------------------------------------------------------------------


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

