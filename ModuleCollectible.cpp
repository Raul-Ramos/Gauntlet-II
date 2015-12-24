#include "ModuleCollectible.h"
#include "Collider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModulePlayer.h"

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

//On collision with a collectible
void ModuleCollectible::OnCollision(Collider* col1, Collider* col2){
	if (col2->type == COLLIDER_PLAYER){

		switch (type)
		{
		case COLLECTIBLE_TREASURE: {
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father); //TODO: If it's common on everyone, declare outside
			player->score += 100;
			break; }
		case COLLECTIBLE_MEAT:
		case COLLECTIBLE_DRINK:{
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father);
			player->health += 100;
			break; }
		case COLLECTIBLE_KEY: {
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father);
			player->score += 100;
			player->numKeys += 1;
			break; }
		case COLLECTIBLE_EXIT:
			break;
		case COLLECTIBLE_EXIT_TO_6:
			break;
		default:
			break;
		}

		//Deletion
		col1->toDelete = true;
		
		ModuleMap* map = App->map;
		for (int i = 0; i < map->collectibles.size(); i++)
		{
			if (map->collectibles[i] == this){
				map->floor.push_back(&position);
				map->collectibles.erase(map->collectibles.begin() + i);
				break;
			}
		}
		//End Deletion
	}
}