#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "text.h"
#include <vector>
#include "SDL.h"


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

    // Enemy spawning logic
    float spawnTime;
    float currentSpawnTimer;
    std::vector<Enemy*> spawnedEnemies;

    int points;

    void spawn();
    void despawnEnemy(Enemy* enemy);

    void triggerExplosion(int x, int y);

    void doSpawnLogic();
    void doCollisionLogic();

    // Explosion variables
    SDL_Texture* explosionTexture;  
    bool explosionActive;           
    int explosionX, explosionY;     
    int explosionWidth, explosionHeight; 
    int explosionTimer;
};