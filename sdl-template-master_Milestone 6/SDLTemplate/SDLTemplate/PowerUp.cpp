#include "PowerUp.h"
#include "Scene.h"
#include "util.h"
#include "Player.h"

void PowerUp::start()
{
    directionX = 1;
    directionY = 1;
    speed = 7;
    width = 0;
    height = 0;

    // Load the power-up texture
    texture = loadTexture("gfx/powerup1.png");

    if (!texture) {
        std::cerr << "Failed to load PowerUp texture from gfx/powerup.png" << std::endl; // Error message
    }

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void PowerUp::update()
{
    // Power-up falls down the screen
    y += speed;

}

void PowerUp::draw()
{
    blit(texture, x, y);
}

void PowerUp::setPosition(int xPos, int yPos)
{
    this->x = xPos;
    this->y = yPos;
}

int PowerUp::getPositionX() const
{
    return x;
}

int PowerUp::getPositionY() const
{
    return y;
}

int PowerUp::getWidth() const
{
    return width;
}

int PowerUp::getHeight() const
{
    return height;
}
