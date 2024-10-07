#include "GameScene.h"
#include "Enemy.h"

GameScene::GameScene()
{
	// Register and add game objects on constructor
	player = new Player();
	this->addGameObject(player);
 }

GameScene::~GameScene()
{
	delete player;
}

void GameScene::start()
{
	Scene::start();
	// Initialize any scene logic here
	currentSpawnTimer = 300;
	spawnTime = 300; // Spawn time of 5 seconds

	for (int i = 0; i < 3; i++)
	{
		spawn();
	}
}

void GameScene::draw()
{
	Scene::draw();
}

void GameScene::update()
{
	Scene::update();

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
		if (spawnedEnemies[i]->getPositionX() < -spawnedEnemies[i]->getWidth()) // Off-screen check
		{
			Enemy* enemyToErase = spawnedEnemies[i];
			spawnedEnemies.erase(spawnedEnemies.begin() + i);
			delete enemyToErase;  // Free memory
			i--;  // Adjust index after removal
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
