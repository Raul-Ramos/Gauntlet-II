#ifndef __MODULEMAP_H__
#define __MODULEMAP_H__

#include "Globals.h"
#include "Module.h"

#include <vector>

using namespace std;

struct SDL_Texture;

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
	int x, y;
};

class ModuleMap : public Module
{
public:
	ModuleMap();
	~ModuleMap();

	bool Start();
	update_status Update();

private:
	SDL_Rect wallPreset[16];
	int width;
	int height;

	vector<Wall*> walls;
	SDL_Texture* graphics = nullptr;
};

#endif // __MODULEMAP_H__