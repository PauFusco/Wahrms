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
	
	wallBody[1] = App->physics->CreateRectangle(25, 1, pos);
	pos.x = PIXEL_TO_METERS(SCREEN_WIDTH);
	wallBody[2] = App->physics->CreateRectangle(1, 20, pos);
	pos.x = 9.5;
	pos.y = 6;
	wallBody[3] = App->physics->CreateRectangle(3, 0.5, pos);
	pos.x = 3.7;
	pos.y = 8;
	wallBody[4] = App->physics->CreateRectangle(2.5, 0.5, pos);
	pos.x = 16;
	pos.y = 4.8;
	wallBody[5] = App->physics->CreateRectangle(1, 0.5, pos);
	


	graphics = App->textures->Load("Assets/Wahssets/Background.png");

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
	App->renderer->Blit(graphics, 0, 0);


	return UPDATE_CONTINUE;
}
