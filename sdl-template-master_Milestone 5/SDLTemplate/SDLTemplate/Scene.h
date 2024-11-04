#pragma once
#include <vector>
#include "app.h"
#include <algorithm>
#include "draw.h"
#include "SDL.h"
#include "PowerUp.h" 

extern App app;
class GameObject;

class Scene
{
public:
	Scene();
	~Scene();

	static void setActiveScene(Scene* scene);
	static Scene* getActiveScene();

	void addGameObject(GameObject* obj);
	void removeGameObject(GameObject* obj);

	virtual void start();
	virtual void update();
	virtual void draw();

protected:
	std::vector <GameObject*> objects;

	// background related members
	SDL_Texture* backgroundTexture = nullptr;
	SDL_Rect backgroundRect;

	float backgroundY;
	float scrollSpeed;

private:
	bool hasStarted;
	static Scene* activeScene;
	SDL_Texture* texture;
};