#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Directions.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "SoundLibrary.h"
#include "SDL/include/SDL.h"

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
		if (particles[i]->duration != -1 && particles[i]->begun + particles[i]->duration <= time){
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

Projectile* ModuleParticles::CreateProjectile(const projectile_type type, const fPoint position, const Facing facing, SDL_Texture* graphics, ModulePlayer* sender){

	//Creates the projectile
	Projectile* projectile = new Projectile({ position.x, position.y }, graphics);

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
	projectile->speed = { dirH * 4, dirV * 4 };

	//Does the throw animation
	projectile->animation = new Animation();
	int dim = 18 - 2;						//Dimension. Tilesize - (border*2). Both width and height.

	switch (type)
	{
	case PROJECTILE_WARRIOR: {

			int yValue = (18 * 9) + 1;				//(Tilesize * line) + border
			for (int i = 0; i < 8; i++){
				//Xvalue = Tilesize * ( column + animationStep ) + border
				projectile->animation->frames.push_back({ 18 * (24 + i) + 1, yValue, dim, dim });
			}
			projectile->animation->speed = 0.5f;
			App->soundLib->playSound(SOUND_WARRIOR_ATTACK);

			break;
		}

	case PROJECTILE_VALKYRIE: {

			int firstImage = 10 * 44 + 17;
			projectile->animation->frames.push_back({ (18 * ((firstImage + facing) % 44)) + 1, (18 * ((firstImage + facing) / 44)) + 1, dim, dim });
			projectile->animation->speed = 0.0f;
			App->soundLib->playSound(SOUND_VALKYRIE_ATTACK);

			break;
		}

	case PROJECTILE_WIZARD:{

			int firstImage = 11 * 44 + 10;
			projectile->animation->frames.push_back({ (18 * ((firstImage + facing) % 44)) + 1, (18 * ((firstImage + facing) / 44)) + 1, dim, dim });
			projectile->animation->speed = 0.0f;
			App->soundLib->playSound(SOUND_WIZARD_ATTACK);

			break;
	}

	case PROJECTILE_ELF: {

			int firstImage = 12 * 44 + 3;
			projectile->animation->frames.push_back({ (18 * ((firstImage + facing) % 44)) + 1, (18 * ((firstImage + facing) / 44)) + 1, dim, dim });
			projectile->animation->speed = 0.0f;
			App->soundLib->playSound(SOUND_ELF_ATTACK);

			break;
	}

	default:
		break;
	}

	projectile->damage = 12 * 3;

	if (sender != nullptr){
		projectile->sender = sender;
	}

	return projectile;
}