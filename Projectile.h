#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "Particle.h"

struct ModulePlayer;

enum projectile_type {
	PROJECTILE_WARRIOR,
	PROJECTILE_VALKYRIE,
	PROJECTILE_WIZARD,
	PROJECTILE_ELF,
	PROJECTILE_DEMON
};

struct Projectile : Particle {

	Projectile(fPoint position, SDL_Texture* graphics) : Particle(position, graphics){};

	projectile_type projectileType;
	int damage = 0;
	Module* sender = nullptr;

	void OnCollision(Collider* col1, Collider* col2);

};

#endif //__PROJECTILE_H__