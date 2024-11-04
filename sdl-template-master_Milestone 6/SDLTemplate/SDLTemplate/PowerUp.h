#pragma once
#include "GameObject.h"
#include "SoundManager.h"

class PowerUp : public GameObject
{
public:
    void start();
    void update();
    void draw();
    void setPosition(int xPos, int yPos);

    int getPositionX() const;
    int getPositionY() const;
    int getWidth() const;
    int getHeight() const;
    
private:
    int x;
    int y;
    int width;
    int height;
    int directionX;
    int directionY;
    int speed;
    
    SDL_Texture* texture;   // PowerUp texture
    Mix_Chunk* sound;       // SoundTexture
};