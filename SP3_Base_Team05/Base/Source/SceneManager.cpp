#include "SceneManager.h"
#include "SceneDetlaff.h"

SceneManager::SceneManager()
{
	currentScene = NULL;
}


SceneManager::~SceneManager()
{
	if (currentScene)
	{
		delete currentScene;
		currentScene = NULL;
	}
		
}

Scene* SceneManager::GetCurrentScene()
{
	return this->currentScene;
}

void SceneManager::SetScene(Scene* scene)
{
	currentScene = scene;
}

//Clean Scene
void SceneManager::Clean()
{
	currentScene->Exit();
	delete currentScene;
}

//Initializes the Scene
void SceneManager::Init()
{
	currentScene->Init();
}