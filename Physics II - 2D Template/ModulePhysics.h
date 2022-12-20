#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"

// Define Physics Globals here


// b2Vec2 equivalent
class wVec2 {
public:
	wVec2();
	~wVec2();
	
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
	wBody();
	~wBody();

	
	void SetLinearVelocity(wVec2 v);
	void SetPosititon(const wVec2& position);
	void GetPosition(int& x, int& y) const;
	
	void OnCollision(wBody* Body2);

public:
	int width, height;
	ColliderType ctype;

private:
	float mass, x, y;
	wVec2 speed;
};

class Floor
{
public:
	Floor();
	~Floor();

	Floor(wVec2 gravity, float friction);
	
public:
	wVec2 gravity;
	float frictionQuoef;
	int x, y, width, height;

};


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	
	void CheckCollision(); // Check collisions of all the elements in the body list
	
	void addBodyToList(wBody* body); // Add body to the list to be able to check collisions

	void CreateFloor(); // Create rectangle on the bottom on the screen and put it in the list
	


private:
	p2List<wBody*>* Bodies;
	bool debug;
};