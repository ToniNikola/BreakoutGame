#pragma once
#include <vector>

#include "UserStructures.h"

class Level
{
private:
	sf::Vector2i m_WidnowResolution;
	sf::Vector2i m_SpriteDimensions;

	int m_ColumnCount;
	int m_RowCount;
	int m_ColumnSpacing;
	int m_RowSpacing;

	sf::Font m_MenuFont;
	sf::Text m_MenuText;

	sf::RectangleShape m_LeftBorder;
	sf::RectangleShape m_TopBorder;
	sf::RectangleShape m_RightBorder;



	sf::Sprite m_BackgroundSprite;

	std::vector<Brick*> m_Bricks;
	std::map<Sides,sf::RectangleShape> m_Borders;

public:

	void setUpBorders();
	void addBrick(Brick* brick);

	void setRowCount(int count);
	void setColumnCount(int count);
	void setRowSpacing(int spacing);
	void setColumnSpacing(int spacing);
	void setUpBackground(sf::Texture* texture);
	void setSpriteDimensions(sf::Vector2i dimensions);
	void setWindowResolution(sf::Vector2i resolution);

	int getRowCount();
	int getRowSpacing();
	int getColumnCount();
	int getColumnSpacing();
	sf::Vector2i  getSpriteDimensions();
	sf::Vector2i  getWindowResolution();
	sf::Sprite getBackgroundSprite();

	void setMenuText(std::string text);
	void setMenuFont(std::string pathToFont);

	sf::Text* getMenuText();
	void setUpMenu();

	std::map<Sides, sf::RectangleShape>& getBorders();
	std::vector<Brick*>& getBricks();

	~Level();
};

