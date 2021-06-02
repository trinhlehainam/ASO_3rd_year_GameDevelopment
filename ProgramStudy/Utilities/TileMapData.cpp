#include "TileMapData.h"

#include <fstream>
#include <sstream>

#include <rapidxml.hpp>
#include <DxLib.h>

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
			m_numTileX = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "height") == 0)
		{
			m_numTileY = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "tileheight") == 0)
		{
			m_tileHeight = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "tilewidth") == 0)
		{
			m_tileWidth = std::atoi(pAttr->value());
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

	std::string imageFile;
	for (auto* pAttr = pImageNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			imageFile = pAttr->value();
		}
	}
	m_mapImageID = DxLib::LoadGraph(imageFile.c_str());
	imageDoc.clear();
	//

	// Read tile's data (image ID and tile's position) to each layer
	for (auto* pLayer = pMap->first_node("layer"); pLayer; pLayer = pLayer->next_sibling())
	{
		auto* pData = pLayer->first_node("data");
		std::string layerName{ pLayer->first_attribute("name")->value() };
		std::stringstream data{ pData->value() };
		int tilePos = 0;
		while (!data.eof())
		{
			std::string temp;
			data >> temp;
			int found = 0;
			std::stringstream ssTemp{ temp };
			while (ssTemp >> found)
			{
				if (!(found == 0))
				{
					m_layers[layerName].emplace_back(tilePos, found);
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
