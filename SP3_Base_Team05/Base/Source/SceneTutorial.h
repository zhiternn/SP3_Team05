#ifndef SCENE_TUTORIAL_H
#define SCENE_TUTORIAL_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "SceneManager.h"

#include <vector>
#include <sstream>

using std::vector;

class SceneManager;
class SceneTutorial : public SceneBase
{
public:
	SceneTutorial();
	~SceneTutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGPass();
	void RenderMain();
	void RenderWorld();
	void RenderHUD();
	void RenderMinimap(float zoom);

	void RenderSkybox();
	void RenderSkyPlane();

private:
	bool pauseGame;
	bool weaponChecks[Weapon::W_END];
	bool projChecks[CProjectile::MAX];
	bool canExit;
	std::ostringstream tutorialLines;
	bool firstTimeKill;
	bool firstTimeCapture;

	virtual void PlayerController(double dt);
	virtual void GetGamePadInput(double dt);
	virtual void UpdateGameObjects(double dt);
	virtual void RenderGameObjects();

	Enemy* enemy;

	SceneManager &manager;
};

#endif