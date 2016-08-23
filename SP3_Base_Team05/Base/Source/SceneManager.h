#pragma once
#include "Scene.h"
class SceneManager
{

private:
	Scene *currentScene;
public:
	SceneManager();
	~SceneManager();
	
	Scene* GetCurrentScene();
	void SetScene(Scene *scene);

	//Cleanup Scene
	void Clean();

	//Initialises the Scene
	void Init();
};

