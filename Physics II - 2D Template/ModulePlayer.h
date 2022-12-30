#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"


struct Object
{
	SDL_Texture* graphic;
	uint fx;
	

	

	Object() : graphic(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	wBody* plBody;
	p2Point<float> pos; // Render Position (pixels)

	p2List<wBody*> circles;
	uint launch_fx;

	int HP = 5;
	bool alive = true;

	bool isTurn = true;

	bool DestroyBullet = false;

	int angle = 0;
	int strength = 0;

	const char* pltexchar = "Assets/Wahssets/Wahrmio.png";

	SDL_Texture* pltex;

	const char* bulltexchar = "Assets/Wahssets/Ball.png";

	SDL_Texture* balltex;
};