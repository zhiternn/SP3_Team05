#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include "LoadHmap.h"
#include "Controls.h"
#include "MeshManager.h"


#include <sstream>

SceneBase::SceneBase():
player(NULL),
options(OptionManager::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	// Get a handle for our fog
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	// Get a handle for our shadows
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	// Use our shader
	glUseProgram(m_programID);

	//light settings
	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(-10, -10, 50);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.0f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.2f, -0.2f, 1.f);

	//lights[1].type = Light::LIGHT_POINT;
	//lights[1].position.Set(1, 1, 0);
	//lights[1].color.Set(1, 1, 0.5f);
	//lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	//Fog settings
	Color fogColor(0.5f, 0.1f, 0.9f);

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 100);
	glUniform1f(m_parameters[U_FOG_END], 3000.0f);
	glUniform1f(m_parameters[U_FOG_DENSITY], 1.0f);
	glUniform1f(m_parameters[U_FOG_TYPE], 1);
	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	m_lightDepthFBO.Init(1024, 1024);

	//World Space
	m_worldHeight = 300;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Camera Space View
	m_orthoHeight = 200;
	m_orthoWidth = m_orthoHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	backgroundList.push_back(meshList[GEO_BACKGROUND2]);
	backgroundList.push_back(meshList[GEO_BACKGROUND3]);
	backgroundList.push_back(meshList[GEO_BACKGROUND2]);
	backgroundList.push_back(meshList[GEO_BACKGROUND3]);

	mainCamera = new Camera();
	mainCamera->Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	bLightEnabled = true;
	isCulled = true;
	isWireFrame = false;

	if (player == NULL)
		player = new Player();

	player->SetScale(3, 3, 3);

	GamePad = Gamepad(1);
	glfwController = GLFWController();

	//Get player Controls
	useController = options.UseControl();

	mainCamera->Include(&(player->pos));
	if (!(GamePad.IsConnected() && useController))
	{
		mainCamera->Include(&mousePos_worldBased);
	}
	else
	{
		mainCamera->Include(&controllerStick_Pos);
	}
}

void SceneBase::PlayerController(double dt)
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
	if (Controls::GetInstance().OnPress(Controls::KEY_NUMPAD_0))
	{
		if (player->IsDead())
		{
			player->Init(Vector3(m_worldWidth / 2, m_worldHeight / 2, 0));
		}
		player->SetHP(player->maxHealth);
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_NUMPAD_1))
	{
		player->inventory->AddCurrency(10000);
	}
}

void SceneBase::GetGamePadInput(double dt)
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
	if (GamePad.LeftTrigger() > 0.2f)
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
	if (GamePad.GetButtonDown(8) > 0.5f)
	{
		player->ChangeProjectileUp();
	}
	//= Right Bumper
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

void SceneBase::Update(double dt)
{
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
	if (mainCamera->Deadzone(&player->GetPosition(), mainCamera->GetPosition(), m_orthoHeight))
	{
		//Check if Gamepad is connected for controller input
		if (useController && GamePad.IsConnected())
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

	if (Controls::GetInstance().OnPress(Controls::KEY_1))
		isCulled = true;
	if (Controls::GetInstance().OnPress(Controls::KEY_2))
		isCulled = false;
	if (Controls::GetInstance().OnPress(Controls::KEY_3))
		isWireFrame = false;
	if (Controls::GetInstance().OnPress(Controls::KEY_4))
		isWireFrame = true;

	if (Controls::GetInstance().OnPress(Controls::KEY_5))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Controls::GetInstance().OnPress(Controls::KEY_6))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Controls::GetInstance().OnPress(Controls::KEY_7))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	if (Controls::GetInstance().OnPress(Controls::KEY_8))
	{
		bLightEnabled = true;
	}
	else if (Controls::GetInstance().OnPress(Controls::KEY_9))
	{
		bLightEnabled = false;
	}

	if (Controls::GetInstance().OnHold(Controls::KEY_I))
		lights[0].position.z -= (float)(30.f * dt);
	if (Controls::GetInstance().OnHold(Controls::KEY_K))
		lights[0].position.z += (float)(30.f * dt);
	if (Controls::GetInstance().OnHold(Controls::KEY_J))
		lights[0].position.x -= (float)(30.f * dt);
	if (Controls::GetInstance().OnHold(Controls::KEY_L))
		lights[0].position.x += (float)(30.f * dt);
	if (Controls::GetInstance().OnHold(Controls::KEY_O))
		lights[0].position.y -= (float)(30.f * dt);
	if (Controls::GetInstance().OnHold(Controls::KEY_P))
		lights[0].position.y += (float)(30.f * dt);

	fps = (float)(1.f / dt);

	if (isCulled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (isWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Update particles
	for (int i = 0; i < Particle::particleList.size(); ++i)
	{
		if (Particle::particleList[i]->active)
			Particle::particleList[i]->Update(dt);
	}

	//static float time = 0.0f;
	//time += dt * 0.1f;
	//lights[0].position.x = sinf(time) * 1000.0f;
	//lights[0].position.z = cosf(time) * 1000.0f;
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.6f + 0.5f, 0.5f, 0); //0.6f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	modelStack.PopMatrix();
}

void SceneBase::RenderUI(Mesh* mesh, float size, float x, float y, float scaleX, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	modelStack.Scale(scaleX, 1, 1);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	modelStack.PopMatrix();
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	if (m_renderPass == RENDER_PASS_PRE)
	{
		for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
			{
				Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

			}
		}

		mesh->Render();
		return;
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}

	mesh->Render();
}

void SceneBase::RenderMinimap(float zoom)
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
			GameObject *go = GameObject::goList[i];
			if (go->GetType() == GameObject::GO_WALL)
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
				modelStack.Translate(pos.x, pos.y, pos.z);

				switch (go->GetCollider().type)
				{
				case Collider::COLLIDER_BALL:
					modelStack.Scale(scale.x, scale.y, scale.z);
					RenderMesh(meshList[GEO_SPHERE], false);
					break;
				case Collider::COLLIDER_BOX:
				{
					float degree = Math::RadianToDegree(atan2(go->GetFront().y, go->GetFront().x));
					modelStack.Rotate(degree, 0, 0, 1);
					modelStack.Scale(scale.x, scale.y, scale.z);
					RenderMesh(meshList[GEO_QUAD], false);
				}
					break;
				default:break;
				}

				modelStack.PopMatrix();
			}
			else
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
	}

	glDisable(GL_STENCIL_TEST);

	glLineWidth(5.0f);
	RenderMesh(meshList[GEO_MINIMAP_BORDER], false);
	glLineWidth(1.0f);
}

void SceneBase::RenderHUD()
{
	//Render Minimap
	modelStack.PushMatrix();
	modelStack.Translate(70, 50, 0);
	modelStack.Scale(18, 18, 1);
	RenderMinimap(1.0f);
	modelStack.PopMatrix();

	if (!((GamePad.IsConnected() && useController)))
	{
		// Render the crosshair
		modelStack.PushMatrix();
		modelStack.Translate(mousePos_screenBased.x * 80 / m_orthoWidth, mousePos_screenBased.y * 60 / m_orthoHeight, 6);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_CROSSHAIR], false);
		modelStack.PopMatrix();
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.5f, 0, 0);

	ss.str("");
	ss.precision(4);
	ss << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderUI(meshList[GEO_UI_BACKGROUND], 7, 0, 60, 15, false);

	ss.str("");
	ss << "Weapon: ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 57);
	switch (player->weapon->weapon_type)
	{
	case Weapon::W_SHOTGUN:
		RenderUI(meshList[GEO_WEAPON_SHOTGUN], 6, 17, 58.f, 1, false);
		break;
	case Weapon::W_MACHINEGUN:
		RenderUI(meshList[GEO_WEAPON_MACHINEGUN], 6, 17, 58.f, 1, false);
		break;
	case Weapon::W_SPLITGUN:
		RenderUI(meshList[GEO_WEAPON_SPLITGUN], 6, 17, 58.f, 1, false);
		break;
	}

	ss.str("");
	ss << "Projectile: ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 22, 57);
	switch (player->projectile->GetProjType())
	{
	case CProjectile::BULLET:
		RenderUI(meshList[GEO_PROJECTILE_BULLET], 4, 44, 58.f, 1, false);
		break;
	case CProjectile::HOOK:
		RenderUI(meshList[GEO_PROJECTILE_HOOK], 4, 44, 58.f, 1, false);
		break;
	case CProjectile::TRAP:
		RenderUI(meshList[GEO_PROJECTILE_TRAP], 4, 44, 58.f, 1, false);
		break;
	}

	ss.str("");
	ss.precision(1);
	ss << "HP";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 53);
	RenderUI(meshList[GEO_BORDER], 2, (player->maxHealth / 10) + 11, 54.5f, player->maxHealth / 10, false);
	RenderUI(meshList[GEO_HEALTH], 2, (player->GetHP() / 10) + 11, 54.5f, player->GetHP() / 10, false);

	ss.str("");
	ss.precision(2);
	ss << "Dash";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 50);
	RenderUI(meshList[GEO_BORDER], 2, (DASH_COOLDOWN * player->maxHealth / 10) + 11, 51.5f, DASH_COOLDOWN * player->maxHealth / 10, false);
	RenderUI(meshList[GEO_DASH], 2, ((DASH_COOLDOWN - player->cooldownTimer) * player->maxHealth / 10) + 11, 51.5f, (DASH_COOLDOWN - player->cooldownTimer) * player->maxHealth / 10, false);
}

void SceneBase::RenderGameObjects()
{
	for (int i = 0; i < GameObject::goList.size(); ++i)
	{
		if (GameObject::goList[i]->IsActive())
		{
			modelStack.PushMatrix();

			GameObject::goList[i]->SetupMesh();
			if (GameObject::goList[i]->mesh)
				RenderMesh(GameObject::goList[i]->mesh, GameObject::goList[i]->mesh->enableLight);

			modelStack.PopMatrix();
			Enemy* enemy = dynamic_cast<Enemy*>(GameObject::goList[i]);
			if (enemy)
			{
				if (!enemy->IsDead())
				{
					if ((mousePos_worldBased - enemy->pos).LengthSquared() < enemy->GetScale().LengthSquared() || enemy->IsCapturing())
					{
						float healthRatio = (float)enemy->GetHP() / (float)enemy->GetMaxHP();
						modelStack.PushMatrix();
						modelStack.Translate(enemy->pos.x, enemy->pos.y + enemy->GetScale().y, 50);
						modelStack.Translate(-enemy->GetScale().x, 0, 0);

						modelStack.PushMatrix();
						modelStack.Scale(enemy->GetScale().x * 2 * healthRatio, 5, 1);
						modelStack.Translate(0.5f, 0.5f, 1);
						RenderMesh(meshList[GEO_HEALTH], false);
						modelStack.PopMatrix();

						modelStack.PushMatrix();
						modelStack.Scale(enemy->GetScale().x * 2, 5, 1);
						modelStack.Translate(0.5f, 0.5f, 0);
						RenderMesh(meshList[GEO_BORDER], false);
						modelStack.PopMatrix();

						modelStack.PopMatrix();
					}
				}
				if (enemy->IsCapturing())
				{
					float healthRatio = (float)enemy->GetHP() / (float)enemy->GetMaxHP();
					float captureRatio = (float)enemy->GetCaptureRate() / (float)enemy->GetHP();
					modelStack.PushMatrix();
					modelStack.Translate(enemy->pos.x, enemy->pos.y + enemy->GetScale().y + 5.0f, 50);
					modelStack.Translate(-enemy->GetScale().x, 0, 0);

					modelStack.PushMatrix();
					modelStack.Scale((enemy->GetScale().x * 2 * healthRatio) * captureRatio, 5, 1);
					modelStack.Translate(0.5f, 0.5f, 1);
					RenderMesh(meshList[GEO_CAPTURE], false);
					modelStack.PopMatrix();

					modelStack.PopMatrix();
					//modelStack.PushMatrix();
					//modelStack.Translate(enemy->pos.x, enemy->pos.y + enemy->GetScale().x + 7, 50);
					//modelStack.Scale(2, 5, 2);
					//modelStack.Scale(enemy->GetCaptureRate() / 10.f, 1, 1);
					//RenderMesh(meshList[GEO_CAPTURE], false);
					//modelStack.PopMatrix();
				}
			}
		}
	}
}

void SceneBase::RenderBackground()
{
	SetHUD(true, -0.5f, 0.5f, -0.5f, 0.5f);

	RenderMesh(meshList[GEO_BACKGROUND1], false);

	for (int i = 0; i < backgroundList.size(); ++i)
	{
		float xOffset = -mainCamera->target.x / ((backgroundList.size()) + 0.1f - i);
		float yOffset = -mainCamera->target.y / ((backgroundList.size()) + 0.1f - i);

		if (abs(xOffset) > 1)
		{
			xOffset -= Math::GetSign(xOffset);
		}
		if (abs(yOffset) > 1)
		{
			yOffset -= Math::GetSign(yOffset);
		}

		modelStack.PushMatrix();
		modelStack.Translate(xOffset / m_worldWidth, yOffset / m_worldHeight, 0);
		RenderMesh(backgroundList[i], false);

		modelStack.PushMatrix();
		modelStack.Translate(1, 0, 0);
		RenderMesh(backgroundList[i], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 1, 0);
		RenderMesh(backgroundList[i], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(1, 1, 0);
		RenderMesh(backgroundList[i], false);
		modelStack.PopMatrix();
		
		modelStack.PopMatrix();
	}

	SetHUD(false);
}

void SceneBase::RenderParticles()
{
	for (int i = 0; i < Particle::particleList.size(); ++i)
	{
		if (Particle::particleList[i]->active)
		{
			modelStack.PushMatrix();
			Particle::particleList[i]->SetupMesh();
			RenderMesh(Particle::particleList[i]->mesh, false);
			modelStack.PopMatrix();
		}
	}
}

void SceneBase::UpdateGameObjects(double dt)
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
					if (go2->IsActive() && go2->GetType() != GameObject::GO_PROJECTILE)
					{
						go->HandleInteraction(go2, dt);
					}
				}
			}

			go->HandleOutOfBounds(0, m_worldWidth, 0, m_worldHeight);
		}
	}
}

//void SceneBase::RenderGPass()
//{
//	//m_renderPass = RENDER_PASS_PRE;
//
//	//m_lightDepthFBO.BindForWriting();
//	//glEnable(GL_CULL_FACE);
//	//glCullFace(GL_BACK);
//	//glClear(GL_DEPTH_BUFFER_BIT);
//	//glUseProgram(m_gPassShaderID);
//	////These matrices should change when light position or direction changes
//	//if (lights[0].type == Light::LIGHT_DIRECTIONAL)
//	//	m_lightDepthProj.SetToOrtho(-800, 800, -600, 600, 0, 10000);
//	//else
//	//	m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
//
//	//m_lightDepthView.SetToLookAt(lights[0].position.x,
//	//	lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
//}
//
//void SceneBase::RenderMain()
//{
//	//m_renderPass = RENDER_PASS_MAIN;
//
//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	//glViewport(0, 0, Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
//	//glEnable(GL_CULL_FACE);
//	//glCullFace(GL_BACK);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//glUseProgram(m_programID);
//	////pass light depth texture
//	//m_lightDepthFBO.BindForReading(GL_TEXTURE8);
//	//glUniform1i(m_parameters[U_SHADOW_MAP], 8);
//}
//
//void SceneBase::RenderWorld()
//{
//	//RenderGround();
//	//RenderSkybox();
//}
// SceneBase::void RenderHUD()
//{
//
//}

void SceneBase::SetupLights()
{
	//if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	//{
	//	Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
	//	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	//}
	//else if (lights[0].type == Light::LIGHT_SPOT)
	//{
	//	Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
	//	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	//	Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
	//	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	//}
	//else
	//{
	//	Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
	//	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	//}
	//if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	//{
	//	Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
	//	Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
	//	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	//}
	//else if (lights[1].type == Light::LIGHT_SPOT)
	//{
	//	Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
	//	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	//	Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
	//	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	//}
	//else
	//{
	//	Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
	//	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	//}
	for (int i = 0; i < 2; ++i)
	{
		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * 10)], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * 10)], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION + (i * 10)], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + (i * 10)], 1, &lightPosition_cameraspace.x);
		}
	}
}
void SceneBase::SetHUD(const bool HUDmode, float fromX, float toX, float fromY, float toY, float fromZ, float toZ)
{
	if (HUDmode)
	{
		glDisable(GL_DEPTH_TEST);

		viewStack.PushMatrix();
		viewStack.LoadIdentity();
		projectionStack.PushMatrix();
		projectionStack.LoadIdentity();

		Mtx44 ortho;
		ortho.SetToOrtho(fromX, toX, fromY, toY, fromZ, toZ);
		projectionStack.LoadMatrix(ortho);
	}
	else if (!HUDmode)
	{
		glEnable(GL_DEPTH_TEST);

		projectionStack.PopMatrix();
		viewStack.PopMatrix();
	}
}
void SceneBase::SetCull(const bool cullMode)
{
	if (cullMode)
	{
		isCulled = true;

		glEnable(GL_CULL_FACE);
	}
	else if (!cullMode)
	{
		isCulled = false;

		glDisable(GL_CULL_FACE);
	}
}
void SceneBase::SetWireFrame(const bool wiredMode)
{
	if (wiredMode && isWireFrame == false)
	{
		isWireFrame = true;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (!wiredMode && isWireFrame == true)
	{
		isWireFrame = false;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUniform1f(m_parameters[U_FOG_ENABLED], 1);
	//RenderGPass();
	//RenderMain();
	//glUniform1f(m_parameters[U_FOG_ENABLED], 0);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	glDeleteProgram(m_gPassShaderID);
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	while (GameObject::goList.size() > 0)
	{
		delete GameObject::goList.back();
		GameObject::goList.pop_back();
	}
	if (player)
		delete player;
}