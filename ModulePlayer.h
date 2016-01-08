#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "Directions.h"
#include "Characters.h"
#include "Projectile.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	bool active = false;
	characters characterType = CHARACTER_WARRIOR;
	projectile_type projectileType = PROJECTILE_WARRIOR;

	//Controls
	int attackKey, upKey, downKey, leftKey, rightKey;

	float speed = 1;
	int score = 0;
	int health = 0;
	int numKeys = 0;

	int shootCooldown = 45;
	int actualShootCooldown = shootCooldown;

	SDL_Texture* graphics = nullptr;
	fPoint position;
	Facing facing = DOWN;

	Animation animations[8];
	Collider* collider;

};

#endif // __MODULEPLAYER_H__