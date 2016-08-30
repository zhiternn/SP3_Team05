#include "SceneDetlaff.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"


#include <sstream>

#define ENEMY_FIRE_COOLDOWN 2;
#define ENEMY_MOVE_DELAY 5;

SceneDetlaff::SceneDetlaff():
mainCamera(NULL),
manager(SceneManager::GetInstance()),
options(OptionManager::GetInstance())
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

	player->Init(Vector3(0, 1, 0), Vector3(2.5f, 2.5f, 2.5f), Vector3(1, 0, 0));
	GameObject::goList.push_back(player);

	detlaff = new CDetlaff();
	GameObject::goList.push_back(detlaff);
	detlaff->SetTarget(player);
	detlaff->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0));
	detlaff->SetScale(10, 10, 10);
	detlaff->SetActive(true);
	detlaff->SetColliderType(Collider::COLLIDER_BALL);

	mainCamera->Include(&(player->pos));
	
	enemyFireDelay = ENEMY_FIRE_COOLDOWN;
	enemyMovementDelay = ENEMY_MOVE_DELAY;


	if (!(glfwController.isConnected() && useController))
	{
		mainCamera->Include(&mousePos_worldBased);
		Keyboard = false;
	}
	else
	{
		mainCamera->Include(&controllerStick_Pos);
		Keyboard = true;
	}

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
	
	//Handle Gamepad movement
	
	//= Y Axis Movement 
	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_Y) > 0.2f)
	{
		forceDir.y += 5 * glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_Y);
	}
	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_Y) < -0.2f)
	{
		forceDir.y += 5 * glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_Y);
	}

	


	//= X Axis Movement
	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_X) > 0.2f)
	{
		forceDir.x += 5 * glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_X);
	}
	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_X) < -0.2f)
	{
		forceDir.x += 5 * glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::L_THUMBSTICK_X);
	}

	//= Dash
	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::TRIGGERS) > 0.5f)
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
	//= Left Bumper

	if (glfwController.GetJoyStickButtonPressed(GLFWController::CONTROLLER_BUTTON::L_SHOULDER) != NULL)
	{
		player->ChangeProjectileUp();
	}
	if (glfwController.GetJoyStickButtonPressed(GLFWController::CONTROLLER_BUTTON::R_SHOULDER) != NULL)
	{
		player->ChangeWeaponUp();
	}
	

	//Shooting

	if (glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_X) > 0.2f
		|| glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_X) < -0.2f
		|| glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_Y) > 0.2f
		|| glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_Y) < -0.2f)
	{
		stickDir = Vector3(glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_X)
			, -(glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_Y))
			, 0);
		player->Shoot(stickDir.Normalized());
	}
	

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

	controllerStick_Pos.Set((glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_X) * 100) + player->pos.x, 
		(-(glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_Y)) * 100) + player->pos.y, 0);
	controllerStick_WorldPos.Set(
		glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_X) + mainCamera->target.x - (m_orthoWidth * 0.5f),
		(-(glfwController.GetJoyStickTriggerPressed(GLFWController::CONTROLLER_STICKS::R_THUMBSTICK_Y)) * 100) + mainCamera->target.y - (m_orthoHeight * 0.5f),
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
		detlaff->Teleport(m_worldWidth - 50.f, m_worldHeight - 50.f);
	}

	
	//Restrict the player from moving past the deadzone
	if (mainCamera->Deadzone(&player->GetPosition(), mainCamera->GetPosition(), m_orthoHeight))
	{
		//Check if Gamepad is connected for controller input
		if (useController && glfwController.isConnected())
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

	//Update Camera target scheme if Controller is plugged in
	if (GamePad.IsConnected() && Keyboard)
	{
		Keyboard = false;

		mainCamera->entityList.pop_back();
		mainCamera->Include(&controllerStick_Pos);
	}
	else if (!(GamePad.IsConnected()))
	{
		Keyboard = true;
		mainCamera->entityList.pop_back();
		mainCamera->Include(&mousePos_worldBased);
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

	//RenderBackground
	RenderBackground();

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
	//Render Minimap
	modelStack.PushMatrix();
	modelStack.Translate(70, 50, 0);
	modelStack.Scale(18, 18, 1);
	RenderMinimap(1.0f);
	modelStack.PopMatrix();

	if (!(useController && glfwController.isConnected()))
	{
		//Render the crosshair
		modelStack.PushMatrix();
		modelStack.Translate(mousePos_screenBased.x * 80 / m_orthoWidth, mousePos_screenBased.y * 60 / m_orthoHeight, 6);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_CROSSHAIR], false);
		modelStack.PopMatrix();
	}
	

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

void SceneDetlaff::RenderMinimap(float zoom)
{
	glEnable(GL_STENCIL_TEST);

	// Draw floor
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

	RenderMesh(meshList[GEO_MINIMAP], false);

	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

	for (int i = 0; i < GameObject::goList.size(); ++i)
	{
		if (GameObject::goList[i]->IsActive())
		{
			Entity* entity = dynamic_cast<Entity*>(GameObject::goList[i]);
			if (entity && entity->IsActive())
			{
				modelStack.PushMatrix();
				Vector3 pos = entity->pos;
				pos.x -= player->pos.x;// Move to player pos
				pos.y -= player->pos.y;// Move to player pos
				//sphere space == radius = 1
				pos.x /= m_worldWidth * zoom; //convert to regular sphere space
				pos.y /= m_worldHeight * zoom;//convert to regular sphere space
				Vector3 scale = entity->GetScale();
				scale.x /= m_worldWidth * zoom; //convert to regular sphere space
				scale.y /= m_worldHeight * zoom;//convert to regular sphere space
				modelStack.Translate(pos.x, pos.y, pos.z);
				modelStack.Scale(scale.x, scale.y, scale.z);

				switch (entity->GetEntityType())
				{
				case Entity::ENTITY_BOSS_MAIN:
					RenderMesh(meshList[GEO_MINIMAP_BOSS_MAIN_ICON], false);
					break;
				case Entity::ENTITY_BOSS_BODY:
					RenderMesh(meshList[GEO_MINIMAP_BOSS_BODY_ICON], false);
					break;
				case Entity::ENTITY_PLAYER:
					RenderMesh(meshList[GEO_MINIMAP_PLAYER_ICON], false);
					break;
				default:break;
				}

				modelStack.PopMatrix();
			}
		}
	}

	glDisable(GL_STENCIL_TEST);

	glLineWidth(5.0f);
	RenderMesh(meshList[GEO_MINIMAP_BORDER], false);
	glLineWidth(1.0f);
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