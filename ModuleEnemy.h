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
	ModuleEnemy(const fPoint position);
	~ModuleEnemy();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	int life = 1;

	SDL_Texture* graphics = nullptr;
	fPoint position;
	Facing facing = DOWN;

	Animation animations[8];
	Collider* collider;

};

#endif // __MODULEENEMY_H__