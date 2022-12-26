#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "p2Point.h"

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
	
	CreateFloor();

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	if (Bodies != nullptr)
	{
		/*p2List_item<wBody*>* bodies;
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
		}*/
		integrator();

		//CheckCollision();
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

				App->renderer->DrawCircle(place.x, place.y, bodies->data->GetWidth() / 2, 255, 255, 255);
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
	
	wbody->SetWidth(METERS_TO_PIXELS(r * 0.5));
	wbody->SetHeight(METERS_TO_PIXELS(r * 0.5));

	wbody->ctype = ColliderType::UNKNOWN;
	wbody->btype = bodyType::DYNAMIC;

	addBodyToList(wbody);

	return wbody;
}

void ModulePhysics::integrator()
{
	p2List_item<wBody*>* bodies;
	for (bodies = Bodies->getFirst(); bodies != NULL; bodies = bodies->next)
	{
		if (bodies->data->btype != bodyType::STATIC)
		{
			//CALCULATE FORCES
			wVec2 gF, bF, fF, dF, tF; // Gravity, bounce, fregament, drag, total
			
			float bodyMass = bodies->data->GetMass();
			
			wVec2 g = wVec2(GRAVITY_X, GRAVITY_Y);
			gF = wVec2(bodyMass * g.x, bodyMass * g.y);

			// If collision with bouncer, apply bounce force
			bF = wVec2(0, 0);
			// If collision with floor, apply fregament
			fF = wVec2(0, 0);
			// If in the air, apply drag force
			dF = wVec2(0, 0);

			float tFx = gF.x + bF.x + fF.x + dF.x;
			float tFy = gF.y + bF.y + fF.y + dF.y;
			wVec2 aF = wVec2(tFx/bodyMass, tFy/bodyMass);

			// TAKE ACCELERATION FROM FORCES CALCULATION AND USE IT TO FIND SPEED
			// AND POSITION (ORDER DEPENDS ON WHAT INTEGRATION METHOD WE ARE USING)
			p2Point<float> actualPosition = bodies->data->GetPosition();
			wVec2 actualVelocity = bodies->data->GetSpeed();
			
			float px, py, vx, vy;
			px = PIXEL_TO_METERS(actualPosition.x);
			py = PIXEL_TO_METERS(actualPosition.y);
			vx = PIXEL_TO_METERS(actualVelocity.x);
			vy = PIXEL_TO_METERS(actualVelocity.y);
			float tx, ty;
			tx = bodies->data->tx;
			ty = bodies->data->ty;
			
			switch (IntMeth) {
			case(IntegrationMethod::IMPLICIT_EULER):
				// Implicit Euler --> x += v At -> v += a At
				px += vx * tx;
				py += vy * ty;

				vx = aF.x * tx;
				vy = aF.y * ty;
				break;

			case(IntegrationMethod::SYMPLECTIC_EULER):
				vx = aF.x * tx;
				vy = aF.y * ty;
				
				px += vx * tx;
				py += vy * ty;
				break;
			
			case(IntegrationMethod::VELOCITY_VERLET):
				px += vx * tx + 0.5 * aF.x * tx * tx;
				py += vy * ty + 0.5 * aF.y * ty * ty;

				vx = aF.x * tx;
				vy = aF.y * ty;
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

			actualPosition.x = METERS_TO_PIXELS(px);
			actualPosition.y = METERS_TO_PIXELS(py);

			actualVelocity.x = METERS_TO_PIXELS(vx);
			actualVelocity.y = METERS_TO_PIXELS(vy);

			bodies->data->SetPosition(actualPosition);
			bodies->data->SetLinearVelocity(actualVelocity);
			
			// If speed on a variable is 0, reset timer for when it starts moving again
			bodies->data->tx += dt;
			bodies->data->ty += dt;
		}
	}
}


void wBody::SetLinearVelocity(wVec2 v)
{
	speed = v;
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
float wBody::GetMass()
{
	return mass;
}
void wBody::OnCollision(wBody* Body2)
{
	LOG("This is a collision");
}