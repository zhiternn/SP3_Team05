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

	//SceneGolem
	GEO_GOLEMHEAD,
	GEO_GOLEMHAND,

    //Environment
	GEO_BACKGROUND1,
	GEO_BACKGROUND2,
	GEO_BACKGROUND3,
    GEO_FLOOR,
	GEO_FLOOR_HEX,
	GEO_TRAP,

    //UI
	GEO_MINIMAP,
	GEO_MINIMAP_BORDER,
	GEO_MINIMAP_PLAYER_ICON,
	GEO_MINIMAP_BOSS_MAIN_ICON,
	GEO_MINIMAP_BOSS_BODY_ICON,
    GEO_BORDER,
    GEO_HEALTH,
    GEO_DASH,
	GEO_CAPTURE,

    GEO_WEAPON_MACHINEGUN,
    GEO_WEAPON_SHOTGUN,
    GEO_WEAPON_SPLITGUN,

    // MENU THINGS
    GEO_MENU_CURSOR,
    GEO_MENU_BACKGROUND,
    GEO_MENU_CHOICEBOX,
    GEO_MENU_SELECTION,
    GEO_MENU_FRAME,
    GEO_MENU_LVLSELECT,

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
    void InitMainMenu();

};



#endif