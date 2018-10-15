#include "stdafx.h"

#include "Level.h"
#include "UserStructures.h"



void  Level::setWindowResolution(sf::Vector2i resolution)
{
	this->m_WidnowResolution = resolution;
}

void  Level::setSpriteDimensions(sf::Vector2i dimensions)
{
	this->m_SpriteDimensions = dimensions;
}

void Level::setColumnCount(int count)
{
	this->m_ColumnCount = count;
}

void Level::setRowCount(int count)
{
	this->m_RowCount = count;
}

void Level::setColumnSpacing(int spacing)
{
	this->m_ColumnSpacing = spacing;
}

void Level::setRowSpacing(int spacing)
{
	this->m_RowSpacing = spacing;
}

void Level::setUpBackground(sf::Texture* texture)
{


	float scaleByX = (float)this->m_WidnowResolution.x / texture->getSize().x;
	float scaleByY = (float)this->m_WidnowResolution.y / texture->getSize().y;
	
	this->m_BackgroundSprite.setTexture(*texture);

	this->m_BackgroundSprite.setScale(scaleByX, scaleByY);
}

sf::Vector2i Level::getWindowResolution()
{
	return this->m_WidnowResolution;
}

sf::Vector2i Level::getSpriteDimensions()
{
	return this->m_SpriteDimensions;
}

int Level::getColumnCount()
{
	return this->m_ColumnCount;
}

int Level::getRowCount()
{
	return this->m_RowCount;
}

int Level::getColumnSpacing()
{
	return this->m_ColumnSpacing;
}

int Level::getRowSpacing()
{
	return this->m_RowSpacing;
}

void Level::addBrick(Brick* brick)
{
	this->m_Bricks.push_back(brick);
}

std::vector<Brick*>& Level::getBricks()
{
	return this->m_Bricks;
}

sf::Sprite Level::getBackgroundSprite()
{
	return this->m_BackgroundSprite;
}

void Level::setMenuText(std::string text)
{
	this->m_MenuText.setString(text);
}

void Level::setMenuFont(std::string pathToFont)
{
	if (!this->m_MenuFont.loadFromFile(pathToFont))
	{
		exit(EXIT_FAILURE);
	}
	
}

void Level::setUpMenu()
{
	float arenaWidth = (this->m_ColumnCount * this->m_SpriteDimensions.x) + (this->m_ColumnCount * this->m_ColumnSpacing) + LevelConfig::LEFT_TEXT_OFFSET;

	float positionX = (this->m_WidnowResolution.x - arenaWidth) / 2;
	

	this->m_MenuText.setFont(this->m_MenuFont);

	this->m_MenuText.setPosition(
		sf::Vector2f(
			positionX,
			LevelConfig::TOP_TEXT_OFFSET
		)
	);

	this->m_MenuText.setCharacterSize(LevelConfig::CHARACTER_SIZE);
	this->m_MenuText.setFillColor(sf::Color::Red);
}

sf::Text* Level::getMenuText()
{
	return &this->m_MenuText;
}


void Level::setUpBorders()
{
	this->m_Borders.clear();

	float arenaWidth = (this->m_ColumnCount * this->m_SpriteDimensions.x) + (this->m_ColumnCount * this->m_ColumnSpacing);
	float arenaHeight = (this->m_RowCount * this->m_SpriteDimensions.y) + (this->m_RowCount * this->m_RowSpacing);

	m_LeftBorder.setSize(sf::Vector2f(this->m_SpriteDimensions.x, arenaHeight));
	m_TopBorder.setSize(sf::Vector2f(arenaWidth + this->m_ColumnSpacing, this->m_SpriteDimensions.x));
	m_RightBorder.setSize(sf::Vector2f(this->m_SpriteDimensions.x, arenaHeight));
	
	m_LeftBorder.setPosition(sf::Vector2f(
		((this->m_WidnowResolution.x - arenaWidth)/2) - (this->m_SpriteDimensions.x),
		this->m_WidnowResolution.y/4 - this->m_SpriteDimensions.x
	));

	m_TopBorder.setPosition(sf::Vector2f(
		(this->m_WidnowResolution.x - arenaWidth) / 2 - this->m_ColumnSpacing,
		this->m_WidnowResolution.y / 4 - this->m_SpriteDimensions.x
	)); 

	m_RightBorder.setPosition(sf::Vector2f(
		((this->m_WidnowResolution.x - arenaWidth) / 2) + arenaWidth,
		this->m_WidnowResolution.y / 4 - this->m_SpriteDimensions.x
	));

	m_LeftBorder.setFillColor(sf::Color::Blue);
	m_TopBorder.setFillColor(sf::Color::Blue);
	m_RightBorder.setFillColor(sf::Color::Blue);

	

	this->m_Borders.insert(std::make_pair(Sides::LEFT, m_LeftBorder));
	this->m_Borders.insert(std::make_pair(Sides::TOP, m_TopBorder));
	this->m_Borders.insert(std::make_pair(Sides::RIGHT, m_RightBorder));
}

std::map<Sides, sf::RectangleShape>& Level::getBorders()
{
	return this->m_Borders;
}

Level::~Level()
{
	for (auto it = this->m_Bricks.begin(); it != this->m_Bricks.end(); ++it)
	{
		delete (*it);
	}
	
	this->m_Bricks.clear();

}
