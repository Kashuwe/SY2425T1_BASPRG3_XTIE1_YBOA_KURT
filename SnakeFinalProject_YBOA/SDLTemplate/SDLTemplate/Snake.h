#pragma once
#include "GameObject.h"
#include "draw.h"
#include "common.h"
#include <vector>


class Snake : public GameObject
{
public:
	Snake();
	~Snake();

	void start();
	void draw (SDL_Renderer* renderer);
	void update();
	void move();
	void grow();
	void doDeath();

	int getHeadX() const;
	int getHeadY() const;
	int getWidth() const;
	int getHeight() const;
	void reset();

	int getBodySize() const;
	int getBodySegmentX(int index) const;
	int getBodySegmentY(int index) const;


	bool isGameOver = false;

	SDL_Texture* tail_left = nullptr;
	SDL_Texture* tail_up = nullptr;
	SDL_Texture* tail_down = nullptr;
	SDL_Texture* tail_right = nullptr;
	SDL_Texture* body_bottomright = nullptr;
	SDL_Texture* body_bottomleft = nullptr;
	SDL_Texture* body_topleft = nullptr;
	SDL_Texture* body_topright = nullptr;
	SDL_Texture* body_horizontal = nullptr;
	SDL_Texture* body_vertical = nullptr;
	SDL_Texture* head_down = nullptr;
	SDL_Texture* head_right = nullptr;
	SDL_Texture* head_left = nullptr;
	SDL_Texture* head_up = nullptr;

	std::vector<SDL_Rect> snake;
private:
	
	SDL_Texture* texture;
	SDL_Point direction = { 40, 0 }; // Initial movement direction (right)
	Uint32 lastMoveTime = 0;
	Uint32 lastDirectionChangeTime = 0; 
	const Uint32 directionChangeCooldown = 120;
	int moveDelay = 130;
	int gridSize = 40;            // Size of each grid cell        
	int headX;
	int headY;
	int width;
	int height;   
};

