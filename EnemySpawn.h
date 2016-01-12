#ifndef __ENEMYSPAWN_H__
#define __ENEMYSPAWN_H__

#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "Enemies.h"
#include "Animation.h"

enum colliderDirection{
	CDIRECTION_CENTER = 0, CDIRECTION_UP, CDIRECTION_DOWN, CDIRECTION_LEFT, CDIRECTION_RIGHT
};

struct Collider;
struct SDL_Texture;
struct TimeFunction;

struct SpawnPoint : Module {

	SpawnPoint(const enemyType type, const fPoint position);

	enemyType type;
	fPoint position;
	Collider* colliders[5];

	SDL_Texture* graphics = nullptr;
	Animation animation;

	TimeFunction* spawnTimer = nullptr;
	int baseTimeToSpawn = 2500;
	bool spawning = false;

	int health = 1;

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void onTimeFunction(TimeFunction* timeFunction);

};

#endif // __MODULEENEMIES_H__