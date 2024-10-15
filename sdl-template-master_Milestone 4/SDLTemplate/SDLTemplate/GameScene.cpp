#include "GameScene.h"
#include "Enemy.h"
#include "Bullet.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
	player = new Player();
	this->addGameObject(player);
	explosionActive = false;  // Initialize the explosion as inactive
 }

GameScene::~GameScene()
{
	delete player;
}

void GameScene::start()
{
	Scene::start();
	// Initialize any scene logic here

	initFonts();

	points = 0;

	currentSpawnTimer = 300;
	spawnTime = 300; // Spawn time of 5 seconds

	// Initialize explosion texture
	explosionTexture = loadTexture("gfx/explosion.png");


	for (int i = 0; i < 3; i++)
	{
		spawn();
	}
}

void GameScene::draw()
{
	Scene::draw();

	drawText(110, 20, 255, 255, 255, TEXT_CENTER, "POINTS: %03d", points);

	// Draw explosion effect (if active)
	if (explosionActive)
	{
		SDL_Rect destRect = { explosionX, explosionY, explosionWidth, explosionHeight };
		SDL_RenderCopy(app.renderer, explosionTexture, NULL, &destRect);
	}

	if (!player->getIsAlive())
	{
	drawText(SCREEN_WIDTH / 2, 600, 255, 0, 0, TEXT_CENTER, "GAME OVER: ");
	}
}

void GameScene::update()
{
	Scene::update();

	doSpawnLogic();

	doCollisionLogic();

	// Explosion timer logic
	if (explosionTimer > 0)
	{
		explosionTimer--;
		if (explosionTimer <= 0)
		{
			explosionActive = false; // Disable explosion after it ends
		}
	}
}

void GameScene::spawn()
{
	Enemy* enemy = new Enemy();
	this->addGameObject(enemy);
	enemy->setPlayerTarget(player);

	// To Set the enemy to spawn beyond the right side of the screen
	int spawnX = SCREEN_WIDTH + 100; // 100 pixels beyond the right side
	int spawnY = 300 + (rand() % 300); // Random Y position between 300 and 600

	enemy->setPosition(spawnX, spawnY);
	spawnedEnemies.push_back(enemy);
}

void GameScene::despawnEnemy(Enemy* enemy)
{
	int index = -1;
	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		// if the pointer matches
		if (enemy == spawnedEnemies[i])
		{
			index = i;
			break;
		}
	}

	// if match is found
	if (index != -1)
	{
		// Trigger the explosion at the enemy's position before removing it
		triggerExplosion(enemy->getPositionX(), enemy->getPositionY());

		// Remove and delete the enemy
		spawnedEnemies.erase(spawnedEnemies.begin() + index);
		delete enemy;
	}
}

void GameScene::triggerExplosion(int x, int y)
{
	// Set explosion position and size
	explosionX = x;
	explosionY = y;
	explosionWidth = 64;  // explosion width
	explosionHeight = 64; // explosion height

	// Activate explosion effect and set the timer
	explosionActive = true;
	explosionTimer = 30;  // Explosion lasts for 30 frames (half a second at 60fps)
}

void GameScene::doSpawnLogic()
{
	if (currentSpawnTimer > 0)
		currentSpawnTimer--;

	if (currentSpawnTimer <= 0)
	{
		for (int i = 0; i < 3; i++)
		{
			spawn();
		}

		currentSpawnTimer = spawnTime;
	}

	// Memory manage enemies: remove enemies that are off the screen (to the left)
	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		Enemy* currentEnemy = spawnedEnemies[i];

		// Off-screen check (left side, right side, above, or below the screen)
		if (currentEnemy->getPositionX() < -currentEnemy->getWidth() ||  //left side
			currentEnemy->getPositionY() < -currentEnemy->getHeight() || //top
			currentEnemy->getPositionY() > SCREEN_HEIGHT)                //bottom
		{
			// If the enemy is outside the screen, remove and delete it
			spawnedEnemies.erase(spawnedEnemies.begin() + i);
			delete currentEnemy;
			i--;  // Adjust index after removal
		}
	}
}

void GameScene::doCollisionLogic()
{
	// Collision Logic
	for (int i = 0; i < objects.size(); i++)
	{
		// Cast to bullet
		Bullet* bullet = dynamic_cast<Bullet*>(objects[i]);

		// Check if the cast iss successful (i.e. objects[i] is a bullet)
		if (bullet != NULL)
		{
			// if the bullet is coming from the enemy side, check againsts the player
			if (bullet->getSide() == Side::ENEMY_SIDE)
			{
				int collision = checkCollision(
					player->getPositionX(), player->getPositionY(), player->getWidth(), player->getHeight(),
					bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight()
				);

				if (collision == 1)
				{
					player->doDeath();
					break;
				}
			}
			// if the bullet is coming from the player side, check againsts the enemy
			else if (bullet->getSide() == Side::PLAYER_SIDE)
			{
				for (int i = 0; i < spawnedEnemies.size(); i++)
				{
					Enemy* currentEnemy = spawnedEnemies[i];

					if (bullet->getSide() == Side::PLAYER_SIDE)
					{
						int collision = checkCollision(
							currentEnemy->getPositionX(), currentEnemy->getPositionY(), currentEnemy->getWidth(), currentEnemy->getHeight(),
							bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight()
						);

						if (collision == 1)
						{
							despawnEnemy(currentEnemy);
							points++;
							break;
						}
					}
				}
			}
		}
	}
}
