#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "draw.h"
#include "Snake.h"
#include "Food.h"
#include "util.h"
#include "text.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void start();
	void draw();
	void update();
	
	void SpawnLogic();
	void spawnApple();
	void despawnApple(Food* food);
	void doCollision();

	void restart();

private:
	Snake* snake;
	Food* food;

	SDL_Texture* appleTexture;
	SDL_Texture* highScoreTexture;
	SDL_Renderer* renderer = nullptr;
	std::vector<Food*> spawnedApples;

	int score;
	int highScore;

	bool isAppleOnSnake(int appleX, int appleY);
	bool shouldSpawnApple;
	int appleSpawnTimer;
	
	
};

