#pragma once

#include "Ball.h"
#include "Level.h"
#include "Player.h"

class Collision
{
private:
	Level * m_LevelPtr;
	Player * m_PlayerPtr;
	Ball * m_BallPtr;

public:
	void setColliders(Level* levelPtr, Player* playerPtr, Ball* ballPtr);

	void checkBallBrick();
	void checkBallBorder();
	void checkBallPedal();
	                              
	bool checkPedalBorder(Sides side);

	~Collision();
};

