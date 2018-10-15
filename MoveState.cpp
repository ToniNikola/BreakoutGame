#include "stdafx.h"

#include "MoveState.h"
#include "IdleState.h"

PlayerState* MoveState::handleInput(Collision& collision, Player& player)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !collision.checkPedalBorder(RIGHT))
	{
		player.setFacingSide(LEFT);
		return &PlayerState::move;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !collision.checkPedalBorder(LEFT))
	{
		player.setFacingSide(RIGHT);
		return &PlayerState::move;
	}

	return &PlayerState::idle;
};

void MoveState::update(Player& player, float elapsedTime)
{
	if (player.getFacingSide() == LEFT)
	{
		player.setPosition(sf::Vector2f(
			player.getPosition().x - PlayerConfig::PLAYER_SPEED * elapsedTime, 
			player.getPosition().y
		));
	}
	else if (player.getFacingSide() == RIGHT)
	{
		player.setPosition(sf::Vector2f(
			player.getPosition().x + PlayerConfig::PLAYER_SPEED * elapsedTime,
			player.getPosition().y
		));
	}
};

MoveState PlayerState::move;