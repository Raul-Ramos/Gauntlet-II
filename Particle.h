#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "Directions.h"
#include "Animation.h"

struct Collider;
struct SDL_Texture;

struct Particle : Module
{
	Particle(fPoint position, SDL_Texture* graphics);
	~Particle();

	void MarkForDead();
	bool isMarkedForDead() const;

	SDL_Texture* graphics = nullptr;
	fPoint position;
	iPoint speed = { 0, 0 };
	Collider* collider = nullptr;
	Animation* animation;
	Uint32 begun;
	Uint32 duration = -1;

	virtual update_status Update();

private:
	bool toDelete = false;
};

#endif //__PARTICLE_H__