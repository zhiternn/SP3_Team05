#include "SceneManager.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	
}
//Clean Scene
void SceneManager::ChangeScene(int num)
{
	if (currentScene != NULL)
	{
		currentScene->Exit();
		delete currentScene;
	}

	switch (num)
	{
	case 1:
	{
		currentScene = new SceneGolem();
		break;
	}
	case 2:
	{
		currentScene = new SceneSnakeBoss();
		break;
	}
	case 3:
	{
		currentScene = new SceneSummoner();
		break;
	}
	case 4:
	{
		currentScene = new SceneDetlaff();
		break;
	}
	}
	
	currentScene->Init();
}

void SceneManager::SetScene(Scene* scene)
{
	this->currentScene = scene;
}

Scene* SceneManager::GetScene()
{
	return this->currentScene;
}