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

	Collider* AddCollider(const COLLIDER_TYPE type, const fRect box, Module* father = NULL);

private:

	std::vector<Collider*> colliders;
	bool matrix[COLLIDER_NONE][COLLIDER_NONE];
};

#endif // __MODULE_COLLISIONS_H__