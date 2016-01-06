#ifndef __MODULEPLAYERS_H__
#define __MODULEPLAYERS_H__

#include "Globals.h"
#include "Module.h"

struct ModulePlayer;

class ModulePlayers : public Module
{
public:
	ModulePlayers();
	~ModulePlayers();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	ModulePlayer* players[4];
};

#endif // __MODULEPLAYERS_H__