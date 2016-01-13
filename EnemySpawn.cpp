#include "EnemySpawn.h"
#include "Collider.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleEnemies.h"
#include "Projectile.h"
#include "ModuleEnemy.h"
#include "SDL/include/SDL.h"
#include "ModulePlayer.h"
#include "ModuleTimeFunctions.h"
#include <vector>

SpawnPoint::SpawnPoint(const enemyType type, const fPoint position) :
type(type), position(position){

	colliders[CDIRECTION_CENTER] = App->collisions->AddCollider(COLLIDER_WALL, { position.x, position.y, 16, 16 }, this);
	
	for (int i = 1; i < 5; i++)
		colliders[i] = NULL;

}

bool SpawnPoint::Start(){
	graphics = App->textures->Load("graphics//gauntlet2.png");
	if (spawnTimer == nullptr) spawnTimer = App->timeFunctions->AddTimeFunction(baseTimeToSpawn, this);
	return true;
}

bool SpawnPoint::CleanUp()
{
	LOG("Unloading enemy");

	App->textures->Unload(graphics);

	for (int i = 0; i < 5; i++)
	if (colliders[i] != NULL)
		colliders[i]->toDelete = true;

	spawnTimer->toDelete = true;

	return true;
}

update_status SpawnPoint::Update(){

	//If we're in the process of creating a new enemy
	if (spawning){

		fRect spawner = colliders[0]->box;
		fRect camera = {
			-App->renderer->camera.x / 2, -App->renderer->camera.y / 2,
			(App->renderer->camera.w / 2) - 112, (App->renderer->camera.h / 2) - 4 };

		//Check if the spawner is within the camera view
		if (spawner.x < camera.x + camera.w && spawner.x + spawner.w > camera.x &&
			spawner.y < camera.y + camera.h && spawner.y + spawner.h > camera.y){

			//Checks the free spaces
			std::vector<fPoint> freeSpawnSpaces;

			//Check temporal colliders
			for (int i = 1; i < 5; i++) {

				//If it's not invalid
				if (colliders[i] != NULL){

					//Stores the position
					fPoint enemyPosition = position;
					if (i == CDIRECTION_DOWN) enemyPosition.y += 16;
					if (i == CDIRECTION_UP) enemyPosition.y -= 16;
					if (i == CDIRECTION_RIGHT) enemyPosition.x += 16;
					if (i == CDIRECTION_LEFT) enemyPosition.x -= 16;
					freeSpawnSpaces.push_back(enemyPosition);

					//Deletes the collider
					colliders[i]->toDelete = true;
					colliders[i] = NULL;
				}
			}

			//If there are free locations
			if (freeSpawnSpaces.size() > 0) {

				//Adds an enemy in a random free location.
				App->enemies->AddEnemy(freeSpawnSpaces.at(rand() % freeSpawnSpaces.size()));

				//Sets the next spawn time with some random percent offset
				int min = 30, max = 110;
				float offset = rand() % (max - min + 1) + min;
				spawnTimer->duration = baseTimeToSpawn * (offset / 100);
				spawnTimer->begin = SDL_GetTicks();

				//If there are no disponible locations it will not wait for the spawn cooldown
				//and will spawn a enemy as soon as posible
				spawning = false;
			}
		}
	}

	//Print graphic
	App->renderer->Blit(graphics, position.x, position.y, &(animation.GetCurrentFrame()), 1.0f);

	return UPDATE_CONTINUE;
}

void SpawnPoint::OnCollision(Collider* c1, Collider* c2){
	if (c1->type == COLLIDER_WALL && c2->type == COLLIDER_PROJECTILE){

		Projectile* projectile = dynamic_cast<Projectile*>(c2->father);

		//If a projectile hits it, take damage
		health -= projectile->damage;
		if (health < 1){

			App->map->floor.push_back(new iPoint{ (int)position.x, (int)position.y });

			if (projectile->sender != nullptr) {
				ModulePlayer* player = dynamic_cast<ModulePlayer*>(projectile->sender);
				if (player != NULL)
					player->score += 10;
			}
		}

	}
	else if (c1->type == COLLIDER_SPAWNPOINT){

		//If a temporal collider collides with something, this can't
		//be used for spawning new enemies
		for (int i = 1; i < 5; i++) {
			if (c1 == colliders[i]){
				colliders[i]->toDelete = true;
				colliders[i] = NULL;
			}
		}

	}
}

//When a new enemy can be spawned
void SpawnPoint::onTimeFunction(TimeFunction* timeFunction){

	//If we're not already trying to spawn an enemy
	if (!spawning){
		spawning = true;

		//If it can create a new monster, create temporal colliders
		colliders[CDIRECTION_LEFT] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { position.x - 16, position.y, 16, 16 }, this);
		colliders[CDIRECTION_RIGHT] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { position.x + 16, position.y, 16, 16 }, this);
		colliders[CDIRECTION_UP] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { position.x, position.y - 16, 16, 16 }, this);
		colliders[CDIRECTION_DOWN] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { position.x, position.y + 16, 16, 16 }, this);
	}
}

