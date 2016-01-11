#include "Particle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "Collider.h"

Particle::Particle(fPoint position, SDL_Texture* graphics) :
position(position), graphics(graphics){
	begun = SDL_GetTicks();
}

Particle::~Particle(){
	delete animation;
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

	if (collider != nullptr){
		collider->box.x += speed.x;
		collider->box.y += speed.y;
	}

	App->renderer->Blit(graphics, position.x, position.y, &(animation->GetCurrentAnimatedFrame()), 1.0f);

	return UPDATE_CONTINUE;
}