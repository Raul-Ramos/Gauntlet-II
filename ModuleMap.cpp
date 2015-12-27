#include "ModuleMap.h"
#include "tinyxml2/tinyxml2.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "Animation.h"
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

	doorPreset[WALLDIRECTION_NONE] = { (39 * 18) + 1, (6 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP] = { (40 * 18) + 1, (6 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_RIGHT] = { (41 * 18) + 1, (6 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_RIGHT] = { (42 * 18) + 1, (6 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_DOWN] = { (43 * 18) + 1, (6 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_DOWN] = { (0 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_DOWN_RIGHT] = { (1 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_DOWN_RIGHT] = { (2 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_LEFT] = { (3 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_LEFT] = { (4 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_LEFT_RIGHT] = { (5 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_LEFT_RIGHT] = { (6 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_DOWN_LEFT] = { (7 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_DOWN_LEFT] = { (8 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_DOWN_LEFT_RIGHT] = { (9 * 18) + 1, (7 * 18) + 1, dim, dim };
	doorPreset[WALLDIRECTION_UP_DOWN_LEFT_RIGHT] = { (10 * 18) + 1, (7 * 18) + 1, dim, dim };

}

ModuleMap::~ModuleMap(){

	for (vector<Wall*>::iterator it = walls.begin(); it != walls.end(); ++it)
		RELEASE(*it);
	walls.clear();

	for (vector<iPoint*>::iterator it = floor.begin(); it != floor.end(); ++it)
		RELEASE(*it);
	floor.clear();

	for (vector<Door*>::iterator it = doors.begin(); it != doors.end(); ++it)
		RELEASE(*it);
	doors.clear();

	for (vector<ModuleCollectible*>::iterator it = collectibles.begin(); it != collectibles.end(); ++it)
		RELEASE(*it);
	collectibles.clear();

}

bool ModuleMap::CleanUp(){
	for (int i = 0; i < collectibles.size(); i++)
	{
		collectibles[i]->CleanUp();
	}

	return true;
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
	int doorGroupId = 0;

	for (i = 0; i < worldValues.size(); i++){

		//Gets X and Y
		x = (i % width);
		y = (i / width);

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

			if (x + 1 == width || worldValues[(y*width) + x + 1] == WALL)
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

		case DOOR: {

			//Searches for the door other positions and
			//gets them under the same group ID
			vector<iPoint> openPositions = {{x,y}};
			iPoint it; //Iterator
			int newPosition;

			while (openPositions.size() > 0){

				//Operates on the last open door and closes it
				//so it won't be read either by other open
				//positions nor by the resumed map drawing
				it = openPositions.back();
				openPositions.pop_back();
				worldValues[(it.y*width) + it.x] = CLOSED_DOOR;

				Door* door = new Door();
				direction = 0;

				//Checks if there is another door or border in every direction
				//if the position is not a border the position is opened for inspection
				newPosition = ((it.y - 1)*width) + it.x;
				if (it.y == 0 || worldValues[newPosition] == DOOR || worldValues[newPosition] == CLOSED_DOOR){
					direction += WALLDIRECTION_UP;
					if (it.y != 0 && worldValues[newPosition] == DOOR)
						openPositions.push_back({ it.x, it.y - 1 });
				}

				newPosition = ((it.y + 1)*width) + it.x;
				if (it.y + 1 == height || worldValues[newPosition] == DOOR || worldValues[newPosition] == CLOSED_DOOR){
					direction += WALLDIRECTION_DOWN;
					if (it.y + 1 != height && worldValues[newPosition] == DOOR)
						openPositions.push_back({ it.x, it.y + 1 });
				}

				newPosition = (y*width) + it.x - 1;
				if (it.x == 0 || worldValues[newPosition] == DOOR || worldValues[newPosition] == CLOSED_DOOR) {
					direction += WALLDIRECTION_LEFT;
					if (it.x != 0 && worldValues[newPosition] == DOOR)
						openPositions.push_back({ it.x - 1, it.y });
				}

				newPosition = (y*width) + it.x + 1;
				if (it.x + 1 == width || worldValues[newPosition] == DOOR || worldValues[newPosition] == CLOSED_DOOR) {
					direction += WALLDIRECTION_RIGHT;
					if (it.x + 1 != width && worldValues[newPosition] == DOOR)
						openPositions.push_back({ it.x + 1, it.y });
				}

				door->position.x = it.x * 16;
				door->position.y = it.y * 16;
				door->direction = static_cast<WallDirection>(direction);
				door->groupID = doorGroupId;

				doors.push_back(door);
				
			}

			//Increases the door group id for the next door
			doorGroupId++;
			
			break;
			}

		case CHEST:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_TREASURE, {x*16,y*16}));
			break;

		case MEAT:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_MEAT, { x * 16, y * 16 }));
			break;

		case DRINK:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_DRINK, { x * 16, y * 16 }));
			break;

		case KEY:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_KEY, { x * 16, y * 16 }));
			break;

		case EXIT:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_EXIT, { x * 16, y * 16 }));
			break;

		case EXIT_TO_6:
			collectibles.push_back(CreateCollectible(COLLECTIBLE_EXIT_TO_6, { x * 16, y * 16 }));
			break;

		default:
			break;
		}
	}

	return true;
}

update_status ModuleMap::Update(){
	
	//Renders the walls
	for (int i = 0; i < walls.size(); i++)
	{
		App->renderer->Blit(graphics, walls[i]->position.x, walls[i]->position.y, &wallPreset[walls[i]->direction], 1.0f);
	}

	//Renders the floor
	for (int i = 0; i < floor.size(); i++){
		App->renderer->Blit(graphics, floor[i]->x, floor[i]->y, &floorPreset, 1.0f);
	}

	//Renders the doors
	for (int i = 0; i < doors.size(); i++){
		App->renderer->Blit(graphics, doors[i]->position.x, doors[i]->position.y, &doorPreset[doors[i]->direction], 1.0f);
	}

	//Renders the collectibles
	for (int i = 0; i < collectibles.size(); i++)
	{
		collectibles[i]->Update();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleMap::PostUpdate(){

	//Deletes collectibles marked to it
	std::vector<std::string>::size_type i = 0;
	while (i < collectibles.size()) {
		if (collectibles[i]->toDelete) {

			floor.push_back(new iPoint{ collectibles[i]->position.x, collectibles[i]->position.y });
			delete collectibles[i];
			collectibles.erase(collectibles.begin() + i);

		}
		else {
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

//Creates a collectible
ModuleCollectible* ModuleMap::CreateCollectible(const TypeCollectible type, const iPoint position){
	ModuleCollectible* collectible = new ModuleCollectible(type);

	Animation* animation = new Animation();
	int dim = 18 - 2;

	switch (type)
	{
	case COLLECTIBLE_TREASURE:
		animation->frames.push_back({ (21 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->frames.push_back({ (22 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->frames.push_back({ (23 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.1f;
		break;
	case COLLECTIBLE_MEAT:
		animation->frames.push_back({ (16 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.0f;
		break;
	case COLLECTIBLE_DRINK:
		animation->frames.push_back({ (13 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.0f;
		break;
	case COLLECTIBLE_KEY:
		animation->frames.push_back({ (19 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.0f;
		break;
	case COLLECTIBLE_EXIT:
		animation->frames.push_back({ (0 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.0f;
		break;
	case COLLECTIBLE_EXIT_TO_6:
		animation->frames.push_back({ (1 * 18) + 1, (6 * 18) + 1, dim, dim });
		animation->speed = 0.0f;
		break;
	default:
		break;
	}

	collectible->animation = animation;
	collectible->position = position;
	collectible->graphics = this->graphics;
	collectible->collider = App->collisions->AddCollider(COLLIDER_COLLECTIBLE, { position.x, position.y, 16, 16 }, collectible);

	return collectible;
}