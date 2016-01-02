#include "ModuleEnemies.h"
#include "ModuleEnemy.h";

using namespace std;

ModuleEnemies::ModuleEnemies(){
}

ModuleEnemies::~ModuleEnemies(){
	for (vector<ModuleEnemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		RELEASE(*it);

	enemies.clear();
}

bool ModuleEnemies::Start(){
	AddEnemy();
	return true;
}

update_status ModuleEnemies::PreUpdate(){
	std::vector<std::string>::size_type i = 0;
	while (i < enemies.size()) {
		if (enemies[i]->life > 0){
			enemies[i]->PreUpdate();
			++i;
		}
		else {
			deleteEnemy(i);
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::Update(){
	std::vector<std::string>::size_type i = 0;
	while (i < enemies.size()) {
		if (enemies[i]->life > 0){
			enemies[i]->Update();
			++i;
		}
		else {
			deleteEnemy(i);
		}
	}
	return UPDATE_CONTINUE;
}
update_status ModuleEnemies::PostUpdate(){
	std::vector<std::string>::size_type i = 0;
	while (i < enemies.size()) {
		if (enemies[i]->life > 0){
			enemies[i]->PostUpdate();
			++i;
		}
		else {
			deleteEnemy(i);
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleEnemies::CleanUp(){
	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->CleanUp();
	return true;
}

ModuleEnemy* ModuleEnemies::AddEnemy(){
	ModuleEnemy* enemy = new ModuleEnemy();
	enemies.push_back(enemy);
	enemy->Start();
	return enemy;
}

void ModuleEnemies::deleteEnemy(int i){
	enemies[i]->CleanUp();
	delete enemies[i];
	enemies.erase(enemies.begin() + i);
}