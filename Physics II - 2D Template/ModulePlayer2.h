#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"


struct Object2
{
	SDL_Texture* graphic;
	uint fx;

	Object2() : graphic(NULL)
	{}
};

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	wBody* plBody;
	p2Point<float> pos; // Render Position (pixels)

	p2List<wBody*> circles;

	int HP = 5;
	bool alive = true;

	bool DestroyBullet = false;

	bool isTurn = false;

	int angle = 0;
	int strength = 0;

	const char* pltexchar = "Assets/Wahssets/Wahrmluigi.png";

	SDL_Texture* pltex;

	const char* bulltexchar = "Assets/Wahssets/Ball.png";

	SDL_Texture* balltex;
};