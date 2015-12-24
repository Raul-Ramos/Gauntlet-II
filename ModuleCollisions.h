#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#include <vector>
#include "Collider.h"
#include "Module.h"

class ModuleCollisions : public Module
{
public:
	
	ModuleCollisions();
	~ModuleCollisions();

	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	Collider* AddCollider(COLLIDER_TYPE type, SDL_Rect box, Module* father = NULL);

private:

	std::vector<Collider*> colliders;
	bool matrix[COLLIDER_NONE - 1][COLLIDER_NONE - 1];
};

#endif // __MODULE_COLLISIONS_H__