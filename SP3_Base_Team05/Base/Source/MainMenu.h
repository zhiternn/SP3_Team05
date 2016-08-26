#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "OptionManager.h"

#include "ButtonPosition.h"

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
    void LevelSelectPage();

    void confirmExit();
    void standardLayout();

    void RenderPlayButton();
    void RenderOptionButton();
    void RenderCreditButton();
    void RenderExitButton();

    void MusicBGMButton();
    void MusicVFXButton();
    void ControllerButton();

private:

    float m_worldWidth, m_worldHeight;
    float m_orthoWidth, m_orthoHeight;

    Camera* mainCamera;

    Vector3 mousePos_screenBased;
    Vector3 mousePos_worldBased;

    MENU_STATE state;
    SceneManager &sm;
	OptionManager &om;

    bool isHover, isClicked;
    float continuetime;

    ButtonPosition b_Play;
    ButtonPosition b_Continue;
    ButtonPosition b_Option;
    ButtonPosition b_Credit;
    ButtonPosition b_Exit;
    ButtonPosition b_Back;
    ButtonPosition b_Yes;
    ButtonPosition b_No;

    ButtonPosition b_On;
    ButtonPosition b_Off;

    bool MusicOn, SoundOn, ControllerOn;
};

#endif