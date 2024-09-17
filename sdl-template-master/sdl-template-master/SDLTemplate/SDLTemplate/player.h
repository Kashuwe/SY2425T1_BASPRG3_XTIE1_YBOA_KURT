#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

class Player : public GameObject
{
public:
	void start();
	void update();
	void draw();
private:
	int x;
	int y;
	int width;
	int height;
	int speed;
	const int baseSpeed = 3;
	SDL_Texture* texture;
};