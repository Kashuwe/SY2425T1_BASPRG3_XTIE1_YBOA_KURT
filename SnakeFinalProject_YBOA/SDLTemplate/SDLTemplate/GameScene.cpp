#include "GameScene.h"

GameScene::GameScene()
{
	snake = new Snake();
	this->addGameObject(snake);

	food = nullptr;
	
	appleSpawnTimer = 200; 
	shouldSpawnApple = true; 

	highScore = 0;
}

GameScene::~GameScene()
{
	delete snake;
	delete food;
}

void GameScene::start()
{
	Scene::start();

	initFonts();

	snake->start();

	score = 0;

	appleTexture = loadTexture("gfx/apple.png");
	highScoreTexture = loadTexture("gfx/hs.png");
}

void GameScene::draw()
{
	// Setting the background
	SDL_SetRenderDrawColor(app.renderer, 44, 44, 44, SDL_ALPHA_OPAQUE); // DarkGreenBackground
	SDL_RenderClear(app.renderer);
	
	SDL_SetRenderDrawColor(app.renderer, 79, 79, 79, SDL_ALPHA_OPAQUE); // GreenBackground
	SDL_Rect firstrect = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(app.renderer, &firstrect);

	SDL_SetRenderDrawColor(app.renderer, 107, 107, 107, SDL_ALPHA_OPAQUE); //LightGreenBackground
	SDL_Rect secondrect = { 40, 120, SCREEN_WIDTH-80, SCREEN_HEIGHT-160 };//Gameplay Area
	SDL_RenderFillRect(app.renderer, &secondrect);
	
	// checkered area
	for (int i = 3; i < 14; i++) {
		for (int j = 1; j < 14; j++) {
			if ((i+j) % 2 == 0){
				SDL_SetRenderDrawColor(app.renderer, 135, 135, 135, SDL_ALPHA_OPAQUE);
				SDL_Rect rect = {j*40, i*40, 40, 40};
				SDL_RenderFillRect(app.renderer, &rect);
			}
		}
	}

	snake->draw(app.renderer);
	SDL_RenderPresent(app.renderer);

	// For Score
	int xPos = 10;  
	int yPos = 10; 

	// For HighScore
	int x = 100;

	// Draw the apple texture
	blit(appleTexture, xPos, yPos);  // Use the blit function to draw the texture

	// Draw the HS texture
	blit(highScoreTexture, x, yPos);  // Use the blit function to draw the texture

	// Draw the score text next to the apple texture
	drawText(xPos + 50, yPos + 10, 255, 255, 255, 0, "%d", score);  // Display score next to the apple

	// Draw the high score next to HS texture
	drawText(x + 50, yPos + 10, 255, 215, 0, 0, "%d", highScore); // High score

	// Check if game is over and display "Game Over" text
	if (snake->isGameOver) {
		drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 255, 0, 0, TEXT_CENTER, "G A M E  O V E R");
		drawText(SCREEN_WIDTH / 2, 500, 255, 255, 0, TEXT_CENTER, "PRESS 'R' TO RESTART");
	}

	Scene::draw();
}

void GameScene::update()
{
	Scene::update();

	// Check for restart condition
	if (snake->isGameOver && app.keyboard[SDL_SCANCODE_R]) {
		restart();  // Call restart if R is pressed
	}

	// Update's highscore if current score excceds it
	if (score > highScore) {
		highScore = score;
	}

	// apple spawning logic
	if (shouldSpawnApple)
	{
		spawnApple();      // Spawn new apple
		shouldSpawnApple = false; 
	}

	doCollision();

}

void GameScene::SpawnLogic()
{
	if (appleSpawnTimer > 0)
	{
		appleSpawnTimer--;
	}

	// When the timer hits 0, spawn an apple and reset the timer
	if (appleSpawnTimer <= 0 && shouldSpawnApple)
	{
		spawnApple(); 
		appleSpawnTimer = 200; // Reset timer for consistent spawning
	}
}

void GameScene::spawnApple()
{
	if (food != nullptr)
	{
		despawnApple(food);
	}

	Food* apple = new Food();
	this->addGameObject(apple);

	int gridXStart = 40; 
	int gridYStart = 120;

	// Calculate a random position on the grid
	int gridSize = 40;
	int gridColumns = (SCREEN_WIDTH - 80) / gridSize;  
	int gridRows = (SCREEN_HEIGHT - 160) / gridSize;   

	int gridX, gridY;
	bool validPosition = false;

	// Randomized spawn of apple
	// To avoid apple spawn on snake
	while (!validPosition) {
		// To not spawn an apple outside the gameplay grid
		gridX = rand() % (gridColumns - 1);
		gridY = rand() % (gridRows - 1);

		// Position the apple in the valid area of the grid
		int appleX = gridXStart + gridX * gridSize;
		int appleY = gridYStart + gridY * gridSize;

		// Check if the apple is on the snake's segments
		if (!isAppleOnSnake(appleX, appleY)) {
			validPosition = true;
			apple->setPosition(appleX, appleY);  // Set the position only when valid
		}
	}

	spawnedApples.push_back(apple);
}

void GameScene::despawnApple(Food* food)
{
	int index = -1;

	for (int i = 0; i < spawnedApples.size(); i++)
	{
		//if the pointer matches
		if (food == spawnedApples[i])
		{
			index = i;
			break;

		}
	}

	//if match is found
	if (index != -1)
	{
		spawnedApples.erase(spawnedApples.begin() + index);
		delete food;
		shouldSpawnApple = true;
	}
}

bool GameScene::isAppleOnSnake(int appleX, int appleY)
{
	// Check if the apple is on the snake's head or body
	for (int i = 0; i < snake->getBodySize(); i++) {
		if (snake->getBodySegmentX(i) == appleX && snake->getBodySegmentY(i) == appleY) {
			return true;  // The apple is on top of the snake
		}
	}
	return false;  // snek not on spawn grid.exe
}

void GameScene::doCollision()
{
	// Collision Logic
	for (int i = 0; i < objects.size(); i++)
	{
		// Cast to food
		Food* food = dynamic_cast<Food*>(objects[i]);

		if (food != nullptr)
		{
			std::cout << "Checking collision..." << std::endl;

			int collision = checkCollision(
				snake->getHeadX(), snake->getHeadY(), snake->getWidth(), snake->getHeight(),
				food->getPositionX(), food->getPositionY(), food->getWidth(), food->getHeight()
			);

			if (collision == 1)
			{
				std::cout << "Collision detected!" << std::endl;
				despawnApple(food);
				score++;
				std::cout << "Apple consumed! Current Score: " << score << std::endl;
				snake->grow();
				break;
			}
		}
	}
	// Collision with walls
	if (snake->getHeadX() < 40 || snake->getHeadX() >= 540 ||
		snake->getHeadY() < 120 || snake->getHeadY() >= 560){
		std::cout << "Game Over! Snake hit the wall." << std::endl;
		snake->doDeath();
		shouldSpawnApple = false;
	}

	// Collision with self (body)
	for (int i = 1; i < snake->getBodySize(); i++) { // Start from 1 since 0 is the head
		if (snake->getHeadX() == snake->getBodySegmentX(i) &&
			snake->getHeadY() == snake->getBodySegmentY(i)) {
			std::cout << "Game Over! Snake hit itself." << std::endl;
			snake->doDeath();  // End game
			shouldSpawnApple = false;
		}
	}
}

void GameScene::restart()
{
	// reset snake
    snake->reset(); 

    score = 0;

    // Clear all apples
    for (Food* apple : spawnedApples) {
        delete apple;
    }
    spawnedApples.clear(); 

    // Reset apple spawn logic
    shouldSpawnApple = true; 
    appleSpawnTimer = 200;   

}