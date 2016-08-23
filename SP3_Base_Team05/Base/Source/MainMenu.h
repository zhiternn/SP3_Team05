#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"

#include <vector>

using std::vector;

class MainMenu : public SceneBase
{
public:
    MainMenu();
    ~MainMenu();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    void RenderGPass();
    void RenderMain();
    void RenderWorld();
    void RenderHUD();


private:

    float m_worldWidth, m_worldHeight;
    float m_orthoWidth, m_orthoHeight;

    Camera* mainCamera;

    Vector3 mousePos_screenBased;
    Vector3 mousePos_worldBased;
};

#endif