#include "MeshManager.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

MS MeshManager::_modelStack;
Mesh* MeshManager::_meshList[GEO_DEFAULT_END];

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
	for (int i = 0; i < GEO_DEFAULT_END; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
}

void MeshManager::Init()
{
	for (int i = 0; i < GEO_DEFAULT_END; ++i)
	{
		if (meshList[i])
			delete meshList[i];

		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
    meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("crosshair", Color(0, 0, 0), 1.f);
    meshList[GEO_CROSSHAIR]->textureArray[0] = LoadTGA("Image//target.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1));
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);

	//meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	//meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_BULLET]->material.kAmbient.Set(0.5f, 0.1f, 0.1f);
	meshList[GEO_BULLET]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_BULLET]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_GRENADE];
	meshList[GEO_SHIELD];

	meshList[GEO_ROPE] = MeshBuilder::GenerateQuad("rope", Color(1, 1, 1), 1);
	meshList[GEO_ROPE]->textureArray[0] = LoadTGA("Image//rope.tga");
	meshList[GEO_TRAP] = MeshBuilder::GenerateSphere("trap", Color(1, 0, 0), 9, 18, 1.f);

	meshList[GEO_FIELD] = MeshBuilder::GenerateSphere("Forcefield", Color(1, 1, 1), 9, 18, 1.f);

	meshList[GEO_PLAYER_TOP] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_top.obj");
	meshList[GEO_PLAYER_TOP]->textureArray[0] = LoadTGA("Image//player.tga");
	//meshList[GEO_PLAYER_TOP]->textureArray[1] = LoadTGA("Image//player2.tga");
	//meshList[GEO_PLAYER_TOP]->textureArray[2] = LoadTGA("Image//player3.tga");
	meshList[GEO_PLAYER_BOTTOM] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_bottom.obj");
	meshList[GEO_PLAYER_BOTTOM]->textureArray[0] = LoadTGA("Image//player.tga");
	//meshList[GEO_PLAYER_BOTTOM]->textureArray[1] = LoadTGA("Image//player2.tga");
	//meshList[GEO_PLAYER_BOTTOM]->textureArray[2] = LoadTGA("Image//player3.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.4f, 0.4f, 0.4f), 1.f, 8.0f);
	meshList[GEO_FLOOR]->textureArray[0] = LoadTGA("Image//floor.tga");

	meshList[GEO_MINIMAP] = MeshBuilder::GenerateCircle("floor", Color(0.4f, 0.4f, 0.4f));
	meshList[GEO_MINIMAP]->textureArray[0] = LoadTGA("Image//floor.tga");

	meshList[GEO_BORDER] = MeshBuilder::GenerateQuad("border", Color(0, 0, 0), 1.f);
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("health", Color(0, 1, 0), 1.f);
	meshList[GEO_DASH] = MeshBuilder::GenerateQuad("dash", Color(0, 0.3f, 0.7f), 1.f);

	meshList[GEO_WEAPON_MACHINEGUN] = MeshBuilder::GenerateQuad("machinegun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_MACHINEGUN]->textureArray[0] = LoadTGA("Image//machinegun.tga");
	meshList[GEO_WEAPON_SHOTGUN] = MeshBuilder::GenerateQuad("shotgun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_SHOTGUN]->textureArray[0] = LoadTGA("Image//shotgun.tga");
	meshList[GEO_WEAPON_SPLITGUN] = MeshBuilder::GenerateQuad("splitgun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_SPLITGUN]->textureArray[0] = LoadTGA("Image//splitgun.tga");

	//menu
	meshList[GEO_MENU_CURSOR] = MeshBuilder::GenerateQuad("GEO_MENU_CURSOR", Color(0, 0, 0), 1.f);
	meshList[GEO_MENU_CURSOR]->textureArray[0] = LoadTGA("Image//menucursor.tga");

	meshList[GEO_MENU_BACKGROUND] = MeshBuilder::GenerateQuad("GEO_MENU_BACKGROUND", Color(0, 0, 0), 1.f);
	meshList[GEO_MENU_BACKGROUND]->textureArray[0] = LoadTGA("Image//background.tga");

	InitSceneSnake();
	InitSceneDetlaff();
	InitSceneSummoner();
	InitSceneGolem();
}

void MeshManager::InitSceneDetlaff()
{
	meshList[GEO_DETLAFF_1] = MeshBuilder::GenerateQuad("Detlaff_1", Color(1, 1, 1), 1.f);
	meshList[GEO_DETLAFF_1]->textureArray[0] = LoadTGA("Image//Detlaff_TGA//detlaff_stage1.tga");
	meshList[GEO_DETLAFF_2] = MeshBuilder::GenerateCube("cube", Color(0, 1, 1));
	meshList[GEO_DETLAFF_3] = MeshBuilder::GenerateCube("cube", Color(0, 1, 1));
}

void MeshManager::InitSceneSnake()
{
	meshList[GEO_SCENESNAKE_WALLS] = MeshBuilder::GenerateCube("SceneSnake_walls", Color(0.4f, 0.4f, 0.4f));
	meshList[GEO_SCENESNAKE_WALLS]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCENESNAKE_WALLS]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCENESNAKE_WALLS]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);

	meshList[GEO_SNAKE_HEAD] = MeshBuilder::GenerateOBJ("snake_head", "Obj\\snake_head.obj");
	meshList[GEO_SNAKE_HEAD]->textureArray[0] = LoadTGA("Image//snake_head.tga");
	meshList[GEO_SNAKE_BODY] = MeshBuilder::GenerateOBJ("snake_body", "Obj\\snake_body.obj");
	meshList[GEO_SNAKE_BODY]->textureArray[0] = LoadTGA("Image//snake_body.tga");
}

void MeshManager::InitSceneSummoner()
{
	//meshList[GEO_SUMMONER] = MeshBuilder::GenerateOBJ("summoner", "Obj\\summoner.obj");
	meshList[GEO_SUMMONER] = MeshBuilder::GenerateSphere("Forcefield", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_SUMMONER]->textureArray[0] = LoadTGA("Image//summoner.tga");
	meshList[GEO_SUMMONS] = MeshBuilder::GenerateOBJ("summoner", "Obj\\summons.obj");
}

void MeshManager::InitSceneGolem()
{
    //meshList[GEO_GOLEMHEAD] = MeshBuilder::GenerateOBJ("GEO_GOLEMHEAD", "Obj//golemhead.obj");
    //meshList[GEO_GOLEMHEAD]->textureArray[0] = LoadTGA("Image//golemhead.tga");
    //meshList[GEO_GOLEMHAND] = MeshBuilder::GenerateOBJ("GEO_GOLEMHAND", "Obj//golemhead.obj");
    //meshList[GEO_GOLEMHAND]->textureArray[0] = LoadTGA("Image//fist.tga");

	meshList[GEO_GOLEMHEAD] = MeshBuilder::GenerateSphere("Forcefield", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_GOLEMHAND] = MeshBuilder::GenerateSphere("Forcefield", Color(1, 1, 1), 9, 18, 1.f);
}