#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
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
			LOG("this exixts");
			p2Point<float> place = bodies->data->GetPosition();
			//bodies->data->width;

			App->renderer->DrawCircle(place.x, place.y, bodies->data->width, 255, 255, 255);
			App->renderer->DrawCircle(place.x, place.y, 1, 255, 0, 0);

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

}


wBody* ModulePhysics::CreateCircle(float r, p2Point<float> pos)
{
	wBody* wbody = new wBody();
	//wBody* wbody = nullptr;

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