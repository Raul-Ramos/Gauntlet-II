#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayers;
class ModuleEnemies;
class ModuleCollisions;
class ModuleParticles;
class ModuleMap;
class ModuleGUI;
class SoundLibrary;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleParticles* particles;
	ModuleCollisions* collisions;
	ModuleMap* map;
	ModulePlayers* players;
	ModuleEnemies* enemies;
	ModuleGUI* GUI;
	ModuleFadeToBlack* fade;
	SoundLibrary* soundLib;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif // __APPLICATION_CPP__