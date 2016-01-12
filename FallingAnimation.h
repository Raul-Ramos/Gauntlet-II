#ifndef __ANIMATION_FALLING_H__
#define __ANIMATION_FALLING_H__

#include "Animation.h"
#include "Application.h"

class FallingAnimation : public Animation {

	//Animation of the player falling in the hole that ends the level upon ending
	SDL_Rect& GetCurrentAnimatedFrame()
	{
		if (current_frame + speed >= frames.size())
			App->restart = true;

		return Animation::GetCurrentAnimatedFrame();
	}

};

#endif // __ANIMATION_FALLING_H__