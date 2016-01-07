#include "Application.h"
#include "ModulePlayers.h"
#include "ModulePlayer.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_scancode.h"
#include <vector>

ModulePlayers::ModulePlayers(){

	for (int i = 0; i < 4; i++){
		players[i] = new ModulePlayer();
	}

	playerJoinSpawnPoints[0] = nullptr;

	playerJoinButton[0] = SDL_SCANCODE_F1;
	playerJoinButton[1] = SDL_SCANCODE_F2;
	playerJoinButton[2] = SDL_SCANCODE_F3;
	playerJoinButton[3] = SDL_SCANCODE_F4;

	players[0]->attackKey = SDL_SCANCODE_Q;
	players[0]->upKey = SDL_SCANCODE_E;
	players[0]->downKey = SDL_SCANCODE_D;
	players[0]->leftKey = SDL_SCANCODE_S;
	players[0]->rightKey = SDL_SCANCODE_F;

	players[1]->attackKey = SDL_SCANCODE_Y;
	players[1]->upKey = SDL_SCANCODE_I;
	players[1]->downKey = SDL_SCANCODE_K;
	players[1]->leftKey = SDL_SCANCODE_J;
	players[1]->rightKey = SDL_SCANCODE_L;

	players[2]->attackKey = SDL_SCANCODE_RSHIFT;
	players[2]->upKey = SDL_SCANCODE_UP;
	players[2]->downKey = SDL_SCANCODE_DOWN;
	players[2]->leftKey = SDL_SCANCODE_LEFT;
	players[2]->rightKey = SDL_SCANCODE_RIGHT;

	players[3]->attackKey = SDL_SCANCODE_END;
	players[3]->upKey = SDL_SCANCODE_KP_8;
	players[3]->downKey = SDL_SCANCODE_KP_5;
	players[3]->leftKey = SDL_SCANCODE_KP_4;
	players[3]->rightKey = SDL_SCANCODE_KP_6;

	joinPlayer(0);
};

ModulePlayers::~ModulePlayers(){
	for (int i = 0; i < 4; i++)
		delete players[i];
};

bool ModulePlayers::Start(){

	for (int i = 0; i < 4; i++)
		players[i]->Start();

	return true;
};

update_status ModulePlayers::PreUpdate(){
	for (int i = 0; i < 4; i++)
		players[i]->PreUpdate();

	//Checks for new players wainting to join
	for (int i = 0; i < 4; i++){
		//If they push the button and they're not active
		if (App->input->GetKey(playerJoinButton[i]) && !(players[i]->active)){
			//Create a spawnPoint collider up, down, left and right to the first player
			playerJoining = i;
			playerJoinSpawnPoints[PSDIRECTION_UP] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { players[0]->position.x, players[0]->position.y - 16, 16, 16 }, this);
			playerJoinSpawnPoints[PSDIRECTION_DOWN] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { players[0]->position.x, players[0]->position.y + 16, 16, 16 }, this);
			playerJoinSpawnPoints[PSDIRECTION_LEFT] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { players[0]->position.x - 16, players[0]->position.y, 16, 16 }, this);
			playerJoinSpawnPoints[PSDIRECTION_RIGHT] = App->collisions->AddCollider(COLLIDER_SPAWNPOINT, { players[0]->position.x + 16, players[0]->position.y, 16, 16 }, this);
			break;
		}
	}

	return UPDATE_CONTINUE;
};

update_status ModulePlayers::Update(){

	adjustCamera();

	for (int i = 0; i < 4; i++)
		players[i]->Update();

	//If a player is joining
	if (playerJoining != 0){

		bool joined = false;

		for (int i = 0; i < 4; i++){

			//If a spawn point is valid
			if (playerJoinSpawnPoints[i] != nullptr){

				//If the player hasn't joined yet, he joins in this position
				if (joined == false){
					joinPlayer(playerJoining);
					players[playerJoining]->position = { playerJoinSpawnPoints[i]->box.x, playerJoinSpawnPoints[i]->box.y };
					players[playerJoining]->collider->box.x = players[playerJoining]->position.x;
					players[playerJoining]->collider->box.y = players[playerJoining]->position.y;
					joined = true;
				}
				
				playerJoinSpawnPoints[i]->toDelete = true;
				playerJoinSpawnPoints[i] == nullptr;
			}
		}

		playerJoining = 0;
	}

	return UPDATE_CONTINUE;
};


update_status ModulePlayers::PostUpdate(){

	for (int i = 0; i < 4; i++)
		players[i]->PostUpdate();

	return UPDATE_CONTINUE;
};


bool ModulePlayers::CleanUp(){
	for (int i = 0; i < 4; i++)
		players[i]->CleanUp();

	return UPDATE_CONTINUE;
};

void ModulePlayers::OnCollision(Collider* c1, Collider* c2){
	//If there is a collision on a spawn point, it can't be used
	for (int i = 0; i < 4; i++){
		if (c1 == playerJoinSpawnPoints[i]){
			c1->toDelete = true;
			playerJoinSpawnPoints[i] = nullptr;
			break;
		}
	}
}

void ModulePlayers::joinPlayer(int i){
	players[i]->active = true;
	players[i]->health = 2000;
};

//Adjusts the camera given the players position
void ModulePlayers::adjustCamera(){

	//Includes the lefmost, rightest, uppest and downest
	//players in the map. Could be more than one
	std::vector<ModulePlayer*> leftmostPlayer;
	std::vector<ModulePlayer*> rightestPlayer;
	std::vector<ModulePlayer*> uppestPlayer;
	std::vector<ModulePlayer*> downestPlayer;

	//Puts the first players, as he's always playing
	leftmostPlayer.push_back(players[0]);
	rightestPlayer.push_back(players[0]);
	uppestPlayer.push_back(players[0]);
	downestPlayer.push_back(players[0]);

	//Searches the players in the extremes
	for (int i = 1; i < 4; i++){
		if (players[i]->active){
			if (players[i]->position.x <= leftmostPlayer[0]->position.x){
				if (players[i]->position.x < leftmostPlayer[0]->position.x){
					leftmostPlayer.clear();
				}
				leftmostPlayer.push_back(players[i]);
			}
			if (players[i]->position.x >= rightestPlayer[0]->position.x){
				if (players[i]->position.x > rightestPlayer[0]->position.x){
					rightestPlayer.clear();
				}
				rightestPlayer.push_back(players[i]);
			}
			if (players[i]->position.y <= uppestPlayer[0]->position.y){
				if (players[i]->position.y < uppestPlayer[0]->position.y){
					uppestPlayer.clear();
				}
				uppestPlayer.push_back(players[i]);
			}
			if (players[i]->position.y >= downestPlayer[0]->position.y){
				if (players[i]->position.y > downestPlayer[0]->position.y){
					downestPlayer.clear();
				}
				downestPlayer.push_back(players[i]);
			}
		}
	}

	//If the players are separated by a maximum value (the screen size)...
	if (rightestPlayer[0]->position.x - leftmostPlayer[0]->position.x > 176){

		bool rightToBorder = false;
		bool leftToBorder = false;

		//If the leftmost player doesn't go to the right, he will be outside the border
		for (int i = 0; i < leftmostPlayer.size(); i++){
			if (!(leftmostPlayer[i]->collider->box.x > leftmostPlayer[i]->position.x)){
				leftToBorder = true;
				break;
			}
		}

		//If the rightest player doesn't go to the left, he will be outside the border
		for (int i = 0; i < rightestPlayer.size(); i++){
			if (!(rightestPlayer[i]->collider->box.x < rightestPlayer[i]->position.x)){
				rightToBorder = true;
				break;
			}
		}

		//If both players are going to be inside the border...
		if (leftToBorder && rightToBorder){

			//Block their movement
			for (int i = 0; i < leftmostPlayer.size(); i++){
				if (!(leftmostPlayer[i]->collider->box.x > leftmostPlayer[i]->position.x)){
					leftmostPlayer[i]->collider->box.x = leftmostPlayer[i]->position.x;
					leftmostPlayer[i]->collider->box.x = leftmostPlayer[i]->position.x;
				}
			}
			for (int i = 0; i < rightestPlayer.size(); i++){
				if (!(rightestPlayer[i]->collider->box.x < rightestPlayer[i]->position.x)){
					rightestPlayer[i]->collider->box.x = rightestPlayer[i]->position.x;
					rightestPlayer[i]->collider->box.x = rightestPlayer[i]->position.x;
					break;
				}
			}
		}
	}

	//Same in the vertical direction
	if (downestPlayer[0]->position.y - uppestPlayer[0]->position.y > 176){
		
		bool rightToBorder = false;
		bool leftToBorder = false;

		for (int i = 0; i < uppestPlayer.size(); i++){
			if (!(uppestPlayer[i]->collider->box.y > uppestPlayer[i]->position.y)){
				leftToBorder = true;
				break;
			}
		}

		for (int i = 0; i < downestPlayer.size(); i++){
			if (!(downestPlayer[i]->collider->box.y < downestPlayer[i]->position.y)){
				rightToBorder = true;
				break;
			}
		}

		if (leftToBorder && rightToBorder){
			for (int i = 0; i < uppestPlayer.size(); i++){
				if (!(uppestPlayer[i]->collider->box.y > uppestPlayer[i]->position.y)){
					uppestPlayer[i]->collider->box.y = uppestPlayer[i]->position.y;
					uppestPlayer[i]->collider->box.y = uppestPlayer[i]->position.y;
				}
			}
			for (int i = 0; i < downestPlayer.size(); i++){
				if (!(downestPlayer[i]->collider->box.y < downestPlayer[i]->position.y)){
					downestPlayer[i]->collider->box.y = downestPlayer[i]->position.y;
					downestPlayer[i]->collider->box.y = downestPlayer[i]->position.y;
					break;
				}
			}
		}
	}

	//Camera position

	//Position is 2*(16 + 88) - ( x * 2) - ( y - x )
	//Where the first numbers are the left interface border and half the game screen * 2,
	//x is the leftmost/uppest player and y is the rightest/downest player
	App->renderer->camera.x = 208 - leftmostPlayer[0]->position.x - rightestPlayer[0]->position.x;
	App->renderer->camera.y = 208 - uppestPlayer[0]->position.y - downestPlayer[0]->position.y;

	//Left border = ((16 - 4) * 2), as one brick in the wall is not visible 
	if (App->renderer->camera.x > 24)
		App->renderer->camera.x = 24;

	//Right border = - (mapWidth * tileSize) - (rightInterfaceSize + brick)
	if (App->renderer->camera.x < -(App->map->width * 16) - 112 + 8)
		App->renderer->camera.x = -(App->map->width * 16) - 112 + 8;

	//Up border = 5, the interface
	if (App->renderer->camera.y > 5)
		App->renderer->camera.y = 5;

	//Down border = - (mapHeight * tileSize) - (downInterfaceSize + brick)
	if (App->renderer->camera.y < -(App->map->height * 16) - 134 + 8)
		App->renderer->camera.y = -(App->map->height * 16) - 134 + 8;
}