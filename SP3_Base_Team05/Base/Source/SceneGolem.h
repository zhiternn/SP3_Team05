#ifndef SCENE_GOLEM_H
#define SCENE_GOLEM_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "WeaponList.h"
#include "ProjectileList.h"
#include "SnakeHead.h"
#include "Summoner.h"

#include "GolemHead.h"
#include "GolemHand.h"

#include <vector>

using std::vector;

class SceneGolem : public SceneBase
{
    enum GEOMETRY_TYPE
    {
        NUM_GEOMETRY = GEOMETRY_TYPE_DEFAULT::GEO_DEFAULT_END
    };
public:
    SceneGolem();
    ~SceneGolem();

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

    Player* player;

    Vector3 mousePos_screenBased;
    Vector3 mousePos_worldBased;

    SnakeHead* enemy;
    Summoner* summoner;

    GolemHead* golemhead;
    GolemLeftHand* golemlhead;
    GolemRightHand* golemrhead;
};

#endif