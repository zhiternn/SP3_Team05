#include "SceneTutorial.h"

#include "Application.h"
#include "Controls.h"
#include "MeshManager.h"
#include "SnakeHead.h"
#include "Summoner.h"

SceneTutorial::SceneTutorial() :
manager(SceneManager::GetInstance())
{
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	std::vector<GameObject*>::iterator it;
	for (it = GameObject::goList.begin(); it != GameObject::goList.end();)
	{
		it = GameObject::goList.erase(it);
	}
	GameObject::goList.clear();

	SceneBase::player->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f + 20, 0));
    GameObject::goList.push_back(SceneBase::player);

	enemy = FetchEnemy();
	enemy->SetScale(5, 5, 5);
	enemy->SetTarget(SceneBase::player);
	enemy->SetEntityType(Entity::ENTITY_BOSS_MAIN);
	enemy->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f));
	enemy->SetHP(200);
	enemy->SetMaxHP(enemy->GetHP());

	mainCamera->Include(&SceneBase::player->pos);

	if (!(glfwController.isConnected() && useController))
	{
		mainCamera->Include(&mousePos_worldBased);
	}
	else
	{
		mainCamera->Include(&controllerStick_Pos);
	}

	pauseGame = false;
	firstTimeKill = true;
	firstTimeCapture = true;
	canExit = false;
	for (int i = 0; i < Weapon::W_END; ++i)
	{
		weaponChecks[i] = true;
	}
	for (int i = 0; i < CProjectile::MAX; ++i)
	{
		projChecks[i] = true;
	}
}

void SceneTutorial::PlayerController(double dt)
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

		if (weaponChecks[player->weapon->weapon_type] == true)
		{
			weaponChecks[player->weapon->weapon_type] = false;
			pauseGame = true;

			tutorialLines.str("");
			tutorialLines.clear();
			switch (player->weapon->weapon_type)
			{
			case Weapon::W_MACHINEGUN:
				tutorialLines << "Machinegun that fires rapidly";
				break;
			case Weapon::W_SHOTGUN:
				tutorialLines << "Shotgun that fires randomly in a cone";
				break;
			case Weapon::W_SPLITGUN:
				tutorialLines << "Splitgun that fires evenly in a cone";
				break;
			default:
				break;
			}
		}
	}
	if (Controls::GetInstance().mouse_ScrollY > 0)
	{
		player->ChangeWeaponUp();
		Controls::GetInstance().mouse_ScrollY = 0;

		if (weaponChecks[player->weapon->weapon_type] == true)
		{
			weaponChecks[player->weapon->weapon_type] = false;
			pauseGame = true;

			tutorialLines.str("");
			tutorialLines.clear();
			switch (player->weapon->weapon_type)
			{
			case Weapon::W_MACHINEGUN:
				tutorialLines << "Machinegun that fires rapidly";
				break;
			case Weapon::W_SHOTGUN:
				tutorialLines << "Shotgun that fires randomly in a cone";
				break;
			case Weapon::W_SPLITGUN:
				tutorialLines << "Splitgun that fires evenly in a cone";
				break;
			default:
				break;
			}
		}
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_E))
	{
		player->ChangeProjectileUp();

		if (weaponChecks[player->projectile->GetProjType()] == true)
		{
			weaponChecks[player->projectile->GetProjType()] = false;
			pauseGame = true;

			tutorialLines.str("");
			tutorialLines.clear();
			switch (player->projectile->GetProjType())
			{
			case CProjectile::BULLET:
				tutorialLines << "Bullet deals minor damage and knockback";
				break;
			case CProjectile::HOOK:
				tutorialLines << "Hook chains enemies to the ground";
				break;
			case CProjectile::TRAP:
				tutorialLines << "Keep enemies within trap to capture";
				break;
			default:
				break;
			}
		}
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_Q))
	{
		player->ChangeProjectileDown();

		if (weaponChecks[player->projectile->GetProjType()] == true)
		{
			weaponChecks[player->projectile->GetProjType()] = false;
			pauseGame = true;

			tutorialLines.str("");
			tutorialLines.clear();
			switch (player->projectile->GetProjType())
			{
			case CProjectile::BULLET:
				tutorialLines << "Bullet deals minor damage and knockback";
				break;
			case CProjectile::HOOK:
				tutorialLines << "Hook chains enemies to the ground";
				break;
			case CProjectile::TRAP:
				tutorialLines << "Keep enemies within trap to capture";
				break;
			default:
				break;
			}
		}
	}
}

void SceneTutorial::GetGamePadInput(double dt)
{
	SceneBase::GetGamePadInput(dt);
}

void SceneTutorial::Update(double dt)
{
	if (!pauseGame)
	{
		SceneBase::Update(dt);
		mainCamera->Update(dt);
		mainCamera->Constrain(*player, mainCamera->target);
		if (enemy->IsDead() || enemy->IsCaptured())
		{
			if (enemy->IsDead() && firstTimeKill && !enemy->IsCaptured())
			{
				firstTimeKill = false;
				pauseGame = true;
				tutorialLines.str("");
				tutorialLines.clear();
				tutorialLines << "You successfully killed an enemy";
			}
			if (enemy->IsCaptured() && firstTimeCapture)
			{
				firstTimeCapture = false;
				pauseGame = true;
				tutorialLines.str("");
				tutorialLines.clear();
				tutorialLines << "You successfully captured an enemy";
			}

			enemy->SetScale(5, 5, 5);
			enemy->SetTarget(player);
			enemy->SetEntityType(Entity::ENTITY_BOSS_MAIN);
			enemy->Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f));
			enemy->SetHP(200);
			enemy->SetMaxHP(enemy->GetHP());
		}
		else if (!enemy->UpdateMovement(dt))
		{
			enemy->ChangeDestination(Enemy::MOVETO_TARGET, player->pos);
		}
		UpdateGameObjects(dt);
	}
	else if (Controls::GetInstance().OnRelease(Controls::KEY_RETURN))
	{
		tutorialLines.str("");
		tutorialLines.clear();
		pauseGame = false;
	}
	if (Controls::GetInstance().OnRelease(Controls::KEY_BACKSPACE) && !firstTimeKill && !firstTimeCapture)
	{
		manager.ChangeScene(SCENE::SCENE_MENU);
		dynamic_cast<MainMenu*>(manager.GetScene())->SetState(MainMenu::MENU_WIN);

		delete this;
	}
}

void SceneTutorial::Render()
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
	if (!firstTimeKill && !firstTimeCapture)
		RenderTextOnScreen(meshList[GEO_TEXT], "You may now press \"Backspace\" to return", Color(0, 1, 0), 3, 4, 30);
	SetHUD(false);

	SetHUD(true);
	RenderHUD();
	SetHUD(false);
}

static const float SKYBOXSIZE = 1000.f;
void SceneTutorial::RenderSkybox()
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

void SceneTutorial::RenderSkyPlane()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 2500, 0);
	//modelStack.Scale(2.0f, 2.0f, 2.0f);
	//RenderMesh(meshList[GEO_SKYPLANE], false);
	//modelStack.PopMatrix();
}

void SceneTutorial::RenderGPass()
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

void SceneTutorial::RenderMain()
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

void SceneTutorial::RenderWorld()
{
	{//Render Floor
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
		modelStack.Scale(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_FLOOR_HEX], false);
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

void SceneTutorial::RenderHUD()
{
	SceneBase::RenderHUD();

	if (pauseGame)
	{
		modelStack.PushMatrix();
		modelStack.Translate(40, 30.0f, 0);
		modelStack.Scale(80, 10, 1);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "Press \"Enter\" to continue", Color(0, 0, 0), 2.0f, 5, 27.5f);
		RenderTextOnScreen(meshList[GEO_TEXT], tutorialLines.str(), Color(0, 0, 0), 3, 5, 30);
	}
}

void SceneTutorial::RenderMinimap(float zoom)
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
			GameObject* go = GameObject::goList[i];
			if (go->IsActive())
			{
				modelStack.PushMatrix();
				Vector3 pos = go->pos;
				pos.x -= player->pos.x;// Move to player pos
				pos.y -= player->pos.y;// Move to player pos
				//sphere space == radius = 1
				pos.x /= m_worldWidth * zoom; //convert to regular sphere space
				pos.y /= m_worldHeight * zoom;//convert to regular sphere space
				Vector3 scale = go->GetScale();
				scale.x /= m_worldWidth * zoom; //convert to regular sphere space
				scale.y /= m_worldHeight * zoom;//convert to regular sphere space
				
				switch (go->GetType())
				{
				case GameObject::GO_ENVIRONMENT:
					switch (go->collider.type)
					{
					case Collider::COLLIDER_BALL:
						modelStack.Translate(pos.x, pos.y, pos.z);
						modelStack.Scale(scale.x, scale.y, scale.z);
						RenderMesh(meshList[GEO_MINIMAP_PLAYER_ICON], false);
						break;
					case Collider::COLLIDER_BOX:
					{
						float degree = Math::RadianToDegree(atan2(go->GetFront().y, go->GetFront().x));
						modelStack.Translate(pos.x, pos.y, pos.z);
						modelStack.Rotate(degree, 0, 0, 1);
						modelStack.Scale(scale.x, scale.y, scale.z);
						RenderMesh(meshList[GEO_QUAD], false);
					}
						break;

					default:break;
					}
					break;
				case GameObject::GO_PROJECTILE:
					break;
				case GameObject::GO_ENTITY:
				{
					Entity* entity = dynamic_cast<Entity*>(go);
					if (entity)
					{
					  switch (entity->GetEntityType())
					  {
					  case Entity::ENTITY_PLAYER:
						  modelStack.Translate(pos.x, pos.y, pos.z);
						  modelStack.Scale(scale.x, scale.y, scale.z);
						  RenderMesh(meshList[GEO_MINIMAP_PLAYER_ICON], false);
						  break;
					  case Entity::ENTITY_BOSS_MAIN:
						  modelStack.Translate(pos.x, pos.y, pos.z);
						  modelStack.Scale(scale.x, scale.y, scale.z);
						  RenderMesh(meshList[GEO_MINIMAP_BOSS_MAIN_ICON], false);
						  break;
					  case Entity::ENTITY_BOSS_BODY:
						  modelStack.Translate(pos.x, pos.y, pos.z);
						  modelStack.Scale(scale.x, scale.y, scale.z);
						  RenderMesh(meshList[GEO_MINIMAP_BOSS_BODY_ICON], false);
						  break;

					  default:
						  break;
					  }
					}
					break;
				}
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

void SceneTutorial::Exit()
{
	SceneBase::Exit();
}

void SceneTutorial::UpdateGameObjects(double dt)
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
					if (go2->IsActive() &&
						go2->GetType() != GameObject::GO_PROJECTILE)//only allow projectiles to check against GOs
					{
						go->HandleInteraction(go2, dt);
					}
				}
			}

			go->HandleOutOfBounds(0, m_worldWidth, 0, m_worldHeight);
		}
	}
}

void SceneTutorial::RenderGameObjects()
{
	SceneBase::RenderGameObjects();
}