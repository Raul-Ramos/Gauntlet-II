#include "ModulePlayers.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_scancode.h"

ModulePlayers::ModulePlayers(){

	for (int i = 0; i < 4; i++){
		players[i] = new ModulePlayer();
	}

	players[0]->attackKey = SDL_SCANCODE_Q;
	players[0]->upKey = SDL_SCANCODE_E;
	players[0]->downKey = SDL_SCANCODE_D;
	players[0]->leftKey = SDL_SCANCODE_S;
	players[0]->rightKey = SDL_SCANCODE_F;
	
	players[0]->active = true;
	players[0]->health = 2000;

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

	return UPDATE_CONTINUE;
};

update_status ModulePlayers::Update(){
	for (int i = 0; i < 4; i++)
		players[i]->Update();

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