#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	this->m_PlayerPedal.setSize(
		sf::Vector2f(
			PlayerConfig::PEDAL_WIDTH,
			PlayerConfig::PEDAL_HEIGHT
		)
	);
	this->m_PlayerPedal.setFillColor(sf::Color::White);

	this->m_Lives = PlayerConfig::PLAYER_LIVES;
}


void Player::loseLive()
{
	this->m_Lives -= 1;
}

int Player::getLives()
{
	return this->m_Lives;
}

int Player::getScore()
{
	return this->m_Score;
}

void Player::setScore(int score)
{
	this->m_Score = score;
}

void Player::resetLives()
{
	this->m_Lives = PlayerConfig::PLAYER_LIVES;
}

void Player::setVelocity(float velocity)
{
	this->m_Velocity = velocity;
}

void Player::setPosition(sf::Vector2f position)
{
	this->m_PlayerPedal.setPosition(position);
}

sf::RectangleShape Player::getSprite()
{
	return this->m_PlayerPedal;
}

sf::Vector2f Player::getPosition()
{
	return this->m_PlayerPedal.getPosition();
}

float Player::getVelocity(float velocity)
{
	return this->m_Velocity;
}

void Player::setFacingSide(Sides side)
{
	this->m_FacingSide = side;
}

Sides Player::getFacingSide()
{
	return this->m_FacingSide;
}

void Player::handleCollision(Sides side, const sf::RectangleShape& rect)
{
	// If Player Pedal collides to the right Border
	if (side == Sides::LEFT)
	{
		float DeltaX = (this->m_PlayerPedal.getGlobalBounds().left + this->m_PlayerPedal.getGlobalBounds().width) - rect.getGlobalBounds().left;

		this->m_PlayerPedal.move(
			-DeltaX,
			0
		);
	}

	// If Player Pedal collides to the left Border
	if (side == Sides::RIGHT)
	{
		float DeltaX = this->m_PlayerPedal.getGlobalBounds().left - rect.getGlobalBounds().left - (rect.getGlobalBounds().width);

		this->m_PlayerPedal.move(
			std::abs(DeltaX),
			0
		);
	}
}
