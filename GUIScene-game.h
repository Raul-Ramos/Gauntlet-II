#ifndef __GUISCENE_GAME_H__
#define __GUISCENE_GAME_H__

#include "Globals.h"
#include "Module.h"
#include "Gauntlet_Colors.h"
#include "Characters.h"
#include "Point.h"
#include <vector>

struct SDL_Texture;

enum GUI_STATE {
	GUI_STATE_PLAYER_SELECT,
	GUI_STATE_GAME
};

class GUIScene_Game : public Module {

public:

	GUIScene_Game();
	~GUIScene_Game();

	bool Start();
	bool CleanUp();
	update_status Update();

	GUI_STATE state;

private:

	SDL_Texture* playerSelectGraphics = nullptr;
	SDL_Texture* borderGraphics = nullptr;
	SDL_Texture* interfaceGraphics = nullptr;

	SDL_Rect playerSelect;
	SDL_Rect border;

	SDL_Rect level;
	SDL_Rect selectHero;
	SDL_Rect key;

	SDL_Rect numbers[COLOR_WHITE + 1][10];
	SDL_Rect characters[COLOR_GREEN + 1][CHARACTER_ELF + 1];
	SDL_Rect ScoreHealth[COLOR_GREEN + 1];

	//Prints a number x in the screen,
	//position being the leftmost number
	void printNumber(const int x, const iPoint position, const Gauntlet_Colors color);
	//Gets the digits of a number
	void getDigits(const int x, std::vector<int>* digits);

};

#endif // __GUISCENE_GAME_H__