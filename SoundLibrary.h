#ifndef __SOUNDLIBRARY_H__
#define __SOUNDLIBRARY_H__

#include "Globals.h"
#include "Module.h"
#include <vector>

enum sound {
	//HEROES
	SOUND_WARRIOR_NAME, SOUND_WARRIOR_ATTACK,				
	SOUND_VALKYRIE_NAME, SOUND_VALKYRIE_ATTACK,
	SOUND_WIZARD_NAME, SOUND_WIZARD_ATTACK,
	SOUND_ELF_NAME, SOUND_ELF_ATTACK,
	//ENEMIES
	SOUND_GHOST_ATTACK,
	//OBJECTS
	SOUND_TAKE_KEY, SOUND_TAKE_TREASURE, SOUND_TAKE_FOOD,
	//ENVIROMENT
	SOUND_OPEN_DOOR, SOUND_FLOOR_TRIGGER,
	//NARRATOR SOUNDS
	SOUND_NARRATOR_WELCOME, SOUND_NARRATOR_DYING,
	SOUND_NARRATOR_LOW_LIFE, SOUND_NARRATOR_LOW_LIFE_2,
	//COLORS
	SOUND_NARRATOR_RED, SOUND_NARRATOR_BLUE,
	SOUND_NARRATOR_YELLOW, SOUND_NARRATOR_GREEN,
	//INTERFACE SOUNDS
	SOUND_LOW_HEALTH, SOUND_GAME_OVER,
	//---
	NO_SOUND
};

struct SoundSuccesion;

//Module that contains all the sounds references so they're not loaded twice
//It also plays the sucesive sounds
class SoundLibrary : public Module {

public:
	SoundLibrary();
	~SoundLibrary();

	void playSound(sound id);

	update_status Update();
	void AddSoundSuccesion(SoundSuccesion* soundSuccesion);

private:
	int sounds[NO_SOUND];
	std::vector<SoundSuccesion*> soundSuccessions;
};

#endif //__SOUNDLIBRARY_H__