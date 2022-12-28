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
	plBody->IsCollisionListener = true;
	plBody->SetRestitution(0.9);

	position.x = 8;
	//plBody2 = App->physics->CreateCircle(3, position);
	
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
		vel.x = 0.5;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = 0;
		vel.x = -0.5;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = -0.5;
		vel.x = 0;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		wVec2 vel;
		vel.y = 0.5;
		vel.x = 0;
		plBody->SetLinearVelocity(vel);
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		LOG("The x velocity is: %f", plBody->GetSpeed().x);
		LOG("The y velocity is: %f", plBody->GetSpeed().y);
		LOG("X %f", plBody->GetPosition().x);
		LOG("Y %f", plBody->GetPosition().y);
	}


	if (plBody2 == nullptr)
	{
		p2Point<float> position;
		position.x = 5;
		position.y = 1;


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			plBody2 = App->physics->CreateCircle(1, position);
			wVec2 vel;
			vel.y = 0;
			vel.x = 50;
			plBody2->SetLinearVelocity(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			plBody2 = App->physics->CreateCircle(3, position);
			wVec2 vel;
			vel.y = 0;
			vel.x = -50;
			plBody2->SetLinearVelocity(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			plBody2 = App->physics->CreateCircle(1, position);
			wVec2 vel;
			vel.y = -0;
			vel.x = 0;
			plBody2->SetLinearVelocity(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			plBody2 = App->physics->CreateCircle(3, position);
			wVec2 vel;
			vel.y = 50;
			vel.x = 0;
			plBody2->SetLinearVelocity(vel);
		}

		
		
	}

	if (plBody2 != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			App->physics->destroyBody(plBody2);
			
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{

		p2Point<float> position;
		position.x = 5;
		position.y = 1;

		circles.add(App->physics->CreateCircle(3, position));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
	{
		if (circles.getLast() != NULL)
		{
			App->physics->destroyBody(circles.getLast()->data);
			circles.del(circles.getLast());
		}
		else
		{
			circles.clear();
		}
		
	}

	return UPDATE_CONTINUE;
}