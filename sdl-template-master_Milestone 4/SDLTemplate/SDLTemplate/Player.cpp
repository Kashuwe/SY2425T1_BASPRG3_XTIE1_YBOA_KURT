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
	x = 100;
	y = 100;
	width = 0;
	height = 0;
	speed = 3;
	reloadTime = 8; // Reload time of 8 frame, or 0.13 secs.
	currentReloadTime = 0;
	wingtipReloadTime = 12;
	currentWingtipReloadTime = 0;

	IsAlive = true;
	// query the texture to set w&h 
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	sound->volume = 64;
}



void Player::update()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_WIDTH)
		{
			// Cache the variable to be deleted later
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			break; // Delete one bullet per frame

		}
	}

	if (!IsAlive) return;

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
		Bullet* bullet = new Bullet(x + width / 1, y - 4 + height / 2, 1, 0, 10, Side::PLAYER_SIDE);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);
		

		// After firing, reset our relod timer
		currentReloadTime = reloadTime;
	}

	// Fire wingtip bullets on G press
	if (app.keyboard[SDL_SCANCODE_G] && currentWingtipReloadTime == 0)
	{
		SoundManager::playSound(sound);

		// Left wingtip bullet
		Bullet* leftBullet = new Bullet(x + width / 3, y - 22 + height / 2, 1, 0, 10, Side::PLAYER_SIDE);
		bullets.push_back(leftBullet);
		getScene()->addGameObject(leftBullet);
		

		// Right wingtip bullet
		Bullet* rightBullet = new Bullet(x + width / 3, y + 13 + height / 2, 1, 0, 10, Side::PLAYER_SIDE);
		bullets.push_back(rightBullet);
		getScene()->addGameObject(rightBullet);
		

		currentWingtipReloadTime = wingtipReloadTime; // Reset wingtip bullet reload time
	}

	// Bullet cleanup: remove bullets that are off the screen
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() > SCREEN_WIDTH)
		{
			// Cache the variable to be deleted later
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			break; // Delete one bullet per frame
			
		}
	}
}

void Player::draw()
{
	if (!IsAlive) return;
	blit(texture, x, y);
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



