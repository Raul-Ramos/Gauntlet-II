#ifndef __MODULEENEMIES_H__
#define __MODULEENEMIES_H__

#include "Globals.h"
#include "Module.h"
#include <vector>

struct ModuleEnemy;

class ModuleEnemies : public Module
{
public:
	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	ModuleEnemy* AddEnemy();

private:

	std::vector<ModuleEnemy*> enemies;
	void deleteEnemy(int i);

};

#endif // __MODULEENEMIES_H__