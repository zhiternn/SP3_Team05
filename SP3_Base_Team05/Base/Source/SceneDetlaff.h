#ifndef SCENE_DETLAFF_H
#define SCENE_DETLAFF_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "Detlaff.h"
#include "SceneManager.h"
#include "OptionManager.h"

#include <vector>

using std::vector;
class SceneManager;

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
	void RenderMinimap(float zoom);

	void RenderSkybox();
	void RenderSkyPlane();

	float enemyFireDelay;
	float enemyMovementDelay;

	virtual void PlayerController(double dt);
	virtual void GetGamePadInput(double dt);
	virtual void UpdateGameObjects(double dt);
	virtual void RenderGameObjects();
	virtual void RenderGO(GameObject* go);

private:
	CDetlaff* detlaff;

	SceneManager &manager;

};

#endif