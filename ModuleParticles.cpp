#include "ModuleParticles.h"
#include "SDL/include/SDL.h"

ModuleParticles::ModuleParticles()
{
}

ModuleParticles::~ModuleParticles(){
}

bool ModuleParticles::CleanUp(){
	return true;
}

update_status ModuleParticles::Update(){
	Uint32 time = SDL_GetTicks();

	for (int i = 0; i < particles.size(); i++){
		if (particles[i]->begun + particles[i]->duration <= time){
			particles[i]->MarkForDead();
		}
		else {
			particles[i]->Update();
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate(){

	//Deletes colliders marked for it
	std::vector<std::string>::size_type i = 0;
	while (i < particles.size()) {
		if (particles[i]->isMarkedForDead()) {
			particles.erase(particles.begin() + i);
		}
		else {
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticles(Particle* particle){
	particles.push_back(particle);
}