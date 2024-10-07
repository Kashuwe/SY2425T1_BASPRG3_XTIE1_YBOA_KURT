#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>

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

    void spawn();
};