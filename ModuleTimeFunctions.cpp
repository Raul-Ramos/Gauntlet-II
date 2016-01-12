#include "ModuleTimeFunctions.h"

ModuleTimeFunctions::~ModuleTimeFunctions(){
	for (std::vector<TimeFunction*>::iterator it = timeFunctions.begin(); it != timeFunctions.end(); ++it)
		RELEASE(*it);
	timeFunctions.clear();
}

update_status ModuleTimeFunctions::Update(){

	std::vector<std::string>::size_type i = 0;

	while (i < timeFunctions.size()) {
		if (timeFunctions[i]->toDelete == true){
			delete timeFunctions[i];
			timeFunctions.erase(timeFunctions.begin() + i);
		}
		else {
			timeFunctions[i]->Update();
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

TimeFunction* ModuleTimeFunctions::AddTimeFunction(const Uint32 duration, Module* target, const std::string message){
	TimeFunction* timeFunction = new TimeFunction(duration, target, message);
	timeFunctions.push_back(timeFunction);
	return timeFunction;
}