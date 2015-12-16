#ifndef __MODULEENEMY_H__
#define __MODULEENEMY_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "Collider.h"
#include "Directions.h"

struct SDL_Texture;

class ModuleEnemy : public Module
{
public:
	ModuleEnemy(bool start_enabled = true);
	~ModuleEnemy();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	iPoint position;
	Facing facing = DOWN;

	Animation animations[8];
	Collider* collider;

};

#endif // __MODULEENEMY_H__