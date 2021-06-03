#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "../Math/vec2.h"

struct Tile
{
	int TilePos;
	int ID;
};

class TileMapData
{
public:
	TileMapData();
	explicit TileMapData(const std::string& xmlFile);
	~TileMapData();
public:
	void LoadMapDataFromXML(const std::string& fileName);

	void Update(float deltaTime_s);
	void Render();
private:
	vec2f GetTileWorldPos(int tilePos);
	vec2i GetTileSourcePos(int sourceID);
private:
	vec2f m_worldPos;
	vec2i m_numTile;
	vec2i m_tileSize;
	vec2i m_mapImageSize;
	int m_mapImageID;

	std::unordered_map<std::string, std::vector<Tile>> m_layerMap;
};

