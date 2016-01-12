#ifndef __GUISCENE_INTRO_H__
#define __GUISCENE_INTRO_H__

#include "Globals.h"
#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "Point.h"

struct SDL_Texture;

enum STATE {
	IMAGE_1 = 0, IMAGE_2, IMAGE_3, IMAGE_4, IMAGE_5, GO_TO_PLAYER_SELECT
};

class GUIScene_Intro : public Module {

public:

	GUIScene_Intro();
	~GUIScene_Intro();

	bool Start();
	bool CleanUp();
	update_status Update();
	void onTimeFunction(TimeFunction* timeFunction);
	void nextState();

private:

	//When each part started
	Uint32 partStarted = -1;

	STATE state;

	Uint8 fadeToBlackAlpha = 255;

	SDL_Texture* intro1_image_graphics = nullptr;
	SDL_Texture* intro2_background_graphics = nullptr;
	SDL_Texture* intro2_demon_graphics = nullptr;
	SDL_Texture* intro2_title_graphics = nullptr;
	SDL_Texture* intro2_II_graphics = nullptr;
	SDL_Texture* intro3_image_graphics = nullptr;
	SDL_Texture* intro4_image_graphics = nullptr;
	SDL_Texture* intro5_image_graphics = nullptr;

	SDL_Rect intro1_image;
	SDL_Rect intro2_background;
	SDL_Rect intro2_demon;
	SDL_Rect intro2_title;
	SDL_Rect intro2_II;
	SDL_Rect intro3_image;
	SDL_Rect intro4_image;
	SDL_Rect intro5_image;

	fPoint intro2_title_position;
	fPoint intro2_II_position;

};

#endif // __GUISCENE_INTRO_H__