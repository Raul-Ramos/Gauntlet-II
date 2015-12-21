#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Globals.h"
#include "Module.h"
#include "Particle.h"

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

private:

	std::vector<Particle*> particles;
};

#endif //__MODULEPARTICLES_H__