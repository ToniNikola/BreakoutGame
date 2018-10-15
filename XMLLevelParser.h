#pragma once

#include "Level.h"
#include <string>
#include "rapidxml.hpp"
#include "UserStructures.h"
#include "rapidxml_utils.hpp"

class XMLLevelParser
{
private:
	Level* m_LevelPtr;

	int m_yAxisThreshold;

	std::map<std::string, sf::Texture*> m_LoadedTextures;
	std::map<std::string, sf::SoundBuffer*> m_LoadedSounds;

public:
	void parseLevel(int level);
	void setLevelPtr(Level* levelPtr);

	~XMLLevelParser();
};

