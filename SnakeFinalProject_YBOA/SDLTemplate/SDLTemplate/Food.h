#pragma once
#include "GameObject.h"
#include "draw.h"
#include "common.h"
#include <vector>

class Food : public GameObject

{
public:
	void start();
	void draw();
	void update();

	void setPosition(int xPos, int yPos);
	int getPositionX() const;
	int getPositionY() const;
	int getWidth() const;
	int getHeight() const;

	SDL_Texture* getAppleTexture() const {
		return appleTexture;
	}
private:
	int x;
	int y;
	int width;
	int height;
	SDL_Texture* appleTexture;
	SDL_Texture* texture;
};