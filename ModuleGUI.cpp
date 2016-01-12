#include "ModuleGUI.h"
#include "GUIScene-intro.h"

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
	return scene->Update();
}

void ModuleGUI::onTimeFunction(TimeFunction* timeFunction){
	scene->onTimeFunction(timeFunction);
}

void ModuleGUI::changeScene(Module* change_scene){
	newScene = change_scene;
}