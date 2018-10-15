#include "stdafx.h"
#include "Ball.h"
#include <random>
#include "UserStructures.h"
inline Sides calculateCollisionSide(sf::RectangleShape* rectPtr, Ball* ballPtr);

Ball::Ball()
{
	this->m_Ball.setRadius(BallConfig::BALL_RADIUS);
	this->m_Ball.setOrigin(BallConfig::BALL_RADIUS, BallConfig::BALL_RADIUS);
	this->m_Speed = BallConfig::BALL_SPEED;
	this->m_Angle = BallConfig::BALL_STARTING_ANGLE;
}

void Ball::update(float elapsedTime)
{
	this->m_Direction.x = std::cos(this->m_Angle * (std::_Pi / 180));
	this->m_Direction.y = std::sin(this->m_Angle * (std::_Pi / 180));

	this->m_Ball.move(this->m_Direction * (float)this->m_Speed * elapsedTime);
}


void Ball::handleBrickCollision(Brick* brick, Player* playerPtr)
{
	
	int angle = 0;

	float brickX = brick->sprite->getPosition().x;
	float brickY = brick->sprite->getPosition().y;

	float circleCenterX = this->m_Ball.getPosition().x + this->m_Ball.getRadius();
	float circleCenterY = this->m_Ball.getPosition().y + this->m_Ball.getRadius();

	float brickCenterX = brick->sprite->getPosition().x + (brick->sprite->getGlobalBounds().width / 2);
	float brickCenterY = brick->sprite->getPosition().y + (brick->sprite->getGlobalBounds().height / 2);

	float yMin = 0;
	if (brickY > this->m_Ball.getPosition().y)
		yMin = brickY;
	else
		yMin = this->m_Ball.getPosition().y;

	float yMax = 0;
	if (brickY + brick->sprite->getGlobalBounds().height < this->m_Ball.getPosition().y + this->m_Ball.getGlobalBounds().height)
		yMax = brickY + brick->sprite->getGlobalBounds().height;
	else
		yMax = this->m_Ball.getPosition().y + this->m_Ball.getGlobalBounds().height;

	float ySize = yMax - yMin;

	float xMin = 0;
	if (brickX > this->m_Ball.getPosition().x)
		xMin = brickX;
	else
		xMin = this->m_Ball.getPosition().x;

	float xMax = 0;
	if (brickX + brick->sprite->getGlobalBounds().width < this->m_Ball.getPosition().x + this->m_Ball.getGlobalBounds().width)
		xMax = brickX + brick->sprite->getGlobalBounds().width;
	else
		xMax = this->m_Ball.getPosition().x + this->m_Ball.getGlobalBounds().width;

	float xSize = xMax - xMin;

	// The origin is at the top-left corner of the screen
	// Set the collision response
	if (xSize > ySize)
	{
		if (circleCenterY > brickCenterY)
		{
			angle = 360;
			this->m_Ball.move(0,1);
		}
		else
		{
			angle = 360;
			this->m_Ball.move(0,-1);
		}
	}
	else
	{
		if (circleCenterX < brickCenterX)
		{
			// Left
			angle = 180;
			this->m_Ball.move(-(1),	0);
		}
		else
		{
			// Right
			angle = 180;
			this->m_Ball.move(1,0);
		}
	}

	this->m_Angle = angle - this->m_Angle;
	
	if(brick->HitPoints == 1)
	{ 
		brick->HitPoints = 0;
		this->m_Sound.setBuffer(*brick->breakSound);
			
		delete brick->sprite;
		brick->sprite = NULL;
		playerPtr->setScore(playerPtr->getScore() + brick->breakScore);
	}
	else
	{
		brick->HitPoints -= 1;
		this->m_Sound.setBuffer(*brick->hitSound);
	}
	this->m_Sound.play();
}

void Ball::handleBorderCollision(sf::RectangleShape* rect)
{
	int angle = 0;

	switch (calculateCollisionSide(rect, this))
	{
	case Sides::TOP:
		angle = 360;
		this->m_Ball.move(0,1);
		break;

	case Sides::BOTTOM:
		break;

	case Sides::LEFT:
		angle = 180;
		this->m_Ball.move(-(1),0);
		break;

	case Sides::RIGHT:
		angle = 180;
		this->m_Ball.move(1,0);
		break;
	}

	this->m_Angle = angle - this->m_Angle;	
}

void Ball::handlePedalCollision(sf::RectangleShape* rect)
{
	int angle = 0;
	float xLocation = 0;
	float multyplier = 0;

	switch (calculateCollisionSide(rect, this))
	{
	case Sides::TOP:
		xLocation = this->m_Ball.getPosition().x - rect->getPosition().x - PlayerConfig::PEDAL_WIDTH/2;

		if (xLocation < 0) //Left Pedal side
			multyplier = (xLocation / (PlayerConfig::PEDAL_WIDTH / 2)) * BallConfig::DEGREE_MULTIPLIER;
		else // Right pedal side
			multyplier = (xLocation / (PlayerConfig::PEDAL_WIDTH / 2)) * BallConfig::DEGREE_MULTIPLIER;

		angle = 360;
		this->m_Ball.move(0,-1);
		break;

	case Sides::BOTTOM:
		angle = 360;
		this->m_Ball.move(0,1);
		break;

	case Sides::LEFT:
		angle = 180;
		this->m_Ball.move(-(1), 0);
		break;

	case Sides::RIGHT:
		angle = 180;
		this->m_Ball.move(1, 0);
		break;
	}

	this->m_Angle = 360 - this->m_Angle + multyplier;
}

void Ball::setAngle(int angle)
{
	this->m_Angle = angle;
}

int Ball::getAngle()
{
	return this->m_Angle;
}

sf::Vector2f Ball::getPosition()
{
	return this->m_Ball.getPosition();
}

void Ball::setDirection(float x, float y)
{
	this->m_Direction.x = x;
	this->m_Direction.y = y;

}

sf::Vector2f Ball::getDirection()
{
	return this->m_Direction;
}

void Ball::setPosition(sf::Vector2f position)
{
	this->m_Ball.setPosition(position);
}

sf::CircleShape& Ball::getSprite()
{
	return this->m_Ball;
}

inline Sides calculateCollisionSide(sf::RectangleShape* rectPtr, Ball* ballPtr)
{
	float brickX = rectPtr->getPosition().x;
	float brickY = rectPtr->getPosition().y;

	float ballX = ballPtr->getPosition().x;
	float ballY = ballPtr->getPosition().y;

	float circleCenterX = ballPtr->getPosition().x + BallConfig::BALL_RADIUS;
	float circleCenterY = ballPtr->getPosition().y + BallConfig::BALL_RADIUS;


	float brickCenterX = brickX + (rectPtr->getGlobalBounds().width / 2);
	float brickCenterY = brickY + (rectPtr->getGlobalBounds().height / 2);

	float yMin = 0;
	if (brickY > ballY)
		yMin = brickY;
	else
		yMin = ballY;

	float yMax = 0;
	if (brickY + rectPtr->getGlobalBounds().height < ballY + ballPtr->getSprite().getGlobalBounds().height)
		yMax = brickY + rectPtr->getGlobalBounds().height;
	else
		yMax = ballY + ballPtr->getSprite().getGlobalBounds().height;

	float ySize = yMax - yMin;

	float xMin = 0;
	if (brickX > ballX)
		xMin = brickX;
	else
		xMin = ballX;

	float xMax = 0;
	if (brickX + rectPtr->getGlobalBounds().width < ballX + ballPtr->getSprite().getGlobalBounds().width)
		xMax = brickX + rectPtr->getGlobalBounds().width;
	else
		xMax = ballX + ballPtr->getSprite().getGlobalBounds().width;

	float xSize = xMax - xMin;

	// The origin is at the top-left corner of the screen
	// Set the collision response
	if (xSize > ySize)
	{
		if (circleCenterY > brickCenterY)
		{
			return Sides::BOTTOM;// Bottom
		}
		else
		{
			return Sides::TOP;// Top
		}
	}
	else
	{
		if (circleCenterX < brickCenterX)
		{
			return Sides::LEFT;// Left
		}
		else
		{
			return Sides::RIGHT;// Right
		}
	}
}