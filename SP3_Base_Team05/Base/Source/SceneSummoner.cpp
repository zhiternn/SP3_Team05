#include "SceneSummoner.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"
#include "Particle.h"

#include <sstream>

SceneSummoner::SceneSummoner() :
manager(SceneManager::GetInstance())
{
}

SceneSummoner::~SceneSummoner()
{
}

void SceneSummoner::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	GameObject::goList.clear();

	player->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f - 5, 0));
    GameObject::goList.push_back(SceneBase::player);

	summoner = new Summoner();
	GameObject::goList.push_back(summoner);
	summoner->SetTarget(player);
	summoner->Init(Vector3(m_worldWidth * 0.5f + 5, m_worldHeight * 0.5f, 0));

	GenerateWorld();
}

void SceneSummoner::PlayerController(double dt)
{
	SceneBase::PlayerController(dt);
}

void SceneSummoner::GetGamePadInput(double dt)
{
	SceneBase::GetGamePadInput(dt);
}

void SceneSummoner::Update(double dt)
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

	if (summoner->IsDead())
	{
		player->inventory->AddCurrency(50);
		manager.ChangeScene(SCENE::SCENE_MENU);
		dynamic_cast<MainMenu*>(manager.GetScene())->SetState(MainMenu::MENU_WIN);

		delete this;
	}
	else if (summoner->IsCaptured())
	{
		player->inventory->AddCurrency(50 + summoner->GetHP());
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

void SceneSummoner::GenerateWorld()
{
	for (int i = 0; i < NUMBER_OF_WORLD_OBJECTS; ++i)
	{
		float randPosX = Math::RandFloatMinMax(0, m_worldWidth);
		float randPosY = Math::RandFloatMinMax(0, m_worldHeight);
		float randScaleX = Math::RandFloatMinMax(7, 10);
		float randScaleY = Math::RandFloatMinMax(7, 10);
		GameObject *wall = new GameObject();
		GameObject::goList.push_back(wall);
		wall->SetActive(true);
		wall->SetColliderType(Collider::COLLIDER_BOX);
		wall->SetPostion(randPosX, randPosY, 0);
		wall->SetScale(randScaleX, randScaleY, 1);
	}
}

void SceneSummoner::Render()
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
void SceneSummoner::RenderSkybox()
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

void SceneSummoner::RenderSkyPlane()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 2500, 0);
	//modelStack.Scale(2.0f, 2.0f, 2.0f);
	//RenderMesh(meshList[GEO_SKYPLANE], false);
	//modelStack.PopMatrix();
}

void SceneSummoner::RenderGPass()
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

void SceneSummoner::RenderMain()
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

void SceneSummoner::RenderWorld()
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

void SceneSummoner::RenderHUD()
{
	SceneBase::RenderHUD();
}

void SceneSummoner::Exit()
{
	SceneBase::Exit();
}

void SceneSummoner::UpdateGameObjects(double dt)
{
	SceneBase::UpdateGameObjects(dt);
}

void SceneSummoner::RenderGameObjects()
{
	SceneBase::RenderGameObjects();
}