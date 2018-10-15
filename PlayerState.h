#pragma once

#include "UserStructures.h"
#include "Player.h"
#include "Collision.h"

class IdleState;
class MoveState;

class PlayerState
{
protected:
	Sides m_MovingSide;

public:
	static IdleState idle;
	static MoveState move;

	virtual ~PlayerState() {};

	virtual PlayerState* handleInput(Collision& collision, Player& player)
	{
		return this;
	}

	virtual void update(Player& player, float elapsedTime) {};
};

