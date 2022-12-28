#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "math.h"
#include "p2Point.h"
#include <string>

using namespace std;

float dt = 0.1666667;

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
	
	const char* fontPath = "Assets/Fonts/rtype_font3.png";
	const char* chars = "! c,_./0123456789e;><?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	App->fonts->Load(fontPath, chars, 2);

	CreateFloor();

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	if (Bodies != nullptr)
	{

		// Gravity control
		if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			floor->gravity.y -= 0.001;
		}
		if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		{
			floor->gravity.y += 0.001;
		}
		
		// Integration Method control
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		{
			IntMeth = IntegrationMethod::IMPLICIT_EULER;
		}

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			IntMeth = IntegrationMethod::SYMPLECTIC_EULER;
		}
		if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			IntMeth = IntegrationMethod::VELOCITY_VERLET;
		}
		

		integrator();

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


				App->renderer->DrawCircle(METERS_TO_PIXELS(place.x), METERS_TO_PIXELS(place.y), bodies->data->GetWidth(), 255, 255, 255);
				App->renderer->DrawCircle(METERS_TO_PIXELS(place.x), METERS_TO_PIXELS(place.y), 1, 255, 0, 0);

        
			}

			if (bodies->data->wclass == wBodyClass::SQUARE)
			{
				SDL_Rect thisRect;
				p2Point<float> recPos = bodies->data->GetPosition();
				thisRect.x = METERS_TO_PIXELS(recPos.x);
				thisRect.y = METERS_TO_PIXELS(recPos.y);
				thisRect.w = bodies->data->GetWidth();
				thisRect.h = bodies->data->GetHeight();

				App->renderer->DrawQuad(thisRect, 255, 255, 255, 255, false, true);

				App->renderer->DrawCircle(thisRect.x, thisRect.y, 1, 255, 0, 0);

			}
		}
	}
	
	printDebugInfo();
	
	return UPDATE_CONTINUE;
}

void ModulePhysics::printDebugInfo()
{
	// Integration Method debug
	App->fonts->BlitText(0, 0, 0, "INTEGRATION METHOD;");
	switch (IntMeth)
	{
	case(IntegrationMethod::IMPLICIT_EULER):
		App->fonts->BlitText(160, 0, 0, methCharie);
		break;
	case(IntegrationMethod::SYMPLECTIC_EULER):
		App->fonts->BlitText(160, 0, 0, methCharse);
		break;
	case(IntegrationMethod::VELOCITY_VERLET):
		App->fonts->BlitText(160, 0, 0, methCharvv);
		break;
	}

	// Gravity acceleration debug

	string temp = to_string(floor->gravity.y);

	gravChar = temp.c_str();
	App->fonts->BlitText(0, 15, 0, "ACTUAL GRAVITY;");
	App->fonts->BlitText(130, 15, 0, gravChar);
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
						
						float radius = PIXEL_TO_METERS(bodies->data->GetWidth()) + PIXEL_TO_METERS(bodies2->data->GetWidth());
						float distance = bodies->data->GetPosition().DistanceTo(bodies2->data->GetPosition());

						if (distance < radius)
						{
							bodies->data->OnCollision(bodies2->data);
						}
						
					}
					else if (bodies2->data->wclass == wBodyClass::SQUARE)
					{
						if (bodies->data->GetPosition().y < bodies2->data->GetPosition().y)
						{
							p2Point<float> FloorPos;

							FloorPos.x = bodies->data->GetPosition().x;
							FloorPos.y = bodies2->data->GetPosition().y;

							float radius = PIXEL_TO_METERS(bodies->data->GetWidth());
							float distance = bodies->data->GetPosition().DistanceTo(FloorPos);

							if (distance < radius)
							{
								bodies->data->OnCollision(bodies2->data);
								
								

							}
						}
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
	floorBody->SetLinearVelocity(wVec2(0, 0));

	floorBody->SetWidth(SCREEN_WIDTH);
	floorBody->SetHeight(SCREEN_HEIGHT - floorPos.y);

	floorBody->SetMass(floorBody->GetHeight() * floorBody->GetWidth());

	floorBody->btype = bodyType::STATIC;
	floorBody->wclass = wBodyClass::SQUARE;
	floorBody->ctype = ColliderType::FLOOR;

	addBodyToList(floorBody);

	floor = new Floor(wVec2(GRAVITY_X, GRAVITY_Y), 0, floorBody);
}

void ModulePhysics::integrator()
{
	p2List_item<wBody*>* bodies;
	for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
	{
		if (bodies->data->btype != bodyType::STATIC)
		{
			//CALCULATE FORCES
			float bodyMass = bodies->data->GetMass();
      
			wVec2 g = wVec2(PIXEL_TO_METERS(floor->gravity.x), PIXEL_TO_METERS(floor->gravity.y));

			bodies->data->gF = wVec2(bodyMass * g.x, bodyMass * g.y);

			// If collision with bouncer, apply bounce force
			bodies->data->bF = wVec2(0, 0);
			// If collision with floor, apply fregament
			bodies->data->fF = wVec2(0, 0);
			// If in the air, apply drag force
			bodies->data->dF = wVec2(0, 0);

			float tFx = bodies->data->gF.x + bodies->data->bF.x + bodies->data->fF.x + bodies->data->dF.x;
			float tFy = bodies->data->gF.y + bodies->data->bF.y + bodies->data->fF.y + bodies->data->dF.y;
			wVec2 aF = wVec2(tFx / bodyMass, tFy / bodyMass);
			

			// TAKE ACCELERATION FROM FORCES CALCULATION AND USE IT TO FIND SPEED
			// AND POSITION (ORDER DEPENDS ON WHAT INTEGRATION METHOD WE ARE USING)
			p2Point<float> actualPosition = bodies->data->GetPosition();
			wVec2 actualVelocity = bodies->data->GetSpeed();

			float px, py, vx, vy;
			//px = PIXEL_TO_METERS(actualPosition.x);
			//py = PIXEL_TO_METERS(actualPosition.y);
			//vx = PIXEL_TO_METERS(actualVelocity.x);
			//vy = PIXEL_TO_METERS(actualVelocity.y);
			px = actualPosition.x;
			py = actualPosition.y;
			vx = actualVelocity.x;
			vy = actualVelocity.y;
			float tx, ty;
			tx = bodies->data->tx;
			ty = bodies->data->ty;


			switch (IntMeth) {
			case(IntegrationMethod::IMPLICIT_EULER):
				px += vx * tx;
				py += vy * ty;

				vx += aF.x * tx;
				vy += aF.y * ty;
				break;

			case(IntegrationMethod::SYMPLECTIC_EULER):
				vx += aF.x * tx;
				vy += aF.y * ty;

				px += vx * tx;
				py += vy * ty;
				break;

			case(IntegrationMethod::VELOCITY_VERLET):
				px += vx * tx + 0.5 * aF.x * tx * tx;
				py += vy * ty + 0.5 * aF.y * ty * ty;

				vx += aF.x * tx;
				vy += aF.y * ty;
				break;

			default:
				IntMeth = IntegrationMethod::IMPLICIT_EULER;
			}

			if (vx == 0)
			{
				bodies->data->tx = 0;
			}
			if (vy == 0)
			{
				bodies->data->ty = 0;
			}

			//actualPosition.x = METERS_TO_PIXELS(px);
			//actualPosition.y = METERS_TO_PIXELS(py);
			//
			//actualVelocity.x = METERS_TO_PIXELS(vx);
			//actualVelocity.y = METERS_TO_PIXELS(vy);

			actualPosition.x = px;
			actualPosition.y = py;

			actualVelocity.x = vx;
			actualVelocity.y = vy;

			LOG("vy: %f", vy);

			bodies->data->SetPosition(actualPosition);
			bodies->data->SetLinearVelocity(actualVelocity);

			// If speed on a variable is 0, reset timer for when it starts moving again
			bodies->data->tx = dt;
			bodies->data->ty = dt;
		}
	}
}

// wBody Functions
wBody* ModulePhysics::CreateCircle(float r, p2Point<float> pos)
{
	wBody* wbody = new wBody();
	//pos.x = METERS_TO_PIXELS(pos.x);
	//pos.y = METERS_TO_PIXELS(pos.y);

	wbody->wclass = wBodyClass::CIRCLE;
	wbody->SetPosition(pos);
	wbody->SetLinearVelocity(wVec2(0, 0));

	wbody->SetWidth(METERS_TO_PIXELS(r * 0.5));
	wbody->SetHeight(METERS_TO_PIXELS(r * 0.5));

	wbody->SetMass(3.14 * r);

	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
}

wBody* ModulePhysics::CreateRectangle(float width, float height, p2Point<float> position)
{
	wBody* wbody = new wBody();

	wbody->wclass = wBodyClass::SQUARE;
	wbody->SetPosition(position);
	wbody->SetLinearVelocity(wVec2(0, 0));
	
	wbody->SetWidth(METERS_TO_PIXELS(width));
	wbody->SetHeight(METERS_TO_PIXELS(height));

	wbody->SetMass(1);

	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
}

void wBody::SetMass(float _mass)
{
	mass = _mass;

}


void wBody::SetLinearVelocity(wVec2 v)
{
	speed = v;
}
void wBody::SetPosition(p2Point<float> position)
{
	bPos = position;
}
unsigned int wBody::GetMass()
{
	return mass;
}
void wBody::SetHeight(int iheight)
{
	height = iheight;
}
void wBody::SetWidth(int iwidth)
{
	width = iwidth;
}
void wBody::SetRestitution(float _restitution)
{
	restitution = _restitution;
}

wVec2 wBody::GetSpeed()
{
	return speed;
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
float wBody::GetRestitution()
{
	return restitution;
}
void wBody::OnCollision(wBody* Body2)
{
	if (wclass == wBodyClass::CIRCLE && Body2->wclass == wBodyClass::CIRCLE)
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
	}

	if (GetPosition().y < Body2->GetPosition().y)
	{
		p2Point<float> pos;
		pos.x = GetPosition().x;
		pos.y = Body2->GetPosition().y - PIXEL_TO_METERS(GetWidth());
		SetPosition(pos);
	}
}