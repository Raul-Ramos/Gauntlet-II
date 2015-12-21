#include "ModuleCollisions.h"

ModuleCollisions::ModuleCollisions()
{
	//Valid collision array
	matrix[WALL][WALL] = false;
	matrix[WALL][PLAYER] = true;
	matrix[WALL][ENEMY] = true;
	matrix[WALL][PLAYER_PROJECTILE] = true;

	matrix[PLAYER][PLAYER] = false;
	matrix[PLAYER][ENEMY] = true;
	matrix[PLAYER][PLAYER_PROJECTILE] = false;

	matrix[ENEMY][ENEMY] = true;
	matrix[ENEMY][PLAYER_PROJECTILE] = true;

	matrix[PLAYER_PROJECTILE][PLAYER_PROJECTILE] = false;

	//Fills the other half of the array
	for (int y = 1; y < NONE - 1; y++){
		for (int x = 0; x < y; x++){
			matrix[x][y] = matrix[y][x];
		}
	}
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{}

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
	colliders.clear();
	return true;
}

Collider* ModuleCollisions::AddCollider(COLLIDER_TYPE type, SDL_Rect box, Module* father)
{
	Collider* ret = new Collider(type, box, father);
	colliders.push_back(ret);
	return ret;
}