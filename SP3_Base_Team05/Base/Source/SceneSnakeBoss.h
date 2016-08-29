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

	void RenderGPass();
	void RenderMain();
	void RenderWorld();
	void RenderHUD();
	void RenderMinimap(float zoom);

	void RenderSkybox();
	void RenderSkyPlane();

private:

	void PlayerController(double dt);
	void GetGamePadInput(double dt);
	void UpdateGameObjects(double dt);
	void RenderGameObjects();

	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;

	Camera* mainCamera;

	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	SceneManager &manager;
};

#endif