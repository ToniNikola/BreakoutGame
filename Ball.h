#pragma once

#include <SFML/Graphics.hpp>
#include "UserStructures.h"
#include "Player.h"

class Ball
{
private:
	int m_Angle;
	int m_Speed;
	sf::CircleShape m_Ball;
	sf::Vector2f m_Direction;
	sf::Sound m_Sound;
	float* m_ScorePtr;

	
public:
	Ball();
	
	void update(float elapsedTime);
	void setDirection(float x, float y);

	sf::Vector2f getDirection();
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition();

	void setAngle(int angle);
	int getAngle();
	sf::CircleShape& getSprite();

	void handleBrickCollision(Brick* brickPtr, Player* playerPtr);
	void handleBorderCollision(sf::RectangleShape* rectPtr);
	void handlePedalCollision(sf::RectangleShape* rectPtr);
};

