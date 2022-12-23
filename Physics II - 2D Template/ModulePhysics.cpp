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
	Bodies = new p2List<wBody*>;
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	if (Bodies != nullptr)
	{
		p2List_item<wBody*>* bodies;
		for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
		{
			
			p2Point<float> place = bodies->data->GetPosition();
			if (bodies->data->btype == bodyType::DYNAMIC) {
				



				// changing the velocity according to the acceleration
				wVec2 newVel;
				newVel.x = bodies->data->GetSpeed().x + bodies->data->GetAcceleration().x;
				newVel.y = bodies->data->GetSpeed().y + bodies->data->GetAcceleration().y;

				bodies->data->SetLinearVelocity(newVel);


				// changing the position according to the velocity, in the future we should make sure that it follows the dt
				p2Point<float> newPos;
				newPos.x = (bodies->data->GetSpeed().x / 10 + bodies->data->GetPosition().x) ;
				newPos.y = (bodies->data->GetSpeed().y / 10 + bodies->data->GetPosition().y);

				bodies->data->SetPosition(newPos);
			}
		}

		CheckCollision();
		
	}

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
			
			p2Point<float> place = bodies->data->GetPosition();
			if (bodies->data->wclass == wBodyClass::CIRCLE) 
			{
				App->renderer->DrawCircle(place.x, place.y, bodies->data->width, 255, 255, 255);
				//App->renderer->DrawCircle(place.x, place.y, 1, 255, 0, 0);
			}
		}
	}
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	Bodies->clear();
	delete Bodies;

	p2List_item<wBody*>* bodies;
	//for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
	{
		

		
		//Bodies->del(bodies);
			
		
		
	}

	return true;
}

wBody* ModulePhysics::CreateCircle(float r, p2Point<float> pos)
{
	wBody* wbody = new wBody();
	pos.x = METERS_TO_PIXELS(pos.x);
	pos.y = METERS_TO_PIXELS(pos.y);

	wbody->wclass = wBodyClass::CIRCLE;
	wbody->SetPosition(pos);
	wbody->SetLinearVelocity(wVec2(0, 0));
	wbody->SetLinearAcceleration(wVec2(0, 1.0));
	wbody->width = METERS_TO_PIXELS(r * 0.5);
	wbody->height = METERS_TO_PIXELS(r * 0.5);
	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
}

void ModulePhysics::destroyBody(wBody* body)
{
	p2List_item<wBody*>* bodies;
	for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
	{
		if (bodies->data == body)
		{
			
			
			Bodies->del(bodies);
			delete body;
			body->~wBody();
			return;
		}
	}
}

void ModulePhysics::addBodyToList(wBody* body)
{
	if (body != nullptr)
	{
		Bodies->add(body);
	}
}

void ::ModulePhysics::CheckCollision()
{
	p2List<wBody*> aux;

	p2List_item<wBody*>* bodies;
	p2List_item<wBody*>* bodies2;
	for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
	{
		if (bodies->data->IsCollisionListener)
		{
			aux.add(bodies->data);
		}
	}

	for (bodies = aux.getFirst(); bodies != NULL; bodies = bodies->next)
	{
		
		for (bodies2 = Bodies->getFirst(); bodies2 != NULL; bodies2 = bodies2->next)
		{
			if (bodies->data != bodies2->data)
			{
				
				if (bodies->data->wclass == wBodyClass::CIRCLE)
				{
					if (bodies2->data->wclass == wBodyClass::CIRCLE)
					{
						
						float radius = bodies->data->width + bodies2->data->width;
						float distance = bodies->data->GetPosition().DistanceTo(bodies2->data->GetPosition());

						if (distance < radius)
						{
							bodies->data->OnCollision(bodies2->data);
						}
						
					}
					else if (bodies2->data->wclass == wBodyClass::SQUARE)
					{

					}
				}
				else if (bodies->data->wclass == wBodyClass::SQUARE)
				{

				}
			}
		}
	}



	aux.clear();
}


// wBody Functions
wBody::wBody()
{
	
}
wBody::~wBody()
{

}

void wBody::SetLinearVelocity(wVec2 v)
{
	speed = v;
}
void wBody::SetLinearAcceleration(wVec2 a)
{
	acceleration = a;
}
void wBody::SetPosition(p2Point<float> position)
{
	bPos = position;
}
wVec2 wBody::GetSpeed()
{
	return speed;
}
wVec2 wBody::GetAcceleration()
{
	return acceleration;
}
p2Point<float> wBody::GetPosition()
{
	return bPos;
}
void wBody::OnCollision(wBody* Body2)
{
	LOG("This is a collision");
}