#ifndef SCENE_GOLEM_H
#define SCENE_GOLEM_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "GolemHead.h"
#include "GolemHand.h"
#include "Summoner.h"

#include <vector>

using std::vector;

class SceneManager;
class SceneGolem : public SceneBase
{
public:
    SceneGolem();
    ~SceneGolem();

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
    GolemHead* golemhead;
    GolemLeftHand* golemlhead;
    GolemRightHand* golemrhead;

	SceneManager &manager;
};

#endif