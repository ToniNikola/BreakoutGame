#include "stdafx.h"

#include "IdleState.h"
#include "MoveState.h"

PlayerState* IdleState::handleInput(Collision& collision, Player& player)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !collision.checkPedalBorder(LEFT))
	{
		player.setFacingSide(LEFT);
		return &PlayerState::move;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !collision.checkPedalBorder(RIGHT))
	{
		player.setFacingSide(RIGHT);
		return &PlayerState::move;
	}

	return this;
};


void IdleState::update(Player& player, float elapsedTime)
{
	// Nothing special to do in idle state...
};

IdleState PlayerState::idle;