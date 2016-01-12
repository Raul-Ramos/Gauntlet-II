#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayers.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleTimeFunctions.h"
#include "ModuleEnemies.h"
#include "ModuleMap.h"
#include "ModuleGUI.h"
#include "SoundLibrary.h"

#include "GUIScene-game.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(map = new ModuleMap(false));
	modules.push_back(timeFunctions = new ModuleTimeFunctions());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(collisions = new ModuleCollisions());

	// Game Modules
	modules.push_back(enemies = new ModuleEnemies(false));
	modules.push_back(players = new ModulePlayers(false));
	modules.push_back(GUI = new ModuleGUI());
	modules.push_back(fade = new ModuleFadeToBlack());
	modules.push_back(soundLib = new SoundLibrary());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

		if (restart) Restart();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

//Restarts the game. Temporal as there are not more levels.
void Application::Restart(){

	map->CleanUp();
	particles->CleanUp();
	collisions->CleanUp();
	enemies->CleanUp();
	players->CleanUp();

	modules.remove(map);
	modules.remove(particles);
	modules.remove(collisions);
	modules.remove(enemies);
	modules.remove(players);
	
	modules.remove(GUI);
	modules.remove(fade);
	modules.remove(soundLib);

	RELEASE(map);
	RELEASE(particles);
	RELEASE(collisions);
	RELEASE(enemies);
	RELEASE(players);

	modules.push_back(map = new ModuleMap(false));
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(collisions = new ModuleCollisions());
	modules.push_back(enemies = new ModuleEnemies(false));
	modules.push_back(players = new ModulePlayers(false));

	modules.push_back(GUI);
	modules.push_back(fade);
	modules.push_back(soundLib);

	particles->Start();
	collisions->Start();
	players->Start();

	GUI->changeScene(new GUIScene_Game());

	restart = false;
}

