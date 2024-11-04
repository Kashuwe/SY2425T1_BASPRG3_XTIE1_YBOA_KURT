#include "Scene.h"
#include "GameObject.h"

Scene* Scene::activeScene = NULL;

Scene::Scene()
{
	hasStarted = false;
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

void Scene::setActiveScene(Scene* scene)
{
	if (Scene::activeScene != NULL)
	{
		delete Scene::activeScene;
		Scene::activeScene = NULL;
	}

	Scene::activeScene = scene;
	scene->start();
}

Scene* Scene::getActiveScene()
{
	return Scene::activeScene;
}

void Scene::addGameObject(GameObject* obj)
{
	obj->setScene(this);
	objects.push_back(obj);
	// Call start on the object if this was added while the scene is running
	if (hasStarted) obj->start();
}

void Scene::removeGameObject(GameObject* obj)
{
	std::vector<GameObject*>::iterator itr = std::find(objects.begin(), objects.end(), obj);
	objects.erase(itr);
}

void Scene::update()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
}


void Scene::draw()
{
	if (backgroundTexture != nullptr)
	{
		// Draw the background twice to create seamless scrolling
		SDL_Rect firstBG = backgroundRect;
		SDL_Rect secondBG = backgroundRect;

		// For vertical scrolling:
		firstBG.y = (int)backgroundY;
		secondBG.y = (int)backgroundY - SCREEN_HEIGHT;  // Place second copy above the first

		// Draw both copies
		SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &firstBG);
		SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &secondBG);

		// Update the position
		backgroundY += scrollSpeed;

		// Reset when the first background is fully off screen
		if (backgroundY >= SCREEN_HEIGHT)
			backgroundY = 0;
	}

	// Then draw all game objects
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw();
	}
}

void Scene::start()
{
	backgroundTexture = loadTexture("gfx/background.png");
	if (backgroundTexture != nullptr)
	{
		backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		backgroundY = 0;
		scrollSpeed = 2;  
	}
	else
	{
		SDL_Log("Failed to load background texture");
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->start();
	}

	hasStarted = true;
}