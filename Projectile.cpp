#include "Projectile.h"
#include "Collider.h"

void Projectile::OnCollision(Collider* col1, Collider* col2){

	if (col2->type == COLLIDER_WALL ||
		col2->type == COLLIDER_COLLECTIBLE ||
		col2->type == COLLIDER_ENEMY){
		MarkForDead();
	}

}