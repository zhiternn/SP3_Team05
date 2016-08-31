#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "SceneBase.h"
#include "Camera.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "OptionManager.h"

#include "ButtonPosition.h"
#include "Inventory.h"
#include "WeaponList.h"
#include "ProjectileList.h"

#include <vector>

using std::vector;

class MainMenu : public SceneBase
{
public:
    MainMenu();
    ~MainMenu();

	enum MENU_STATE
	{
		MENU_MAIN = 0,
		MENU_PLAY,
		MENU_LOADLEVEL,
		MENU_INSTRUCTIONS,
		MENU_OPTIONS,
		MENU_CREDITS,
		MENU_LEVEL,
		MENU_WIN,
		MENU_LOSE,
		MENU_EXIT,

		MENU_TOTAL
	};

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
    void LevelUpPage();
    void LevelSelectPage();
	void WinPage();
	void LosePage();

    void confirmExit();
    void standardLayout();

    void RenderPlayButton();
    void RenderOptionButton();
    void RenderCreditButton();
    void RenderLevelUpButton();
    void RenderExitButton();

    void MusicBGMButton();
    void MusicVFXButton();
    void ControllerButton();

	void ContinueButton();

    void UpgradeMG();
    void UpgradeSHG();
	void UpgradeSPG();

    void UpgradeLifetime();
    void UpgradeProjSpd();
    void UpgradeDmg();

	void SetState(MENU_STATE state);
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

    ButtonPosition b_Play, b_Continue, b_Option, b_Credit, b_LevelUp, b_Exit, b_Back, b_Yes, b_No, b_On, b_Off, upgradeGun, upgradeFireRate, upgradeProjSpd, upgradeDmg;

    bool MusicOn, SoundOn, ControllerOn;
};

#endif