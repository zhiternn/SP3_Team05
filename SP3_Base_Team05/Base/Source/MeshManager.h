#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "MatrixStack.h"
#include "Mesh.h"

#define modelStack MeshManager::GetInstance()._modelStack
#define meshList MeshManager::GetInstance()._meshList

enum GEOMETRY_TYPE_DEFAULT
{
	GEO_AXES,
	GEO_CROSSHAIR,
	GEO_LIGHTBALL,
	GEO_SPHERE,
	GEO_QUAD,
	GEO_CUBE,
	GEO_RING,
	GEO_CONE,
	GEO_TEXT,
	GEO_LIGHT_DEPTH_QUAD,

	//Player stuff
	GEO_PLAYER_TOP,
	GEO_PLAYER_BOTTOM,

	//Projectile stuff
	GEO_BULLET,
	GEO_GRENADE,
	GEO_SHIELD,
	GEO_ROPE,

	//Environment
	GEO_FLOOR,

	//SceneGolem

	//SceneSummoner
	GEO_SUMMONER,
	GEO_SUMMONS,

	//SceneSnake
	GEO_SNAKE_HEAD,
	GEO_SNAKE_BODY,
	GEO_SCENESNAKE_WALLS,

	//SceneDetlaff
	GEO_DETLAFF_1,
	GEO_DETLAFF_2,
	GEO_DETLAFF_3,
	
	//UI
	GEO_MINIMAP,
	GEO_BORDER,
	GEO_HEALTH,
	GEO_DASH,

	GEO_WEAPON_MACHINEGUN,
	GEO_WEAPON_SHOTGUN,
	GEO_WEAPON_SPLITGUN,

	GEO_DEFAULT_END,
};

class MeshManager
{
public:
	static MeshManager& GetInstance()
	{
		static MeshManager mm;
		return mm;
	}

	virtual ~MeshManager();

	void Init();

	static MS _modelStack;
	static Mesh* _meshList[GEO_DEFAULT_END];

protected:
	MeshManager();

	void InitSceneDetlaff();
	void InitSceneSnake();
	void InitSceneSummoner();
	void InitSceneGolem();

};



#endif