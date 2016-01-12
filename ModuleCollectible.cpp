#include "ModuleCollectible.h"
#include "Collider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleMap.h"
#include "ModuleParticles.h"
#include "SoundLibrary.h"
#include "Animation.h"
#include "FallingAnimation.h"

ModuleCollectible::ModuleCollectible(const TypeCollectible type) : Module(), type(type)
{
}

ModuleCollectible::~ModuleCollectible()
{
	delete animation;
}

// Unload assets
bool ModuleCollectible::CleanUp()
{
	App->textures->Unload(graphics);
	return true;
}

// Update
update_status ModuleCollectible::Update()
{
	App->renderer->Blit(graphics, position.x, position.y, &(animation->GetCurrentAnimatedFrame()), 1.0f);
	return UPDATE_CONTINUE;
}

//On collision with a collectible
void ModuleCollectible::OnCollision(Collider* col1, Collider* col2){
	if (col2->type == COLLIDER_PLAYER){

		//Specifies if it will be deleted after doing its function
		bool deleteAfterFunction = true;

		switch (type)
		{
		case COLLECTIBLE_TREASURE: {
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father); //TODO: If it's common on everyone, declare outside
			player->score += 100;
			App->soundLib->playSound(SOUND_TAKE_TREASURE);
			break; }
		case COLLECTIBLE_MEAT:
		case COLLECTIBLE_DRINK:{
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father);
			player->health += 100;
			App->soundLib->playSound(SOUND_TAKE_FOOD);
			break; }
		case COLLECTIBLE_KEY: {
			ModulePlayer * player = dynamic_cast<ModulePlayer*>(col2->father);
			player->score += 100;
			player->numKeys += 1;
			App->soundLib->playSound(SOUND_TAKE_KEY);
			break; }
			break;
		case COLLECTIBLE_FLOOR_TRIGGER:
			floorTriggerFunction();
			App->soundLib->playSound(SOUND_FLOOR_TRIGGER);
			break;
		case COLLECTIBLE_EXIT:
		case COLLECTIBLE_EXIT_TO_6: {

			deleteAfterFunction = false;

			//Creates a particle of the player falling in the hole that ends the level upon ending
			Particle* PlayerFalling = new Particle({ (float)position.x, (float)position.y }, graphics);
			PlayerFalling->animation = new FallingAnimation();
			PlayerFalling->duration = 10000;

			//Grid position of the first image. Y * 44 + X
			int firstImage = 0;

			switch (dynamic_cast<ModulePlayer*>(col2->father)->characterType)
			{
			case CHARACTER_WARRIOR:		firstImage = 9 * 44 + 32; break;
			case CHARACTER_VALKYRIE :	firstImage = 10 * 44 + 25; break;
			case CHARACTER_WIZARD :		firstImage = 11 * 44 + 18; break;
			case CHARACTER_ELF :		firstImage = 12 * 44 + 11; break;
			default:
				break;
			}

			int dim = 16;
			PlayerFalling->animation->speed = 0.1f;
			for (int i = 0; i < 5; i++)
				PlayerFalling->animation->frames.push_back({ (18 * ((firstImage + i) % 44)) + 1, (18 * ((firstImage + i) / 44)) + 1, dim, dim });

			//Adds the particle, deactivates the player so it won't show and plays the level completed sound
			App->particles->AddParticles(PlayerFalling);
			dynamic_cast<ModulePlayer*>(col2->father)->active = false;
			App->soundLib->playSound(SOUND_LEVEL_COMPLETE);

			break;

			}
		default:
			break;
		}

		//Deletion
		col1->toDelete = true;
		if (deleteAfterFunction) {
			toDelete = true;
		}
		//End Deletion
	}
}

void ModuleCollectible::floorTriggerFunction() {

	//Searches for the trigger group
	vector<ModuleFloorTrigger*>* floorTriggers = &App->map->floorTriggers;
	ModuleFloorTrigger* triggerGroup = nullptr;
	int index;

	for (int i = 0; i < floorTriggers->size(); i++)
	{
		for (int z = 0; z < floorTriggers->at(i)->triggers.size(); z++)
		{
			if (floorTriggers->at(i)->triggers.at(z) == this){
				triggerGroup = floorTriggers->at(i);
				index = i;
				break;
			}
		}

		if (triggerGroup != nullptr) break;
	};

	//If it's found
	if (triggerGroup != nullptr){

		vector<Wall*> wallsToDelete = triggerGroup->walls;

		//Set the trigger floor to delete
		for (int i = 0; i < triggerGroup->triggers.size(); i++)
		{
			triggerGroup->triggers[i]->collider->toDelete = true;
			triggerGroup->triggers[i]->toDelete = true;
		}

		//Delete the group
		delete triggerGroup;
		floorTriggers->erase(floorTriggers->begin() + index);

		//Delete the walls
		vector<string>::size_type i = 0;
		iPoint position;
		Wall* wall;
		int w;
		bool found;

		while (wallsToDelete.size() > 0) {
			found = false;

			//Checks if the given wall is in the delete list
			for (int z = 0; z < wallsToDelete.size(); z++)
			{
				if (App->map->walls[i] == wallsToDelete[z]){
					position = wallsToDelete[z]->position;

					//If it is, deletes and erases it from both list and puts a floor on his place
					App->map->floor.push_back(new iPoint{ position });

					wallsToDelete.at(z)->collider->toDelete = true;
					wallsToDelete.erase(wallsToDelete.begin() + z);

					delete App->map->walls[i];
					App->map->walls.erase(App->map->walls.begin() + i);

					//Adjust the surrounding walls index
					if (App->map->walls.size() > 0){
						w = 0;
						do {
							wall = App->map->walls[w];
							if (wall->position.y == position.y){
								if (wall->position.x == position.x - 16) {
									wall->direction = static_cast<WallDirection>(wall->direction - WALLDIRECTION_RIGHT);
								}
								else if (wall->position.x == position.x + 16){
									wall->direction = static_cast<WallDirection>(wall->direction - WALLDIRECTION_LEFT);
								}
							}
							else if (wall->position.x == position.x) {
								if (wall->position.y == position.y - 16){
									wall->direction = static_cast<WallDirection>(wall->direction - WALLDIRECTION_UP);
								}
								else if (wall->position.y == position.y + 16){
									wall->direction = static_cast<WallDirection>(wall->direction - WALLDIRECTION_DOWN);
								}
							}

							++w;
						} while (wall->position.y <= position.y + 16);
					}

					found = true;
					break;

				}
			}

			if (!found) ++i;
		}
	}
}