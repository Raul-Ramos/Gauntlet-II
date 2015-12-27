#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "SDL/include/SDL.h"

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
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("gauntlet2.png");
	collider = App->collisions->AddCollider(COLLIDER_PLAYER, { position.x, position.y, 16, 16 }, this);

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
update_status ModulePlayer::PreUpdate()
{
	int facingH = 0;
	int facingV = 0;

	//Key reading
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) facingH = 1;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) facingH = -1;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) facingV = 1;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) facingV = -1;

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

	//Shoot projectile. You can't move while attacking
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT){

		//If the shoot cooldown is over
		if (actualShootCooldown > shootCooldown){
			App->particles->AddParticles(App->particles->CreateProjectile(PROJECTILE_WARRIOR, position, facing, graphics));
			actualShootCooldown = 1;
		}
		else { 

			//Reduces cooldown. For some reason, diagonal shooting cools faster
			if (facing == UP || facing == DOWN || facing == LEFT || facing == RIGHT){
				++actualShootCooldown;
			} else {
				actualShootCooldown += 2;
			}
			
		}
	
	} else {
	//If it's not shooting, it's moving

	//Move the character collider + speed * facingDirection
	//to see if you can go ahead
	collider->box.x += facingH * speed;
	collider->box.y -= facingV * speed;

	}

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update(){

	//If you can move in that direction because the collider didn't correct himself
	if (collider->box.x != position.x || collider->box.y != position.y) {
		
		iPoint difference = {
			collider->box.x - position.x,
			collider->box.y - position.y };

		position.x += difference.x;
		position.y += difference.y;

		App->renderer->camera.x -= difference.x * 2;
		App->renderer->camera.y -= difference.y * 2;

		//There's movement. Animate.
		App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentAnimatedFrame()), 1.0f);

	} else {

		//There's no movement. Static image
		App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentFrame()), 1.0f);

	}

	return UPDATE_CONTINUE;
}

//On a collision
void ModulePlayer::OnCollision(Collider* col1, Collider* col2){
	switch (col2->type)
	{
	case COLLIDER_WALL: {
		//If you can't go in that direction

		SDL_Rect* col1b = &col1->box;
		SDL_Rect* col2b = &col2->box;

		//Vertical correction
		if (position.x < col2b->x + col2b->w && position.x + col1b->w > col2b->x)
			collider->box.y = position.y;

		//Horizontal correction
		if (position.y < col2b->y + col2b->h && position.y + col1b->h > col2b->y)
			collider->box.x = position.x; 

		break;
		}

	case COLLIDER_ENEMY:
		LOG("UUUGH! Collision!");
		break;
	default:
		break;
	}
}