#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H

#include <vector>
#include "Vector3.h"

struct Terrain
{
	Terrain(float scaleX, float scaleY, float scaleZ, float gLevel, float wLevel);
	~Terrain();

	static Terrain SceneTextTerrain;

	Vector3 scale;
	float groundLevel;
	float waterLevel;
	std::vector<unsigned char> heightMap;

	float GetHeight(float xPos, float zPos);
};

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);
float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

#endif