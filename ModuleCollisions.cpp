#include "ModuleCollisions.h"

using namespace std;

ModuleCollisions::ModuleCollisions()
{
	//Valid collision array
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PROJECTILE] = true;
	matrix[COLLIDER_WALL][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_WALL][COLLIDER_SPAWNPOINT] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PROJECTILE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPAWNPOINT] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PROJECTILE] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_SPAWNPOINT] = true;

	matrix[COLLIDER_PROJECTILE][COLLIDER_PROJECTILE] = false;
	matrix[COLLIDER_PROJECTILE][COLLIDER_COLLECTIBLE] = true;
	matrix[COLLIDER_PROJECTILE][COLLIDER_SPAWNPOINT] = false;

	matrix[COLLIDER_COLLECTIBLE][COLLIDER_COLLECTIBLE] = false;
	matrix[COLLIDER_COLLECTIBLE][COLLIDER_SPAWNPOINT] = true;
	
	matrix[COLLIDER_SPAWNPOINT][COLLIDER_SPAWNPOINT] = false;

	//Fills the other half of the array
	for (int y = 1; y < COLLIDER_NONE; y++){
		for (int x = 0; x < y; x++){
			matrix[y][x] = matrix[x][y];
		}
	}
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{
	for (vector<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);
	colliders.clear();

}

// Called before render is available
update_status ModuleCollisions::Update()
{
	Collider* c1;
	Collider* c2;

	//Does every element with every other element except itself
	for (int y = 0; y < colliders.size() - 1; y++){

		c1 = colliders.at(y);

		for (int x = y + 1; x < colliders.size(); x++){

			//Checks if there is a collision
			if (matrix[c1->type][colliders.at(x)->type]){			//If their collision is allowed
				if (c1->CheckCollision(colliders.at(x)->box)){		//If they actually collide

					c2 = colliders.at(x);

					//Tells them they have collided
					if (c1->father != NULL) c1->father->OnCollision(c1, c2);
					if (c2->father != NULL) c2->father->OnCollision(c2, c1);

				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate(){

	//Deletes colliders marked for it
	std::vector<std::string>::size_type i = 0;
	while (i < colliders.size()) {
		if (colliders[i]->toDelete) {
			delete colliders[i];
			colliders.erase(colliders.begin() + i);
		}
		else {
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	return true;
}

Collider* ModuleCollisions::AddCollider(COLLIDER_TYPE type, fRect box, Module* father)
{
	Collider* ret = new Collider(type, box, father);
	colliders.push_back(ret);
	return ret;
}