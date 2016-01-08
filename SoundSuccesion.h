#ifndef _SoundSuccesion_h
#define _SoundSuccesion_h

#include "Application.h"
#include "SoundLibrary.h"
#include "SDL\include\SDL_stdinc.h"
#include "SDL\\include\SDL_timer.h"
#include <vector>

struct SoundSuccesionPart {

	SoundSuccesionPart(){}

	SoundSuccesionPart(sound sound, Uint32 waitTime) : sound(sound), waitTime(waitTime){
	}

	sound sound;
	Uint32 waitTime = 0;
};

//Plays succesive sounds with a given time between them
struct SoundSuccesion {

	SoundSuccesion(){};

	//Plays the succesion, returns true if it has ended
	bool playSuccesion(){

		//Get time
		Uint32 time = SDL_GetTicks();

		//If the wait time is over
		if (index == -1 || time >= begun + succesion[index].waitTime){

			//If there are more sounds
			if (index + 1 < succesion.size()){

				//Play next sound
				++index;
				App->soundLib->playSound(succesion[index].sound);
				begun = time;

			} else {
				//If there are not more sounds, end
				return true;
			}
		}

		return false;
	}

	std::vector<SoundSuccesionPart> succesion;

private:
	Uint32 begun;
	int index = -1;
};


#endif // _SoundSuccesion_h