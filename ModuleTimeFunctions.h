#ifndef __MODULETIMEFUNCTIONS_H__
#define __MODULETIMEFUNCTIONS_H__

#include "Globals.h"
#include "Module.h"
#include "SDL\include\SDL_timer.h"
#include <vector>
#include <string>

struct TimeFunction {

	Uint32 begin = -1;
	Uint32 duration;
	Module* target;
	std::string message;

	bool toDelete = false;

	TimeFunction(const Uint32 duration, Module* target, const std::string message) :
		duration(duration), target(target), message(message){
		begin = SDL_GetTicks();
	}

	void Update(){
		if (begin + duration < SDL_GetTicks()){
			target->onTimeFunction(this);
			begin = SDL_GetTicks();
		}
	}
};

//Contains timers that call the target module and can be recognized by their message
class ModuleTimeFunctions : public Module
{
public:
	ModuleTimeFunctions(){};
	~ModuleTimeFunctions();

	update_status Update();

	TimeFunction* AddTimeFunction(const Uint32 duration, Module* target, const std::string message = "");

private:
	std::vector<TimeFunction*> timeFunctions;
};

#endif // __MODULETIMEFUNCTIONS_H__