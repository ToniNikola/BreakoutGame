#pragma once

#include "UserStructures.h"

class Player
{
private:

	int m_Lives;
	float m_Speed;
	int m_Score;
	float m_Velocity;
	Sides m_FacingSide;
	sf::RectangleShape m_PlayerPedal;

public:
	Player();

	void setVelocity(float velocity);
	void setPosition(sf::Vector2f position);

	sf::RectangleShape getSprite();
	sf::Vector2f getPosition();
	float getVelocity(float velocity);

	void setFacingSide(Sides side);
	Sides getFacingSide();

	void loseLive();
	int getLives();

	int getScore();
	void setScore(int score);
	void resetLives();

	void handleCollision(Sides side, const sf::RectangleShape&);
};

