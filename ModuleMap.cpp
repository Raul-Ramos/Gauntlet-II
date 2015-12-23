#include "ModuleMap.h"
#include "tinyxml2/tinyxml2.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include <sstream>

using namespace tinyxml2;
using namespace std;

#define WALL 575

ModuleMap::ModuleMap(){

	//Creates the walls presets
	int dim = 18 - 2;			//Dimension. Tilesize - (border*2). Both width and height.

	//X = (Tilesize * col) + border
	//Y = (Tilesize * line) + border
	wallPreset[WALLDIRECTION_NONE] = { (36 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP] = { (37 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_RIGHT] = { (38 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_RIGHT] = { (39 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_DOWN] = { (40 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_DOWN] = { (41 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_DOWN_RIGHT] = { (42 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_DOWN_RIGHT] = { (43 * 18) + 1, (12 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_LEFT] = { (0 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_LEFT] = { (1 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_LEFT_RIGHT] = { (2 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_LEFT_RIGHT] = { (3 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_DOWN_LEFT] = { (4 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_DOWN_LEFT] = { (5 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_DOWN_LEFT_RIGHT] = { (6 * 18) + 1, (13 * 18) + 1, dim, dim };
	wallPreset[WALLDIRECTION_UP_DOWN_LEFT_RIGHT] = { (7 * 18) + 1, (13 * 18) + 1, dim, dim };
}

ModuleMap::~ModuleMap(){
}

bool ModuleMap::Start(){

	///////////////////// XML READING /////////////////////
	//Loads the level
	XMLDocument doc;
	doc.LoadFile("../Ogmo/Level_1.oel");

	//Reads width and height
	width = stoi(doc.FirstChildElement("level")->Attribute("width")) / 16;
	height = stoi(doc.FirstChildElement("level")->Attribute("height")) / 16;

	//Loads the World values and separates them by commas
	vector<int> worldValues;
	stringstream ss(doc.FirstChildElement("level")->FirstChildElement("World")->GetText());

	int i;
	while (ss >> i)
	{
		worldValues.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}

	///////////////////// MAP DRAWING /////////////////////
	//Loads the texture
	graphics = App->textures->Load("gauntlet2.png");

	int x, y, direction;
	for (i = 0; i < worldValues.size(); i++){

		//If its a wall
		if (worldValues[i] == WALL){

			Wall* wall = new Wall();

			//Gets X and Y
			x = (i % width);
			y = (i / width);
			direction = 0;

			//Checks if there is another wall or border in every direction
			if (y == 0 || worldValues[((y - 1)*width) + x] == WALL)
				direction += WALLDIRECTION_UP;

			if (y + 1 == height || worldValues[((y + 1)*width) + x] == WALL)
				direction += WALLDIRECTION_DOWN;

			if (x == 0 || worldValues[(y*width) + x - 1] == WALL)
				direction += WALLDIRECTION_LEFT;

			if (x + 1 == width || worldValues[(y*width) + x + 1] == 575)
				direction += WALLDIRECTION_RIGHT;


			wall->x = x * 16;
			wall->y = y * 16;
			wall->direction = static_cast<WallDirection>(direction);

			walls.push_back(wall);
		}
	}


	return true;
}

update_status ModuleMap::Update(){
	
	for (int i = 0; i < walls.size(); i++)
	{
		App->renderer->Blit(graphics, walls[i]->x, walls[i]->y, &wallPreset[walls[i]->direction], 1.0f);
	}

	return UPDATE_CONTINUE;
}