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
#define BONES 266
#define ENEMY_SPAWN 271
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

struct Wall {
	WallDirection direction;
	iPoint position;
};

class ModuleMap : public Module
{
public:
	ModuleMap();
	~ModuleMap();

	bool Start();
	update_status Update();

private:
	//Map properties
	int width;
	int height;

	//Graphics
	SDL_Texture* graphics = nullptr;

	//Elements in the map
	vector<Wall*> walls;
	vector<iPoint*> floor;
	vector<ModuleCollectible*> collectibles;

	//Images prepared for rendering
	SDL_Rect wallPreset[16];
	SDL_Rect floorPreset;

	//Collectible factory
	ModuleCollectible* CreateCollectible(TypeCollectible type, iPoint position);

};

#endif // __MODULEMAP_H__