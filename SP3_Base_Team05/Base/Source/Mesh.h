#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"

class Mesh
{
public:
	static const int MAX_TEXTURES = 3;
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	virtual ~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID;
	unsigned textureArray[MAX_TEXTURES];
	bool enableLight;
};

#endif