#include "Particle.h"
#include "Application.h"
#include "ModuleRender.h"

update_status Particle::Update()
{
	position.x += speed.x;
	position.y += speed.y;

	App->renderer->Blit(graphics, position.x, position.y, &(animation.GetCurrentAnimatedFrame()), 1.0f);

	return UPDATE_CONTINUE;
}