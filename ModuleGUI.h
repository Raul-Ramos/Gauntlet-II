#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Globals.h"
#include "Module.h"
#include "Characters.h"
#include "Point.h"
#include <vector>

enum colors {
	COLOR_RED = 0,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_GREEN,
	COLOR_WHITE
};

struct SDL_Texture;

class ModuleGUI : public Module
{
public:
	ModuleGUI();
	~ModuleGUI();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* borderGraphics = nullptr;
	SDL_Texture* interfaceGraphics = nullptr;

	SDL_Rect border;

	SDL_Rect level;
	SDL_Rect selectHero;
	SDL_Rect key;

	SDL_Rect numbers[COLOR_WHITE + 1][10];
	SDL_Rect characters[COLOR_GREEN + 1][CHARACTER_ELF + 1];
	SDL_Rect ScoreHealth[COLOR_GREEN + 1];

	//Prints a number x in the screen,
	//position being the leftmost number
	void printNumber(int x, iPoint position, colors color);
	//Gets the digits of a number
	void getDigits(int x, std::vector<int>* digits);
};

#endif // __MODULEINTERFACE_H__