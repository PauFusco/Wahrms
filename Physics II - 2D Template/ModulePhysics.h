#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"

// Define Physics Globals here
#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.005f

// Meters to pixels and reverse (transformation and coeficient)
#define PIXELS_PER_METER 100.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.01f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) (PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// b2Vec2 equivalent
class wVec2 {
public:
	wVec2() {}
	~wVec2() {}
	
	wVec2(float x, float y) : x(x), y(y) {}
public:
	float x, y;
};

// This defines whether the bodies will react
// to collisions with movementor they will stay static
enum class bodyType
{
	DYNAMIC,
	STATIC,
	UNKNOWN
};

enum class wBodyClass
{
	CIRCLE,
	SQUARE,
	UNKOWN
};

enum class ColliderType
{
	PLAYER,
	FLOOR,
	BUMPER,
	AIR, // If we make a static square
		 //	to occupy everything and make
		 // it slow with air friction
	UNKNOWN
};

enum class IntegrationMethod
{
	IMPLICIT_EULER,
	SYMPLECTIC_EULER,
	VELOCITY_VERLET,
	UNKNOWN
};

// Collider of a wBody is itself
class wBody
{
public:

	wBody(){}
	~wBody(){}

	// Setters
	void SetLinearVelocity(wVec2 v);
	void SetPosition(p2Point<float> position);
	
	void SetWidth(int iwidth);
	void SetHeight(int iheight);
		
	// Getters
	wVec2 GetSpeed();
	p2Point<float> GetPosition();
	
	int GetWidth();
	int GetHeight();

	float GetMass();

	void OnCollision(wBody* Body2);


public:
	
	ColliderType ctype;
	bodyType btype;
	wBodyClass wclass;

	float tx = 0;
	float ty = 0;
	
	bool IsCollisionListener = false;

private:
	int width, height;
	unsigned int mass = 1;
	unsigned int elasticCoef = 1;
	p2Point<float> bPos; // Position in meters
	wVec2 speed;
};

class Floor
{
public:
	Floor();
	~Floor();

	Floor(wVec2 gravity, float friction, wBody* floorBody) :
		  gravity(gravity), frictionCoef(friction), floorBody(floorBody) {}
	
public:
	wBody* floorBody;
	
	wVec2 gravity = wVec2(GRAVITY_X, GRAVITY_Y);
	float frictionCoef;

};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate(); // Apply ALL forces to ALL bodies (use list)
	update_status PostUpdate();
	bool CleanUp();
	
public:
	wBody* CreateCircle(float r, p2Point<float> pos);

	void CheckCollision(); // Check collisions of all the elements in the body list
	
	void addBodyToList(wBody* body); // Add body to the list to be able to check collisions

	void destroyBody(wBody* body);


	void CreateFloor(); // Create rectangle on the bottom on the screen and put it in the list (use class??)

	void integrator();

private:
	p2List<wBody*>* Bodies;
	bool debug = true;

	IntegrationMethod IntMeth = IntegrationMethod::IMPLICIT_EULER;
};