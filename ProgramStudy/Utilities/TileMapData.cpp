#include "TileMapData.h"

#include <fstream>
#include <sstream>

#include <rapidxml.hpp>
#include <DxLib.h>

#include "ImageMng.h"

namespace
{
	std::string LoadXMLToStringBuffer(const std::string& fileName)
	{
		std::ifstream file(fileName);
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
}

TileMapData::TileMapData():m_mapImageID(0) {}

TileMapData::TileMapData(const std::string& xmlFile)
{
	LoadMapDataFromXML(xmlFile);
}

TileMapData::~TileMapData()
{
}

void TileMapData::LoadMapDataFromXML(const std::string& fileName)
{
	rapidxml::xml_document<> doc;
	auto content = LoadXMLToStringBuffer(fileName);
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<>* pMap = doc.first_node();

	// Tile info
	for (auto* pAttr = pMap->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "width") == 0)
		{
			m_numTile.x = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "height") == 0)
		{
			m_numTile.y = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "tileheight") == 0)
		{
			m_tileSize.x = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "tilewidth") == 0)
		{
			m_tileSize.y = std::atoi(pAttr->value());
		}
	}
	//

	// Map Image Info
	auto pTileSet = pMap->first_node("tileset");
	std::string imageSource;
	for (auto* pAttr = pTileSet->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			imageSource += pAttr->value();
		}
	}

	auto imageContent = LoadXMLToStringBuffer(imageSource.c_str());
	rapidxml::xml_document imageDoc;
	imageDoc.parse<0>(&imageContent[0]);
	auto* pImageTileset = imageDoc.first_node();
	auto* pImageNode = pImageTileset->first_node("image");

	for (auto* pAttr = pImageNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			std::string imageFile{ pAttr->value() };

			// Load source image to ImageMng and take image info
			auto& imageMng = ImageMng::Instance();
			imageMng.AddImage("map", imageFile);
			m_mapImageID = imageMng.GetID("map");
			DxLib::GetGraphSize(m_mapImageID, &m_mapImageSize.x, &m_mapImageSize.y);
			//
			imageDoc.clear();
		}
	}
	//

	// Read tile's data (image ID and tile's position) to each layer
	for (auto* pLayer = pMap->first_node("layer"); pLayer; pLayer = pLayer->next_sibling())
	{
		auto* pData = pLayer->first_node("data");
		std::string layerName{ pLayer->first_attribute("name")->value() };
		std::stringstream data{ std::move(pData->value()) };
		int tilePos = 0;
		while (!data.eof())
		{
			std::string line;
			data >> line;
			int sourceID = 0;
			std::stringstream ssTemp{ line };
			while (ssTemp >> sourceID)
			{
				if (!(sourceID == 0))
				{
					// SourceID is added 1 by Tile tool to separate with empty (0)
					// --> Minus 1 to move back to right ID
					m_layerMap[layerName].emplace_back(tilePos, sourceID - 1);
				}

				if (ssTemp.peek() == ',')
					ssTemp.ignore();

				++tilePos;
			}
		}
	}
	//

	doc.clear();
}

void TileMapData::Update(float deltaTime_s)
{
			
}

void TileMapData::Render()
{
	for (const auto& layer : m_layerMap)
		for (const auto& tile : layer.second)
		{
			auto tileWorldPos = GetTileWorldPos(tile.TilePos);
			auto sourcePos = GetTileSourcePos(tile.ID);
			DxLib::DrawRectGraphF(tileWorldPos.x, tileWorldPos.y, sourcePos.x, sourcePos.y, m_tileSize.x, m_tileSize.y, m_mapImageID, 1);
		}
}

vec2f TileMapData::GetTileWorldPos(int tilePos)
{
	float x = (tilePos % m_numTile.x) * static_cast<float>(m_tileSize.x);
	float y = (tilePos / m_numTile.x) * static_cast<float>(m_tileSize.y);
	return vec2f{m_worldPos.x + x , m_worldPos.y + y};
}

vec2i TileMapData::GetTileSourcePos(int sourceID)
{
	const int kNumImageTileX = m_mapImageSize.x / m_tileSize.x;
	int x = (sourceID % kNumImageTileX) * m_tileSize.x;
	int y = (sourceID / kNumImageTileX) * m_tileSize.y;
	return vec2i{ x,y };
}
