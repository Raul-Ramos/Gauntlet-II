#ifndef __MODULEMAP_H__
#define __MODULEMAP_H__

#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "ModuleCollectible.h"

#include <vector>

using namespace std;

struct SDL_Texture;

//Elements ID in the XML
#define FLOOR 544
#define WALL 575
#define CHEST 285
#define KEY 283
#define DOOR 313
#define CLOSED_DOOR -2
#define BONES 266
#define GRUNT_SPAWN 271
#define DEMON_SPAWN 269
#define MEAT 280
#define DRINK 277
#define EXIT 264
#define EXIT_TO_6 265

//Direction where a given wall is connected to another
//Follows a binary system
enum WallDirection{
	WALLDIRECTION_NONE,
	WALLDIRECTION_UP,
	WALLDIRECTION_DOWN,
	WALLDIRECTION_UP_DOWN,
	WALLDIRECTION_LEFT,
	WALLDIRECTION_UP_LEFT,
	WALLDIRECTION_DOWN_LEFT,
	WALLDIRECTION_UP_DOWN_LEFT,
	WALLDIRECTION_RIGHT,
	WALLDIRECTION_UP_RIGHT,
	WALLDIRECTION_DOWN_RIGHT,
	WALLDIRECTION_UP_DOWN_RIGHT,
	WALLDIRECTION_LEFT_RIGHT,
	WALLDIRECTION_UP_LEFT_RIGHT,
	WALLDIRECTION_DOWN_LEFT_RIGHT,
	WALLDIRECTION_UP_DOWN_LEFT_RIGHT,
	
};

struct Wall : Module {
	WallDirection direction;
	iPoint position;
	Collider* collider;
};

struct Door : Wall {
	int groupID;
	bool toDelete = false;

	void OnCollision(Collider* col1, Collider* col2);
};

struct ModuleFloorTrigger : Module {
	int Id;
	vector<Wall*> walls;
	vector<ModuleCollectible*> triggers;
};

class ModuleMap : public Module
{
public:
	ModuleMap();
	~ModuleMap();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Elements in the map
	vector<Wall*> walls;
	vector<iPoint*> floor;
	vector<Door*> doors;
	vector<ModuleFloorTrigger*> floorTriggers;
	vector<ModuleCollectible*> collectibles;

private:
	//Map properties
	int width;
	int height;

	//Graphics
	SDL_Texture* graphics = nullptr;

	//Images prepared for rendering
	SDL_Rect wallPreset[16];
	SDL_Rect floorPreset;
	SDL_Rect doorPreset[16];

	//Collectible factory
	ModuleCollectible* CreateCollectible(const TypeCollectible type, const iPoint position);
	ModuleFloorTrigger* FindFloorTriggerGroup(const int id);

};

#endif // __MODULEMAP_H__