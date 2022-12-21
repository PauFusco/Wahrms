#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"

// Define Physics Globals here
// meters to pixels and reverse (transformation and coeficient)

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
enum bodyType
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


// Collider of a wBody is itself
class wBody
{
public:
	wBody(){}
	~wBody();

	// Setters
	void SetLinearVelocity(wVec2 v);
	void SetPosition(p2Point<float> position);
	void SetWidth(int width);
	void SetHeight(int height);
	
	
	
	// Getters
	wVec2 GetSpeed();
	p2Point<float> GetPosition();
	
	void OnCollision(wBody* Body2);

public:
	int width, height;
	ColliderType ctype;
	bodyType btype;
	wBodyClass wclass;

private:
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
	
	wVec2 gravity;
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

	//void CreateFloor(); // Create rectangle on the bottom on the screen and put it in the list (use class??)

private:
	p2List<wBody*>* Bodies;
	bool debug = true;
};