#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Globals.h"
#include "Module.h"
#include "Projectile.h"
#include "Point.h"
#include "Animation.h"
#include "Characters.h"
#include "Gauntlet_Colors.h"

struct SDL_Texture;
struct Collider;
struct SoundSuccesion;
struct TimeFunction;

enum critical_state {
	state_stable,
	state_minus_500,
	state_minus_200
};

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
	void onTimeFunction(TimeFunction* timeFunction);

	//Returns a sound succession with is color and class
	SoundSuccesion* characterSoundSuccesion();

public:

	bool active = false;
	characters characterType = CHARACTER_WARRIOR;
	projectile_type projectileType = PROJECTILE_WARRIOR;
	Gauntlet_Colors color;

	//Controls
	int attackKey, upKey, downKey, leftKey, rightKey;

	float speed = 1;
	int score = 0;
	int health = 0;
	int numKeys = 0;

	SDL_Texture* graphics = nullptr;
	fPoint position;
	Facing facing = DOWN;

	TimeFunction* lifeLose = nullptr;
	TimeFunction* dyingSound = nullptr;

	Animation animations[8];
	Collider* collider = nullptr;

private:

	//Goes to 1 when < 500 and to 2 when < 200,
	//used for sound alarm
	critical_state criticalState = state_stable;

	//Shooting cooldown
	int shootCooldown = 38;
	int actualShootCooldown = shootCooldown;
};

#endif // __MODULEPLAYER_H__