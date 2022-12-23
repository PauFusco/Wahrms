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
	
	CreateFloor();
	
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
				App->renderer->DrawCircle(place.x, place.y, bodies->data->GetWidth(), 255, 255, 255);
				//App->renderer->DrawCircle(place.x, place.y, 1, 255, 0, 0);
			}
			if (bodies->data->wclass == wBodyClass::SQUARE)
			{
				SDL_Rect thisRect;
				p2Point<float> recPos = bodies->data->GetPosition();
				thisRect.x = METERS_TO_PIXELS(recPos.x);
				thisRect.y = METERS_TO_PIXELS(recPos.y);
				thisRect.w = bodies->data->GetWidth();
				thisRect.h = bodies->data->GetHeight();

				App->renderer->DrawQuad(thisRect, 255, 255, 255, 255, true, true);

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

void ModulePhysics::CheckCollision()
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
						
						float radius = bodies->data->GetWidth() + bodies2->data->GetWidth();
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

void ModulePhysics::CreateFloor()
{
	wBody* floorBody = new wBody();

	p2Point<float> floorPos;
	floorPos.x = PIXEL_TO_METERS(0);
	floorPos.y = PIXEL_TO_METERS(600);

	floorBody->SetPosition(floorPos);

	floorBody->SetWidth(SCREEN_WIDTH);
	floorBody->SetHeight(SCREEN_HEIGHT - floorPos.y);

	floorBody->btype = bodyType::STATIC;
	floorBody->wclass = wBodyClass::SQUARE;
	floorBody->ctype = ColliderType::FLOOR;

	addBodyToList(floorBody);

	Floor* floor = new Floor(wVec2(GRAVITY_X, GRAVITY_Y), 0, floorBody);
}

// wBody Functions
wBody* ModulePhysics::CreateCircle(float r, p2Point<float> pos)
{
	wBody* wbody = new wBody();
	pos.x = METERS_TO_PIXELS(pos.x);
	pos.y = METERS_TO_PIXELS(pos.y);

	wbody->wclass = wBodyClass::CIRCLE;
	wbody->SetPosition(pos);
	wbody->SetLinearVelocity(wVec2(0, 0));
	wbody->SetLinearAcceleration(wVec2(0, 1.0));
	wbody->SetWidth(METERS_TO_PIXELS(r * 0.5));
	wbody->SetHeight(METERS_TO_PIXELS(r * 0.5));
	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
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
void wBody::SetHeight(int iheight)
{
	height = iheight;
}
void wBody::SetWidth(int iwidth)
{
	width = iwidth;
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
int wBody::GetHeight()
{
	return height;
}
int wBody::GetWidth()
{
	return width;
}
void wBody::OnCollision(wBody* Body2)
{
	LOG("This is a collision");
}