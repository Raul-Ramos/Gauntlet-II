#include "ModuleCollectible.h"
#include "Collider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModuleCollectible::ModuleCollectible(TypeCollectible type) : Module(), type(type)
{
}

ModuleCollectible::~ModuleCollectible()
{
}

// Unload assets
bool ModuleCollectible::CleanUp()
{
	return true;
}

// Update
update_status ModuleCollectible::Update()
{
	App->renderer->Blit(graphics, position.x, position.y, &(animation.GetCurrentAnimatedFrame()), 1.0f);
	return UPDATE_CONTINUE;
}