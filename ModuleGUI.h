#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Globals.h"
#include "Module.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI();
	~ModuleGUI();

	bool Start();
	update_status Update();
	bool CleanUp();
	void onTimeFunction(TimeFunction* timeFunction);

	void changeScene(Module* change_scene);

	Module* scene;

private:

	Module* newScene = nullptr;
};

#endif // __MODULEINTERFACE_H__