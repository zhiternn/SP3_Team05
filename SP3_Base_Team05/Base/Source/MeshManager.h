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

	GEO_P_BULLET,
	GEO_P_GRENADE,
	GEO_P_SHIELD,

	GEO_ROPE,

	GEO_SNAKE_HEAD,
	GEO_SNAKE_BODY,

	GEO_PLAYER_TOP,
	GEO_PLAYER_BOTTOM,
	GEO_FLOOR,

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

};



#endif