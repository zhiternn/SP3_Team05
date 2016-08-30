#include "SceneManager.h"

SceneManager::SceneManager() : currentScene(NULL)
{
}


SceneManager::~SceneManager()
{
	
}
//Clean Scene
void SceneManager::ChangeScene(SCENE scene)
{

	if (currentScene != NULL)
	{
		Clean();
	}

	switch (scene)
	{
	case SCENE::SCENE_GOLEM:
	{
		currentScene = new SceneGolem();
		break;
	}
	case SCENE::SCENE_SNAKE:
	{
		currentScene = new SceneSnakeBoss();
		break;
	}
	case SCENE::SCENE_SUMMONER:
	{
		currentScene = new SceneSummoner();
		break;
	}
	case SCENE::SCENE_DETLAFF:
	{
		currentScene = new SceneDetlaff();
		break;
	}
	case SCENE::SCENE_MENU:
	{
		currentScene = new MainMenu();
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