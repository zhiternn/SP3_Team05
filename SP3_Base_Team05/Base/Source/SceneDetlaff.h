#ifndef SCENE_DETLAFF_H
#define SCENE_DETLAFF_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "Detlaff.h"

#include <vector>

using std::vector;

class SceneDetlaff : public SceneBase
{
public:
	SceneDetlaff();
	~SceneDetlaff();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGPass();
	void RenderMain();
	void RenderWorld();
	void RenderHUD();

	void RenderSkybox();
	void RenderSkyPlane();

	float enemyFireDelay;
	float hehexd;


private:

	void PlayerController(double dt);
	void UpdateGameObjects(double dt);
	void RenderGameObjects();
	void RenderGO(GameObject* go);

	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;

	Camera* mainCamera;

	Player* player;

	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	CDetlaff* detlaff;
};

#endif