#pragma once
#include "Scene.h"
#include "SceneDetlaff.h"
#include "SceneGolem.h"
#include "SceneSnakeBoss.h"
#include "SceneSummoner.h"

class SceneManager
{
	
public:
	SceneManager();
	~SceneManager();
	
	Scene* currentScene;
	static SceneManager& GetInstance()
	{
		static SceneManager sm;
		return sm;
	}
	void ChangeScene(int num);
	void SetScene(Scene *scene);
	Scene* GetScene();
};

