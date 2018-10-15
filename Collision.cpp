#include "stdafx.h"
#include "Collision.h"

#include <algorithm>
#include <random>
#include <iostream>

inline bool calculatCollision(Ball* ballPtr, sf::RectangleShape* rect);
inline bool handleLiveLosingRules(Level* levelPtr, Ball* ballPtr, Player* playerPtr);

void Collision::setColliders(Level* levelPtr, Player* playerPtr, Ball* ballPtr)
{
	this->m_LevelPtr = levelPtr;
	this->m_PlayerPtr = playerPtr;
	this->m_BallPtr = ballPtr;
}

void Collision::checkBallBrick()
{
	auto& bricks = this->m_LevelPtr->getBricks();

	for (auto it = bricks.begin(); it != bricks.end(); ++it)
	{

		float DeltaX = this->m_BallPtr->getPosition().x - std::max(
			(*it)->sprite->getPosition().x,
			std::min(
				this->m_BallPtr->getPosition().x,
				(*it)->sprite->getPosition().x + m_LevelPtr->getSpriteDimensions().x
			)
		);

		float DeltaY = this->m_BallPtr->getPosition().y - std::max(
			(*it)->sprite->getPosition().y,
			std::min(
				this->m_BallPtr->getPosition().y,
				(*it)->sprite->getPosition().y + m_LevelPtr->getSpriteDimensions().y
			)
		);

		if ((DeltaX * DeltaX + DeltaY * DeltaY) < (this->m_BallPtr->getSprite().getRadius() * this->m_BallPtr->getSprite().getRadius()))
		{
			this->m_BallPtr->handleBrickCollision(*it, this->m_PlayerPtr);
		}

	}
	// Remove Bricks if they have 0 hitPoints left and are not cosmetic
	bricks.erase(std::remove_if(bricks.begin(), bricks.end(), [](Brick* brick) {return !brick->cosmetic && brick->HitPoints == 0; }), bricks.end());
}

void Collision::checkBallBorder()
{
	auto& border = this->m_LevelPtr->getBorders();

	if (handleLiveLosingRules(this->m_LevelPtr, this->m_BallPtr, this->m_PlayerPtr))
		return;

	for (auto it = border.begin(); it != border.end(); ++it)
	{
		if (calculatCollision(this->m_BallPtr, &it->second))
		{
			this->m_BallPtr->handleBorderCollision(&it->second);
			return;
		}
	}
}

void Collision::checkBallPedal()
{
	// Check collision only if the ball is closer than 30 pixels to the pedal.
	if (this->m_BallPtr->getPosition().y + this->m_BallPtr->getSprite().getRadius() < this->m_PlayerPtr->getPosition().y - 30)
	{
		return;
	}

	if (calculatCollision(this->m_BallPtr, &this->m_PlayerPtr->getSprite()))
	{
		this->m_BallPtr->handlePedalCollision(&this->m_PlayerPtr->getSprite());
	}

}

bool Collision::checkPedalBorder(Sides side) // DONE
{
	if (side == Sides::LEFT) //Right to left collision
	{
		if (
			this->m_PlayerPtr->getSprite().getGlobalBounds().intersects(
				this->m_LevelPtr->getBorders().find(RIGHT)->second.getGlobalBounds())
			)
		{
			this->m_PlayerPtr->handleCollision(side, this->m_LevelPtr->getBorders().find(RIGHT)->second);
			return true;
		}
	}
	if (side == Sides::RIGHT) // Left to right collision
	{
		if (
			this->m_PlayerPtr->getSprite().getGlobalBounds().intersects(
				this->m_LevelPtr->getBorders().find(LEFT)->second.getGlobalBounds())
			)
		{
			this->m_PlayerPtr->handleCollision(side, this->m_LevelPtr->getBorders().find(LEFT)->second);
			return true;
		}
	}

	return false;
}

inline bool calculatCollision(Ball* ballPtr, sf::RectangleShape* rect)
{
	float DeltaX = ballPtr->getPosition().x - std::max(
		rect->getPosition().x,
		std::min(
			ballPtr->getPosition().x,
			rect->getPosition().x + rect->getSize().x
		)
	);


	float DeltaY = ballPtr->getPosition().y - std::max(
		rect->getPosition().y,
		std::min(
			ballPtr->getPosition().y,
			rect->getPosition().y + rect->getSize().y
		)
	);

	return ((DeltaX * DeltaX + DeltaY * DeltaY) < (ballPtr->getSprite().getRadius() * ballPtr->getSprite().getRadius()));

}

inline bool handleLiveLosingRules(Level* levelPtr, Ball* ballPtr, Player* playerPtr)
{
	auto& ballSprite = ballPtr->getSprite();
	// Check if ball went ouf of the arena
	if (ballSprite.getPosition().y > (playerPtr->getPosition().y + playerPtr->getSprite().getSize().y))
	{
		// Lose a live 
		playerPtr->loseLive();

		// Restart the ball position
		ballPtr->setPosition(sf::Vector2f(
			(float)levelPtr->getWindowResolution().x / 2,
			(float)levelPtr->getWindowResolution().y / 4
			+ ((levelPtr->getRowCount() * levelPtr->getSpriteDimensions().y)
				+ (levelPtr->getRowCount() + levelPtr->getRowSpacing()))
			- ballPtr->getSprite().getRadius()
		));
		ballPtr->setAngle(270);

		playerPtr->setPosition(sf::Vector2f(
			(float)levelPtr->getWindowResolution().x / 2 - playerPtr->getSprite().getSize().x / 2,
			(float)levelPtr->getWindowResolution().y / 4
			+ ((levelPtr->getRowCount() * levelPtr->getSpriteDimensions().y)
				+ (levelPtr->getRowCount() + levelPtr->getRowSpacing()))
		));

		return true;
	}
	return false;
}

Collision::~Collision()
{
}