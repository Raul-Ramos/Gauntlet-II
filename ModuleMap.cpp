#include "ModuleMap.h"
#include "tinyxml2/tinyxml2.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include <sstream>

using namespace tinyxml2;
using namespace std;

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

	floorPreset = { (16 * 18) + 1, (12 * 18) + 1, dim, dim };
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

		//Gets X and Y
		x = (i % width);
		y = (i / width);
		
		if (x == 1 && y == 1){
			LOG("%i", worldValues[i]);
		}

		switch (worldValues[i])
		{
		case WALL: {
			Wall* wall = new Wall();
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

			//Creates the wall
			wall->position.x = x * 16;
			wall->position.y = y * 16;
			wall->direction = static_cast<WallDirection>(direction);

			walls.push_back(wall);

			break; }

		case FLOOR:
			floor.push_back(new iPoint(x * 16, y * 16));
			break;

		case CHEST:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_TREASURE, {x*16,y*16}));
			break;


		default:
			break;
		}
	}

	return true;
}

update_status ModuleMap::Update(){
	
	for (int i = 0; i < walls.size(); i++)
	{
		App->renderer->Blit(graphics, walls[i]->position.x, walls[i]->position.y, &wallPreset[walls[i]->direction], 1.0f);
	}

	for (int i = 0; i < floor.size(); i++){
		App->renderer->Blit(graphics, floor[i]->x, floor[i]->y, &floorPreset, 1.0f);
	}

	for (int i = 0; i < collectibles.size(); i++)
	{
		collectibles[i]->Update();
	}

	return UPDATE_CONTINUE;
}

ModuleCollectible* ModuleMap::CreateCollectible(TypeCollectible type, iPoint position){
	ModuleCollectible* collectible = new ModuleCollectible(type);

	Animation animation;
	int dim = 18 - 2;

	if (type == COLLECTIBLE_TREASURE){
		animation.frames.push_back({ (21 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation.frames.push_back({ (22 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation.frames.push_back({ (23 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation.speed = 0.1f;
	}

	collectible->animation = animation;
	collectible->position = position;
	collectible->graphics = this->graphics;

	return collectible;
}