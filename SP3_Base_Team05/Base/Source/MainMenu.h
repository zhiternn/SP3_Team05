#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "SceneManager.h"

#include <vector>

using std::vector;

class MainMenu : public SceneBase
{
    enum MENU_STATE
    {
        MENU_MAIN = 0,
        MENU_PLAY,
        MENU_LOADLEVEL,
        MENU_INSTRUCTIONS,
        MENU_OPTIONS,
        MENU_CREDITS,
        MENU_EXIT,

        MENU_TOTAL
    };
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
    void RenderBG();

    void MainMenuPage();
    void InstructionPage();
    void HelpPage();
    void OptionsPage();
    void CreditsPage();

    void confirmExit();
    void standardLayout();

private:

    float m_worldWidth, m_worldHeight;
    float m_orthoWidth, m_orthoHeight;

    Camera* mainCamera;

    Vector3 mousePos_screenBased;
    Vector3 mousePos_worldBased;

    MENU_STATE state;
    SceneManager *sm;

    float delaytime;

    float helpBtnspd;
    float optBtnspd;
    float credBtnspd;

    float loadDown;
    float loadUp;
};

#endif