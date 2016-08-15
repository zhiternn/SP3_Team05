#include "TileMap.h"

TileMap::TileMap(void)
: screenHeight(0)
, screenWidth(0)
, numTiles_Height(0)
, numTiles_Width(0)
, tileSize(0)
{
	screenMap.clear();
}

TileMap::~TileMap(void)
{
	screenMap.clear();
}

void TileMap::Init(const int screenHeight, const int screenWidth, const int numTiles_Height, const int numTiles_Width, const int tileSize)
{
	this->screenHeight		= screenHeight;
	this->screenWidth		= screenWidth;
	this->numTiles_Height	= numTiles_Height;
	this->numTiles_Width	= numTiles_Width;
	this->tileSize			= tileSize;

	screenMap.resize(numTiles_Height);
	for (int i = 0; i < numTiles_Height; ++i)
		screenMap[i].resize(numTiles_Width);
}

bool TileMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool TileMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if  (theLineCounter>=numTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if ( theMaxNumOfColumns != numTiles_Width)
						return false;
				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ',') && (theColumnCounter<numTiles_Width))
					{
						screenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

int TileMap::GetNumTiles_Height(void)
{
	return numTiles_Height;
}

int TileMap::GetNumTiles_Width(void)
{
	return numTiles_Width;
}

int TileMap::GetTileSize(void)
{
	return tileSize;
}