#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	p2Point<float> pos;
	pos.x = -1;
	pos.y = -1;

	wallBody[0] = App->physics->CreateRectangle(1, 15, pos);
	
	wallBody[1] = App->physics->CreateRectangle(15, 1, pos);
	pos.x = PIXEL_TO_METERS(SCREEN_WIDTH);
	wallBody[0] = App->physics->CreateRectangle(1, 15, pos);

	//player1 = ModulePlayer(App, true);
	////player2 = ModulePlayer(App, true);
	//
	//player1.isTurn = true;
	////player2.isTurn = false;
	//App->renderer->camera.x = App->renderer->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
	//
	//	if (player1.isTurn == true) {
	//
	//		player1.isTurn = false;
	//
	//	}
	//
	//	if (player1.isTurn == false) {
	//
	//		player1.isTurn = true;
	//
	//	}
	//
	//	if (player2.isTurn == true) {
	//
	//		player2.isTurn = false;
	//
	//	}
	//
	//	if (player2.isTurn == false) {
	//
	//		player2.isTurn = true;
	//
	//	}
	//}

	return UPDATE_CONTINUE;
}
