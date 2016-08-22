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
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1));
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 9, 18, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);

	//meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	//meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	meshList[GEO_P_BULLET];
	meshList[GEO_P_GRENADE];
	meshList[GEO_P_ROPE];
	meshList[GEO_P_SHIELD];

	meshList[GEO_SNAKE_HEAD];
	meshList[GEO_SNAKE_BODY] = MeshBuilder::GenerateOBJ("snake_body", "Obj\\snake_body.obj");
	meshList[GEO_SNAKE_BODY]->textureArray[0] = LoadTGA("Image//snake_body.tga");

	meshList[GEO_PLAYER_TOP] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_top.obj");
	//meshList[GEO_PLAYER_TOP]->textureArray[0] = LoadTGA("Image//player.tga");
	meshList[GEO_PLAYER_BOTTOM] = MeshBuilder::GenerateOBJ("player_top", "Obj//player_bottom.obj");
	//meshList[GEO_PLAYER_BOTTOM]->textureArray[0] = LoadTGA("Image//player.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.4f, 0.4f, 0.4f), 1.f);
    meshList[GEO_FLOOR]->textureArray[0] = LoadTGA("Image//floor.tga");
}