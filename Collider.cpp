#include "Collider.h"

bool Collider::CheckCollision(SDL_Rect box2) const
{
	return (box.x < box2.x + box2.w &&		// box X before box2 horizontal end
		box.x + box.w > box2.x &&			// box X horizontal end after box2 begin
		box.y < box2.y + box2.h &&			// same in the vertical direction
		box.h + box.y > box2.y);
}