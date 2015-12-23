#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "SDL/include/SDL.h"

//TODO: Could be innecesary
#include "Particle.h"
#include "ModuleParticles.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 0;
	position.y = 0;

	// Reads the animations out of the spritesheet
	
	int yValue = (18 * 9) + 1;	//(Tilesize * line) + border
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

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("gauntlet2.png");
	collider = App->collisions->AddCollider(PLAYER, { position.x, position.y, 18, 18 }, this);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	int facingH = 0;
	int facingV = 0;

	//Key reading
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) facingH = 1;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) facingH = -1;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) facingV = 1;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) facingV = -1;

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

	//Move the character position & collider
	//+ speed * facingDirection
	position.x += 1 * facingH;
	position.y -= 1 * facingV;
	collider->setPos(position.x, position.y);

	App->renderer->camera.x -= 2 * facingH;
	App->renderer->camera.y += 2 * facingV;

	//If there's movement, animate. If not, static image.
	if (facingH != 0 || facingV != 0){
		App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentAnimatedFrame()), 1.0f);
	}
	else {
		App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentFrame()), 1.0f);
	}

	//Attack
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN){

		//Creates the particle
		Particle* particle = new Particle();
		particle->facing = facing;
		particle->graphics = graphics;
		particle->collider = App->collisions->AddCollider(PLAYER_PROJECTILE, { position.x, position.y, 18, 18 });
		particle->position = { position.x, position.y };
		particle->duration = 10000;

		int dirH, dirV;
		switch (facing)
		{
		case UP:		dirH = 0; dirV = -1;  break;
		case UPRIGHT:	dirH = 1; dirV = -1;  break;
		case RIGHT:		dirH = 1; dirV = 0;   break;
		case DOWNRIGHT: dirH = 1; dirV = 1;   break;
		case DOWN:		dirH = 0; dirV = 1;   break;
		case DOWNLEFT:	dirH = -1; dirV = 1;  break;
		case LEFT:		dirH = -1; dirV = 0;  break;
		case UPLEFT:	dirH = -1; dirV = -1; break;
		default:		dirH = 0; dirV = -1;  break;
		}
		particle->speed = { dirH * 2, dirV * 2 };
		
		//Does the throw animation
		int yValue = (18 * 9) + 1;				//(Tilesize * line) + border
		int dim = 18 - 2;						//Dimension. Tilesize - (border*2). Both width and height.
		
		for (int i = 0; i < 8; i++){
			//Xvalue = Tilesize * ( column + animationStep ) + border
			particle->animation.frames.push_back({ 18 * (24 + i) + 1, yValue, dim, dim });
		}
		particle->animation.speed = 0.5f;

		App->particles->AddParticles(particle);

	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* col1, Collider* col2){
	if (col2->type == ENEMY){
		LOG("UUUGH! Collision!");
	}
}