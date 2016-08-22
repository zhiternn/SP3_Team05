#include "SceneGolem.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"

#include <sstream>

SceneGolem::SceneGolem() :
player(NULL),
mainCamera(NULL)
{
}

SceneGolem::~SceneGolem()
{
}

void SceneGolem::Init()
{
    SceneBase::Init();
    Math::InitRNG();

    //meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1));
    //meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
    //meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1));
    //meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");
    //meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1));
    //meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
    //meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1));
    //meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
    //meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1));
    //meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
    //meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1));
    //meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

    //meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("skyplane", Color(0, 0, 0), 64, 256.f, 2000.f, 1.f, 1.f);
    //meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//sky3.tga");

    //// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
    //Mtx44 perspective;
    //perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    ////perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    //projectionStack.LoadMatrix(perspective);

    //World Space
    m_worldHeight = 300;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //World Space View
    m_orthoHeight = 100;
    m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    mainCamera = new Camera();
    mainCamera->Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

    GameObject *go = FetchGO();
    go->SetActive(true);
    go->SetScale(20, 20, 20);
    go->SetFront(1, 0, 0);
    go->SetPostion(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
    go->SetType(GameObject::GO_ENVIRONMENT);
    go->SetColliderType(Collider::COLLIDER_BOX);

    player = new Player();
    player->Init(Vector3(0, 1, 0), Vector3(2.5f, 2.5f, 2.5f), Vector3(1, 0, 0));
    GameObject::goList.push_back(player);

    golemhead = new GolemHead();
    GameObject::goList.push_back(golemhead);
    golemhead->SetTarget(player);
    golemhead->SetType(GameObject::GO_ENTITY);
    golemhead->SetActive(true);
    golemhead->SetColliderType(Collider::COLLIDER_BALL);
    golemhead->SetScale(10, 10, 10);
    golemhead->SetMass(3);
    golemhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 100.f, 0));

    golemlhead = new GolemLeftHand();
    GameObject::goList.push_back(golemlhead);
    golemlhead->SetTarget(player);
    golemlhead->SetType(GameObject::GO_ENTITY);
    golemlhead->SetActive(true);
    golemlhead->SetColliderType(Collider::COLLIDER_BALL);
    golemlhead->SetScale(6, 6, 6);
    golemlhead->SetMass(3);
    golemlhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 70.f, 0));

    golemrhead = new GolemRightHand();
    GameObject::goList.push_back(golemrhead);
    golemrhead->SetTarget(player);
    golemrhead->SetType(GameObject::GO_ENTITY);
    golemrhead->SetActive(true);
    golemrhead->SetColliderType(Collider::COLLIDER_BALL);
    golemrhead->SetScale(6, 6, 6);
    golemrhead->SetMass(3);
    golemrhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 50.f, 0));

    mainCamera->Include(&(player->pos));
    mainCamera->Include(&mousePos_worldBased);
}

void SceneGolem::PlayerController(double dt)
{
    Vector3 lookDir = (mousePos_worldBased - player->pos).Normalized();
    player->SetFront(lookDir);
    Vector3 forceDir;

    if (Controls::GetInstance().OnHold(Controls::KEY_W))
    {
        forceDir.y += 1;
    }
    if (Controls::GetInstance().OnHold(Controls::KEY_S))
    {
        forceDir.y -= 1;
    }
    if (Controls::GetInstance().OnHold(Controls::KEY_A))
    {
        forceDir.x -= 1;
    }
    if (Controls::GetInstance().OnHold(Controls::KEY_D))
    {
        forceDir.x += 1;
    }

    if (Controls::GetInstance().OnPress(Controls::KEY_SPACE))
    {
        player->Dash(forceDir, dt);
    }
    if (forceDir.IsZero() == false)
    {
        forceDir.Normalize();
        player->Move(forceDir, dt);
    }
    if (Controls::GetInstance().OnHold(Controls::MOUSE_LBUTTON))
    {
        Vector3 mouseDir;
        mouseDir = (mousePos_worldBased - player->pos).Normalized();
        player->Shoot(mouseDir);
    }
    if (Controls::GetInstance().OnHold(Controls::KEY_LSHIFT))
    {
        CProjectile *proj_trap = new Trap();
        proj_trap->SetTeam(CProjectile::TEAM_PLAYER);
        proj_trap->SetVelocity(0, 0, 0);
        proj_trap->SetColliderType(Collider::COLLIDER_BOX);
        player->weapon->AssignProjectile(proj_trap);

        for (int i = 0; i < GameObject::goList.size(); i++)
        {
            if (GameObject::goList[i]->GetType() == CProjectile::TRAP)
            {
                //Trap found in current map
                break;
            }
            else
            {
                //push it into the list to check for next iteration
                GameObject::goList.push_back(proj_trap);

                Vector3 pos;
                pos = player->pos.Normalized();
                player->Shoot(pos);
                break;
            }
        }
    }

    //if (Controls::GetInstance().mouse_ScrollY < 1)
    if (Controls::GetInstance().OnPress(Controls::KEY_E))
    {
        player->ChangeWeaponDown();
    }
    //if (Controls::GetInstance().mouse_ScrollY > 1)
    if (Controls::GetInstance().OnPress(Controls::KEY_Q))
    {
        player->ChangeWeaponUp();
    }
}

void SceneGolem::Update(double dt)
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
    //Restrict the player from moving past the deadzone
    if (mainCamera->Deadzone(&player->GetPosition(), mainCamera->GetPosition()))
    {
        PlayerController(dt);
    }

    mainCamera->Update(dt);
    mainCamera->Constrain(*player, mainCamera->target);
    UpdateGameObjects(dt);

    if (golemhead->IsActive() == false)
    {
        golemrhead->SetActive(false);
        golemlhead->SetActive(false);
    }

    //if (golemhead->GetHP() <= 1500)
    //{
    //    golemlhead->speedLimit = 55.f;
    //    golemrhead->speedLimit = 55.f;
    //}
    //if (golemhead->GetHP() <= 500)
    //{
    //    golemlhead->speedLimit = 70.f;
    //    golemrhead->speedLimit = 70.f;
    //}
    //if (golemhead->GetHP() <= 250)
    //{
    //    golemlhead->speedLimit = 180.f;
    //    golemrhead->speedLimit = 180.f;
    //    golemlhead->movementSpeed = 1800.f;
    //    golemrhead->movementSpeed = 1800.f;
    //}
}

void SceneGolem::Render()
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

static const float SKYBOXSIZE = 1000.f;
void SceneGolem::RenderSkybox()
{
    ////front
    //modelStack.PushMatrix();
    //modelStack.Translate(0, 0, -((SKYBOXSIZE / 2) - 2.f));
    //modelStack.Rotate(0, 1, 0, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_FRONT], false);
    //modelStack.PopMatrix();

    ////back
    //modelStack.PushMatrix();
    //modelStack.Translate(0, 0, ((SKYBOXSIZE / 2) - 2.f));
    //modelStack.Rotate(180, 0, 1, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_BACK], false);
    //modelStack.PopMatrix();

    ////left
    //modelStack.PushMatrix();
    //modelStack.Translate(-((SKYBOXSIZE / 2) - 2.f), 0, 0);
    //modelStack.Rotate(90, 0, 1, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_LEFT], false);
    //modelStack.PopMatrix();

    ////right
    //modelStack.PushMatrix();
    //modelStack.Translate(((SKYBOXSIZE / 2) - 2.f), 0, 0);
    //modelStack.Rotate(-90, 0, 1, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_RIGHT], false);
    //modelStack.PopMatrix();

    ////top
    //modelStack.PushMatrix();
    //modelStack.Translate(0, ((SKYBOXSIZE / 2) - 2.f), 0);
    //modelStack.Rotate(-90, 0, 1, 0);
    //modelStack.Rotate(90, 1, 0, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_TOP], false);
    //modelStack.PopMatrix();

    ////bottom
    //modelStack.PushMatrix();
    //modelStack.Translate(0, -((SKYBOXSIZE / 2) - 2.f), 0);
    //modelStack.Rotate(90, 0, 1, 0);
    //modelStack.Rotate(-90, 1, 0, 0);
    //modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    //RenderMesh(meshList[GEO_BOTTOM], false);
    //modelStack.PopMatrix();
}

void SceneGolem::RenderSkyPlane()
{
    //modelStack.PushMatrix();
    //modelStack.Translate(0, 2500, 0);
    //modelStack.Scale(2.0f, 2.0f, 2.0f);
    //RenderMesh(meshList[GEO_SKYPLANE], false);
    //modelStack.PopMatrix();
}

void SceneGolem::RenderGPass()
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

void SceneGolem::RenderMain()
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

void SceneGolem::RenderWorld()
{
    //for (int i = m_worldWidth * 0.5 - 100; i < m_worldWidth * 0.5 + 100; i+=10)
    //{//Render Floor
    //    for (int j = m_worldHeight * 0.5 - 100; j < m_worldHeight * 0.5 + 100; j+= 10)
    //    {
    //        modelStack.PushMatrix();
    //        modelStack.Translate(i, j, 0);
    //        modelStack.Scale(10, 10, 0);
    //        RenderMesh(meshList[GEO_FLOOR], false);
    //        modelStack.PopMatrix();
    //    }
    //}
    modelStack.PushMatrix();
    modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
    modelStack.Scale(m_worldWidth, m_worldHeight, 0);
    RenderMesh(meshList[GEO_FLOOR], false);
    modelStack.PopMatrix();

    RenderGameObjects();

}

void SceneGolem::RenderHUD()
{
    // Render the crosshair
    modelStack.PushMatrix();
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_CROSSHAIR], false);
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

    std::ostringstream ss2;
    ss2.precision(2);
    ss2 << "Dash cooldown: " << player->cooldownTimer;
    RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 9);

    std::ostringstream ss3;
    ss3.precision(2);
    ss3 << "Weapon: " << player->weaponIter;
    RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 12);


    std::ostringstream ss4;
    ss4.precision(2);
    ss4 << "HP: " << golemhead->GetHP();
    RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(0, 1, 0), 3, 0, 15);
}

void SceneGolem::Exit()
{
    if (mainCamera)
        delete mainCamera;
    if (player)
        delete player;

    SceneBase::Exit();
}

void SceneGolem::UpdateGameObjects(double dt)
{
    for (int i = 0; i < GameObject::goList.size(); ++i)
    {
        GameObject *go = GameObject::goList[i];
        if (go->IsActive())
        {
            go->Update(dt);

            if (go->GetCollider().type == Collider::COLLIDER_BALL)
            {
                for (int j = 0; j < GameObject::goList.size(); ++j)
                {
                    GameObject *go2 = GameObject::goList[j];
                    if (go2->IsActive() && go->GetTeam() != go2->GetTeam() && go2->GetType() != GameObject::GO_PROJECTILE)
                    {
                        go->HandleInteraction(go2, dt);
                    }
                }
            }

            {//Handles out of bounds
                //Check Horizontally against edges
                if ((go->GetPosition().x + go->GetScale().x > m_worldWidth && go->GetVelocity().x > 0) ||
                    (go->GetPosition().x - go->GetScale().x < 0 && go->GetVelocity().x < 0))
                {
                    go->SetVelocity(-go->GetVelocity().x, go->GetVelocity().y, go->GetVelocity().z);
                }
                //remove if it cant be seen completely
                else if (go->GetPosition().x - go->GetScale().x > m_worldWidth ||
                    go->GetPosition().x + go->GetScale().x < 0)
                {
                    go->SetActive(false);
                }
                //Check Vertically against edges
                if ((go->GetPosition().y + go->GetScale().y > m_worldHeight && go->GetVelocity().y > 0) ||
                    (go->GetPosition().y - go->GetScale().y < 0 && go->GetVelocity().y < 0))
                {
                    go->SetVelocity(go->GetVelocity().x, -go->GetVelocity().y, go->GetVelocity().z);
                }
                //remove if it cant be seen completely
                else if (go->GetPosition().y - go->GetScale().y > m_worldWidth ||
                    go->GetPosition().y + go->GetScale().y < 0)
                {
                    go->SetActive(false);
                }
            }
        }
    }
}

void SceneGolem::RenderGO(GameObject* go)
{
    modelStack.PushMatrix();

    switch (go->GetType())
    {
    case GameObject::GO_ENVIRONMENT:
    {
        float degree = Math::RadianToDegree(atan2(go->GetFront().y, go->GetFront().x));

        modelStack.Translate(go->GetPosition().x, go->GetPosition().y, go->GetPosition().z);
        modelStack.Rotate(degree, 0, 0, 1);
        modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
        if (go->GetCollider().type == Collider::COLLIDER_BALL)
            RenderMesh(meshList[GEO_SPHERE], false);
        else
            RenderMesh(meshList[GEO_CUBE], false);
    }
    break;
    case GameObject::GO_PROJECTILE:
    {
        float degree = Math::RadianToDegree(atan2(go->GetFront().y, go->GetFront().x));

        modelStack.Translate(go->GetPosition().x, go->GetPosition().y, go->GetPosition().z);
        modelStack.Rotate(degree, 0, 0, 1);
        modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
        RenderMesh(meshList[GEO_SPHERE], false);
    }
    break;
    case GameObject::GO_ENTITY:
    {
        float degree = Math::RadianToDegree(atan2(go->GetFront().y, go->GetFront().x));
        modelStack.Translate(go->GetPosition().x, go->GetPosition().y, go->GetPosition().z);
        modelStack.Rotate(degree, 0, 0, 1);
        modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
        RenderMesh(meshList[GEO_SPHERE], false);
    }
    break;

    default:break;
    }

    modelStack.PopMatrix();
}

void SceneGolem::RenderGameObjects()
{
    for (int i = 0; i < GameObject::goList.size(); ++i)
    {
        if (GameObject::goList[i]->IsActive())
            RenderGO(GameObject::goList[i]);
    }
}