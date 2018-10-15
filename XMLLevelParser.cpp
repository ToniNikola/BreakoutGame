#include "stdafx.h"

#include "XMLLevelParser.h"
#include "UserStructures.h"

#include <sstream>
#include <algorithm>

typedef std::map<std::string, sf::Texture*> MapOfTextures;
typedef std::map<std::string, sf::SoundBuffer*> MapOfSounds;

inline int convertToInt(const char* value);
inline void trimWhitespace(std::string* string);
inline sf::Vector2f calculateBrickPositionOffset(Level* levelPtr);
inline rapidxml::xml_node<>* findBrickById(rapidxml::xml_node<>* node, char searchId);
inline sf::Vector2f calculateBrickPosition(int* brickCount, int* yAxisThreshold, Level* levelPtr);
inline sf::Texture* searchOrLoadTexture(std::string path, MapOfTextures& loadedTextures);
inline sf::SoundBuffer* searchOrLoadSound(std::string path, MapOfSounds& loadedSounds);

void XMLLevelParser::setLevelPtr(Level* levelPtr)
{
	this->m_LevelPtr = levelPtr;
}

void XMLLevelParser::parseLevel(int level)
{
	// Clear up if level allready build
	this->m_yAxisThreshold = 0;
	this->m_LevelPtr->getBricks().clear();
	this->m_LevelPtr->getBorders().clear();


	// Build up the current configuration file name
	std::string level_file_name = 
		ParserConfig::LEVEL_PREFIX 
		+ std::to_string(level) 
		+ ParserConfig::FILE_EXTENSION;

	// Build the xml file
	rapidxml::file<> xmlFile(level_file_name.c_str());

	// Parse the current configuration file
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(xmlFile.data());

	// Step into the level node.
	rapidxml::xml_node<> *levelNode = xmlDoc.first_node(
		ParserConfig::LevelAttribute
	);

	// Convert and set Window Resolution

	this->m_LevelPtr->setWindowResolution(sf::Vector2i(
		sf::VideoMode::getDesktopMode().width,
		sf::VideoMode::getDesktopMode().height
	));

	// Convert and set Sprite Dimensions
	this->m_LevelPtr->setSpriteDimensions(sf::Vector2i(
		convertToInt(
			levelNode->first_attribute(	ParserConfig::SpriteWidthAttribute)->value()
		),
		convertToInt(
			levelNode->first_attribute(ParserConfig::SpriteHeightAttribute)->value()
		)
	));

	// Convert and set Columnt Count
	this->m_LevelPtr->setColumnCount(
		convertToInt(
			levelNode->first_attribute(ParserConfig::ColumnCountAttribute)->value()
		));

	// Convert and set Row Count
	this->m_LevelPtr->setRowCount(
		convertToInt(
			levelNode->first_attribute(ParserConfig::RowCountAttribute)->value()
		));

	// Convert and set Column Spacing
	this->m_LevelPtr->setColumnSpacing(
		convertToInt(
			levelNode->first_attribute(ParserConfig::ColumnSpacingAttribute)->value()
		));

	// Convert and set Row Spacing
	this->m_LevelPtr->setRowSpacing(
		convertToInt(
			levelNode->first_attribute(ParserConfig::RowSpacingAttribute)->value()
		));

	// Set up Background texture
	this->m_LevelPtr->setUpBackground(
		searchOrLoadTexture(
			levelNode->first_attribute(ParserConfig::BackgroundTextureAttribute)->value(),
			this->m_LoadedTextures
		)
	);

	// Set up Menu Font
	this->m_LevelPtr->setMenuFont(
		levelNode->first_attribute(ParserConfig::MenuFontAttribute)->value()
	);


	// Convert Bricks value from string literal to std::string
	std::string bricksNodeValue(
		levelNode->first_node(ParserConfig::BricksNode)->value()
	);

	// Create seperate node and step into the BrickTypes node.
	rapidxml::xml_node<> *BrickTypesNode = levelNode->first_node(ParserConfig::BrickTypesNode)->first_node();

	//Trim Whitespace 
	trimWhitespace(&bricksNodeValue);

	// BrickCount tracks current brick processing.
	int brickCount = 0;

	// Loop all bricks and build up a brick vector inside the Level object.
	// DODATI RANGE ITERATOR
	for (auto elem : bricksNodeValue)
	{
	
		//Ignore the blank brick spots. Increment the brickCount so the next brick nows its position.
		if (elem == ParserConfig::BlandBrickPlaceholder)
		{
			if (brickCount >= m_LevelPtr->getColumnCount())
			{
				brickCount = 0;
				this->m_yAxisThreshold += 1;
			}
			brickCount += 1;
			continue;
		}

		// Get the appropriate BrickType
		rapidxml::xml_node<>* currentBrickNode = NULL;
		currentBrickNode = findBrickById(BrickTypesNode, elem);

		// Allocate heap objects
		Brick* brick = new Brick();
		sf::Sprite* sprite = new sf::Sprite();

		// Set Texture with Texture path.
		
		sprite->setTexture(
			*searchOrLoadTexture(
				currentBrickNode->first_attribute(ParserConfig::BrickTextureAttribute)->value(),
				this->m_LoadedTextures
			)
		);
		// Calculate brick position and set it to sprite.
		sprite->setPosition(
			calculateBrickPosition(
				&brickCount,
				&this->m_yAxisThreshold,
				this->m_LevelPtr
			)
		);

		// Connect the sprite to the brick.
		brick->sprite = sprite;

		// Set the hitSound sound.
		brick->hitSound = searchOrLoadSound(
			currentBrickNode->first_attribute(ParserConfig::BrickHitSoundAttribute)->value(),
			this->m_LoadedSounds
		);

		// <-----Optional attributes----->		
		// Check if the block is cosmetical (non-destroyable)
		std::string currentHitPoints(
			currentBrickNode->first_attribute(ParserConfig::BrickHitPointsAttribute)->value()
		);

		// Conver string literal to std::string for comparison
		std::string infiniteBrick = ParserConfig::CosmeticBrickDelimiter;

		if (currentHitPoints == infiniteBrick)
		{
			brick->HitPoints = 0;
			brick->breakSound = NULL;
			brick->breakScore = 0;
			brick->cosmetic = true;
		}
		else
		{
			// Set the pointToHit value.
			// Convert string literal to int and assign it to member
			brick->HitPoints = convertToInt(
				currentBrickNode->first_attribute(ParserConfig::BrickHitPointsAttribute)->value()
			);

			// Set the breakSound sound.
			brick->breakSound = searchOrLoadSound(
				currentBrickNode->first_attribute(ParserConfig::BrickBreakSoundAttribute)->value(),
				this->m_LoadedSounds
			);
			// Set the breakScore.
			// Convert string literal to int and assign it to member
			brick->breakScore = convertToInt(
				currentBrickNode->first_attribute(ParserConfig::BrickBreakScoreAttribute)->value()
			);

			brick->cosmetic = false;
		}
		this->m_LevelPtr->addBrick(brick);

		brickCount += 1;
		
		// Remove Node from pointer, and delete it.
		currentBrickNode = NULL;
		delete currentBrickNode;
	}
}


inline int convertToInt(const char* value)
{
	std::stringstream ssValue;
	ssValue << value;

	int convertedValue;

	ssValue >> convertedValue;

	return convertedValue;
}

inline rapidxml::xml_node<>* findBrickById(rapidxml::xml_node<>* node, char searchId)
{
	while (node)
	{
		if (searchId == *node->first_attribute("Id")->value())
			return node;
	
		node = node->next_sibling();
	}
}

inline void trimWhitespace(std::string* string)
{
	std::string::size_type pos = 0;
	while ((pos = string->find("\r\n", pos)) != std::string::npos)
	{
		string->erase(pos, 2);
	}
	std::string::iterator end_pos = std::remove(string->begin(), string->end(), ' ');
	string->erase(end_pos, string->end());
}

inline sf::Vector2f calculateBrickPosition(int* brickCount, int* yAxisThreshold, Level* levelPtr)
{
	if (*brickCount >= levelPtr->getColumnCount())
	{
		*brickCount = 0;
		(*yAxisThreshold)++;
	}

	sf::Vector2f brickOffset = calculateBrickPositionOffset(levelPtr);

	return sf::Vector2f(
		(*brickCount * levelPtr->getSpriteDimensions().x) + (*brickCount * levelPtr->getColumnSpacing()) + brickOffset.x,
		(*yAxisThreshold * levelPtr->getSpriteDimensions().y) + (*yAxisThreshold * levelPtr->getRowSpacing()) + brickOffset.y
	);
}

inline sf::Vector2f calculateBrickPositionOffset(Level* levelPtr)
{
	// Calculate the total width of the bricks so we can center position it into the screen, on the X position.
	// Fix the Y position to the 1/4 of the total resolution
	sf::Vector2f result(
		(levelPtr->getWindowResolution().x - ((levelPtr->getColumnCount() * levelPtr->getSpriteDimensions().x) + (levelPtr->getColumnCount() * levelPtr->getColumnSpacing()))) / 2,
		levelPtr->getWindowResolution().y / 4.f
	);

	return result;
}

inline sf::SoundBuffer* searchOrLoadSound(std::string path, MapOfSounds& loadedSounds)
{
	// Check if the sound is allready loaded.
	auto searchSound = loadedSounds.find(path);
	if (searchSound != loadedSounds.end())
	{
		// Return the found sound
		return searchSound->second;
	}

	// If its not loaded create a new SoundBuffer object and place its pointer into the map.
	sf::SoundBuffer* sound = new sf::SoundBuffer();
	if (!sound->loadFromFile(path))
	{
		exit(EXIT_FAILURE);
	}
	sound->loadFromFile(path);
	loadedSounds.insert(std::make_pair(path, sound));

	return sound;
}

inline sf::Texture* searchOrLoadTexture(std::string path, MapOfTextures& loadedTextures)
{
	// Check if the sound is allready loaded.
	auto searchTexture = loadedTextures.find(path);
	if (searchTexture != loadedTextures.end())
	{
		// Return the found sound
		return searchTexture->second;
	}

	// If its not loaded create a new SoundBuffer object and place its pointer into the map.
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(path))
	{
		exit(EXIT_FAILURE);
	}
	//texture->loadFromFile(path);
	loadedTextures.insert(std::make_pair(path, texture));

	return texture;
}

XMLLevelParser::~XMLLevelParser()
{
	for (auto it = this->m_LoadedTextures.begin(); it != this->m_LoadedTextures.end(); ++it)
	{
		delete (it->second);
	}

	for (auto it = this->m_LoadedSounds.begin(); it != this->m_LoadedSounds.end(); ++it)
	{
		delete (it->second);
	}

	this->m_LoadedTextures.clear();

	this->m_LoadedTextures.clear();
}

