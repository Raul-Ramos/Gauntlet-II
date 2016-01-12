#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;

protected:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f)
	{}

	~Animation(){
		frames.clear();
	}

	virtual SDL_Rect& GetCurrentAnimatedFrame()
	{
		current_frame += speed;
		if(current_frame >= frames.size())
			current_frame = 0.0f;
		return frames[(int)current_frame];
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[(int)current_frame];
	}
};