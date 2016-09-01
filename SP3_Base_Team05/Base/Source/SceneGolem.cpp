#include "SceneGolem.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"

#include <sstream>

SceneGolem::SceneGolem() :
manager(SceneManager::GetInstance())
{
}

SceneGolem::~SceneGolem()
{
}

void SceneGolem::Init()
{
    SceneBase::Init();
    Math::InitRNG();

	GameObject::goList.clear();

    player->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f + 20, 0));
    GameObject::goList.push_back(SceneBase::player);

    golemhead = new GolemHead();
    GameObject::goList.push_back(golemhead);
    golemhead->SetTarget(player);
    golemhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 100.f, 0));

    golemlhead = new GolemLeftHand();
    GameObject::goList.push_back(golemlhead);
    golemlhead->SetTarget(player);
    golemlhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 70.f, 0));

    golemrhead = new GolemRightHand();
    GameObject::goList.push_back(golemrhead);
    golemrhead->SetTarget(player);
    golemrhead->Init(Vector3(m_worldWidth*0.5f + 20.f, m_worldHeight*0.5f - 50.f, 0));

}

//CONTROL SCHEMES
//= KEYBOARD AND MOUSE
void SceneGolem::PlayerController(double dt)
{
	SceneBase::PlayerController(dt);
}

//CONTROLLER
void SceneGolem::GetGamePadInput(double dt)
{
	SceneBase::GetGamePadInput(dt);
}

void SceneGolem::Update(double dt)
{
    SceneBase::Update(dt);

	//Update Camera target scheme if Controller is plugged in
	if (glfwController.isConnected() && Keyboard)
	{
		Keyboard = false;

		mainCamera->entityList.pop_back();
		mainCamera->Include(&controllerStick_Pos);
	}
	else if (!(glfwController.isConnected()))
	{
		Keyboard = true;
		mainCamera->entityList.pop_back();
		mainCamera->Include(&mousePos_worldBased);
	}

    UpdateGameObjects(dt);

    if (golemhead->GetHP() <= 500)
    {
        golemrhead->SetSpeedLimit(100);
        golemrhead->SetMovementSpeed(5500);
        golemrhead->SetMass(99);
        golemlhead->SetSpeedLimit(100);
        golemlhead->SetMovementSpeed(5500);
        golemlhead->SetMass(99);
        golemhead->SetMovementSpeed(300);
        golemhead->SetSpeedLimit(35);
    }
    else if (golemhead->GetHP() <= 1500)
    {
        golemrhead->SetSpeedLimit(70);
        golemrhead->SetMovementSpeed(2300);
        golemrhead->SetMass(10);
        golemlhead->SetSpeedLimit(70);
        golemlhead->SetMovementSpeed(2300);
        golemlhead->SetMass(10);
        golemhead->SetMovementSpeed(150.f);
        golemhead->SetSpeedLimit(10);
    }
    else if (golemhead->GetHP() <= 3000)
    {
        golemrhead->SetSpeedLimit(60);
        golemrhead->SetMovementSpeed(1700);
        golemrhead->SetMass(6);
        golemlhead->SetSpeedLimit(60);
        golemlhead->SetMovementSpeed(1700);
        golemlhead->SetMass(6);
        golemhead->SetMovementSpeed(150.f);
        golemhead->SetSpeedLimit(10);
    }
    
    if (golemhead->GetHP() <= 0)
    {
        golemrhead->SetActive(false);
        golemlhead->SetActive(false);
    }

	if (golemhead->IsDead())
	{
		player->inventory->AddCurrency(100);
		manager.ChangeScene(SCENE::SCENE_MENU);
		dynamic_cast<MainMenu*>(manager.GetScene())->SetState(MainMenu::MENU_WIN);

		delete this;
	}

	if (player->IsDead())
	{
		manager.ChangeScene(SCENE::SCENE_MENU);
		dynamic_cast<MainMenu*>(manager.GetScene())->SetState(MainMenu::MENU_LOSE);

		delete this;
	}
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

	//RenderBackground
	RenderBackground();

	RenderWorld();

	RenderParticles();

    //RenderSkyPlane();
}

void SceneGolem::RenderWorld()
{
    {//Render Floor
        modelStack.PushMatrix();
        modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
        modelStack.Scale(m_worldWidth, m_worldHeight, 0);
        RenderMesh(meshList[GEO_FLOOR], false);
        modelStack.PopMatrix();
    }

    if (player && player->IsActive())
    {
        modelStack.PushMatrix();
        modelStack.Translate(player->pos.x, player->pos.y, player->pos.z);
        modelStack.Scale(player->GetScale().x, player->GetScale().y, player->GetScale().z);

        modelStack.PushMatrix();
        Vector3 toMouse = mousePos_worldBased - player->pos;
        float toMouseAngle = Math::RadianToDegree(atan2(toMouse.y, toMouse.x));
        modelStack.Rotate(toMouseAngle, 0, 0, 1);
        RenderMesh(meshList[GEO_PLAYER_TOP], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        float degree = Math::RadianToDegree(atan2(player->GetVelocity().y, player->GetVelocity().x));
        modelStack.Rotate(degree, 0, 0, 1);
        RenderMesh(meshList[GEO_PLAYER_BOTTOM], true);
        modelStack.PopMatrix();

        modelStack.PopMatrix();
    }

    RenderGameObjects();

}

void SceneGolem::RenderHUD()
{
	SceneBase::RenderHUD();
}

//void SceneGolem::RenderMinimap(float zoom)
//{
//	glEnable(GL_STENCIL_TEST);
//
//	// Draw floor
//	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	glStencilMask(0xFF); // Write to stencil buffer
//	glDepthMask(GL_FALSE); // Don't write to depth buffer
//	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
//
//	RenderMesh(meshList[GEO_MINIMAP], false);
//
//	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
//	glStencilMask(0x00); // Don't write anything to stencil buffer
//	glDepthMask(GL_TRUE); // Write to depth buffer
//
//	for (int i = 0; i < GameObject::goList.size(); ++i)
//	{
//		if (GameObject::goList[i]->IsActive())
//		{
//			Entity* entity = dynamic_cast<Entity*>(GameObject::goList[i]);
//			if (entity && entity->IsActive())
//			{
//				modelStack.PushMatrix();
//				Vector3 pos = entity->pos;
//				pos.x -= player->pos.x;// Move to player pos
//				pos.y -= player->pos.y;// Move to player pos
//				//sphere space == radius = 1
//				pos.x /= m_worldWidth * zoom; //convert to regular sphere space
//				pos.y /= m_worldHeight * zoom;//convert to regular sphere space
//				Vector3 scale = entity->GetScale();
//				scale.x /= m_worldWidth * zoom; //convert to regular sphere space
//				scale.y /= m_worldHeight * zoom;//convert to regular sphere space
//				modelStack.Translate(pos.x, pos.y, pos.z);
//				modelStack.Scale(scale.x, scale.y, scale.z);
//
//				switch (entity->GetEntityType())
//				{
//				case Entity::ENTITY_BOSS_MAIN:
//					RenderMesh(meshList[GEO_MINIMAP_BOSS_MAIN_ICON], false);
//					break;
//				case Entity::ENTITY_BOSS_BODY:
//					RenderMesh(meshList[GEO_MINIMAP_BOSS_BODY_ICON], false);
//					break;
//				case Entity::ENTITY_PLAYER:
//					RenderMesh(meshList[GEO_MINIMAP_PLAYER_ICON], false);
//					break;
//				default:break;
//				}
//
//				modelStack.PopMatrix();
//			}
//		}
//	}
//
//	glDisable(GL_STENCIL_TEST);
//
//	glLineWidth(5.0f);
//	RenderMesh(meshList[GEO_MINIMAP_BORDER], false);
//	glLineWidth(1.0f);
//}

void SceneGolem::Exit()
{
    SceneBase::Exit();
}

void SceneGolem::UpdateGameObjects(double dt)
{
	SceneBase::UpdateGameObjects(dt);
}

void SceneGolem::RenderGameObjects()
{
	SceneBase::RenderGameObjects();
}