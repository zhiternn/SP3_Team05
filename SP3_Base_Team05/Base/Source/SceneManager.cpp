#include "SceneManager.h"

SceneManager::SceneManager() : currentScene(NULL)
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
		Clean();
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
		//currentScene = new SceneDetlaff();
		break;
	}
	}
	
	currentScene->Init();
	currentScene->Render();
}

void SceneManager::SetScene(Scene* scene)
{
	this->currentScene = scene;
}

Scene* SceneManager::GetScene()
{
	return this->currentScene;
}

void SceneManager::Init()
{
	currentScene->Init();
}

void SceneManager::Clean()
{
	currentScene->Exit();
	//delete currentScene;
}