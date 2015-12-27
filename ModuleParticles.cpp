#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "Directions.h"
#include "ModuleCollisions.h"

ModuleParticles::ModuleParticles()
{
}

ModuleParticles::~ModuleParticles(){
	for (vector<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
		RELEASE(*it);

	particles.clear();
}

bool ModuleParticles::CleanUp(){
	for (int i = 0; i < particles.size(); i++)
	{
		App->textures->Unload(particles[i]->graphics);
	}
	return true;
}

update_status ModuleParticles::Update(){
	Uint32 time = SDL_GetTicks();

	for (int i = 0; i < particles.size(); i++){
		if (particles[i]->begun + particles[i]->duration <= time){
			particles[i]->MarkForDead();
		}
		else {
			particles[i]->Update();
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate(){

	//Deletes colliders marked for it
	std::vector<std::string>::size_type i = 0;
	while (i < particles.size()) {
		if (particles[i]->isMarkedForDead()) {
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
		else {
			++i;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticles(Particle* particle){
	particles.push_back(particle);
}

Projectile* ModuleParticles::CreateProjectile(projectile_type type, iPoint position, Facing facing, SDL_Texture* graphics){

	//Creates the projectile
	Projectile* projectile = new Projectile();
	projectile->facing = facing;
	projectile->graphics = graphics;
	projectile->position = { position.x, position.y };
	projectile->collider = App->collisions->AddCollider(COLLIDER_PROJECTILE, { position.x, position.y, 16, 16 }, projectile);
	projectile->duration = 10000;
	projectile->projectileType = type;

	int dirH, dirV;
	switch (facing)
	{
	case UP:		dirH = 0; dirV = -1;  break;
	case UPRIGHT:	dirH = 1; dirV = -1;  break;
	case RIGHT:		dirH = 1; dirV = 0;   break;
	case DOWNRIGHT: dirH = 1; dirV = 1;   break;
	case DOWN:		dirH = 0; dirV = 1;   break;
	case DOWNLEFT:	dirH = -1; dirV = 1;  break;
	case LEFT:		dirH = -1; dirV = 0;  break;
	case UPLEFT:	dirH = -1; dirV = -1; break;
	default:		dirH = 0; dirV = -1;  break;
	}
	projectile->speed = { dirH * 2, dirV * 2 };

	//Does the throw animation
	int dim = 18 - 2;						//Dimension. Tilesize - (border*2). Both width and height.

	switch (type)
	{
	case PROJECTILE_WARRIOR: {
			
			int yValue = (18 * 9) + 1;				//(Tilesize * line) + border

			for (int i = 0; i < 8; i++){
				//Xvalue = Tilesize * ( column + animationStep ) + border
				projectile->animation.frames.push_back({ 18 * (24 + i) + 1, yValue, dim, dim });
			}
			projectile->animation.speed = 0.5f;

		}
	default:
		break;
	}

	return projectile;
}