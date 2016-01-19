#include "ModuleGUI.h"
#include "GUIScene-intro.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

using namespace std;

ModuleGUI::ModuleGUI(){
	scene = new GUIScene_Intro();
}

ModuleGUI::~ModuleGUI(){
	delete scene;
}

bool ModuleGUI::Start(){
	return scene->Start();
}

bool ModuleGUI::CleanUp(){
	return scene->CleanUp();
}

update_status ModuleGUI::Update(){
	if (newScene != nullptr){
		scene->CleanUp();
		delete scene;
		scene = newScene;
		scene->Start();
		newScene = nullptr;
	}

	//Debug mode GUI
	if (App->debugMode == true){

		//Counts how many enemies there are,
		//enemies are shown in a orange to red graphics
		//depending on the order they operate
		int NumEnemies = 0;

		for (int i = 0; i < App->collisions->colliders.size(); i++)
			if (App->collisions->colliders[i]->type == COLLIDER_ENEMY) ++NumEnemies;

		//Draws the colliders
		fRect box;
		SDL_Color color;
		int countedEnemies = 0;

		for (int i = 0; i < App->collisions->colliders.size(); i++){
			box = App->collisions->colliders[i]->box;

			switch (App->collisions->colliders[i]->type)
			{
			case COLLIDER_WALL:
				color = { 0, 0, 0, 170 };
				break;
			case COLLIDER_PLAYER:
				color = { 0, 255, 255, 170 };
				break;
			case COLLIDER_ENEMY:
				color = { 255, 255 * ((float)(NumEnemies - countedEnemies) / NumEnemies), 0, 170 };
				++countedEnemies;
				break;
			case COLLIDER_PROJECTILE:
				color = { 255, 0, 255, 170 };
				break;
			case COLLIDER_COLLECTIBLE:
				color = { 0, 255, 0, 170 };
				break;
			case COLLIDER_SPAWNPOINT:
				color = { 0, 0, 255, 170 };
				break;
			default:
				color = { 255, 255, 255, 170 };
				break;
			}
			App->renderer->DrawQuad({ box.x, box.y, box.h, box.w }, color.r, color.g, color.b, color.a);
		}
	}
	
	return scene->Update();
}

void ModuleGUI::onTimeFunction(TimeFunction* timeFunction){
	scene->onTimeFunction(timeFunction);
}

void ModuleGUI::changeScene(Module* change_scene){
	newScene = change_scene;
}