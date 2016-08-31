#pragma once
#include "Scene.h"
#include "SceneDetlaff.h"
#include "SceneGolem.h"
#include "SceneSnakeBoss.h"
#include "SceneSummoner.h"
#include "MainMenu.h"

enum SCENE
{
	SCENE_GOLEM,
	SCENE_SNAKE,
	SCENE_SUMMONER,
	SCENE_DETLAFF,
	SCENE_MENU,
	NUM_SCENES
};

class SceneManager
{
	
public:
	SceneManager();
	~SceneManager();
	
	SCENE SceneToLoad;

	Scene* currentScene;
	static SceneManager& GetInstance()
	{
		static SceneManager sm;
		return sm;
	}
	void ChangeScene(SCENE s);
	void SetScene(Scene *scene);
	Scene* GetScene();

	void Init();
	void Clean();
	void Delete();
};

