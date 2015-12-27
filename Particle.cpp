#include "Particle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "Collider.h"

Particle::Particle(){
	begun = SDL_GetTicks();
}

Particle::~Particle(){
}

void Particle::MarkForDead(){
	toDelete = true;
	collider->toDelete = true;
}

bool Particle::isMarkedForDead() const{
	return toDelete;
}

update_status Particle::Update()
{
	position.x += speed.x;
	position.y += speed.y;
	collider->box.x += speed.x;
	collider->box.y += speed.y;

	App->renderer->Blit(graphics, position.x, position.y, &(animation.GetCurrentAnimatedFrame()), 1.0f);

	return UPDATE_CONTINUE;
}