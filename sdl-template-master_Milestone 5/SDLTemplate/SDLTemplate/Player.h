#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include "PowerUp.h"
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

	bool getIsPoweredUp();
	void poweredUp();

	void addPowerUpCount();
	int getPowerUpCount() const;

	void triShot();
	void sideBurst();

private:
	int x;
	int y;
	int width;
	int height;
	int speed;
	const int baseSpeed = 3;
	int currentPowerUpCount = 0;

	float reloadTime;
	float currentReloadTime;

	// For triShot firing pattern
	float wingtipReloadTime;
	float currentWingtipReloadTime;

	// For sideBurst firing pattern
	bool isSideBurstActive;
	int sideBurstDuration;
	int currentSideBurstDuration;
	int sideBurstCooldown;
	int currentSideBurstCooldown;
	int sideBurstFireRate;
	int currentSideBurstFireRate;

	bool IsAlive;
	bool isPoweredUp;

	std::vector<Bullet*> bullets;
	Mix_Chunk* sound;
	SDL_Texture* texture;
};