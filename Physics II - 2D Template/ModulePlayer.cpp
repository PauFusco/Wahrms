#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	p2Point<float> position;
	position.x = 5;
	position.y = 1;

	plBody = App->physics->CreateCircle(2, position);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = 0;
		vel.x = 1;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = 0;
		vel.x = -1;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = -1;
		vel.x = 0;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = 1;
		vel.x = 0;
		plBody->SetLinearVelocity(vel);
	}


	return UPDATE_CONTINUE;
}



