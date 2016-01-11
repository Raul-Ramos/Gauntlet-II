#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePlayers.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "SoundSuccesion.h"
#include "Directions.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	//Specifies speed
	animations[UP].speed = 0.15f;
	animations[UPRIGHT].speed = 0.15f;
	animations[RIGHT].speed = 0.15f;
	animations[DOWNRIGHT].speed = 0.15f;
	animations[DOWN].speed = 0.15f;
	animations[DOWNLEFT].speed = 0.15f;
	animations[LEFT].speed = 0.15f;
	animations[UPLEFT].speed = 0.15f;

	//Starts the life lost time
	lastLifeLostTime = SDL_GetTicks();
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
	collider->setPos(position.x, position.y);

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

	if (active){

		int facingH = 0;
		int facingV = 0;

		//Key reading
		if (App->input->GetKey(rightKey) == KEY_REPEAT) facingH = 1;
		if (App->input->GetKey(leftKey) == KEY_REPEAT) facingH = -1;
		if (App->input->GetKey(upKey) == KEY_REPEAT) facingV = 1;
		if (App->input->GetKey(downKey) == KEY_REPEAT) facingV = -1;

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
		if (App->input->GetKey(attackKey) == KEY_REPEAT){

			//If the shoot cooldown is over
			if (actualShootCooldown > shootCooldown){
				App->particles->AddParticles(App->particles->CreateProjectile(projectileType, position, facing, graphics, this));
				actualShootCooldown = 1;
			}
			else if (facing == UPLEFT || facing == UPRIGHT || facing == DOWNLEFT || facing == DOWNRIGHT) {
				//Extra cooldown. For some reason, diagonal shooting cools faster
				++actualShootCooldown;
			}
	
		} else {
		//If it's not shooting, it's moving

		//Move the character collider + speed * facingDirection
		//to see if you can go ahead
		collider->box.x += facingH * speed;
		collider->box.y -= facingV * speed;

		}

		//Shoot cooldown
		++actualShootCooldown;

	} //END_ACTIVE
	else {

		//Character type change
		if (App->input->GetKey(rightKey) == KEY_REPEAT) characterType = CHARACTER_VALKYRIE;
		if (App->input->GetKey(leftKey) == KEY_REPEAT) characterType = CHARACTER_ELF;
		if (App->input->GetKey(upKey) == KEY_REPEAT) characterType = CHARACTER_WARRIOR;
		if (App->input->GetKey(downKey) == KEY_REPEAT) characterType = CHARACTER_WIZARD;
	
	}

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update(){

	if (active) {

		if (health < 0){
			App->players->playerDies(this);
			return UPDATE_CONTINUE;
		}
		
		//Loses 1 of life every second
		Uint32 time = SDL_GetTicks();
		if (time > lastLifeLostTime + 1000){
			--health;
			lastLifeLostTime = time;
		}

		//// CRITICAL STATE ////
		//The player gets in their correct critical state
		if (health <= 200){
			if (criticalState != state_minus_200){
				SoundSuccesion* succesion = characterSoundSuccesion();
				succesion->succesion.push_back({ SOUND_NARRATOR_DYING, 0 });
				App->soundLib->AddSoundSuccesion(succesion);
				criticalState = state_minus_200;
			}
		}
		else if (health <= 500 && criticalState != state_minus_500){

			SoundSuccesion* succesion = characterSoundSuccesion();
			float result = rand() % 2;
			if (result > 1) succesion->succesion.push_back({ SOUND_NARRATOR_LOW_LIFE, 0 });
			else succesion->succesion.push_back({ SOUND_NARRATOR_LOW_LIFE_2, 0 });
			App->soundLib->AddSoundSuccesion(succesion);
			criticalState = state_minus_500;

		}
		else if (health > 500 && criticalState != state_stable){
			criticalState = state_stable;
		}

		//// MOVEMENT ////
		//Whatever you're animated or not
		bool move = false;

		//If you can move in that direction because the collider didn't correct himself
		if (collider->box.x != position.x || collider->box.y != position.y) {

			fPoint difference = {
				collider->box.x - position.x,
				collider->box.y - position.y };

			position.x += difference.x;
			position.y += difference.y;

			//There's movement. Animate.
			move = true;

		}
		//If you don't move but you're attacking, you're still animated
		else if (App->input->GetKey(attackKey) == KEY_REPEAT){
			move = true;
		}

		if (move)
			App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentAnimatedFrame()), 1.0f);
		else
			App->renderer->Blit(graphics, position.x, position.y, &(animations[facing].GetCurrentFrame()), 1.0f);

	} //END_ACTIVE

	return UPDATE_CONTINUE;
}

//On a collision
void ModulePlayer::OnCollision(Collider* col1, Collider* col2){
	switch (col2->type)
	{
	case COLLIDER_WALL:
	case COLLIDER_PLAYER: {

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

	case COLLIDER_ENEMY:
		LOG("UUUGH! Collision!");
		break;
	default:
		break;
	}
}

//Returns a sound succession with is color and class
SoundSuccesion* ModulePlayer::characterSoundSuccesion(){
	SoundSuccesion* succesion = new SoundSuccesion();

	switch (color)
	{
	case COLOR_RED:
		succesion->succesion.push_back({ SOUND_NARRATOR_RED, 240 });
		break;
	case COLOR_BLUE:
		succesion->succesion.push_back({ SOUND_NARRATOR_BLUE, 305 });
		break;
	case COLOR_YELLOW:
		succesion->succesion.push_back({ SOUND_NARRATOR_YELLOW, 387 });
		break;
	case COLOR_GREEN:
		succesion->succesion.push_back({ SOUND_NARRATOR_GREEN, 241 });
		break;
	default:
		break;
	}

	switch (characterType)
	{
	case CHARACTER_WARRIOR:
		succesion->succesion.push_back({ SOUND_WARRIOR_NAME, 504 });
		break;

	case CHARACTER_VALKYRIE:
		succesion->succesion.push_back({ SOUND_VALKYRIE_NAME, 487 });
		break;

	case CHARACTER_WIZARD:
		succesion->succesion.push_back({ SOUND_WIZARD_NAME, 551 });
		break;

	case CHARACTER_ELF:
		succesion->succesion.push_back({ SOUND_ELF_NAME, 332 });
		break;

	default:
		break;
	}

	return succesion;
}