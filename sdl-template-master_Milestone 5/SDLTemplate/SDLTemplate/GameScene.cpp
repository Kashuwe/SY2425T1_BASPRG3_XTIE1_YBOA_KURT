#include "GameScene.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PowerUp.h"
#include "Boss.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
	player = new Player();
	this->addGameObject(player);

	powerup = new PowerUp();

	explosionActive = false;  // Initialize the explosion as inactive

	disableEnemySpawn = false;

 }

GameScene::~GameScene()
{
	delete player;
	delete powerup;
	delete boss; 
}

void GameScene::start()
{
	Scene::start();
	
	initFonts();

	points = 0;
	isPowerUpOnScreen = false;
	bossActive = false;
	powerUpsSpawned = 0;
	currentSpawnTimer = 300;
	spawnTime = 300; // Spawn time of 5 seconds
	
	powerUpsSpawned = 0;
	powerUpSpawnTimer = MIN_POWERUP_SPAWN_TIME + (rand() % (MAX_POWERUP_SPAWN_TIME - MIN_POWERUP_SPAWN_TIME + 1));

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

	// Display the player's alloted point count on the screen
	drawText(110, 20, 255, 255, 255, TEXT_CENTER, "POINTS: %03d", points);
	
	// Display the player's collected power-up count on the screen
	drawText(110, 60, 255, 255, 255, TEXT_CENTER, "POWER-UPS: %d", player->getPowerUpCount());

	//// Draw boss health bar if boss is alive
	//if (boss->getIsBossAlive())
	//{
	//	// Draw health bar background
	//	int healthBarWidth = 200;
	//	int healthBarHeight = 20;
	//	int healthBarX = (SCREEN_WIDTH - healthBarWidth) / 2;
	//	int healthBarY = 30;

	//	// Draw background (red)
	//	SDL_Rect backgroundRect = { healthBarX, healthBarY, healthBarWidth, healthBarHeight };
	//	SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
	//	SDL_RenderFillRect(app.renderer, &backgroundRect);

	//	// Draw current health (green)
	//	float healthPercentage = (float)boss->getCurrentHealth() / boss->getMaxHealth();
	//	int currentHealthWidth = healthBarWidth * healthPercentage;
	//	SDL_Rect healthRect = { healthBarX, healthBarY, currentHealthWidth, healthBarHeight };
	//	SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
	//	SDL_RenderFillRect(app.renderer, &healthRect);

	//	// Draw health text
	//	drawText(SCREEN_WIDTH / 2, healthBarY - 10, 255, 255, 255, TEXT_CENTER,
	//		"Boss Health: %d/%d", boss->getCurrentHealth(), boss->getMaxHealth());
	//}


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
	if (disableEnemySpawn) {
		return; // Skip enemy spawning if the flag is true
	}

	Enemy* enemy = new Enemy();
	this->addGameObject(enemy);
	enemy->setPlayerTarget(player);

	// To Set the enemy to spawn beyond the top of the screen
	int spawnX = 200 + (rand() % (SCREEN_WIDTH - 620)); // Random X position
	int spawnY = -100; // Spawn 100 pixels above the top of the screen

	enemy->setPosition(spawnX, spawnY);
	spawnedEnemies.push_back(enemy);
}

void GameScene::spawnBoss()
{
	Boss* boss = new Boss();
	this->addGameObject(boss);
	boss->setPlayerTarget(player);
	boss->setPosition((SCREEN_WIDTH - boss->getWidth()) / 2, 100);  // Center boss horizontally
	spawnedBosses.push_back(boss);
}

void GameScene::spawnPowerUp()
{
	PowerUp* power = new PowerUp();
	this->addGameObject(power);

	// Position for the power - sup within screen bounds
	int positionX = rand() % (SCREEN_WIDTH - 50);
	int positionY = -100; // Start at the top of the screen

	power->setPosition(positionX, positionY);
	spawnedPowerUps.push_back(power);
}

void GameScene::doSpawnLogic()
{
	if (currentSpawnTimer > 0)
		currentSpawnTimer--;

	// Spawn boss when player reaches the threshold score and boss is not active
	if (points >= 20 && !bossActive)
	{
		spawnBoss();
		bossActive = true;
		disableEnemySpawn = true;  // Pause normal enemy spawns
		std::cout << "Boss spawned at score: " << points << std::endl;
	}

	// If no boss, spawn regular enemies as usual
	if (!bossActive && currentSpawnTimer <= 0)
	{
		for (int i = 0; i < 3; i++)
		{
			spawn();
		}
		currentSpawnTimer = spawnTime;
	}

	// PowerUp spawn logic
	if (powerUpsSpawned < MAX_POWERUPS)
	{
		if (powerUpSpawnTimer > 0)
		{
			powerUpSpawnTimer--;
			if (powerUpSpawnTimer <= 0)
			{
				spawnPowerUp();
				powerUpsSpawned++;  // Increment the counter

				if (powerUpsSpawned < MAX_POWERUPS)
				{
					// Reset timer with new random time only if we haven't reached the limit
					powerUpSpawnTimer = MIN_POWERUP_SPAWN_TIME + (rand() % (MAX_POWERUP_SPAWN_TIME - MIN_POWERUP_SPAWN_TIME + 1));
				}
			}
		}
	}

	// Memory manage enemies: remove enemies that are off the screen (to the left)
	for (int i = 0; i < spawnedEnemies.size(); i++)
	{
		Enemy* currentEnemy = spawnedEnemies[i];

		// Off-screen check (left side, right side, above, or below the screen)
		if (currentEnemy->getPositionX() < -currentEnemy->getWidth() ||  // left side
			currentEnemy->getPositionX() > SCREEN_WIDTH ||               // right side
			currentEnemy->getPositionY() > SCREEN_HEIGHT)                // below
		{
			// If the enemy is outside the screen, remove and delete it
			spawnedEnemies.erase(spawnedEnemies.begin() + i);
			delete currentEnemy;
			i--;  // Adjust index after removal
		}
	}

	// Memory manage power-ups: check for off-screen power-ups
	for (int i = 0; i < spawnedPowerUps.size(); i++)
	{
		if (spawnedPowerUps[i]->getPositionY() > SCREEN_HEIGHT) // Check if off the screen
		{
			PowerUp* powerToDelete = spawnedPowerUps[i];
			spawnedPowerUps.erase(spawnedPowerUps.begin() + i);
			delete powerToDelete;
			powerUpsSpawned--;
			i--; // Adjust index after removal
		}
	}
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

void GameScene::despawnBoss(Boss* boss)
{
	int index = -1;
	for (int i = 0; i < spawnedBosses.size(); i++)
	{
		// if the pointer matches
		if (boss == spawnedBosses[i])
		{
			index = i;
			break;
		}
	}

	// if match is found
	if (index != -1)
	{
		// Trigger the explosion at the enemy's position before removing it
		triggerExplosion(boss->getPositionX(), boss->getPositionY());

		// Remove and delete the enemy
		spawnedBosses.erase(spawnedBosses.begin() + index);
		delete boss;
	}
}

void GameScene::despawnPowerUp(PowerUp* power)
{
	int index = -1;

	for (int i = 0; i < spawnedPowerUps.size(); i++)
	{
		//if the pointer matches
		if (power == spawnedPowerUps[i])
		{
			index = i;
			break;

		}
	}

	//if match is found
	if (index != -1)
	{
		spawnedPowerUps.erase(spawnedPowerUps.begin() + index);
		delete power;
		isPowerUpOnScreen = false;
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

void GameScene::doCollisionLogic()
{
	// Collision Logic
	for (int i = 0; i < objects.size(); i++)
	{
		// Cast to bullet
		Bullet* bullet = dynamic_cast<Bullet*>(objects[i]);

		// Cast to power
		PowerUp* power = dynamic_cast<PowerUp*>(objects[i]);

		// Cast to boss
		Boss* boss = dynamic_cast<Boss*>(objects[i]);
		
		// Check if the cast is successful
		if (power != nullptr)
		{
			int collision = checkCollision(
				player->getPositionX(), player->getPositionY(), player->getWidth(), player->getHeight(),
				power->getPositionX(), power->getPositionY(), power->getWidth(), power->getHeight()
			);

			if (collision == 1)
			{
				player->poweredUp();
				player->addPowerUpCount();
				despawnPowerUp(power);
				break;
			}
		}
		// Check if the cast is successful (i.e. objects[i] is a bullet)
		if (bullet != nullptr)
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
					std::cout << "Player hit by normal enemy bullet!" << std::endl; // Debug message
					break;
				}
		
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
							std::cout << "Collision detected!" << std::endl;
							// Handle enemy despawning
							despawnEnemy(currentEnemy);
							points++;
							break;
						}
					}
				}

				//// Check for Bullet collision with Boss
				//if (bullet != nullptr && bullet->getSide() == Side::PLAYER_SIDE && bossActive)
				//{
				//	// Assuming you have a pointer to the boss
				//	if (boss != nullptr && boss->getIsBossAlive())
				//	{
				//		int collision = checkCollision(
				//			boss->getPositionX(), boss->getPositionY(), boss->getWidth(), boss->getHeight(),
				//			bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight()
				//		);

				//		if (collision == 1)
				//		{
				//			boss->takeDamage(10); // Adjust damage value as needed

				//			// Check if the boss is dead
				//			if (boss->getCurrentHealth() <= 0)
				//			{
				//				despawnBoss(boss); // Handle boss despawning
				//				points += 50; // Reward points for defeating the boss
				//			}
				//		}
				//	}
				//}
				//for (int i = 0; i < spawnedBosses.size(); i++)
				//{
				//	Boss* currentBoss = spawnedBosses[i];

				//	if (bullet->getSide() == Side::PLAYER_SIDE)
				//	{
				//		int collision = checkCollision(
				//			currentBoss->getPositionX(), currentBoss->getPositionY(), currentBoss->getWidth(), currentBoss->getHeight(),
				//			bullet->getPositionX(), bullet->getPositionY(), bullet->getWidth(), bullet->getHeight()
				//		);

				//		if (collision == 1)
				//		{
				//			// Handle enemy despawning
				//			despawnBoss(currentBoss);
				//			points += 50;
				//			break;
				//		}
				//	}
				//}
			}
		}
	}
}