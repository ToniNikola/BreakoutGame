#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "XMLLevelParser.h"
#include "Level.h"
#include "Player.h"
#include "Ball.h"
#include "Collision.h"

#include "MoveState.h"
#include "IdleState.h"



class Engine
{
private:
	Ball m_Ball;
	Level m_Level;
	Player m_Player;
	sf::Sound m_Sound;

	GameStatus m_GameStatus;

	sf::Clock m_Clock;
	int m_CurrentLevel;
	int m_CurrentScore;
	int m_LevelMaxNumber;
	Collision m_Collision;
	XMLLevelParser m_Parser;
	sf::String m_SpecialMsg;
	sf::RenderWindow m_Window;
	PlayerState* m_PlayerStatePtr;

	void handleNextLevelRules();
	void handleWinRules();
	void handleLoseRules();

	std::string handleGameStatus(GameStatus status);

public:
	Engine();
	
	void start();

	void input();
	void update();
	void draw();


	// Helpers
	void setupLevel();
};

