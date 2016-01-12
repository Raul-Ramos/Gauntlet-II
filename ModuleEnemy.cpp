#include "Globals.h"
#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "ModulePlayers.h"
#include "ModulePlayer.h"
#include "Projectile.h"
#include "SoundLibrary.h"
#include "SDL/include/SDL.h"
#include <math.h>

//TODO: A lot of things in common between ModulePlayer and ModuleEnemy. Inheritance?

ModuleEnemy::ModuleEnemy(const fPoint position) : position(position)
{

	// Reads the animations out of the spritesheet

	int yValue = (18 * 0) + 1;	//(Tilesize * line) + border
	int dim = 18 - 2;			//Dimension. Tilesize - (border*2). Both width and height.

	//Adds the three sprites of walking to their respective animations
	for (int z = 0; z < 8; z++){
		//Xvalue = Tilesize * ( ( animationStep * numOfDirections ) + offset) + border
		animations[z].frames.push_back({ 18 * ((0 * 8) + z) + 1, yValue, dim, dim });
		animations[z].frames.push_back({ 18 * ((1 * 8) + z) + 1, yValue, dim, dim });
		animations[z].frames.push_back({ 18 * ((0 * 8) + z) + 1, yValue, dim, dim });
		animations[z].frames.push_back({ 18 * ((2 * 8) + z) + 1, yValue, dim, dim });
	}

	//Specifies speed
	animations[UP].speed = 0.2f;
	animations[UPRIGHT].speed = 0.2f;
	animations[RIGHT].speed = 0.2f;
	animations[DOWNRIGHT].speed = 0.2f;
	animations[DOWN].speed = 0.2f;
	animations[DOWNLEFT].speed = 0.2f;
	animations[LEFT].speed = 0.2f;
	animations[UPLEFT].speed = 0.2f;
}

ModuleEnemy::~ModuleEnemy()
{
}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading enemy");

	graphics = App->textures->Load("gauntlet2.png");
	collider = App->collisions->AddCollider(COLLIDER_ENEMY, { position.x, position.y, 16, 16 }, this);

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading enemy");

	App->textures->Unload(graphics);
	collider->toDelete = true;

	return true;
}

// Update
update_status ModuleEnemy::PreUpdate()
{
	int facingH = 0;
	int facingV = 0;

	ModulePlayer* closestPlayer = nullptr;
	float closestPlayerDistance = -1;
	float distance;

	for (int i = 0; i < 4; i++) {
		if (App->players->players[i]->active){

			ModulePlayer* player = App->players->players[i];
			int distX = player->position.x - position.x;
			int distY = player->position.y - position.y;
			distance = sqrtf(pow(distX, 2) + pow(distY, 2));

			if (closestPlayer == nullptr || distance < closestPlayerDistance){
				closestPlayerDistance = distance;
				closestPlayer = player;
			}

		}
	}

	//If at least one player is playing
	if (closestPlayerDistance != -1){

		//Angle between the hero and this enemy
		float angle = atan2(closestPlayer->position.y - position.y, closestPlayer->position.x - position.x) * 180 / M_PI;

		//Horizontal direction
		if (angle >= -67.5 && angle <= 67.5) {
			++facingH;
		} else if (abs(angle) >= 112.5){
			--facingH;
		}

		//Vertical direction
		if (angle <= -22.5 && angle >= -157.5) {
			++facingV;
		} else if (angle >= 22.5 && angle <= 157.5){
			--facingV;
		}

		//Determine facing direction
		if (facingV < 0){
			if (facingH > 0){ facing = DOWNRIGHT; }
			else if (facingH < 0){ facing = DOWNLEFT; }
			else { facing = DOWN; }
		}
		else if (facingV > 0){
			if (facingH > 0){ facing = UPRIGHT; }
			else if (facingH < 0){ facing = UPLEFT; }
			else { facing = UP; }
		}
		else if (facingH > 0) { facing = RIGHT; }
		else if (facingH < 0) { facing = LEFT; }

		//Move the character position
		//+ speed * facingDirection
		collider->box.x += 0.6 * facingH;
		collider->box.y -= 0.6 * facingV;

	}

	return UPDATE_CONTINUE;
}

update_status ModuleEnemy::Update(){

	//If you can move in that direction because the collider didn't correct himself
	if (collider->box.x != position.x || collider->box.y != position.y) {

		fPoint difference = {
			collider->box.x - position.x,
			collider->box.y - position.y };

		position.x += difference.x;
		position.y += difference.y;

	}

	//Render always animated
	App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentAnimatedFrame()), 1.0f);

	return UPDATE_CONTINUE;
}

//On a collision
void ModuleEnemy::OnCollision(Collider* col1, Collider* col2){
	switch (col2->type)
	{
	case COLLIDER_WALL:
	case COLLIDER_ENEMY:
	case COLLIDER_COLLECTIBLE:
	{
		//If you can't go in that direction

		fRect* col1b = &col1->box;
		fRect* col2b = &col2->box;

		//Vertical correction
		if (position.x < col2b->x + col2b->w && position.x + col1b->w > col2b->x)
			collider->box.y = position.y;

		//Horizontal correction
		if (position.y < col2b->y + col2b->h && position.y + col1b->h > col2b->y)
			collider->box.x = position.x;

		break;
	}
	case COLLIDER_PLAYER: {
		ModulePlayer* player = dynamic_cast<ModulePlayer*>(col2->father);
		player->health -= 9;
		health = 0;
		player->score += 10;
		App->soundLib->playSound(SOUND_GHOST_ATTACK);
		break;
	}

	case COLLIDER_PROJECTILE: {
		Projectile* projectile = dynamic_cast<Projectile*>(col2->father);
		if (projectile->sender != this){
			health -= projectile->damage;
			if (health < 0 && projectile->sender != nullptr){
				ModulePlayer* player = dynamic_cast<ModulePlayer*> (projectile->sender);
				if (player != NULL)
					player->score += 10;
			}
		}
		break;
	}

	default:
		break;
	}
}