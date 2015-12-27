#include "Globals.h"
#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "SDL/include/SDL.h"

//TODO: A lot of things in common between ModulePlayer and ModuleEnemy. Inheritance?

ModuleEnemy::ModuleEnemy() : Module()
{
	position.x = 100;
	position.y = 50;

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
	LOG("Loading player");

	graphics = App->textures->Load("gauntlet2.png");
	collider = App->collisions->AddCollider(COLLIDER_ENEMY, { position.x, position.y, 16, 16 }, this);

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading enemy");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModuleEnemy::Update()
{
	int facingH = 0;
	int facingV = 0;

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
	position.x += 1 * facingH;
	position.y -= 1 * facingV;
	collider->setPos(position.x, position.y);

	//If there's movement, animate. If not, static image.
	App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentAnimatedFrame()), 1.0f);

	return UPDATE_CONTINUE;
}