#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



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
