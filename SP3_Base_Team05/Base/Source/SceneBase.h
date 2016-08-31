#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Particle.h"
#include "Scene.h"
#include "Mtx44.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Particle.h"
#include "Camera.h"
#include "DepthFBO.h"
#include "Gamepad.h"
#include "GLFWController.h"
#include "Player.h"
#include "OptionManager.h"

class SceneBase : public Scene
{
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGPass();
	//void RenderMain();
	//void RenderWorld();
	//void RenderHUD();

protected:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,

		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_TOTAL,
	};
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

	virtual void RenderText(Mesh* mesh, std::string text, Color color);
	virtual void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	virtual void RenderMesh(Mesh *mesh, bool enableLight);
	virtual void RenderUI(Mesh* mesh, float size, float x, float y, float scaleX, bool enableLight);
	virtual void RenderHUD();
	virtual void RenderGameObjects();
	virtual void RenderBackground();
	virtual void RenderParticles();
	virtual void UpdateGameObjects(double dt);
	virtual void PlayerController(double dt);
	virtual void GetGamePadInput(double dt);

	void RenderMinimap(float zoom);
	void SetupLights();
	void SetHUD(const bool HUDmode, float fromX = 0.0f, float toX = 80.0f, float fromY = 0.0f, float toY = 60.0f, float fromZ = -10.0f, float toZ = 10.0f);
	void SetCull(const bool cullMode);
	void SetWireFrame(const bool wiredMode);

	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	RENDER_PASS m_renderPass;

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;
	bool isCulled;
	bool isWireFrame;
	bool useController;
	bool Keyboard;

	float fps;
	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;
	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	Camera* mainCamera;

	static Player* player;
	OptionManager &options;

	std::vector<Mesh*> backgroundList;

	//Create the Gamepad object
	//Gamepad GamePad;
	Vector3 controllerStick_Pos;
	Vector3 controllerStick_WorldPos;
	Vector3 stickDir;

	//Create glfwController
	GLFWController glfwController;

};

#endif