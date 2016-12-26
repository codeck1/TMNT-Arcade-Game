#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	void OnCollision(Collider* c1, Collider* c2);
	void onTimePassed(int time);
	bool CleanUp();
	


public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation idleLeft;
	Animation up;
	Animation jump;
	Animation upLeft;
	Animation right;
	Animation left;
	Animation down;
	iPoint position;
	bool destroyed = false;
	Collider* collider;
	bool faceRight = true;
	bool inAir = false;

};

#endif