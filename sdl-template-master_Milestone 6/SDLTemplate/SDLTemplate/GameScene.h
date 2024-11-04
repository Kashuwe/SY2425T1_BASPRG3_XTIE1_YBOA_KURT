#pragma once
#include <iostream>
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "text.h"
#include "SDL.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Boss.h"

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();
    void start();
    void draw();
    void update();

private:
    Player* player;
    PowerUp* powerup;
    Boss* boss;

    float spawnTime;
    float currentSpawnTimer;

    std::vector<Enemy*> spawnedEnemies;
    std::vector<PowerUp*> spawnedPowerUps; 
    std::vector<Boss*> spawnedBosses;

    int points;

    bool isPowerUpOnScreen;
    bool disableEnemySpawn;

    void spawn();
    void spawnBoss();
    void despawnPowerUp(PowerUp* power);
    void despawnEnemy(Enemy* enemy);
    void despawnBoss(Boss* boss);
    void triggerExplosion(int x, int y);
    void doSpawnLogic();
    void doCollisionLogic();
    void spawnPowerUp();
    
    // Explosion variables
    SDL_Texture* explosionTexture;  
    bool explosionActive;           
    int explosionX, explosionY;     
    int explosionWidth, explosionHeight; 
    int explosionTimer;

    // Power-up related members
    int powerUpSpawnTimer;
    const static int MIN_POWERUP_SPAWN_TIME = 300;  // 5 seconds (60 fps * 5)
    const static int MAX_POWERUP_SPAWN_TIME = 600;  // 10 seconds (60 fps * 10)
    int powerUpsSpawned;
    const int MAX_POWERUPS = 2;

    bool bossActive = false;

};