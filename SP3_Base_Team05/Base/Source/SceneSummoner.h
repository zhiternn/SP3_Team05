#ifndef SCENE_SUMMONER_H
#define SCENE_SUMMONER_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "Summoner.h"
#include "Summons.h"

#include <vector>

using std::vector;

static int NUMBER_OF_WORLD_OBJECTS = 25;

class SceneManager;

class SceneSummoner : public SceneBase
{
public:
	SceneSummoner();
	~SceneSummoner();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void PlayerController(double dt);
	virtual void GetGamePadInput(double dt);
	virtual void RenderGameObjects();
	virtual void RenderHUD();
	virtual void UpdateGameObjects(double dt);

	void RenderGPass();
	void RenderMain();
	void RenderWorld();

	void RenderSkybox();
	void RenderSkyPlane();

private:
	Summoner* summoner;

	SceneManager &manager;
};

#endif