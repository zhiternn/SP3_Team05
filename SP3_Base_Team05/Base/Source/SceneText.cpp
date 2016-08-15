#include "SceneText.h"

#include "Application.h"
#include "Controls.h"

#include <sstream>

SceneText::SceneText()
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	//Clears meshList
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	
	//Insert default Meshes into meshList
	for (int i = 0; i < GEO_DEFAULT_END; ++i)
	{
		meshList[i] = SceneBase::meshList[i];
	}
	
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

	m_worldHeight = 100.0f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_orthoHeight = 100.0f;
	m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	mainCamera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	m_ghost = new GameObject(GameObject::GO_BALL);

	GameObject *go = FetchGO();
	go->active = true;
	go->scale.Set(20, 20, 20);
	go->front.Set(1, 0, 0);
	go->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	go->type = GameObject::GO_CUBE;
	go->collider.type = Collider::COLLIDER_BOX;
}

void SceneText::Update(double dt)
{
	SceneBase::Update(dt);

	if (Controls::GetInstance().OnPress(Controls::MOUSE_LBUTTON))
	{
		double x, y;
		Application::GetCursorPos(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		x = m_worldWidth * (x / w);
		y = m_worldHeight * ((h - y) / h);

		m_ghost->pos.Set(x, y, 0);
		m_ghost->scale.Set(1, 1, 1);
		m_ghost->active = true;
	}
	else if(Controls::GetInstance().OnRelease(Controls::MOUSE_LBUTTON))
	{
		GameObject* ball = FetchGO();
		ball->pos = m_ghost->pos;

		double x, y;
		Application::GetCursorPos(x, y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		x = m_worldWidth * (x / w);
		y = m_worldHeight * ((h - y) / h);

		ball->vel = m_ghost->pos - Vector3(x, y, 0);
		ball->scale.Set(1, 1, 1);
		ball->mass = 1;
		ball->collider.type = Collider::COLLIDER_BALL;
		m_ghost->active = false;
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_V))
	{
		for (int i = 0; i < 10; ++i)
		{
			double x, y;
			Application::GetCursorPos(x, y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			x = m_worldWidth * (x / w);
			y = m_worldHeight * ((h - y) / h);

			GameObject* ball = FetchGO();
			ball->pos.Set(x, y, 0);
			ball->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5));
			ball->scale.Set(1, 1, 1);
			ball->mass = 1;
			ball->collider.type = Collider::COLLIDER_BALL;
			m_ghost->active = false;
		}
	}

	UpdateGameObjects(dt);

	mainCamera.Update(dt);
}

void SceneText::Render()
{
	Mtx44 perspective;
	//perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	perspective.SetToOrtho(0, m_orthoWidth, 0, m_orthoHeight, -100, 100);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		mainCamera.position.x, mainCamera.position.y, mainCamera.position.z,
		mainCamera.target.x, mainCamera.target.y, mainCamera.target.z,
		mainCamera.up.x, mainCamera.up.y, mainCamera.up.z
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
void SceneText::RenderSkybox()
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

void SceneText::RenderSkyPlane()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 2500, 0);
	//modelStack.Scale(2.0f, 2.0f, 2.0f);
	//RenderMesh(meshList[GEO_SKYPLANE], false);
	//modelStack.PopMatrix();
}

void SceneText::RenderGPass()
{
	m_renderPass = RENDER_PASS_PRE;

	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or direction changes
		if (lights[0].type == Light::LIGHT_DIRECTIONAL)
			m_lightDepthProj.SetToOrtho(-800, 800, -600, 600, 0, 10000);
		else
			m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

	m_lightDepthView.SetToLookAt(lights[0].position.x,
		lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void SceneText::RenderMain()
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

	if (m_ghost->active)
		RenderGO(m_ghost);
}

void SceneText::RenderWorld()
{
	RenderGameObjects();
}

void SceneText::RenderHUD()
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
}

void SceneText::Exit()
{
	SceneBase::Exit();

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
}

void SceneText::UpdateGameObjects(double dt)
{
	for (int i = 0; i < GameObject::goList.size(); ++i)
	{
		GameObject *go = GameObject::goList[i];
		if (go->active)
		{
			go->Update(dt);

			if (go->collider.type == Collider::COLLIDER_BALL)
			{
				for (int j = 0; j < GameObject::goList.size(); ++j)
				{
					GameObject *go2 = GameObject::goList[j];
					if (go2->active)
					{
						if (go->CheckCollision(go2, dt))
						{
							go->CollisionResponse(go2);
						}
					}
				}
			}

			{//Handles out of bounds
				//Check Horizontally against edges
				if ((go->pos.x + go->scale.x > m_worldWidth && go->vel.x > 0) ||
					(go->pos.x - go->scale.x < 0 && go->vel.x < 0))
				{
					go->vel.x *= -1;
				}
				//remove if it cant be seen completely
				else if (go->pos.x - go->scale.x > m_worldWidth ||
					go->pos.x + go->scale.x < 0)
				{
					go->active = false;
				}
				//Check Vertically against edges
				if ((go->pos.y + go->scale.y > m_worldHeight && go->vel.y > 0) ||
					(go->pos.y - go->scale.y < 0 && go->vel.y < 0))
				{
					go->vel.y *= -1;
				}
				//remove if it cant be seen completely
				else if (go->pos.y - go->scale.y > m_worldWidth ||
					go->pos.y + go->scale.y < 0)
				{
					go->active = false;
				}
			}
		}
	}
}

void SceneText::RenderGO(GameObject* go)
{
	modelStack.PushMatrix();

	switch (go->type)
	{
	case GameObject::GO_BALL:
	{
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPHERE], false);
	}
		break;
	case GameObject::GO_CUBE:
	{
		float degree = Math::RadianToDegree(atan2(go->front.y, go->front.x));
		
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(degree, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
	}
		break;


	default:break;
	}

	modelStack.PopMatrix();
}

void SceneText::RenderGameObjects()
{
	for (int i = 0; i < GameObject::goList.size(); ++i)
	{
		if (GameObject::goList[i]->active)
			RenderGO(GameObject::goList[i]);
	}
}