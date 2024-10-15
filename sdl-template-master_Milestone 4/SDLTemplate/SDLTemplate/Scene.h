#pragma once
#include <vector>
#include "app.h"
#include <algorithm>
#include "draw.h"
#include "SDL.h"

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

private:
	bool hasStarted;
	static Scene* activeScene;
	SDL_Texture* texture;
	SDL_Texture* backgroundTexture;  
	SDL_Rect backgroundRect;
};

