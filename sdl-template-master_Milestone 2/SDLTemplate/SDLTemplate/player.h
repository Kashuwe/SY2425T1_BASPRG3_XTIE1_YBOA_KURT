#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
public:
	~Player();
	void start();
	void update();
	void draw();
private:
	int x;
	int y;
	int width;
	int height;
	int speed;
	float reloadTime;
	float currentReloadTime;
	float wingtipReloadTime;
	float currentWingtipReloadTime;
	const int baseSpeed = 3;
	std::vector<Bullet*> bullets;
	Mix_Chunk* sound;
	SDL_Texture* texture;
};