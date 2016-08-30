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

	meshList[GEO_TRAP] = MeshBuilder::GenerateOBJ("trap", "Obj//sphereobj.obj");
	meshList[GEO_TRAP]->textureArray[0] = LoadTGA("Image//trap.tga");
	meshList[GEO_TRAP]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TRAP]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TRAP]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);

	meshList[GEO_PLAYER_TOP] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_top.obj");
	meshList[GEO_PLAYER_TOP]->enableLight = true;
	meshList[GEO_PLAYER_TOP]->textureArray[0] = LoadTGA("Image//player.tga");
	//meshList[GEO_PLAYER_TOP]->textureArray[1] = LoadTGA("Image//player2.tga");
	//meshList[GEO_PLAYER_TOP]->textureArray[2] = LoadTGA("Image//player3.tga");
	meshList[GEO_PLAYER_BOTTOM] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_bottom.obj");
	meshList[GEO_PLAYER_BOTTOM]->enableLight = true;
	meshList[GEO_PLAYER_BOTTOM]->textureArray[0] = LoadTGA("Image//player.tga");
	//meshList[GEO_PLAYER_BOTTOM]->textureArray[1] = LoadTGA("Image//player2.tga");
	//meshList[GEO_PLAYER_BOTTOM]->textureArray[2] = LoadTGA("Image//player3.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.4f, 0.4f, 0.4f), 1.f, 4.0f);
	meshList[GEO_FLOOR]->textureArray[0] = LoadTGA("Image//hex_floor_default.tga");
	meshList[GEO_FLOOR]->textureArray[1] = LoadTGA("Image//hex_floor_emissive.tga");

	meshList[GEO_BACKGROUND1] = MeshBuilder::GenerateQuad("bg1", Color(0.4f, 0.4f, 0.4f), 1.f);
	meshList[GEO_BACKGROUND1]->textureArray[0] = LoadTGA("Image//bg1.tga");
	meshList[GEO_BACKGROUND2] = MeshBuilder::GenerateQuad("bg2", Color(0.4f, 0.4f, 0.4f), 1.f);
	meshList[GEO_BACKGROUND2]->textureArray[0] = LoadTGA("Image//bg2.tga");
	meshList[GEO_BACKGROUND3] = MeshBuilder::GenerateQuad("bg3", Color(0.4f, 0.4f, 0.4f), 1.f);
	meshList[GEO_BACKGROUND3]->textureArray[0] = LoadTGA("Image//bg3.tga");


	meshList[GEO_FLOOR_HEX] = MeshBuilder::GenerateQuad("floor", Color(0.4f, 0.4f, 0.4f), 1.f, 3.0f);
	meshList[GEO_FLOOR_HEX]->enableLight = true;
	meshList[GEO_FLOOR_HEX]->textureArray[0] = LoadTGA("Image//hex_floor_default.tga");
	meshList[GEO_FLOOR_HEX]->textureArray[1] = LoadTGA("Image//hex_floor_emissive.tga");

	meshList[GEO_MINIMAP] = MeshBuilder::GenerateCircle("minimap_background", Color(0.4f, 0.4f, 0.4f));
	meshList[GEO_MINIMAP]->textureArray[0] = LoadTGA("Image//minimap_background.tga");
	meshList[GEO_MINIMAP_BORDER] = MeshBuilder::GenerateCircleOutline("minimap_background_outline", Color(0.4f, 0.4f, 0.4f));
	meshList[GEO_MINIMAP_PLAYER_ICON] = MeshBuilder::GenerateCircle("player", Color(0.1f, 0.9f, 0.1f), 2);
	meshList[GEO_MINIMAP_BOSS_MAIN_ICON] = MeshBuilder::GenerateCircle("minimap_background", Color(1.0f, 0.0f, 0.0f), 2);
	meshList[GEO_MINIMAP_BOSS_BODY_ICON] = MeshBuilder::GenerateCircle("minimap_background", Color(0.5f, 0.1f, 0.1f), 2);

	meshList[GEO_BORDER] = MeshBuilder::GenerateQuad("border", Color(1, 1, 1), 1.f);
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("health", Color(0, 1, 0), 1.f);
	meshList[GEO_DASH] = MeshBuilder::GenerateQuad("dash", Color(0, 0.3f, 0.7f), 1.f);
	meshList[GEO_CAPTURE] = MeshBuilder::GenerateQuad("capture", Color(0.0f, 0.0f, 0.5f), 1.f);

	meshList[GEO_WEAPON_MACHINEGUN] = MeshBuilder::GenerateQuad("machinegun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_MACHINEGUN]->textureArray[0] = LoadTGA("Image//machinegun.tga");
	meshList[GEO_WEAPON_SHOTGUN] = MeshBuilder::GenerateQuad("shotgun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_SHOTGUN]->textureArray[0] = LoadTGA("Image//shotgun.tga");
	meshList[GEO_WEAPON_SPLITGUN] = MeshBuilder::GenerateQuad("splitgun", Color(0, 0, 0), 1.f);
	meshList[GEO_WEAPON_SPLITGUN]->textureArray[0] = LoadTGA("Image//splitgun.tga");

	meshList[GEO_PROJECTILE_BULLET] = MeshBuilder::GenerateQuad("bullet", Color(0, 0, 0), 1.f);
	meshList[GEO_PROJECTILE_BULLET]->textureArray[0] = LoadTGA("Image//bullet_icon.tga");
	meshList[GEO_PROJECTILE_HOOK] = MeshBuilder::GenerateQuad("hook", Color(0, 0, 0), 1.f);
	meshList[GEO_PROJECTILE_HOOK]->textureArray[0] = LoadTGA("Image//hook_icon.tga");
	meshList[GEO_PROJECTILE_TRAP] = MeshBuilder::GenerateQuad("trap", Color(0, 0, 0), 1.f);
	meshList[GEO_PROJECTILE_TRAP]->textureArray[0] = LoadTGA("Image//trap_icon.tga");

	meshList[GEO_UI_BACKGROUND] = MeshBuilder::GenerateQuad("UI_background", Color(0, 0, 0), 1.f);
	meshList[GEO_UI_BACKGROUND]->textureArray[0] = LoadTGA("Image//UI_background.tga");

	InitSceneSnake();
	InitSceneDetlaff();
	InitSceneSummoner();
	InitSceneGolem();
    InitMainMenu();
}

void MeshManager::InitSceneDetlaff()
{
	meshList[GEO_DETLAFF_1] = MeshBuilder::GenerateOBJ("DETLAFF", "obj\\snake_body.obj");
	meshList[GEO_DETLAFF_1]->enableLight = true;
	meshList[GEO_DETLAFF_1]->textureArray[0] = LoadTGA("Image//Detlaff_TGA//deathstar.tga");
}

void MeshManager::InitSceneSnake()
{
	meshList[GEO_SCENESNAKE_WALLS] = MeshBuilder::GenerateCube("SceneSnake_walls", Color(0.4f, 0.4f, 0.4f));
	meshList[GEO_SCENESNAKE_WALLS]->enableLight = true;
	meshList[GEO_SCENESNAKE_WALLS]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCENESNAKE_WALLS]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_SCENESNAKE_WALLS]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);

	meshList[GEO_SNAKE_HEAD] = MeshBuilder::GenerateOBJ("snake_head", "Obj\\snake_head.obj");
	meshList[GEO_SNAKE_HEAD]->enableLight = true;
	meshList[GEO_SNAKE_HEAD]->textureArray[0] = LoadTGA("Image//snake_head.tga");
	meshList[GEO_SNAKE_BODY] = MeshBuilder::GenerateOBJ("snake_body", "Obj\\snake_body.obj");
	meshList[GEO_SNAKE_BODY]->enableLight = true;
	meshList[GEO_SNAKE_BODY]->textureArray[0] = LoadTGA("Image//snake_body.tga");
}

void MeshManager::InitSceneSummoner()
{
	meshList[GEO_SUMMONER] = MeshBuilder::GenerateOBJ("summoner", "Obj\\summoner.obj");
	meshList[GEO_SUMMONER]->enableLight = true;
	meshList[GEO_SUMMONER]->textureArray[0] = LoadTGA("Image//summoner.tga");
	meshList[GEO_SUMMONS] = MeshBuilder::GenerateOBJ("summoner", "Obj\\summons.obj");
	meshList[GEO_SUMMONS]->enableLight = true;
}

void MeshManager::InitSceneGolem()
{
    meshList[GEO_GOLEMHEAD] = MeshBuilder::GenerateOBJ("GEO_GOLEMHEAD", "Obj//golemheadhand.obj");
    meshList[GEO_GOLEMHEAD]->textureArray[0] = LoadTGA("Image//outUV.tga");
    meshList[GEO_GOLEMHAND] = MeshBuilder::GenerateOBJ("GEO_GOLEMHAND", "Obj//golemheadhand.obj");
    meshList[GEO_GOLEMHAND]->textureArray[0] = LoadTGA("Image//outUV2.tga");
}

void MeshManager::InitMainMenu()
{
    //menu
    meshList[GEO_MENU_CURSOR] = MeshBuilder::GenerateQuad("GEO_MENU_CURSOR", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_CURSOR]->textureArray[0] = LoadTGA("Image//menucursor.tga");

    meshList[GEO_MENU_BACKGROUND] = MeshBuilder::GenerateQuad("GEO_MENU_BACKGROUND", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_BACKGROUND]->textureArray[0] = LoadTGA("Image//background.tga");

    meshList[GEO_MENU_CHOICEBOX] = MeshBuilder::GenerateQuad("GEO_MENU_CHOICEBOX", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_CHOICEBOX]->textureArray[0] = LoadTGA("Image//choicebox.tga");

    meshList[GEO_MENU_SELECTION] = MeshBuilder::GenerateQuad("GEO_MENU_SELECTION", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_SELECTION]->textureArray[0] = LoadTGA("Image//selected.tga");

    meshList[GEO_MENU_FRAME] = MeshBuilder::GenerateQuad("GEO_MENU_FRAME", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_FRAME]->textureArray[0] = LoadTGA("Image//framemetal.tga");

    meshList[GEO_MENU_LVLSELECT] = MeshBuilder::GenerateQuad("GEO_MENU_LVLSELECT", Color(0, 0, 0), 1.f);
    meshList[GEO_MENU_LVLSELECT]->textureArray[0] = LoadTGA("Image//levelselectbg.tga");
}