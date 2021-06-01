#pragma once
#include <string>
#include <vector>

class TileMapData
{
public:
	explicit TileMapData(const std::string& xmlFile);
	~TileMapData();
public:
	void LoadMapDataFromXML(const std::string& fileName);
private:
	int m_numTileX, m_numTileY;
	int m_tileWidth, m_tileHeight;
	int m_mapImageID;
};

