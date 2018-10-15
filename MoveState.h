#pragma once

#include "PlayerState.h"

class MoveState : public PlayerState
{	
	PlayerState* handleInput(Collision& collision, Player& player) override;
	void update(Player& player, float elapsedTime) override;
};

