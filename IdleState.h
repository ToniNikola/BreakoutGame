#pragma once

#include "PlayerState.h"

class IdleState : public PlayerState
{
	PlayerState* handleInput(Collision& collision, Player& player) override;
	void update(Player& player, float elapsedTime) override;
};