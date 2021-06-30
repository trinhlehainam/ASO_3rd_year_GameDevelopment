#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Math/vec2.h"

struct Tile
{
	explicit Tile(int tilePos, int id) :TilePos(tilePos), ID(id) {};

	int TilePos;
	int ID;
};

class Entity;

class TileMap
{
public:
	TileMap();
	explicit TileMap(const std::string& xmlFile, const std::string& key);
	~TileMap();
public:
	void LoadMapDataFromXML(const std::string& fileName, const std::string& key);

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
	int m_numColumns;
	int m_mapImageID;

	std::unordered_map<std::string, std::vector<Tile>> m_layerMap;
	std::vector<std::shared_ptr<Entity>> m_colliderObjects;

private:
	friend class GameScene;
};

