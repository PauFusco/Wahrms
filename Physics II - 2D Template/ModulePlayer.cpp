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

	plBody->SetRestitution(0.6);

	plBody->ctype = ColliderType::PLAYER;

	isTurn = true;


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

	if (isTurn)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			wVec2 vel;
			vel.y = 0;
			vel.x = 5;
			plBody->SetLinearVelocity(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			wVec2 vel;
			vel.y = 0;
			vel.x = -5;
			plBody->SetLinearVelocity(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			wVec2 vel;
			vel.y = -100;
			vel.x = 0;
			//plBody->SetLinearVelocity(vel);
			plBody->ApplyForce(vel);
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			wVec2 vel;
			vel.y = 5;

			vel.x = 0;
			plBody->SetLinearVelocity(vel);
		}
		//if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		//{
		//	LOG("The x velocity is: %f", plBody->GetSpeed().x);
		//	LOG("The y velocity is: %f", plBody->GetSpeed().y);
		//	LOG("X %f", plBody->GetPosition().x);
		//	LOG("Y %f", plBody->GetPosition().y);
		//}


		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (strength < 65)	strength++;
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (strength > -65)	strength--;
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			angle++;

			if (angle > 359) {

				angle = 0;

			}
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			angle--;

			if (angle < 0) {

				angle = 359;

			}
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			wVec2 vel;
			vel.x = strength * cos(DEGTORAD * angle);
			vel.y = strength * sin(DEGTORAD * angle);

			p2Point<float> pos;
			pos.x = plBody->GetPosition().x;
			pos.y = plBody->GetPosition().y;
			pos.y -= 4;

			circles.add(App->physics->CreateCircle(1, pos));

			circles.getLast()->data->ctype = ColliderType::BULLET;

			circles.getLast()->data->IsCollisionListener = true;
			circles.getLast()->data->SetRestitution(0.9);

			circles.getLast()->data->SetLinearVelocity(vel);

			isTurn = false;
		}


		// Bullet trajectory line
		p2Point<float> line_Start;
		line_Start.x = METERS_TO_PIXELS(plBody->GetPosition().x);
		line_Start.y = METERS_TO_PIXELS(plBody->GetPosition().y);
		line_Start.y -= 200;
		p2Point<float> line_End;
		line_End.x = line_Start.x + strength * cos(DEGTORAD*angle);
		line_End.y = line_Start.y + strength * sin(DEGTORAD*angle);

		App->renderer->DrawLine(line_Start.x, line_Start.y, line_End.x, line_End.y, 255, 255, 255, 255);

	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			isTurn = true;
		}
	}

	

	if (plBody2 == nullptr)
	{
		p2Point<float> position;
		position.x = 5;
		position.y = 1;
		
	}


	//if (plBody2 != nullptr)
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	//	{
	//		App->physics->destroyBody(plBody2);
	//		
	//	}
	//}
	//
	//if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//
	//	p2Point<float> position;
	//	position.x = 5;
	//	position.y = 1;
	//
	//	circles.add(App->physics->CreateCircle(3, position));
	//}

	if (DestroyBullet)
	{
		if (circles.getFirst() != NULL)
		{
			App->physics->destroyBody(circles.getLast()->data);
			circles.del(circles.getLast());
			DestroyBullet = false;
		}
		else
		{
			circles.clear();
			DestroyBullet = false;
		}

	}

	return UPDATE_CONTINUE;
}