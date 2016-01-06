#include "ModuleEnemies.h"
#include "ModuleEnemy.h"
#include "EnemySpawn.h"
#include "ModuleTextures.h"

using namespace std;

ModuleEnemies::ModuleEnemies(){

}

ModuleEnemies::~ModuleEnemies(){

	for (vector<SpawnPoint*>::iterator it = spawnPoints.begin(); it != spawnPoints.end(); ++it)
		RELEASE(*it);
	spawnPoints.clear();

	for (vector<ModuleEnemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		RELEASE(*it);
	enemies.clear();
}

bool ModuleEnemies::Start(){

	for (int i = 0; i < spawnPoints.size(); i++)
		spawnPoints[i]->Start();

	return true;
}

update_status ModuleEnemies::PreUpdate(){
	for (int i = 0; i < enemies.size(); i++){
		if (enemies[i]->life > 0) enemies[i]->PreUpdate();
	}

	for (int i = 0; i < spawnPoints.size(); i++){
		if (spawnPoints[i]->life > 0) spawnPoints[i]->PreUpdate();
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
			enemies[i]->CleanUp();
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
		}
	}

	i = 0;
	while (i < spawnPoints.size()) {
		if (spawnPoints[i]->life > 0){
			spawnPoints[i]->Update();
			++i;
		}
		else {
			spawnPoints[i]->CleanUp();
			delete spawnPoints[i];
			spawnPoints.erase(spawnPoints.begin() + i);
		}
	}
	return UPDATE_CONTINUE;
}
update_status ModuleEnemies::PostUpdate(){

	for (int i = 0; i < enemies.size(); i++){
		if (enemies[i]->life > 0) enemies[i]->PostUpdate();
	}

	for (int i = 0; i < spawnPoints.size(); i++){
		if (spawnPoints[i]->life > 0) spawnPoints[i]->PostUpdate();
	}

	return UPDATE_CONTINUE;
}

bool ModuleEnemies::CleanUp(){
	for (int i = 0; i < enemies.size(); i++)
		enemies[i]->CleanUp();

	for (int i = 0; i < spawnPoints.size(); i++)
		spawnPoints[i]->CleanUp();
	return true;
}

SpawnPoint* ModuleEnemies::AddSpawnPoint(const enemyType type, const fPoint position){

	SpawnPoint* spawnPoint = new SpawnPoint(type, position);

	int dim = 16;

	switch (type)
	{
	case ENEMYTYPE_GHOST:
		spawnPoint->animation.frames.push_back({ (2 * 18) + 1, (6 * 18) + 1, dim, dim });
		spawnPoint->animation.frames.push_back({ (3 * 18) + 1, (6 * 18) + 1, dim, dim });
		spawnPoint->animation.frames.push_back({ (4 * 18) + 1, (6 * 18) + 1, dim, dim });
		spawnPoint->animation.speed = 0.0f;
		spawnPoint->life = 12 * 3;
		break;
	case ENEMYTYPE_GRUNT:
	case ENEMYTYPE_DEMON: {
		int i = rand() % 3;
		spawnPoint->animation.frames.push_back({ ((i + 5) * 18) + 1, (6 * 18) + 1, dim, dim });
		spawnPoint->animation.speed = 0.0f;
		spawnPoint->life = 12 * 3;
		break;
	}
	case ENEMYTYPE_NONE:
	default:
		break;
	}
	
	spawnPoints.push_back(spawnPoint);

	return spawnPoint;
}

ModuleEnemy* ModuleEnemies::AddEnemy(const fPoint position){
	ModuleEnemy* enemy = new ModuleEnemy(position);
	enemies.push_back(enemy);
	enemy->Start();
	return enemy;
}