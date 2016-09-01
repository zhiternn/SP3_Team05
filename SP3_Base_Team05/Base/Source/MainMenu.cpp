/******************************************************************************/
/*!
\file	MainMenu.h
\author Quek Tse Hiang Reuben
\par	email: 152773F\@nyp.edu.sg
\brief
Rendering Main Menu
*/
/******************************************************************************/
#include "MainMenu.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"

#include <sstream>

MainMenu::MainMenu() :
mainCamera(NULL),
sm(SceneManager::GetInstance()),
om(OptionManager::GetInstance()),
isHover(false),
isClicked(false),
continuetime(0),
MusicOn(true), 
SoundOn(true),
ControllerOn(false)
{
}

MainMenu::~MainMenu()
{
    if (Application::GetInstance().bgm_menu != NULL)
        Application::GetInstance().bgm_menu->drop();
    if (Application::GetInstance().menu_hover != NULL)
        Application::GetInstance().menu_hover->drop();
    if (Application::GetInstance().menu_click != NULL)
        Application::GetInstance().menu_click->drop();
}

void MainMenu::Init()
{
    SceneBase::Init();
    Math::InitRNG();

    //World Space
    m_worldHeight = 300;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //World Space View
    m_orthoHeight = 100;
    m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    mainCamera = new Camera();
    mainCamera->Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

    state = MENU_MAIN;

    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().bgm_menu, true);

    b_Play.Set(0, 25, 70, 80);
    b_Option.Set(0, 25, 55, 65);
    b_Credit.Set(0, 25, 40, 50);
    b_LevelUp.Set(0, 25, 25, 35);
    b_Exit.Set(0, 25, 10, 20);

    b_Back.Set(0, 25, 10, 20);
    b_Yes.Set(42.5, 62.5, 30, 40);
    b_No.Set(67.5, 87.5, 30, 40);
    //b_Continue.Set(117, 132, 12, 18);
    b_On.Set(90, 115, 43, 47);
    //upgradeMG.Set(45, 50, 47, 52);
    //upgradeSHG.Set(0, 25, 70, 80);
    //upgradeSPG.Set(0, 25, 70, 80);
    //29.6, 45

    upgradeGun.Set(45, 50, 47, 52);

    //upgradeFireRate, upgradeProjSpd, upgradeDmg;

	upgradeDmg.Set(98, 106, 47, 52);
	upgradeLifetime.Set(98, 106, 36, 42);
	upgradeProjSpd.Set(98, 106, 26, 32);
    
}

void MainMenu::Update(double dt)
{
    SceneBase::Update(dt);
    {//handles required mouse calculationsdouble x, y;
        double x, y;
        Application::GetCursorPos(x, y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        x = m_orthoWidth * (x / w);
        y = m_orthoHeight * ((h - y) / h);

        mousePos_screenBased.Set(x, y, 0);
        mousePos_worldBased.Set(
            x + mainCamera->target.x - (m_orthoWidth * 0.5f),
            y + mainCamera->target.y - (m_orthoHeight * 0.5f),
            0
            );
    }
    //-50 to 50 x, -65 to 65 y

    if (state == MENU_INSTRUCTIONS)
        continuetime += dt;

    if (MusicOn)
        Application::GetInstance().theSoundEngine->setSoundVolume(1.0f);
    else
        Application::GetInstance().theSoundEngine->setSoundVolume(0.0f);

    mainCamera->Update(dt);
}

void MainMenu::Render()
{
    Mtx44 perspective;
    //perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    perspective.SetToOrtho(-m_orthoWidth * 0.5f, m_orthoWidth * 0.5f, -m_orthoHeight * 0.5f, m_orthoHeight * 0.5f, -100, 100);
    projectionStack.LoadMatrix(perspective);

    // Camera matrix
    viewStack.LoadIdentity();
    viewStack.LookAt(
        mainCamera->position.x, mainCamera->position.y, mainCamera->position.z,
        mainCamera->target.x, mainCamera->target.y, mainCamera->target.z,
        mainCamera->up.x, mainCamera->up.y, mainCamera->up.z
        );
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();

    SetupLights();

    RenderMesh(meshList[GEO_AXES], false);

    //glUniform1f(m_parameters[U_FOG_ENABLED], 0);
    //RenderGPass();
    RenderMain();
    //glUniform1f(m_parameters[U_FOG_ENABLED], 0);

    SetHUD(true);
    RenderBG();
    switch (state)
    {
    case MainMenu::MENU_MAIN:
        MainMenuPage();
        break;
    case MainMenu::MENU_PLAY: 
		state = MENU_INSTRUCTIONS;
        break;
    case MainMenu::MENU_LOADLEVEL:
        LevelSelectPage();
        break;
    case MainMenu::MENU_INSTRUCTIONS: 
        InstructionPage();
        break;
    case MainMenu::MENU_OPTIONS: 
		OptionsPage();
        break;
    case MainMenu::MENU_CREDITS: 
		CreditsPage();
        break;
    case MainMenu::MENU_LEVEL: 
		LevelUpPage();
        break;
    case MainMenu::MENU_EXIT: 
		confirmExit();
        break;
	case MainMenu::MENU_WIN:
		WinPage();
		break;
	case MainMenu::MENU_LOSE:
		LosePage();
		break;
    default:
        break;
    }
	RenderHUD();
	SetHUD(false);
}

void MainMenu::RenderGPass()
{
    m_renderPass = RENDER_PASS_PRE;

    m_lightDepthFBO.BindForWriting();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_gPassShaderID);
    //These matrices should change when light position or direction changes
    if (lights[0].type == Light::LIGHT_DIRECTIONAL)
        m_lightDepthProj.SetToOrtho(-m_worldWidth * 0.5f, m_worldWidth * 0.5f, -m_worldHeight * 0.5f, m_worldHeight * 0.5f, 0, 100);
    else
        m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

    m_lightDepthView.SetToLookAt(lights[0].position.x,
        lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

    RenderWorld();
}

void MainMenu::RenderMain()
{
    m_renderPass = RENDER_PASS_MAIN;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_programID);
    //pass light depth texture
    m_lightDepthFBO.BindForReading(GL_TEXTURE8);
    glUniform1i(m_parameters[U_SHADOW_MAP], 8);

    RenderWorld();

    //RenderSkyPlane();
}

void MainMenu::RenderWorld()
{

}

void MainMenu::RenderBG()
{
    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.5) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 0);
    modelStack.Scale(100, 75, 0);
    RenderMesh(meshList[GEO_MENU_BACKGROUND], false);
    modelStack.PopMatrix();
}

void MainMenu::RenderHUD()
{
    // Render the crosshair
    modelStack.PushMatrix();
    modelStack.Translate((mousePos_screenBased.x + 2.5)* 80 / m_orthoWidth , (mousePos_screenBased.y - 2.5)* 60 / m_orthoHeight, 5);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_MENU_CURSOR], false);
    modelStack.PopMatrix();
    // 65 x, 50 y,

    std::ostringstream ss;
    ss.precision(5);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);
}

void MainMenu::MainMenuPage()
{
    //PLAY BUTTON
    float boxposscale = 0.6;
    float textposscale = 0.575;


    if ((mousePos_screenBased.x >= b_Play.minX && mousePos_screenBased.x <= b_Play.maxX
        && mousePos_screenBased.y >= b_Play.minY && mousePos_screenBased.y <= b_Play.maxY) || (mousePos_screenBased.x >= b_Option.minX && mousePos_screenBased.x <= b_Option.maxX
        && mousePos_screenBased.y >= b_Option.minY && mousePos_screenBased.y <= b_Option.maxY) || (mousePos_screenBased.x >= b_Credit.minX && mousePos_screenBased.x <= b_Credit.maxX
        && mousePos_screenBased.y >= b_Credit.minY && mousePos_screenBased.y <= b_Credit.maxY) || (mousePos_screenBased.x >= b_Exit.minX && mousePos_screenBased.x <= b_Exit.maxX
        && mousePos_screenBased.y >= b_Exit.minY && mousePos_screenBased.y <= b_Exit.maxY) || (mousePos_screenBased.x >= b_LevelUp.minX && mousePos_screenBased.x <= b_LevelUp.maxX
        && mousePos_screenBased.y >= b_LevelUp.minY && mousePos_screenBased.y <= b_LevelUp.maxY))
    {
        if (isHover == false)
        {
            Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_hover);
            isHover = true;
        }
        if (mousePos_screenBased.x >= b_Play.minX && mousePos_screenBased.x <= b_Play.maxX
            && mousePos_screenBased.y >= b_Play.minY && mousePos_screenBased.y <= b_Play.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Play";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));
                state = MENU_PLAY;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Play!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
        else
            RenderPlayButton();


        //OPTION BUTTON
        if (mousePos_screenBased.x >= b_Option.minX && mousePos_screenBased.x <= b_Option.maxX
            && mousePos_screenBased.y >= b_Option.minY && mousePos_screenBased.y <= b_Option.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Options";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));
                state = MENU_OPTIONS;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Options!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();


            }
        }
        else
            RenderOptionButton();

        //CREDIT BUTTON
        if (mousePos_screenBased.x >= b_Credit.minX && mousePos_screenBased.x <= b_Credit.maxX
            && mousePos_screenBased.y >= b_Credit.minY && mousePos_screenBased.y <= b_Credit.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Credits";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                state = MENU_CREDITS;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Credits!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        else
            RenderCreditButton();

        //Upgrade
        if (mousePos_screenBased.x >= b_LevelUp.minX && mousePos_screenBased.x <= b_LevelUp.maxX
            && mousePos_screenBased.y >= b_LevelUp.minY && mousePos_screenBased.y <= b_LevelUp.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Upgrade";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                state = MENU_LEVEL;
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Upgrade!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        else
            RenderLevelUpButton();

        //EXIT BUTTON
        if (mousePos_screenBased.x >= b_Exit.minX && mousePos_screenBased.x <= b_Exit.maxX
            && mousePos_screenBased.y >= b_Exit.minY && mousePos_screenBased.y <= b_Exit.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Exit";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

                state = MENU_EXIT;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Exit!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        else
            RenderExitButton();
    }
    else
    {
        isHover = false;
        isClicked = false;

        RenderPlayButton();
        RenderOptionButton();
        RenderCreditButton();
        RenderLevelUpButton();
        RenderExitButton();
    }
}

void MainMenu::LevelSelectPage()
{
    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.5) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 0);
    modelStack.Scale(100, 75, 0);
    RenderMesh(meshList[GEO_MENU_LVLSELECT], false);
    modelStack.PopMatrix();
    //PLAY BUTTON
    float boxposscale = 0.6;
    float textposscale = 0.575;


    if ((mousePos_screenBased.x >= b_Play.minX && mousePos_screenBased.x <= b_Play.maxX
        && mousePos_screenBased.y >= b_Play.minY && mousePos_screenBased.y <= b_Play.maxY) || (mousePos_screenBased.x >= b_Option.minX && mousePos_screenBased.x <= b_Option.maxX
        && mousePos_screenBased.y >= b_Option.minY && mousePos_screenBased.y <= b_Option.maxY) || (mousePos_screenBased.x >= b_Credit.minX && mousePos_screenBased.x <= b_Credit.maxX
        && mousePos_screenBased.y >= b_Credit.minY && mousePos_screenBased.y <= b_Credit.maxY) || (mousePos_screenBased.x >= b_Exit.minX && mousePos_screenBased.x <= b_Exit.maxX
        && mousePos_screenBased.y >= b_Exit.minY && mousePos_screenBased.y <= b_Exit.maxY) || (mousePos_screenBased.x >= b_LevelUp.minX && mousePos_screenBased.x <= b_LevelUp.maxX
        && mousePos_screenBased.y >= b_LevelUp.minY && mousePos_screenBased.y <= b_LevelUp.maxY))
    {
        if (isHover == false)
        {
            Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_hover);
            isHover = true;
        }
        if (mousePos_screenBased.x >= b_Play.minX && mousePos_screenBased.x <= b_Play.maxX
            && mousePos_screenBased.y >= b_Play.minY && mousePos_screenBased.y <= b_Play.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Golem";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                sm.ChangeScene(SCENE::SCENE_GOLEM);
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Golem!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Golem";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));

			modelStack.PushMatrix();
			modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.15)) * (60 / m_orthoHeight), 1);
			modelStack.Scale(25, 6, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();

			std::ostringstream ss3;
			ss3.precision(5);
			ss3 << "Menu";
			RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.125)) * (60 / m_orthoHeight));
        }

        //OPTION BUTTON
        if (mousePos_screenBased.x >= b_Option.minX && mousePos_screenBased.x <= b_Option.maxX
            && mousePos_screenBased.y >= b_Option.minY && mousePos_screenBased.y <= b_Option.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Snake";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                sm.ChangeScene(SCENE::SCENE_SNAKE);
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Snake!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();


            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss1;
            ss1.precision(5);
            ss1 << "Snake";
            RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));

			modelStack.PushMatrix();
			modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.15)) * (60 / m_orthoHeight), 1);
			modelStack.Scale(25, 6, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();

			std::ostringstream ss3;
			ss3.precision(5);
			ss3 << "Menu";
			RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.125)) * (60 / m_orthoHeight));
        }
        //CREDIT BUTTON
        if (mousePos_screenBased.x >= b_Credit.minX && mousePos_screenBased.x <= b_Credit.maxX
            && mousePos_screenBased.y >= b_Credit.minY && mousePos_screenBased.y <= b_Credit.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Summoner";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                state = MENU_CREDITS;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                sm.ChangeScene(SCENE::SCENE_SUMMONER);
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Summoner!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss2;
            ss2.precision(5);
            ss2 << "Summoner";
            RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));
        }

        //Detlaff
        if (mousePos_screenBased.x >= b_LevelUp.minX && mousePos_screenBased.x <= b_LevelUp.maxX
            && mousePos_screenBased.y >= b_LevelUp.minY && mousePos_screenBased.y <= b_LevelUp.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Detlaff";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                sm.ChangeScene(SCENE::SCENE_DETLAFF);
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Detlaff!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Detlaff";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));
        }

        //EXIT BUTTON
        if (mousePos_screenBased.x >= b_Exit.minX && mousePos_screenBased.x <= b_Exit.maxX
            && mousePos_screenBased.y >= b_Exit.minY && mousePos_screenBased.y <= b_Exit.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Menu";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }

                sm.ChangeScene(SCENE::SCENE_MENU);
				state = MENU_MAIN;
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(25, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Menu!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

            }
        }
        /*else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.15)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss3;
            ss3.precision(5);
            ss3 << "Menu";
            RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.125)) * (60 / m_orthoHeight));
        }*/
    }
    else
    {
        isHover = false;
        isClicked = false;

        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale + 0.15)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        ss.precision(5);
        ss << "Golem";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale + 0.15)) * (60 / m_orthoHeight));

        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss1;
        ss1.precision(5);
        ss1 << "Snake";
        RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale)) * (60 / m_orthoHeight));

        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.15)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss2;
        ss2.precision(5);
        ss2 << "Summoner";
        RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));


        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.3)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss4;
        ss4.precision(5);
        ss4 << "Detlaff";
        RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));


        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.15)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss3;
        ss3.precision(5);
        ss3 << "Menu";
        RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.125)) * (60 / m_orthoHeight));
    }
}

void MainMenu::InstructionPage()
{
    standardLayout();

    std::ostringstream ss0;
    ss0.precision(5);
    ss0 << "CONTROLS";
    RenderTextOnScreen(meshList[GEO_TEXT], ss0.str(), Color(0, 0, 0), 6, (m_orthoWidth * 0.3) * (80 / m_orthoWidth), ((m_orthoHeight * 0.8)) * (60 / m_orthoHeight));

    std::ostringstream ss;
    ss.precision(5);
    ss << "WASD to Move";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.1f) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 5) * (60 / m_orthoHeight));

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Right Button to Shield";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.1f) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5)) * (60 / m_orthoHeight));

    std::ostringstream ss3;
    ss3.precision(5);
    ss3 << "Left Button to Shoot";
    RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.1f) * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 5) * (60 / m_orthoHeight));

    std::ostringstream ss4;
    ss4.precision(5);
    ss4 << "Space Bar to Dash";
    RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.1f) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 10) * (60 / m_orthoHeight));

    std::ostringstream ss5;
    ss5.precision(5);
    ss5 << "Q/E to switch weapons";
    RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.1f) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 20) * (60 / m_orthoHeight));

    if (continuetime >= 0.5)
    {
        std::ostringstream ss6;
        ss6.precision(5);
        ss6 << "Click to Continue";
        RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 0, 0), 3, (m_orthoWidth * 0.25f) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 30) * (60 / m_orthoHeight));
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON) && !(mousePos_screenBased.x >= b_Back.minX && mousePos_screenBased.x <= b_Back.maxX
            && mousePos_screenBased.y >= b_Back.minY && mousePos_screenBased.y <= b_Back.maxY))
        {
            continuetime = 0;
            state = MENU_LOADLEVEL;
        }
    }
}

void MainMenu::OptionsPage()
{
    standardLayout();
    float LayoutScale = 0.1f;

    std::ostringstream ss;
    ss.precision(5);
    ss << "Music BGM";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 3) * (60 / m_orthoHeight));

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Sound VFX";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5 - 2)) * (60 / m_orthoHeight));

    std::ostringstream ss3;
    ss3.precision(5);
    ss3 << "Controller";
    RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 7) * (60 / m_orthoHeight));

    MusicBGMButton();
    MusicVFXButton();
    ControllerButton();
}

void MainMenu::CreditsPage()
{
    float LayoutScale = 0.1f;
    standardLayout();

    std::ostringstream ss0;
    ss0.precision(5);
    ss0 << "CREDITS";
    RenderTextOnScreen(meshList[GEO_TEXT], ss0.str(), Color(0, 0, 0), 6, (m_orthoWidth * 0.3) * (80 / m_orthoWidth), ((m_orthoHeight * 0.8)) * (60 / m_orthoHeight));

    std::ostringstream ss;
    ss.precision(5);
    ss << "Tang Zhi Tern";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 5) * (60 / m_orthoHeight));

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Reuben Quek";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5)) * (60 / m_orthoHeight));

    std::ostringstream ss3;
    ss3.precision(5);
    ss3 << "Lee Yu Xian";
    RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 5) * (60 / m_orthoHeight));

    std::ostringstream ss4;
    ss4.precision(5);
    ss4 << "Wayne Lee";
    RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 10) * (60 / m_orthoHeight));

    // admin no
    float LayoutNoScale = 0.65f;
    std::ostringstream ss5;
    ss5.precision(5);
    ss5 << "155134X";
    RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(0, 0, 0), 3, (m_orthoWidth * LayoutNoScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 5) * (60 / m_orthoHeight));

    std::ostringstream ss6;
    ss6.precision(5);
    ss6 << "152773F";
    RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 0, 0), 3, (m_orthoWidth * LayoutNoScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5)) * (60 / m_orthoHeight));

    std::ostringstream ss7;
    ss7.precision(5);
    ss7 << "155118A";
    RenderTextOnScreen(meshList[GEO_TEXT], ss7.str(), Color(0, 0, 0), 3, (m_orthoWidth * LayoutNoScale) * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 5) * (60 / m_orthoHeight));

    std::ostringstream ss8;
    ss8.precision(5);
    ss8 << "140189H";
    RenderTextOnScreen(meshList[GEO_TEXT], ss8.str(), Color(0, 0, 0), 3, (m_orthoWidth * LayoutNoScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 10) * (60 / m_orthoHeight));
    /*
     (155134X) (152773F) (155118A) (140189H)
    */
}

void MainMenu::LevelUpPage()
{
    standardLayout();

    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.35) * (80 / m_orthoWidth), (m_orthoHeight * 0.8) * (60 / m_orthoHeight), 1);
    modelStack.Scale(35, 10, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss;
    ss.precision(5);
    ss << "Upgrades";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 18, ((m_orthoHeight * 0.8) - 4)* (60 / m_orthoHeight));

    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.75) * (80 / m_orthoWidth), (m_orthoHeight * 0.8) * (60 / m_orthoHeight), 1);
    modelStack.Scale(20, 8, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Currency: " << player->inventory->GetCurrency();
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 1.8, 51, ((m_orthoHeight * 0.8) - 3)* (60 / m_orthoHeight));

    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.6) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
    modelStack.Scale(12, 12, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

	modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.6) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (58 / m_orthoHeight), 1);
	modelStack.Scale(10, 10, 2);

	ss.str("");
	ss.precision(5);
	
	switch (player->inventory->bullets[player->projectileIter]->proj_type)
	{
	case CProjectile::BULLET:
        RenderMesh(meshList[GEO_PROJECTILE_BULLET], false);
		ss << "Damage: ";
		break;
	case CProjectile::TRAP:
        RenderMesh(meshList[GEO_PROJECTILE_TRAP], false);
		ss << "Radius: ";
		break;
	case CProjectile::HOOK:
        RenderMesh(meshList[GEO_PROJECTILE_HOOK], false);
		ss << "Duration: ";
		break;
	default:break;
	}
	ss << player->inventory->bullets[player->projectileIter]->GetEffect();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate((m_orthoWidth * 0.63) * (80 / m_orthoWidth), (m_orthoHeight * 0.325) * (60 / m_orthoHeight), 1);
	modelStack.Scale(20, 12, 2);
	RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, (m_orthoWidth * 0.55) * (80 / m_orthoWidth), (m_orthoHeight * 0.33) * (60 / m_orthoHeight));
	ss.str("");
	ss.precision(5);
	ss << "Lifetime: " << player->inventory->bullets[player->projectileIter]->GetLifetime();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, (m_orthoWidth * 0.55) * (80 / m_orthoWidth), (m_orthoHeight * 0.29) * (60 / m_orthoHeight));
	ss.str("");
	ss.precision(5);
	ss << "Speed: " << player->inventory->bullets[player->projectileIter]->GetProjectileSpeed();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, (m_orthoWidth * 0.55) * (80 / m_orthoWidth), (m_orthoHeight * 0.25) * (60 / m_orthoHeight));

    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.25) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
    modelStack.Scale(12, 12, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.25) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
    modelStack.Scale(10, 10, 2);
	switch (player->inventory->weapons[player->weaponIter]->weapon_type)
	{
	case Weapon::W_MACHINEGUN:
		RenderMesh(meshList[GEO_WEAPON_MACHINEGUN], false);
		break;
	case Weapon::W_SHOTGUN:
		RenderMesh(meshList[GEO_WEAPON_SHOTGUN], false);
		break;
	case Weapon::W_SPLITGUN:
		RenderMesh(meshList[GEO_WEAPON_SPLITGUN], false);
		break;
	default:break;
	}
    modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate((m_orthoWidth * 0.28) * (80 / m_orthoWidth), (m_orthoHeight * 0.37) * (60 / m_orthoHeight), 1);
	modelStack.Scale(22, 4, 2);
	RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
	modelStack.PopMatrix();

	ss.str("");
	ss.precision(5);
	ss << "Firerate: " << player->inventory->weapons[player->weaponIter]->GetFireRate();
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, (m_orthoWidth * 0.18) * (80 / m_orthoWidth), (m_orthoHeight * 0.35) * (60 / m_orthoHeight));
	modelStack.PopMatrix();

        if (mousePos_screenBased.x >= upgradeGun.minX && mousePos_screenBased.x <= upgradeGun.maxX
            && mousePos_screenBased.y >= upgradeGun.minY && mousePos_screenBased.y <= upgradeGun.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.37) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(5, 5, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
					if (player->inventory->IsDeducting(1000))
					{
						switch (player->inventory->weapons[player->weaponIter]->weapon_type)
						{
						case Weapon::W_MACHINEGUN:
							UpgradeMG();
							break;
						case Weapon::W_SHOTGUN:
							UpgradeSHG();
							break;
						case Weapon::W_SPLITGUN:
							UpgradeSPG();
							break;
						default:break;
						}
					}
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.37) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(6, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.37) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(10, 10, 2);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate((m_orthoWidth * 0.37) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
			modelStack.Scale(5, 5, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();
		}

        // Projectile Effect
		if (mousePos_screenBased.x >= upgradeDmg.minX && mousePos_screenBased.x <= upgradeDmg.maxX
			&& mousePos_screenBased.y >= upgradeDmg.minY && mousePos_screenBased.y <= upgradeDmg.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(5, 5, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
				if (player->inventory->IsDeducting(1000))
				{
					UpgradeDmg();
				}
            }
            else
            {
                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(6, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(10, 10, 2);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
			modelStack.Scale(5, 5, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();
		}

        // Projectile Lifetime
		if (mousePos_screenBased.x >= upgradeLifetime.minX && mousePos_screenBased.x <= upgradeLifetime.maxX
			&& mousePos_screenBased.y >= upgradeLifetime.minY && mousePos_screenBased.y <= upgradeLifetime.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.4) * (60 / m_orthoHeight), 1);
                modelStack.Scale(5, 5, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
				if (player->inventory->IsDeducting(1000))
				{
					UpgradeLifetime();
				}
			}
            else
            {
                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.4) * (60 / m_orthoHeight), 1);
                modelStack.Scale(6, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                modelStack.PushMatrix();
				modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.4) * (60 / m_orthoHeight), 1);
                modelStack.Scale(10, 10, 2);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.4) * (60 / m_orthoHeight), 1);
			modelStack.Scale(5, 5, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();
		}

        // Projectile Speed Button
		if (mousePos_screenBased.x >= upgradeProjSpd.minX && mousePos_screenBased.x <= upgradeProjSpd.maxX
			&& mousePos_screenBased.y >= upgradeProjSpd.minY && mousePos_screenBased.y <= upgradeProjSpd.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.3) * (60 / m_orthoHeight), 1);
                modelStack.Scale(5, 5, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();
        
                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
				if (player->inventory->IsDeducting(1000))
				{
					UpgradeProjSpd();
				}
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.3) * (60 / m_orthoHeight), 1);
                modelStack.Scale(6, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();
        
                modelStack.PushMatrix();
                modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.3) * (60 / m_orthoHeight), 1);
                modelStack.Scale(10, 10, 2);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate((m_orthoWidth * 0.8) * (80 / m_orthoWidth), (m_orthoHeight * 0.3) * (60 / m_orthoHeight), 1);
			modelStack.Scale(5, 5, 2);
			RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
			modelStack.PopMatrix();
		}
    
    /*
    upgradeMG.Set(29, 30, 44, 46);
    upgradeSHG.Set(49, 50, 44, 46);
    upgradeSPG.Set(69, 70, 44, 46);
    */
}

void MainMenu::WinPage()
{
	float LayoutScale = 0.1f;
	standardLayout();

	std::ostringstream ss;
	ss.precision(5);
	ss << "YOU WIN!";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 5, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 3) * (60 / m_orthoHeight));

	if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
	{
		state = MENU_MAIN;
	}
}

void MainMenu::LosePage()
{
	float LayoutScale = 0.1f;
	standardLayout();

	std::ostringstream ss;
	ss.precision(5);
	ss << "YOU JUST GOT DESTROYED!";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 5, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 3) * (60 / m_orthoHeight));

	if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
	{
		state = MENU_MAIN;
	}
}

void MainMenu::UpgradeMG()
{
    player->inventory->weapons[player->weaponIter]->SetFireRate(player->inventory->weapons[player->weaponIter]->GetFireRate() + 0.8);
}

void MainMenu::UpgradeSHG()
{
    player->inventory->weapons[player->weaponIter]->SetFireRate(player->inventory->weapons[player->weaponIter]->GetFireRate() + 0.35);
}

void MainMenu::UpgradeSPG()
{
    player->inventory->weapons[player->weaponIter]->SetFireRate(player->inventory->weapons[player->weaponIter]->GetFireRate() + 0.7);
}

void MainMenu::UpgradeLifetime()
{
    player->inventory->bullets[player->projectileIter]->SetLifetime(player->inventory->bullets[player->projectileIter]->GetLifetime() + 1);
}
void MainMenu::UpgradeProjSpd()
{
	player->inventory->bullets[player->projectileIter]->SetProjectileSpeed(player->inventory->bullets[player->projectileIter]->GetProjectileSpeed() + 5);
}
void MainMenu::UpgradeDmg()
{
    player->inventory->bullets[player->projectileIter]->UpgradeEffect(2);
}

void MainMenu::standardLayout()
{
    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.5) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 0);
    modelStack.Scale(75, 60, 0);
    RenderMesh(meshList[GEO_MENU_FRAME], false);
    modelStack.PopMatrix();

    //BACK BUTTON
    if (mousePos_screenBased.x >= b_Back.minX && mousePos_screenBased.x <= b_Back.maxX
        && mousePos_screenBased.y >= b_Back.minY && mousePos_screenBased.y <= b_Back.maxY)
    {
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
        {
            //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * 0.15) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Back";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * 0.125) * (60 / m_orthoHeight));

            state = MENU_MAIN;
            continuetime = 0;

            if (isClicked == false)
            {
                Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                isClicked = true;
            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * 0.15) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 7, 3);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Back!";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * 0.125) * (60 / m_orthoHeight));

            modelStack.PushMatrix();
            modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * 0.15) * (60 / m_orthoHeight), 1);
            modelStack.Scale(26, 15, 15);
            RenderMesh(meshList[GEO_MENU_SELECTION], false);
            modelStack.PopMatrix();

            if (isHover == false)
            {
                Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_hover);
                isHover = true;
            }

        }
    }
    else
    {
        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * 0.15) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        ss.precision(5);
        ss << "Back";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * 0.125) * (60 / m_orthoHeight));
        isHover = false;
        isClicked = false;
    }
}

void MainMenu::confirmExit()
{
    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.5) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
    modelStack.Scale(35, 10, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss;
    ss.precision(5);
    ss << "Confirm Exit?";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.5, 25, ((m_orthoHeight * 0.5) - 2)* (60 / m_orthoHeight));

    if ((mousePos_screenBased.x >= b_Yes.minX && mousePos_screenBased.x <= b_Yes.maxX && mousePos_screenBased.y >= b_Yes.minY && mousePos_screenBased.y <= b_Yes.maxY)
        || (mousePos_screenBased.x >= b_No.minX && mousePos_screenBased.x <= b_No.maxX && mousePos_screenBased.y >= b_No.minY && mousePos_screenBased.y <= b_No.maxY))
    {
        //Yes
        if (mousePos_screenBased.x >= b_Yes.minX && mousePos_screenBased.x <= b_Yes.maxX && mousePos_screenBased.y >= b_Yes.minY && mousePos_screenBased.y <= b_Yes.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) - 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 5)* (60 / m_orthoHeight), 1);
                modelStack.Scale(12, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Yes";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 27, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));

                // Quits the program
                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                exit(0);
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) - 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
                modelStack.Scale(15, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "Yes!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 27, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) - 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
                modelStack.Scale(18, 9, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

                if (isHover == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_hover);
                    isHover = true;
                }
            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(((m_orthoWidth * 0.5) - 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
            modelStack.Scale(12, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Yes";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 27, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));
        }

        //No
        if (mousePos_screenBased.x >= b_No.minX && mousePos_screenBased.x <= b_No.maxX && mousePos_screenBased.y >= b_No.minY && mousePos_screenBased.y <= b_No.maxY)
        {
            if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
            {
                //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) + 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
                modelStack.Scale(12, 6, 2);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "No";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));

                // Returns to Main Menu
                state = MENU_MAIN;

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
            }
            else
            {
                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) + 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
                modelStack.Scale(15, 7, 3);
                RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
                modelStack.PopMatrix();

                std::ostringstream ss;
                ss.precision(5);
                ss << "No!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(((m_orthoWidth * 0.5) + 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
                modelStack.Scale(18, 9, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();

                if (isHover == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_hover);
                    isHover = true;
                }
            }
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(((m_orthoWidth * 0.5) + 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
            modelStack.Scale(12, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "No";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));
        }
    }
    else
    {
        isHover = false;
        isClicked = false;
        modelStack.PushMatrix();
        modelStack.Translate(((m_orthoWidth * 0.5) - 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
        modelStack.Scale(12, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        ss.precision(5);
        ss << "Yes";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 27, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));

        modelStack.PushMatrix();
        modelStack.Translate(((m_orthoWidth * 0.5) + 15)* (80 / m_orthoWidth), ((m_orthoHeight * 0.5) - 15)* (60 / m_orthoHeight), 1);
        modelStack.Scale(12, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss1;
        ss1.precision(5);
        ss1 << "No";
        RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 45, ((m_orthoHeight * 0.5) - 17) * (60 / m_orthoHeight));
    }
}

void MainMenu::RenderPlayButton()
{
    modelStack.PushMatrix();
    modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * 0.75) * (60 / m_orthoHeight), 1);
    modelStack.Scale(25, 6, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss;
    ss.precision(5);
    ss << "Play";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * 0.725) * (60 / m_orthoHeight));
}

void MainMenu::RenderOptionButton()
{
    modelStack.PushMatrix();
    modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.6)) * (60 / m_orthoHeight), 1);
    modelStack.Scale(25, 6, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss1;
    ss1.precision(5);
    ss1 << "Options";
    RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.575)) * (60 / m_orthoHeight));
}

void MainMenu::RenderCreditButton()
{
    modelStack.PushMatrix();
    modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.45)) * (60 / m_orthoHeight), 1);
    modelStack.Scale(25, 6, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Credits";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.425)) * (60 / m_orthoHeight));
}

void MainMenu::RenderLevelUpButton()
{
    modelStack.PushMatrix();
    modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.3)) * (60 / m_orthoHeight), 1);
    modelStack.Scale(25, 6, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Upgrade";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.275)) * (60 / m_orthoHeight));
}

void MainMenu::RenderExitButton()
{
    modelStack.PushMatrix();
    modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (0.15)) * (60 / m_orthoHeight), 1);
    modelStack.Scale(25, 6, 2);
    RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
    modelStack.PopMatrix();

    std::ostringstream ss3;
    ss3.precision(5);
    ss3 << "Exit";
    RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 2, 2, (m_orthoHeight * (0.125)) * (60 / m_orthoHeight));
}

void MainMenu::MusicBGMButton()
{
    float boxposscale = 0.6;
    float textposscale = 0.575;

    // Music BGM Yes No
    if (mousePos_screenBased.x >= b_On.minX && mousePos_screenBased.x <= b_On.maxX
        && mousePos_screenBased.y >= 57 && mousePos_screenBased.y <= 61)
    {
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
        {
            if (MusicOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                MusicOn = false;
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                MusicOn = true;
            }
            //, SoundOn, ControllerOn;
        }
        else
        {
            if (MusicOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5 + 5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5 + 5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
    }
    else
    {
        if (MusicOn == true)
        {
            std::ostringstream ss;
            ss.precision(5);
            ss << "On";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));
        }
        else
        {
            std::ostringstream ss2;
            ss2.precision(5);
            ss2 << "Off";
            RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.05)) * (60 / m_orthoHeight));
        }
    }
}

void MainMenu::MusicVFXButton()
{
    float boxposscale = 0.6;
    float textposscale = 0.575;
    //.Set(90, 115, 40, 50);

    // Controller Yes No
    if (mousePos_screenBased.x >= b_On.minX && mousePos_screenBased.x <= b_On.maxX
        && mousePos_screenBased.y >= 50 && mousePos_screenBased.y <= 54)
    {
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
        {
            if (SoundOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                SoundOn = false;
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                SoundOn = true;
            }
            //, SoundOn, ControllerOn;
        }
        else
        {
            if (SoundOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
    }
    else
    {
        if (SoundOn == true)
        {
            std::ostringstream ss;
            ss.precision(5);
            ss << "On";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));
        }
        else
        {
            std::ostringstream ss2;
            ss2.precision(5);
            ss2 << "Off";
            RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.1)) * (60 / m_orthoHeight));
        }
    }
}

void MainMenu::ControllerButton()
{
    float boxposscale = 0.6;
    float textposscale = 0.575;


    // Controller Yes No
    if (mousePos_screenBased.x >= b_On.minX && mousePos_screenBased.x <= b_On.maxX
        && mousePos_screenBased.y >= b_On.minY && mousePos_screenBased.y <= b_On.maxY)
    {
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
        {
            if (ControllerOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                ControllerOn = false;
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                if (isClicked == false)
                {
                    Application::GetInstance().theSoundEngine->play2D(Application::GetInstance().menu_click);
                    isClicked = true;
                }
                ControllerOn = true;
            }
			om.SetControl(ControllerOn);
            //, SoundOn, ControllerOn;
        }
        else
        {
            if (ControllerOn == true)
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "On";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
            else
            {
                std::ostringstream ss;
                ss.precision(5);
                ss << "Off";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

                modelStack.PushMatrix();
                modelStack.Translate(105 * (80 / m_orthoWidth), (m_orthoHeight * 0.5 - 5) * (60 / m_orthoHeight), 1);
                modelStack.Scale(26, 15, 15);
                RenderMesh(meshList[GEO_MENU_SELECTION], false);
                modelStack.PopMatrix();
            }
        }
    }
    else
    {
        if (ControllerOn == true)
        {
            std::ostringstream ss;
            ss.precision(5);
            ss << "On";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));
        }
        else
        {
            std::ostringstream ss2;
            ss2.precision(5);
            ss2 << "Off";
            RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 4, 60, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));
        }
    }
}

void MainMenu::Exit()
{
    SceneBase::Exit();
}

void MainMenu::SetState(MENU_STATE m_state)
{
	this->state = m_state;
}