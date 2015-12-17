#include "ModuleParticles.h"

ModuleParticles::ModuleParticles()
{
}

ModuleParticles::~ModuleParticles(){
}

bool ModuleParticles::CleanUp(){
	return true;
}

update_status ModuleParticles::Update(){
	for (int i = 0; i < particles.size(); i++){
		particles[i]->Update();
	}

	return UPDATE_CONTINUE;
}


void ModuleParticles::AddParticles(Particle* particle){
	particles.push_back(particle);
}