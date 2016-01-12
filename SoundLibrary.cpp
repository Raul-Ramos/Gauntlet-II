#include "Globals.h"
#include "Application.h"
#include "SoundLibrary.h"
#include "ModuleAudio.h"
#include "SoundSuccesion.h"

//Module that contains all the sounds references so they're not loaded twice
//It also plays the sucesive sounds
SoundLibrary::SoundLibrary(){
	for (int i = 0; i < NO_SOUND; i++)
		sounds[i] = -1;
}

SoundLibrary::~SoundLibrary(){
	for (std::vector<SoundSuccesion*>::iterator it = soundSuccessions.begin(); it != soundSuccessions.end(); ++it)
		RELEASE(*it);
	soundSuccessions.clear();
}

void SoundLibrary::playSound(sound id){
	if (sounds[id] == -1){
		switch (id)
		{
		case SOUND_WARRIOR_NAME:
			sounds[id] = App->audio->LoadFx("sound/narrator_warrior.wav");
			break;
		case SOUND_WARRIOR_ATTACK:
			sounds[id] = App->audio->LoadFx("sound/attack_warrior.wav");
			break;
		case SOUND_VALKYRIE_NAME:
			sounds[id] = App->audio->LoadFx("sound/narrator_valkyrie.wav");
			break;
		case SOUND_VALKYRIE_ATTACK:
			sounds[id] = App->audio->LoadFx("sound/attack_valkyrie.wav");
			break;
		case SOUND_WIZARD_NAME:
			sounds[id] = App->audio->LoadFx("sound/narrator_wizard.wav");
			break;
		case SOUND_WIZARD_ATTACK:
			sounds[id] = App->audio->LoadFx("sound/attack_wizard.wav");
			break;
		case SOUND_ELF_NAME:
			sounds[id] = App->audio->LoadFx("sound/narrator_elf.wav");
			break;
		case SOUND_ELF_ATTACK:
			sounds[id] = App->audio->LoadFx("sound/attack_elf.wav");
			break;
		case SOUND_GHOST_ATTACK:
			sounds[id] = App->audio->LoadFx("sound/attack_ghost.wav");
			break;
		case SOUND_TAKE_KEY:
			sounds[id] = App->audio->LoadFx("sound/take_key.wav");
			break;
		case SOUND_TAKE_TREASURE:
			sounds[id] = App->audio->LoadFx("sound/take_treasure.wav");
			break;
		case SOUND_TAKE_FOOD:
			sounds[id] = App->audio->LoadFx("sound/take_food.wav");
			break;
		case SOUND_OPEN_DOOR:
			sounds[id] = App->audio->LoadFx("sound/door_open.wav");
			break;
		case SOUND_FLOOR_TRIGGER:
			sounds[id] = App->audio->LoadFx("sound/trigger_active.wav");
			break;
		case SOUND_NARRATOR_WELCOME:
			sounds[id] = App->audio->LoadFx("sound/narrator_welcome.wav");
			break;
		case SOUND_NARRATOR_DYING:
			sounds[id] = App->audio->LoadFx("sound/narrator_dying.wav");
			break;
		case SOUND_NARRATOR_LOW_LIFE:
			sounds[id] = App->audio->LoadFx("sound/narrator_low_life.wav");
			break;
		case SOUND_NARRATOR_LOW_LIFE_2:
			sounds[id] = App->audio->LoadFx("sound/narrator_low_life_2.wav");
			break;
		case SOUND_NARRATOR_RED:
			sounds[id] = App->audio->LoadFx("sound/narrator_red.wav");
			break;
		case SOUND_NARRATOR_BLUE:
			sounds[id] = App->audio->LoadFx("sound/narrator_blue.wav");
			break;
		case SOUND_NARRATOR_YELLOW:
			sounds[id] = App->audio->LoadFx("sound/narrator_yellow.wav");
			break;
		case SOUND_NARRATOR_GREEN:
			sounds[id] = App->audio->LoadFx("sound/narrator_green.wav");
			break;
		case SOUND_LOW_HEALTH:
			sounds[id] = App->audio->LoadFx("sound/dying.wav");
			break;
		case SOUND_GAME_OVER:
			sounds[id] = App->audio->LoadFx("sound/game_over.wav");
			break;
		case SOUND_LEVEL_COMPLETE:
			sounds[id] = App->audio->LoadFx("sound/level_complete.wav");
			break;
		case NO_SOUND:
		default:
			break;
		}
	}

	App->audio->PlayFx(sounds[id]);
}

update_status SoundLibrary::Update(){

	std::vector<std::string>::size_type i = 0;
	while (i < soundSuccessions.size()) {
		bool result = soundSuccessions[i]->playSuccesion();

		if (result){
			//If the sound succesion has finished
			delete soundSuccessions[i];
			soundSuccessions.erase(soundSuccessions.begin() + i);
		}
		else {
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

void SoundLibrary::AddSoundSuccesion(SoundSuccesion* soundSuccesion){
	soundSuccessions.push_back(soundSuccesion);
}