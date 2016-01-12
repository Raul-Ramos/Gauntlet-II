#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Globals.h"
#include "Module.h"
#include "Particle.h"
#include "Projectile.h"
#include <vector>

class ModuleParticles : public Module
{
public:

	ModuleParticles();
	~ModuleParticles();

	//update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void AddParticles(Particle* particle);
	Projectile* CreateProjectile(const projectile_type type, const fPoint position, const Facing facing, SDL_Texture* graphics, ModulePlayer* sender = nullptr);

private:

	std::vector<Particle*> particles;
};

#endif //__MODULEPARTICLES_H__