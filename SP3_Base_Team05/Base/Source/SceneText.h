#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Projectile.h"
#include "Player.h"
#include "Weapon.h"
#include "Enemy.h"

#include <vector>

using std::vector;

class SceneText : public SceneBase
{
	enum GEOMETRY_TYPE
	{
		NUM_GEOMETRY = GEOMETRY_TYPE_DEFAULT::GEO_DEFAULT_END
	};
public:
	SceneText();
	~SceneText();

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

private:
	void PlayerController(double dt);
	void UpdateGameObjects(double dt);
	void RenderGameObjects();
	void RenderGO(GameObject* go);

	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;

	Mesh* meshList[GEO_DEFAULT_END];

	Camera* mainCamera;

	GameObject* m_ghost;

	Player* player;

    Weapon* weapon;

	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	Enemy* enemy;
};

#endif