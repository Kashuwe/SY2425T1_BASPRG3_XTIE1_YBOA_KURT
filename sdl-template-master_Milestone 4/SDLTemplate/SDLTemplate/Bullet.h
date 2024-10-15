#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

enum class Side
{
	PLAYER_SIDE,
	ENEMY_SIDE
};

class Bullet : public GameObject
{
public:
	Bullet(int positionX, int positionY, float directionX, float directionY, int speed, Side side);
	void start();
	void update();
	void draw();
	int getPositionX();
	int getPositionY();
	int getWidth();
	int getHeight();
	Side getSide();


private:
	Side side;
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
