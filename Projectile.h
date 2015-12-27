#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "Particle.h"

enum projectile_type {
	PROJECTILE_WARRIOR,
	PROJECTILE_DEMON
};

struct Projectile : Particle {

	projectile_type projectileType;

	void OnCollision(Collider* col1, Collider* col2);

};

#endif //__PROJECTILE_H__