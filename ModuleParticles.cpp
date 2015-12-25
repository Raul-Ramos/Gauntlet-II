#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

ModuleParticles::ModuleParticles()
{
}

ModuleParticles::~ModuleParticles(){
	for (vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
		RELEASE(*it);

	particles.clear();
}

bool ModuleParticles::CleanUp(){
	for (int i = 0; i < particles.size(); i++)
	{
		App->textures->Unload(particles[i]->graphics);
	}
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
			delete particles[i];
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