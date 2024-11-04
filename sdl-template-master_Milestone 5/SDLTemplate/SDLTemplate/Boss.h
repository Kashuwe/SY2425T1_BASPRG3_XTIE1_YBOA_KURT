#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h";
#include <vector>
#include "util.h"
#include "Player.h"

class Boss : public GameObject
{
public:
	Boss();
	~Boss();

	void start();
	void update();
	void draw();

	void setPlayerTarget(Player* player);
	void setPosition(int xPos, int yPos);

	int getPositionX() const;
	int getPositionY() const;
	int getWidth() const;
	int getHeight() const;

	bool getIsBossAlive();
	void doBossDeath();

	void takeDamage(int damage);
	int getCurrentHealth() const;
	int getMaxHealth() const;

private:
	Boss* boss;
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
	int maxHealth;
	int currentHealth;

	

	bool isBossAlive;

	float bossReloadTime;
	float bossCurrentReloadTime;
	float directionChangeTime;
	float currentDirectionChangeTime;
	std::vector<Bullet*> bullets;
};