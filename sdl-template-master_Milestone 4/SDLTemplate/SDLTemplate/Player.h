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

	int getPositionX();
	int getPositionY();
	int getWidth();
	int getHeight();

	bool getIsAlive();
	void doDeath();

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
	bool IsAlive;
	const int baseSpeed = 3;
	std::vector<Bullet*> bullets;
	Mix_Chunk* sound;
	SDL_Texture* texture;
};