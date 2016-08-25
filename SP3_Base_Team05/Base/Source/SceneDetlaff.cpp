#include "SceneDetlaff.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"


#include <sstream>

#define ENEMY_FIRE_COOLDOWN 2;
#define ENEMY_MOVE_DELAY 15;

SceneDetlaff::SceneDetlaff() :
player(NULL),
mainCamera(NULL),
manager(SceneManager::GetInstance())
{
}

SceneDetlaff::~SceneDetlaff()
{
}

void SceneDetlaff::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	//Clear the list from previous scene
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
	m_orthoHeight = 150;
	m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	mainCamera = new Camera();
	mainCamera->Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	/*GameObject *go = FetchGO();
	go->SetActive(true);
	go->SetScale(20, 20, 20);
	go->SetFront(1, 0, 0);
	go->SetPostion(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	go->SetType(GameObject::GO_ENVIRONMENT);
	go->SetColliderType(Collider::COLLIDER_BOX);*/

	player = new Player();
	player->Init(Vector3(0, 1, 0), Vector3(2.5f, 2.5f, 2.5f), Vector3(1, 0, 0));
	GameObject::goList.push_back(player);

	detlaff = new CDetlaff();
	GameObject::goList.push_back(detlaff);
	detlaff->SetTarget(player);
	detlaff->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0));
	detlaff->SetScale(10, 10, 10);
	detlaff->SetActive(true);
	detlaff->SetColliderType(Collider::COLLIDER_BALL);
	detlaff->SetMass(999999);

	mainCamera->Include(&(player->pos));
	if (!GamePad.IsConnected())
	{
		mainCamera->Include(&mousePos_worldBased);
	}
	else
	{
		mainCamera->Include(&controllerStick_Pos);
	}

	enemyFireDelay = ENEMY_FIRE_COOLDOWN;
	enemyMovementDelay = ENEMY_MOVE_DELAY;


}

void SceneDetlaff::PlayerController(double dt)
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
	//if (Controls::GetInstance().OnHold(Controls::KEY_LSHIFT))
	//{
	//	CProjectile *proj_trap = new TrapProjectile();
	//	proj_trap->SetTeam(CProjectile::TEAM_PLAYER);
	//	proj_trap->SetVelocity(0, 0, 0);
	//	proj_trap->SetColliderType(Collider::COLLIDER_BOX);
	//	player->weapon->AssignProjectile(proj_trap);

	//	for (int i = 0; i < GameObject::goList.size(); i++)
	//	{
	//		if (GameObject::goList[i]->GetType() == CProjectile::TRAP)
	//		{
	//			//Trap found in current map
	//			break;
	//		}
	//		else
	//		{
	//			//push it into the list to check for next iteration
	//			GameObject::goList.push_back(proj_trap);

	//			Vector3 pos;
	//			pos = player->pos.Normalized();
	//			player->Shoot(pos);
	//			break;
	//		}
	//	}
	//}

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
	if (Controls::GetInstance().OnPress(Controls::KEY_B))
	{
		manager.ChangeScene(1);
	}
}

void SceneDetlaff::GetGamePadInput(double dt)
{
	Vector3 forceDir;
	Vector3 lookDir = (controllerStick_WorldPos - player->pos).Normalized();
	player->SetFront(lookDir);
	
	//Update Gamepad
	GamePad.Update();
	
	//Handle Gamepad movement
	
	//= Y Axis Movement
	if (GamePad.Left_Stick_Y() > 0.2f)
	{
		forceDir.y += 5 * GamePad.Left_Stick_Y();
	}
	if (GamePad.Left_Stick_Y() < -0.2f)
	{
		forceDir.y += 5 * GamePad.Left_Stick_Y();
	}

	//= X Axis Movement
	if (GamePad.Left_Stick_X() > 0.2f)
	{
		forceDir.x += 5 * GamePad.Left_Stick_X();
	}
	if (GamePad.Left_Stick_X() < -0.2f)
	{
		forceDir.x += 5 * GamePad.Left_Stick_X();
	}

	//= Dash
	if (GamePad.RightTrigger() > 0.2f)
	{
		player->Dash(forceDir, dt);
	}

	//= Update Movement
	if (forceDir.IsZero() == false)
	{
		forceDir.Normalize();
		player->Move(forceDir, dt);
	}

	
	//Change Weapons
	if (GamePad.GetButtonDown(8) > 0.5f)
	{
		player->ChangeWeaponDown();
	}
	if (GamePad.GetButtonDown(9) > 0.5f)
	{
		player->ChangeWeaponUp();
	}

	//Shooting
	if (GamePad.Right_Stick_Y() > 0.2f || GamePad.Right_Stick_Y() < -0.2f || GamePad.Right_Stick_X() > 0.2f || GamePad.Right_Stick_X() < -0.2f)
	{
		stickDir = Vector3(GamePad.Right_Stick_X(), GamePad.Right_Stick_Y(), 0);
		player->Shoot(stickDir.Normalized());
	}

	//Refresh Gamepad
	GamePad.RefreshState();

}

void SceneDetlaff::Update(double dt)
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

	controllerStick_Pos.Set((GamePad.Right_Stick_X() * 100) + player->pos.x, (GamePad.Right_Stick_Y() * 100) + player->pos.y, 0);
	controllerStick_WorldPos.Set(
		GamePad.Right_Stick_X() + mainCamera->target.x - (m_orthoWidth * 0.5f),
		GamePad.Right_Stick_Y() + mainCamera->target.y - (m_orthoHeight * 0.5f),
		0);

	mainCamera->Update(dt);
	mainCamera->Constrain(*player, mainCamera->target);
	UpdateGameObjects(dt);

	enemyFireDelay -= dt;
	enemyMovementDelay -= dt;

	if (enemyFireDelay <= 0.f)
	{
		enemyFireDelay = ENEMY_FIRE_COOLDOWN;

		Vector3 mouseDir;
		mouseDir = (player->pos - detlaff->pos).Normalized();
		detlaff->Shoot(mouseDir);
	}

	if (enemyMovementDelay <= 0.f)
	{
		enemyMovementDelay = ENEMY_MOVE_DELAY;

		detlaff->Teleport(m_worldWidth, m_worldHeight);
	}

	
	//Restrict the player from moving past the deadzone
	if (mainCamera->Deadzone(&player->GetPosition(), mainCamera->GetPosition(), m_orthoHeight))
	{
		//Check if Gamepad is connected for controller input
		if (GamePad.IsConnected())
		{
			//Handle Controller Input 
			GetGamePadInput(dt);
		}
		else
		{
			//Handle Keyboard and Mouse input
			PlayerController(dt);
		}
	}
}

void SceneDetlaff::Render()
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
void SceneDetlaff::RenderSkybox()
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

void SceneDetlaff::RenderSkyPlane()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 2500, 0);
	//modelStack.Scale(2.0f, 2.0f, 2.0f);
	//RenderMesh(meshList[GEO_SKYPLANE], false);
	//modelStack.PopMatrix();
}

void SceneDetlaff::RenderGPass()
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

void SceneDetlaff::RenderMain()
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

	/*if (!detlaff->destinations.empty())
	{
		modelStack.PushMatrix();
		modelStack.Translate(detlaff->destinations.front().x, detlaff->destinations.front().y, 0);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
	}*/

	//RenderSkyPlane();
}

void SceneDetlaff::RenderWorld()
{
	{
		//Render Floor
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
		Vector3 toMouse = stickDir;
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

void SceneDetlaff::RenderHUD()
{
	////Render the crosshair
	//modelStack.PushMatrix();
	//modelStack.Translate(controllerStick_WorldPos.x, controllerStick_WorldPos.y, 6);
	//modelStack.Scale(10, 10, 10);
	//RenderMesh(meshList[GEO_CROSSHAIR], false);
	//modelStack.PopMatrix();

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

	ss.str("");
	ss << "HP";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 54);
	RenderUI(meshList[GEO_BORDER], 2, (player->maxHealth / 5) + 11, 55.5f, player->maxHealth / 5, false);
	RenderUI(meshList[GEO_HEALTH], 2, (player->GetHP() / 5) + 11, 55.5f, player->GetHP() / 5, false);

	ss.str("");
	ss.precision(2);
	ss << "Dash";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 51);
	RenderUI(meshList[GEO_BORDER], 2, (DASH_COOLDOWN * 10) + 11, 52.5f, DASH_COOLDOWN * 10, false);
	RenderUI(meshList[GEO_DASH], 2, ((DASH_COOLDOWN - player->cooldownTimer) * 10) + 11, 52.5f, (DASH_COOLDOWN - player->cooldownTimer) * 10, false);
}

void SceneDetlaff::Exit()
{
	if (mainCamera)
		delete mainCamera;
	if (player)
		delete player;

	SceneBase::Exit();
}

void SceneDetlaff::UpdateGameObjects(double dt)
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

			go->HandleOutOfBounds(0, m_worldWidth, 0, m_worldHeight);
		}
	}
}

void SceneDetlaff::RenderGO(GameObject* go)
{
	modelStack.PushMatrix();

	if (go)
	{
		go->SetupMesh();
		if (go->mesh)
			RenderMesh(go->mesh, false);
	}

	modelStack.PopMatrix();
}

void SceneDetlaff::RenderGameObjects()
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
		}
	}
}