#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h";
#include <vector>
#include "util.h"
#include "Player.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	void start();
	void update();
	void draw();
	void setPlayerTarget(Player* player);
	void setPosition(int xPos, int yPos);

	int getPositionX() const; 
	int getPositionY() const; 
	int getWidth() const;    
	int getHeight() const;    

private:
	Mix_Chunk* sound;
	SDL_Texture* texture;
	Player* playerTarget;
	int x;
	int y;
	float directionX;
	float directionY;
	int width;
	int height;
	int speed;
	float reloadTime;
	float currentReloadTime;
	float directionChangeTime;
	float currentDirectionChangeTime;
	const int baseSpeed = 3;
	std::vector<Bullet*> bullets;
};

