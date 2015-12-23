#ifndef __MODULECOLLECTIBLE_H__
#define __MODULECOLLECTIBLE_H__

#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

enum TypeCollectible{
	COLLECTIBLE_TREASURE
};

class ModuleCollectible : public Module
{
public:
	ModuleCollectible(TypeCollectible type);
	~ModuleCollectible();

	update_status Update();
	bool CleanUp();

	SDL_Texture* graphics = nullptr;
	iPoint position;

	Animation animation;
	Collider* collider;

private:
	TypeCollectible type;

};

#endif // __MODULECOLLECTIBLE_H__