#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Globals.h"
#include "Application.h"
#include "fRect.h"

//Collider identification types
//None is the last one so you can get
//the valid types with None
enum COLLIDER_TYPE {
	COLLIDER_WALL = 0,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_PROJECTILE,
	COLLIDER_COLLECTIBLE,

	COLLIDER_NONE
};

//Class used
struct Collider
{
	~Collider(){
	};

	COLLIDER_TYPE type;
	fRect box;
	Module* father;
	bool toDelete = false;

	Collider(COLLIDER_TYPE type, fRect box, Module* father) :
		type(type), box(box), father(father) {}

	void setPos(const int x, const int y) { box.x = x; box.y = y; }
	void setLong(const int w, const int h) { box.w = w; box.h = h; }

	//Cheks if there are not gasp in horizontal and vertical between two
	//non-rotated rectangles
	bool CheckCollision(fRect box2) const {
		return (box.x < box2.x + box2.w &&
			box.x + box.w > box2.x &&			
			box.y < box2.y + box2.h &&			
			box.h + box.y > box2.y);
	}
};

#endif // __COLLIDER_H__