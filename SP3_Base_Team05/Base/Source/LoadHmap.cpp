#include <iostream>
#include <fstream>
#include "LoadHmap.h"

Terrain Terrain::SceneTextTerrain(1500.0f, 300.0f, 1500.0f, 100.0f, 40.0f);

Terrain::Terrain(float scaleX, float scaleY, float scaleZ, float gLevel, float wLevel)
{
	scale.Set(scaleX, scaleY, scaleZ);

	groundLevel = gLevel / 255.0f;
	waterLevel = wLevel / 255.0f;
	
	for (std::vector<unsigned char>::iterator it = heightMap.begin(); it != heightMap.end();)
	{
		it = heightMap.erase(it);
	}
}
Terrain::~Terrain()
{
}

float Terrain::GetHeight(float xPos, float zPos)
{
	return this->scale.y * ReadHeightMap(this->heightMap, xPos / this->scale.x, zPos / this->scale.z);
}

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}

    fileStream.seekg(0, std::ios::end);
    std::streampos fsize = (unsigned)fileStream.tellg();
	
	fileStream.seekg(0, std::ios::beg);
	heightMap.resize((unsigned)fsize);
	fileStream.read((char *)&heightMap[0], fsize);
	
	fileStream.close();
	return true;
}

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
{
	if(x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
		return 0;
	if(heightMap.size() == 0)
		return 0;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());
	
	unsigned zCoord = (unsigned)((z + 0.5f) * terrainSize);
	unsigned xCoord = (unsigned)((x + 0.5f) * terrainSize);

	return (float)heightMap[zCoord * terrainSize + xCoord] / 256.f;
}
