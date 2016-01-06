#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "Particle.h"

struct ModulePlayer;

enum projectile_type {
	PROJECTILE_WARRIOR,
	PROJECTILE_DEMON
};

struct Projectile : Particle {

	projectile_type projectileType;
	int damage = 0;
	ModulePlayer* sender = nullptr;

	void OnCollision(Collider* col1, Collider* col2);

};

#endif //__PROJECTILE_H__