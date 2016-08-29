#ifndef SCENE_SNAKE_H
#define SCENE_SNAKE_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "SceneManager.h"

#include <vector>

using std::vector;

class SceneManager;
class SceneSnakeBoss : public SceneBase
{
public:
	SceneSnakeBoss();
	~SceneSnakeBoss();

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
	//void RenderMinimap(float zoom);

	void RenderSkybox();
	void RenderSkyPlane();

private:
	SceneManager &manager;

};

#endif