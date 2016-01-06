#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayers.h"
#include "ModulePlayer.h"

using namespace std;

ModuleGUI::ModuleGUI(){

	//Stores the border position
	border = { 0, 0, 320, 200 };

	//Stores texts and icons positions
	level = { 8, 24, 39, 7 };
	selectHero = { 47, 24, 87, 7 };
	key = { 0, 24, 8, 9 };

	//Stores the numbers positions
	for (int color = 0; color <= COLOR_WHITE; color++){
		for (int number = 0; number < 10; number++)
			numbers[color][number] = { 182 + (number*7), 0 + (color * 7), 7, 7 };
	}

	//Stores the name positions
	for (int color = 0; color <= COLOR_GREEN; color++){
		characters[color][CHARACTER_WARRIOR] = { 0, color * 6, 32, 6 };
		characters[color][CHARACTER_VALKYRIE] = { 32, color * 6, 38, 6 };
		characters[color][CHARACTER_WIZARD] = { 70, color * 6, 27, 6 };
		characters[color][CHARACTER_ELF] = { 97, color * 6, 14, 6 };
	}

	//Stores the score and health text positions
	for (int color = 0; color <= COLOR_GREEN; color++)
	{
		ScoreHealth[color] = { 111, color * 6, 71, 6 };
	}

}
ModuleGUI::~ModuleGUI(){}

bool ModuleGUI::Start(){
	LOG("Loading interface");

	borderGraphics = App->textures->Load("margin.png");
	interfaceGraphics = App->textures->Load("interface.png");

	return true;
}

bool ModuleGUI::CleanUp(){
	LOG("Unloading interface");

	App->textures->Unload(borderGraphics);
	App->textures->Unload(interfaceGraphics);

	return true;
}

update_status ModuleGUI::Update(){

	//Border
	App->renderer->Blit(borderGraphics, 0, 0, &border, 0.0F);

	//Level
	App->renderer->Blit(interfaceGraphics, 225, 47, &level, 0.0F);
	App->renderer->Blit(interfaceGraphics, 272, 47, &numbers[COLOR_WHITE][1], 0.0F);

	//Characters
	int base = 57;
	ModulePlayer* player;
	colors color;

	for (int i = 0; i < 4; i++){
		color = static_cast<colors>(i);
		player = App->players->players[i];

		App->renderer->Blit(interfaceGraphics, 256, base, &characters[color][player->characterType], 0.0F);
		App->renderer->Blit(interfaceGraphics, 238, base + 8, &ScoreHealth[color], 0.0F);
		printNumber(player->score, { 256, base + 15 }, color);
		printNumber(player->health, { 304, base + 15 }, color);
		
		//If is active, show the keys. If not, the hero select text
		if (player->active){
			for (int i = 0; i < player->numKeys; i++)
				App->renderer->Blit(interfaceGraphics, 216 + (i * 8), base + 23, &key, 0.0F);
		} else {
			App->renderer->Blit(interfaceGraphics, 225, base + 23, &selectHero, 0.0F);
		}
		
		base += 34;
	}

	return UPDATE_CONTINUE;
}

void ModuleGUI::printNumber(int x, iPoint position, colors color){
	vector<int>* digits = new vector<int>();
	getDigits(x, digits);
	for (int i = 0; i < digits->size(); i++)
		App->renderer->Blit(interfaceGraphics, position.x - (7 * (digits->size() - i - 1)), position.y, &numbers[color][digits->at(i)], 0.0F);
	delete digits;
}

void ModuleGUI::getDigits(int x, vector<int>* digits)
{
	if (x >= 10)
		getDigits(x / 10, digits);

	digits->push_back(x % 10);
}