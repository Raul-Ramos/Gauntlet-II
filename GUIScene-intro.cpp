#include "Globals.h"
#include "GUIScene-intro.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTimeFunctions.h"
#include "SDL\include\SDL_render.h"

#include "ModuleGUI.h"
#include "GUIScene-game.h"

GUIScene_Intro::GUIScene_Intro(){
	
	//Stores the intro background
	intro1_image = { 0, 0, 320, 200 };
	intro2_background = { 0, 0, 320, 200 };
	intro2_demon = { 0, 0, 85, 129 };
	intro2_title = { 0, 0, 320, 60 };
	intro2_II = { 0, 0, 48, 34 };
	intro3_image = { 0, 0, 320, 200 };
	intro4_image = { 0, 0, 320, 200 };
	intro5_image = { 0, 0, 216, 200 };

	intro2_title_position = { 0, 200 };
	intro2_II_position = { 224, 200 };

	state = IMAGE_1;

}

GUIScene_Intro::~GUIScene_Intro(){
}

bool GUIScene_Intro::Start(){

	LOG("Loading intro");
	intro1_image_graphics = App->textures->Load("graphics//intro_1.png");
	intro2_background_graphics = App->textures->Load("graphics//intro_2_background.png");
	intro2_demon_graphics = App->textures->Load("graphics//intro_2_demon.png");
	intro2_title_graphics = App->textures->Load("graphics//gauntlet.png");
	intro2_II_graphics = App->textures->Load("graphics//II.png");
	intro3_image_graphics = App->textures->Load("graphics//intro_3.png");
	intro4_image_graphics = App->textures->Load("graphics//intro_4.png");
	intro5_image_graphics = App->textures->Load("graphics//player_select.png");
	return true;
}

bool GUIScene_Intro::CleanUp(){

	LOG("UnLoading intro");
	App->textures->Unload(intro1_image_graphics);
	App->textures->Unload(intro2_background_graphics);
	App->textures->Unload(intro2_demon_graphics);
	App->textures->Unload(intro2_title_graphics);
	App->textures->Unload(intro2_II_graphics);
	App->textures->Unload(intro3_image_graphics);
	App->textures->Unload(intro4_image_graphics);
	App->textures->Unload(intro5_image_graphics);
	return true;
}

update_status GUIScene_Intro::Update(){

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT){
		state = GO_TO_PLAYER_SELECT;
	}

	switch (state)
	{
	//The arcade
	case IMAGE_1:
		App->renderer->Blit(intro1_image_graphics, 0, 0, &intro1_image, 1.0F);

		//Do this upon entering state
		if (partStarted == -1){
			App->timeFunctions->AddTimeFunction(1000, this, "FadeIn");
			App->timeFunctions->AddTimeFunction(6000, this, "FadeOut");
			partStarted = SDL_GetTicks();
		}
		break;

	//The heroes, demon and title
	case IMAGE_2:
	{
		iPoint demonPos = { intro2_background.w - intro2_demon.w, intro2_background.h - intro2_demon.h };
		App->renderer->Blit(intro2_background_graphics, 0, 0, &intro2_background, 1.0F);
		App->renderer->Blit(intro2_title_graphics, intro2_title_position.x, intro2_title_position.y, &intro2_title, 1.0F);
		App->renderer->Blit(intro2_II_graphics, intro2_II_position.x, intro2_II_position.y, &intro2_II, 1.0F);
		App->renderer->Blit(intro2_demon_graphics, demonPos.x, demonPos.y, &intro2_demon, 1.0F);

		///Black border at the bottom
		SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, 255);
		SDL_Rect rect = { 0, App->renderer->camera.y + (200 * 2), intro2_background.w * 2, 40 * 2 };
		SDL_RenderFillRect(App->renderer->renderer, &rect);

		//Do this upon entering state
		if (partStarted == -1){
			App->timeFunctions->AddTimeFunction(2000, this, "startState2");
			partStarted = 0;
		}
		break;
	}

	//First information page
	case IMAGE_3:
		App->renderer->Blit(intro3_image_graphics, 0, 0, &intro3_image, 1.0F);

		//Do this upon entering state
		if (partStarted == -1){
			App->timeFunctions->AddTimeFunction(5000, this, "FadeOut");
			partStarted = SDL_GetTicks();
		}
		break;

	//Second information page
	case IMAGE_4:
		App->renderer->Blit(intro4_image_graphics, 0, 0, &intro4_image, 1.0F);

		//Do this upon entering state
		if (partStarted == -1){
			App->timeFunctions->AddTimeFunction(5000, this, "FadeOut");
			partStarted = SDL_GetTicks();
		}
		break;

	//Fake player slect screen - third information page over second information page players border
	case IMAGE_5:
		App->renderer->Blit(intro4_image_graphics, 0, 0, &intro4_image, 1.0F);
		App->renderer->Blit(intro5_image_graphics, 0, 0, &intro5_image, 1.0F);

		if (partStarted == -1){
			App->timeFunctions->AddTimeFunction(5000, this, "EndIntro");
			partStarted = SDL_GetTicks();
		}
		break;

	//Go to player select screen
	case GO_TO_PLAYER_SELECT:
		App->timeFunctions->CleanUp();
		App->GUI->changeScene(new GUIScene_Game());

	default:
		break;
	}

	///Black screen to fade out - fade in
	SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 0, fadeToBlackAlpha);
	SDL_Rect osc = { 0, 0, intro2_background.w * 2, intro2_background.h * 2 };
	SDL_RenderFillRect(App->renderer->renderer, &osc);
	SDL_RenderPresent(App->renderer->renderer);

	return UPDATE_CONTINUE;
}


void GUIScene_Intro::onTimeFunction(TimeFunction* timeFunction){

	if (timeFunction->message == "FadeOut"){
		App->audio->PlayMusic("");
		App->timeFunctions->AddTimeFunction(40, this, "FadeOutInProcess");
		timeFunction->toDelete = true;
	}
	else if (timeFunction->message == "FadeOutInProcess"){
		fadeToBlackAlpha += 15;
		if (fadeToBlackAlpha == 255){
			App->timeFunctions->AddTimeFunction(700, this, "FadeIn");
			nextState();
			timeFunction->toDelete = true;
		}
		
	}
	else if (timeFunction->message == "FadeIn"){
		App->timeFunctions->AddTimeFunction(40, this, "FadeInInProcess");
		timeFunction->toDelete = true;
	}
	else if (timeFunction->message == "FadeInInProcess"){
		fadeToBlackAlpha -= 15;
		if (fadeToBlackAlpha == 0)
			timeFunction->toDelete = true;
	}
	else if (timeFunction->message == "startState2"){
		int musicDelay = 1500;
		App->timeFunctions->AddTimeFunction(musicDelay, this, "MusicStart");
		App->timeFunctions->AddTimeFunction(40, this, "MoveTitleUp");
		App->timeFunctions->AddTimeFunction(25000 + musicDelay, this, "FadeOut");
		partStarted = SDL_GetTicks();
		timeFunction->toDelete = true;
	}
	else if(timeFunction->message == "MusicStart"){
		App->audio->PlayMusic("sound//main_theme.ogg", 0.0f);
		timeFunction->toDelete = true;
	}
	else if (timeFunction->message == "MoveTitleUp"){
		Uint32 now = SDL_GetTicks() - partStarted;
		intro2_title_position.y = 200.0F *  (1 - ((float)now / 4000.0f));
		if (intro2_title_position.y <= 0){
			intro2_title_position.y = 0;
			App->renderer->camera.y -= 16;
			App->timeFunctions->AddTimeFunction(200, this, "CorrectCamera");
			App->timeFunctions->AddTimeFunction(timeFunction->duration, this, "MoveIIUp");
			timeFunction->toDelete = true;
		}
	}
	else if (timeFunction->message == "MoveIIUp"){
		Uint32 now = SDL_GetTicks() - (partStarted + 4000);
		intro2_II_position.y = 200.0F *  (1 - ((float)now / 4000.0f));
		if (intro2_II_position.y <= 38){
			intro2_II_position.y = 38;
			App->renderer->camera.y -= 16;
			App->timeFunctions->AddTimeFunction(200, this, "CorrectCamera");
			timeFunction->toDelete = true;
		}
	}
	else if (timeFunction->message == "CorrectCamera"){
		App->renderer->camera.y = 0;
		timeFunction->toDelete = true;
	}
	else if (timeFunction->message == "EndIntro"){
		nextState();
		timeFunction->toDelete = true;
	}

}

void GUIScene_Intro::nextState(){
	state = static_cast<STATE>(static_cast<int>(state)+1);
	partStarted = -1;
}