#ifndef __MODULEENEMIES_H__
#define __MODULEENEMIES_H__

#include "Globals.h"
#include "Module.h"
#include "Enemies.h"
#include "Point.h"
#include <vector>

struct ModuleEnemy;
struct SpawnPoint;

class ModuleEnemies : public Module
{
public:
	ModuleEnemies(bool start_enabled = true);
	~ModuleEnemies();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	SpawnPoint* AddSpawnPoint(const enemyType type, const fPoint position);
	ModuleEnemy* AddEnemy(const fPoint position);

private:

	std::vector<SpawnPoint*> spawnPoints;
	std::vector<ModuleEnemy*> enemies;

};

#endif // __MODULEENEMIES_H__