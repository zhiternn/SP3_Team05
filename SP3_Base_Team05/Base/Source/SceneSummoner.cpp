#include "SceneSummoner.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"

#include <sstream>

SceneSummoner::SceneSummoner() :
player(NULL),
mainCamera(NULL)
{
}

SceneSummoner::~SceneSummoner()
{
}

void SceneSummoner::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	//Clear the List from previous Scene
	GameObject::goList.clear();

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

	//// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	//Mtx44 perspective;
	//perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	////perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	//projectionStack.LoadMatrix(perspective);

	//World Space
	m_worldHeight = 300;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//World Space View
	m_orthoHeight = 300;
	m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	mainCamera = new Camera();
	mainCamera->Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//GameObject *go = FetchGO();
	//go->SetActive(true);
	//go->SetScale(20, 20, 20);
	//go->SetFront(1, 0, 0);
	//go->SetPostion(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	//go->SetType(GameObject::GO_ENVIRONMENT);
	//go->SetColliderType(Collider::COLLIDER_BOX);

	player = new Player();
	player->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f - 5, 0), Vector3(2.5f, 2.5f, 2.5f), Vector3(1, 0, 0));
	GameObject::goList.push_back(player);

	summoner = new Summoner();
	GameObject::goList.push_back(summoner);
	summoner->SetTarget(player);
	summoner->Init(Vector3(m_worldWidth * 0.5f + 5, m_worldHeight * 0.5f, 0));

	mainCamera->Include(&(player->pos));
	mainCamera->Include(&mousePos_worldBased);
}

void SceneSummoner::PlayerController(double dt)
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
	if (forceDir.IsZero() == false)
	{
		forceDir.Normalize();
		player->Move(forceDir, dt);
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_SPACE))
	{
		player->Dash(forceDir, dt);
	}
	if (Controls::GetInstance().OnHold(Controls::MOUSE_LBUTTON))
	{
		Vector3 mouseDir;
		mouseDir = (mousePos_worldBased - player->pos).Normalized();
		player->Shoot(mouseDir);
	}

	if (Controls::GetInstance().mouse_ScrollY < 0)
	{
		player->ChangeWeaponDown();
		Controls::GetInstance().mouse_ScrollY = 0;
	}
	if (Controls::GetInstance().mouse_ScrollY > 0)

	{
		player->ChangeWeaponUp();
		Controls::GetInstance().mouse_ScrollY = 0;
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_E))
	{
		player->ChangeProjectileUp();
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_Q))
	{
		player->ChangeProjectileDown();
	}
}

void SceneSummoner::Update(double dt)
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

	RenderWorld();

	if (!summoner->destinations.empty())
	{
		modelStack.PushMatrix();
		modelStack.Translate(summoner->destinations.front().x, summoner->destinations.front().y, 0);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
	}

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
	// Render the crosshair
    modelStack.PushMatrix();
    modelStack.Translate(mousePos_screenBased.x * 80 / m_orthoWidth, mousePos_screenBased.y * 60 / m_orthoHeight, 6);
    modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_CROSSHAIR], false);
    modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.5f, 0, 0);

	ss.str("");
	ss.precision(4);
	ss << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss << "Weapon: ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 57);
	switch (player->weaponIter)
	{
	case 0:
		RenderUI(meshList[GEO_WEAPON_SHOTGUN], 7, 25, 58.5f, 1, false);
		break;
	case 1:
		RenderUI(meshList[GEO_WEAPON_MACHINEGUN], 7, 25, 58.5f, 1, false);
		break;
	case 2:
		RenderUI(meshList[GEO_WEAPON_SPLITGUN], 7, 25, 58.5f, 1, false);
		break;
	}

	ss.str("");
	ss << "Bullet: ";
	switch (player->projectileIter)
	{
	case 0:
		ss << "Normal";
		break;
	case 1:
		ss << "Hook";
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 40, 57);

	ss.str("");
	ss.precision(1);
	ss << "HP";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 54);
	RenderUI(meshList[GEO_BORDER], 2, (player->maxHealth / 10) + 11, 55.5f, player->maxHealth / 10, false);
	RenderUI(meshList[GEO_HEALTH], 2, (player->GetHP() / 10) + 11, 55.5f, player->GetHP() / 10, false);

	ss.str("");
	ss.precision(2);
	ss << "Dash";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 51);
	RenderUI(meshList[GEO_BORDER], 2, (DASH_COOLDOWN * player->maxHealth / 10) + 11, 52.5f, DASH_COOLDOWN * player->maxHealth / 10, false);
	RenderUI(meshList[GEO_DASH], 2, ((DASH_COOLDOWN - player->cooldownTimer) * player->maxHealth / 10) + 11, 52.5f, (DASH_COOLDOWN - player->cooldownTimer) * player->maxHealth / 10, false);
}

void SceneSummoner::Exit()
{
	if (mainCamera)
		delete mainCamera;
	if (player)
		delete player;

	SceneBase::Exit();
}

void SceneSummoner::UpdateGameObjects(double dt)
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

void SceneSummoner::RenderGameObjects()
{
	for (int i = 0; i < GameObject::goList.size(); ++i)
	{
		if (GameObject::goList[i]->IsActive())
		{
			modelStack.PushMatrix();

			GameObject::goList[i]->SetupMesh();
			if (GameObject::goList[i]->mesh)
				RenderMesh(GameObject::goList[i]->mesh, false);

			modelStack.PopMatrix();
			Enemy* entity = dynamic_cast<Enemy*>(GameObject::goList[i]);
			if (entity && !entity->IsDead())
			{
				modelStack.PushMatrix();
				modelStack.Translate(entity->pos.x, entity->pos.y, 5);
				modelStack.Scale(entity->GetHP() / 5, 3, 1);
				RenderMesh(meshList[GEO_HEALTH], false);
				modelStack.PopMatrix();
			}
		}
	}
}