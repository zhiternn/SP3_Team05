#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include <sstream>

#include "Controls.h"

SceneBase::SceneBase()
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
	lights[0].position.Set(600, 600, 600);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.0f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
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
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].
		exponent);

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

	for (int i = 0; i < GEO_DEFAULT_END; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1));
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

    // Projectile Render
    meshList[P_BULLET] = MeshBuilder::GenerateCube("p_bullet", Color(1, 1, 1));

	bLightEnabled = true;
	isCulled = true;
	isWireFrame = false;
}

void SceneBase::Update(double dt)
{
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
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

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
	for (int i = 0; i < GEO_DEFAULT_END; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	// Cleanup VBO
	glDeleteProgram(m_gPassShaderID);
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}