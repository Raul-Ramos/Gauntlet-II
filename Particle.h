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
	Particle();
	~Particle();

	void MarkForDead();
	bool isMarkedForDead() const;

	SDL_Texture* graphics = nullptr;
	fPoint position;
	iPoint speed;
	Collider* collider;
	Animation animation;
	Uint32 begun;
	Uint32 duration;

	virtual update_status Update();

private:
	bool toDelete = false;
};

#endif //__PARTICLE_H__