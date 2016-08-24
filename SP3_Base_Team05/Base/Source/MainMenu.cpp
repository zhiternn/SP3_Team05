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
mainCamera(NULL)
{
}

MainMenu::~MainMenu()
{
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

    sm = new SceneManager();

    state = MENU_MAIN;
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
    //std::cout << mousePos_worldBased << std::endl;
    //-50 to 50 x, -65 to 65 y

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
    case MainMenu::MENU_PLAY: state = MENU_INSTRUCTIONS; /*MENU_LOADLEVEL*/;
        break;
    case MainMenu::MENU_LOADLEVEL: //sm->ChangeScene(1);
        break;
    case MainMenu::MENU_INSTRUCTIONS: InstructionPage();
        break;
    case MainMenu::MENU_OPTIONS: OptionsPage();
        break;
    case MainMenu::MENU_CREDITS: CreditsPage();
        break;
    case MainMenu::MENU_EXIT: confirmExit();
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
    //std::cout << mousePos_screenBased << std::endl;
    float boxposscale = 0.6;
    float textposscale = 0.575;



    if (mousePos_screenBased.x >= 0 && mousePos_screenBased.x <= 25
        && mousePos_screenBased.y >= 55 && mousePos_screenBased.y <= 65)
    {
        if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
        {
            //RenderButtonsOnScreen(meshList[GEO_MENU_CHOICEBOX], "Play", Color(0, 0, 0), 3, 0, 0, 65, 50);
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * boxposscale) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 6, 2);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Play";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * textposscale) * (60 / m_orthoHeight));
            state = MENU_PLAY;
        }
        else
        {
            modelStack.PushMatrix();
            modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * boxposscale) * (60 / m_orthoHeight), 1);
            modelStack.Scale(25, 7, 3);
            RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
            modelStack.PopMatrix();

            std::ostringstream ss;
            ss.precision(5);
            ss << "Play!";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * textposscale) * (60 / m_orthoHeight));

            modelStack.PushMatrix();
            modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * boxposscale) * (60 / m_orthoHeight), 1);
            modelStack.Scale(26, 15, 15);
            RenderMesh(meshList[GEO_MENU_SELECTION], false);
            modelStack.PopMatrix();
        }
    }
    else
    {
        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * boxposscale) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        ss.precision(5);
        ss << "Play";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * textposscale) * (60 / m_orthoHeight));
    }


    //OPTION BUTTON
    //std::cout << mousePos_screenBased << std::endl;
    if (mousePos_screenBased.x >= 0 && mousePos_screenBased.x <= 25
        && mousePos_screenBased.y >= 40 && mousePos_screenBased.y <= 50)
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
            ss << "Options";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));
            state = MENU_OPTIONS;
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
            ss << "Options";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));

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

        std::ostringstream ss;
        ss.precision(5);
        ss << "Options";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.15)) * (60 / m_orthoHeight));
    }

    //CREDIT BUTTON
    //std::cout << mousePos_screenBased << std::endl;
    if (mousePos_screenBased.x >= 0 && mousePos_screenBased.x <= 25
        && mousePos_screenBased.y >= 25 && mousePos_screenBased.y <= 35)
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
            ss << "Credits";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

            state = MENU_CREDITS;
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
            ss << "Credits!";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));

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
        ss << "Credits";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.3)) * (60 / m_orthoHeight));
    }

    //EXIT BUTTON
    //std::cout << mousePos_screenBased << std::endl;
    if (mousePos_screenBased.x >= 0 && mousePos_screenBased.x <= 25
        && mousePos_screenBased.y >= 10 && mousePos_screenBased.y <= 20)
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
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

            state = MENU_EXIT;
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
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));

            modelStack.PushMatrix();
            modelStack.Translate(10 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
            modelStack.Scale(26, 15, 15);
            RenderMesh(meshList[GEO_MENU_SELECTION], false);
            modelStack.PopMatrix();
        }
    }
    else
    {
        modelStack.PushMatrix();
        modelStack.Translate(5 * (80 / m_orthoWidth), (m_orthoHeight * (boxposscale - 0.45)) * (60 / m_orthoHeight), 1);
        modelStack.Scale(25, 6, 2);
        RenderMesh(meshList[GEO_MENU_CHOICEBOX], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        ss.precision(5);
        ss << "Exit";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, (m_orthoHeight * (textposscale - 0.45)) * (60 / m_orthoHeight));
    }
}

void MainMenu::InstructionPage()
{
    standardLayout();
}

void MainMenu::OptionsPage()
{
    standardLayout();
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
    ss << "Zhi Tern";
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5) + 5) * (60 / m_orthoHeight));

    std::ostringstream ss2;
    ss2.precision(5);
    ss2 << "Reuben Quek";
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 0, 0), 4, (m_orthoWidth * LayoutScale) * (80 / m_orthoWidth), ((m_orthoHeight * 0.5)) * (60 / m_orthoHeight));

    std::ostringstream ss3;
    ss3.precision(5);
    ss3 << "Yu Xian";
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

void MainMenu::standardLayout()
{
    modelStack.PushMatrix();
    modelStack.Translate((m_orthoWidth * 0.5) * (80 / m_orthoWidth), (m_orthoHeight * 0.5) * (60 / m_orthoHeight), 0);
    modelStack.Scale(75, 60, 0);
    RenderMesh(meshList[GEO_MENU_FRAME], false);
    modelStack.PopMatrix();

    //BACK BUTTON
    if (mousePos_screenBased.x >= 0 && mousePos_screenBased.x <= 25
        && mousePos_screenBased.y >= 10 && mousePos_screenBased.y <= 20)
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
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.5, 25, ((m_orthoHeight * 0.5) - 2 )* (60 / m_orthoHeight));

    //Yes
    if (mousePos_screenBased.x >= 42.5 && mousePos_screenBased.x <= 62.5
        && mousePos_screenBased.y >= 30 && mousePos_screenBased.y <= 40)
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

    ////mousePos_screenBased.x >= 67.5 && mousePos_screenBased.x <= 77.5
    //No
    if (mousePos_screenBased.x >= 67.5 && mousePos_screenBased.x <= 87.5
        && mousePos_screenBased.y >= 30 && mousePos_screenBased.y <= 40)
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
            state = MENU_MAIN;
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

void MainMenu::Exit()
{
    if (mainCamera)
        delete mainCamera;

    SceneBase::Exit();
}