#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

class Bullet : public GameObject
{
public:
	Bullet(float positionX, float positionY, float directionX, float directionY, float speed);
	void start();
	void update();
	void draw();
	float getPositonX();
	float getPositionY();
	float getWidth();
	float getHeight();


private:
	int x;
	int y;
	int width;
	int height;
	int speed;
	const int baseSpeed = 3;
	SDL_Texture* texture;
	float directionX;
	float directionY;
};

