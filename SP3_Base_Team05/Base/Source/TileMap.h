#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class TileMap
{
public:
	TileMap(void);
	~TileMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theTileSize=25);
	bool LoadMap(const string mapName);
	int GetNumTiles_Height(void);
	int GetNumTiles_Width(void);
	int GetTileSize(void);

	vector<vector<int> > screenMap;

private:
	int screenHeight;
	int screenWidth;
	int numTiles_Height;
	int numTiles_Width;
	int tileSize;

	bool LoadFile(const string mapName);
};
