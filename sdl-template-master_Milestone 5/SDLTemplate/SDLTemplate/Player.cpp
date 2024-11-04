#include "Player.h"
#include "Scene.h"

Player::~Player()
{
	// Memory manage our bullets. delete all bullets on player deletion/death
	for (int i = 0; i < bullets.size(); i++)\
	{
		delete bullets[i];

	}
	bullets.clear();
	
}

void Player::start()
{
	//load texture
	texture = loadTexture("gfx/player.png");

	//initial to avoid garbage values
	x = 360;
	y = 800;
	width = 0;
	height = 0;
	speed = 3;

	reloadTime = 10; // Reload time of 8 frame, or 0.16 secs.
	currentReloadTime = 0;

	// trioShot
	wingtipReloadTime = 12;
	currentWingtipReloadTime = 0;
	isSideBurstActive = false;

	// sideBurst
	sideBurstDuration = 30;  // Duration of burst in frames (1 second at 60 FPS)
	currentSideBurstDuration = 0;
	sideBurstCooldown = 60; // Cooldown between bursts in frames (1 second at 60 FPS)
	currentSideBurstCooldown = 0;
	sideBurstFireRate = 8;  
	currentSideBurstFireRate = 0;

	IsAlive = true;
	isPoweredUp = false;

	// query the texture to set w&h 
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	sound->volume = 64;
}

void Player::update()
{
	if (!IsAlive || !getScene()) return;

	if (!IsAlive) return;

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_HEIGHT)
		{
			// Cache the variable to be deleted later
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			break; // Delete one bullet per frame

		}
	}

	// Handle SideBurst auto-firing
	if (isPoweredUp && currentPowerUpCount >= 2)
	{
		if (!isSideBurstActive && currentSideBurstCooldown == 0)
		{
			// Start a new burst
			isSideBurstActive = true;
			currentSideBurstDuration = sideBurstDuration;
		}

		if (isSideBurstActive)
		{
			if (currentSideBurstFireRate == 0)
			{
				sideBurst();
				currentSideBurstFireRate = sideBurstFireRate;
			}

			currentSideBurstDuration--;
			if (currentSideBurstDuration <= 0)
			{
				// End burst and start cooldown
				isSideBurstActive = false;
				currentSideBurstCooldown = sideBurstCooldown;
			}
		}
		else
		{
			// Count down the cooldown
			if (currentSideBurstCooldown > 0)
				currentSideBurstCooldown--;
		}

		// Decrement fire rate counter
		if (currentSideBurstFireRate > 0)
			currentSideBurstFireRate--;
	}

	// Movement controls
	if (app.keyboard[SDL_SCANCODE_W])
	{
		y -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_S])
	{
		y += speed;
	}

	if (app.keyboard[SDL_SCANCODE_A])
	{
		x -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_D])
	{
		x += speed;
	}

	if (app.keyboard[SDL_SCANCODE_LSHIFT])
	{
		speed = 5;
	}

	if (app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		speed = baseSpeed;
	}

	// Decrement the player's reload timer
	if (currentReloadTime > 0) currentReloadTime--;
	if (currentWingtipReloadTime > 0) currentWingtipReloadTime--;

	// Fire main bullet on F press
	if (app.keyboard[SDL_SCANCODE_F] && currentReloadTime == 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet(x + 11, y - 2 + height / 2, 0, -10, 5, Side::PLAYER_SIDE);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);
		

		// After firing, reset our relod timer
		currentReloadTime = reloadTime;

		if (isPoweredUp == true && currentPowerUpCount >= 1)
		{
			triShot();
		}
	}
}

void Player::draw()
{
	if (!IsAlive) return;	

	SDL_Rect destRect = { x, y, width, height };

	SDL_RenderCopyEx(app.renderer, texture, NULL, &destRect, -90, NULL, SDL_FLIP_NONE);
}

int Player::getPositionX()
{
	return x;
}

int Player::getPositionY()
{
	return y;
}

int Player::getWidth()
{
	return width;
}

int Player::getHeight()
{
	return height;
}

bool Player::getIsAlive()
{
	return IsAlive;
}

void Player::doDeath()
{
	IsAlive = false;
}

bool Player::getIsPoweredUp()
{
	return isPoweredUp;
}

void Player::poweredUp()
{
	isPoweredUp = true;
}

void Player::triShot()
{
	Bullet* leftBullet = new Bullet(x + 11 - 15, y + 50, 0, -10, 5, Side::PLAYER_SIDE);
	bullets.push_back(leftBullet);
	getScene()->addGameObject(leftBullet);

	Bullet* rightBullet = new Bullet(x + 11 + 15, y + 50, 0, -10, 5, Side::PLAYER_SIDE);
	bullets.push_back(rightBullet);
	getScene()->addGameObject(rightBullet);

	currentWingtipReloadTime = wingtipReloadTime;
}

void Player::sideBurst()
{
	// Left side bullet
	Bullet* leftBullet = new Bullet(x + 11 - 16, y + 70, -2, -8, 7, Side::PLAYER_SIDE);
	bullets.push_back(leftBullet);
	getScene()->addGameObject(leftBullet);

	// Right side bullet
	Bullet* rightBullet = new Bullet(x + 11 + 16, y + 70, 2, -8, 7, Side::PLAYER_SIDE);
	bullets.push_back(rightBullet);
	getScene()->addGameObject(rightBullet);
}

void Player::addPowerUpCount()
{
	if (currentPowerUpCount < 2) {
		currentPowerUpCount++;
	}
}

int Player::getPowerUpCount() const
{
	return currentPowerUpCount;
}