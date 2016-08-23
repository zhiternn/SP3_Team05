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

    ////Restrict the player from moving past the deadzone
    //if (mainCamera->Deadzone(&player->GetPosition(), mainCamera->GetPosition()))
    //{
    //    PlayerController(dt);
    //}

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
    {//Render Floor
        modelStack.PushMatrix();
        modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
        modelStack.Scale(m_worldWidth, m_worldHeight, 0);
        RenderMesh(meshList[GEO_FLOOR], false);
        modelStack.PopMatrix();
    }

}

void MainMenu::RenderHUD()
{
    double x, y;
    Application::GetCursorPos(x, y);    
    int w = Application::GetWindowWidth();
    int h = Application::GetWindowHeight();
    x = m_orthoWidth * (x / w);
    y = m_orthoHeight * ((h - y) / h);

    modelStack.PushMatrix();
    modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
    modelStack.Scale(5, 5, 5);
    //modelStack.Scale(m_orthoWidth, m_orthoWidth, 3);
    RenderMesh(meshList[GEO_MENU_BACKGROUND], false);
    modelStack.PopMatrix();

    // Render the crosshair
    modelStack.PushMatrix();
    modelStack.Translate((mousePos_screenBased.x + 2.5)* 80 / m_orthoWidth , (mousePos_screenBased.y - 2.5)* 60 / m_orthoHeight, 6);
    modelStack.Scale(5, 5, 5);
    RenderMesh(meshList[GEO_MENU_CURSOR], false);
    modelStack.PopMatrix();

    //On screen text
    std::ostringstream ss;
    ss.precision(5);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

    std::ostringstream ss1;
    ss1.precision(4);
    ss1 << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
    RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 3);

}

void MainMenu::Exit()
{
    if (mainCamera)
        delete mainCamera;

    SceneBase::Exit();
}