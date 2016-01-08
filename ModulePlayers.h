#ifndef __MODULEPLAYERS_H__
#define __MODULEPLAYERS_H__

#include "Globals.h"
#include "Module.h"

enum playerSpawnDirection{
	PSDIRECTION_UP = 0, PSDIRECTION_DOWN, PSDIRECTION_LEFT, PSDIRECTION_RIGHT
};

struct ModulePlayer;
struct Collider;

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

	void OnCollision(Collider* c1, Collider* c2);

	ModulePlayer* players[4];
	int playerJoinButton[4];

private:

	//Adjusts the camera given the players position
	void adjustCamera();

	//Used for player joining
	int playerJoining = 0;					//What players is joining. 0 is no one, as the player one is always on.
	Collider* playerJoinSpawnPoints[4];		//Temporal Spawn Collisions to check where the new player can join
	void joinPlayer(const int i);					//Common routines on all joinings
};

#endif // __MODULEPLAYERS_H__