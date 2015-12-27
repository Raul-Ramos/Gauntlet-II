#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "Directions.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:
	int score = 0;
	int health = 0;
	int numKeys = 0;

	SDL_Texture* graphics = nullptr;
	iPoint position;
	Facing facing = DOWN;

	Animation animations[8];
	Collider* collider;

};

#endif // __MODULEPLAYER_H__