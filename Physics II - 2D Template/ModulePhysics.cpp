#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include "math.h"
#include "p2Point.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	Bodies = new p2List<wBody*>;
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;
	
	if (Bodies != nullptr)
	{
		p2List_item<wBody*>* bodies;
		for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
		{
			LOG("this body exists");
			p2Point<float> place = bodies->data->GetPosition();
			if (bodies->data->wclass == wBodyClass::CIRCLE) {
				App->renderer->DrawCircle(METERS_TO_PIXELS(place.x), METERS_TO_PIXELS(place.y), METERS_TO_PIXELS(bodies->data->width), 255, 255, 255);
			}
		}
	}
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}

wBody* ModulePhysics::CreateCircle(float r, p2Point<float> pos)
{
	wBody* wbody = new wBody();

	wbody->wclass = wBodyClass::CIRCLE;
	wbody->SetPosition(pos);
	wbody->SetLinearVelocity(wVec2(0, 0));
	wbody->width = r * 0.5;
	wbody->height = r * 0.5;
	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
}

void ModulePhysics::addBodyToList(wBody* body)
{
	if (body != nullptr)
	{
		Bodies->add(body);
	}
}


// wBody Functions
void wBody::SetLinearVelocity(wVec2 v)
{
	speed = v;
}
void wBody::SetPosition(p2Point<float> position)
{
	bPos = position;
}
wVec2 wBody::GetSpeed()
{
	return speed;
}
p2Point<float> wBody::GetPosition()
{
	return bPos;
}
void wBody::OnCollision(wBody* Body2)
{

		LOG("%f", GetSpeed().x);
		LOG("This is a collision");

		wVec2 velocity1;
		velocity1.x = 0;

		//if (GetSpeed().x != 0)
		{
			velocity1.x = (GetSpeed().x * (mass - Body2->mass) + 2 * Body2->mass * Body2->GetSpeed().x) / (mass + Body2->mass);
			LOG("COLLIDING NOW");
			//velocity1.x = GetSpeed().x - ((2 * Body2->mass) / (mass + Body2->mass)) * (((GetSpeed().x - Body2->GetSpeed().x) / (GetPosition().x - Body2->GetPosition().x)) / ((GetPosition().x - Body2->GetPosition().x) * (GetPosition().x - Body2->GetPosition().x))) * GetPosition().x - Body2->GetPosition().x * 0.1;
		}
		
		//if (GetSpeed().y != 0)
		{
			//velocity1.y = GetSpeed().y - ((2 * Body2->mass) / (mass + Body2->mass)) * (((GetSpeed().y - Body2->GetSpeed().y) / (GetPosition().y - Body2->GetPosition().y)) / ((GetPosition().y - Body2->GetPosition().y) * (GetPosition().y - Body2->GetPosition().y))) * GetPosition().y - Body2->GetPosition().y * 0.1;
			velocity1.y = (GetSpeed().y * (mass - Body2->mass) + 2 * Body2->mass * Body2->GetSpeed().y) / (mass + Body2->mass);
		}
		

		SetLinearVelocity(velocity1);
		LOG("X %f", GetPosition().x);
		LOG("Y %f", GetPosition().y);

	}
	else if (wclass == wBodyClass::CIRCLE && Body2->wclass == wBodyClass::SQUARE)
	{
		//LOG("COLLIDING NOW");

		wVec2 velocity1;
		velocity1.x = GetSpeed().x;

		LOG("COLLIDING NOW %f", (Body2->GetSpeed().x ));
		//velocity1.x = (GetSpeed().x * (mass - Body2->mass) + 2 * Body2->mass * Body2->GetSpeed().x) / (mass + Body2->mass) * GetRestitution() * Body2->GetRestitution();
		velocity1.y = (GetSpeed().y * (mass - Body2->mass) + 2 * Body2->mass * Body2->GetSpeed().y) / (mass + Body2->mass) * GetRestitution() * Body2->GetRestitution();

		SetLinearVelocity(velocity1);

		if (Body2->ctype == ColliderType::PLAYER) {

			App->player->HP--;		
			App->player->isDead = true;


		}
	}

	if (GetPosition().y < Body2->GetPosition().y)
	{
		p2Point<float> pos;
		pos.x = GetPosition().x;
		pos.y = Body2->GetPosition().y - PIXEL_TO_METERS(GetWidth());
		SetPosition(pos);
	}
}