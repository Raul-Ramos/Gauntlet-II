#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Globals.h"
#include "Point.h"
#include "Directions.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;

struct Particle
{
	SDL_Texture* graphics = nullptr;
	iPoint position;
	iPoint speed;
	Facing facing;
	Collider* collider;
	Animation animation;

	virtual update_status Update();
};

#endif //__PARTICLE_H__