#include "stdafx.h"
#include "Engine.h"
#include <iostream>
#include "UserStructures.h"


Engine::Engine()
{
	this->m_CurrentScore = 0;
	this->m_GameStatus = GameStatus::PAUSED;

	this->m_PlayerStatePtr = &PlayerState::idle;

	this->m_Parser.setLevelPtr(&this->m_Level);

	this->m_Window.create(
		sf::VideoMode(
			sf::VideoMode::getDesktopMode().width,
			sf::VideoMode::getDesktopMode().height
		),
		"Breakout"
	);

	this->m_CurrentLevel = EngineConfig::START_LEVEL;
	this->m_LevelMaxNumber = EngineConfig::MAX_LEVELS;
}

void Engine::start()
{
	this->setupLevel();

	while (this->m_Window.isOpen())
	{
		handleNextLevelRules();
		handleWinRules();
		handleLoseRules();

		this->input();
		this->update();
		this->draw();
	}
}

void Engine::input()
{
	sf::Event event;
	while (this->m_Window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			this->m_Window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				this->m_Window.close();
			if (event.key.code == sf::Keyboard::P)
			{
				if (this->m_GameStatus == GameStatus::PLAYING)
				{
					this->m_GameStatus = GameStatus::PAUSED;
				}else if (this->m_GameStatus == GameStatus::PAUSED)
				{
					this->m_GameStatus = GameStatus::PLAYING;
				}
				else if (this->m_GameStatus == GameStatus::WIN)
				{
					this->m_GameStatus = GameStatus::PLAYING;
				}
				else if (this->m_GameStatus == GameStatus::LOSE)
				{
					this->m_GameStatus = GameStatus::PLAYING;
				}
			}
			break;
		}
	}

	if (this->m_GameStatus == GameStatus::PLAYING)
	{
		// Check Player-Border collision
		this->m_PlayerStatePtr = this->m_PlayerStatePtr->handleInput(this->m_Collision, this->m_Player);
	}
}

void Engine::update()
{
	float elapsedTime = this->m_Clock.restart().asSeconds();
	if (this->m_GameStatus == GameStatus::PLAYING)
	{
		this->m_PlayerStatePtr->update(
			this->m_Player,
			elapsedTime
		);

		this->m_Ball.update(elapsedTime);
		
		// Check Ball - Pedal Collision
		this->m_Collision.checkBallPedal();
		// Check Ball - Brick Collision
		this->m_Collision.checkBallBrick();
		// Check Ball - Border Collision
		this->m_Collision.checkBallBorder();
		
	}

	// Update the results and the Informatio Menu
	this->m_Level.setMenuText(
		"Lives: " + std::to_string(this->m_Player.getLives())
		+ "       "
		+ "Level: " + std::to_string(this->m_CurrentLevel)
		+ "       "
		+ "Score: " + std::to_string(this->m_Player.getScore())
		+ "       "
		+ this->handleGameStatus(this->m_GameStatus)
	);
}

void Engine::draw()
{
	this->m_Window.clear(sf::Color::White);

	// Draw the background
	this->m_Window.draw(this->m_Level.getBackgroundSprite());

	for (auto border : this->m_Level.getBorders())
	{
		// Draw the borders.
		this->m_Window.draw(border.second);
	}


	for (auto brick : this->m_Level.getBricks())
	{
		// Draw the bricks.
		this->m_Window.draw(*brick->sprite);
	}

	// Draw Player pedal
	this->m_Window.draw(this->m_Player.getSprite());

	// Draw Ball
	this->m_Window.draw(this->m_Ball.getSprite());

	// Draw the Information Menu
	this->m_Window.draw(*this->m_Level.getMenuText());

	// Display the frame.
	this->m_Window.display();
}

void Engine::setupLevel()
{
	// Parse and build the level.
	this->m_Parser.parseLevel(this->m_CurrentLevel);

	// Build the level borders.
	this->m_Level.setUpBorders();

	// Set up the Information Menu
	this->m_Level.setUpMenu();

	this->m_Ball.setAngle(BallConfig::BALL_STARTING_ANGLE);

	// Set Player starting location
	this->m_Player.setPosition(
		sf::Vector2f(
		(float)this->m_Level.getWindowResolution().x / 2 - this->m_Player.getSprite().getSize().x/2,
			(float)this->m_Level.getWindowResolution().y / 4
			+ ((this->m_Level.getRowCount() * this->m_Level.getSpriteDimensions().y)
				+ (this->m_Level.getRowCount() + this->m_Level.getRowSpacing()))
		));

	// Set Ball starting location
	this->m_Ball.setPosition(
		sf::Vector2f(
		(float)this->m_Level.getWindowResolution().x / 2,
			(float)this->m_Level.getWindowResolution().y / 4
			+ ((this->m_Level.getRowCount() * this->m_Level.getSpriteDimensions().y)
				+ (this->m_Level.getRowCount() + this->m_Level.getRowSpacing()))
					- this->m_Ball.getSprite().getRadius()
		));

	// Set Colliders.
	this->m_Collision.setColliders(
		&this->m_Level,
		&this->m_Player,
		&this->m_Ball
	);
}

void Engine::handleNextLevelRules()
{
	auto& bricks = this->m_Level.getBricks();
	// Count bricks left, igrnore cosmetic one.
	int nonCosmeticBrickCount = std::count_if(bricks.begin(), bricks.end(), [](Brick* brick) {return !brick->cosmetic; });

	if (nonCosmeticBrickCount == 0)
	{
		this->m_CurrentLevel += 1;
		this->setupLevel();
		this->m_GameStatus = GameStatus::PAUSED;
	}
}

void Engine::handleWinRules()
{
	if (this->m_CurrentLevel > EngineConfig::MAX_LEVELS)
	{
		std::cout << this->m_CurrentLevel << "---" << EngineConfig::MAX_LEVELS << std::endl;

		this->m_CurrentScore = this->m_Player.getScore();
		this->m_CurrentLevel = 1;
		this->m_Player.setScore(0);
		this->m_Player.resetLives();
		this->m_GameStatus = GameStatus::WIN;
		this->setupLevel();
	}
}

void Engine::handleLoseRules()
{
	if(this->m_Player.getLives() <= 0)
	{
		this->m_CurrentLevel = EngineConfig::START_LEVEL;
		this->m_Player.setScore(0);
		this->m_Player.resetLives();
		this->m_GameStatus = GameStatus::LOSE;
		this->setupLevel();
	}
}

std::string Engine::handleGameStatus(GameStatus status)
{
	std::string msg = "";

	if (GameStatus::PAUSED == status)
	{
		msg = "Paused (Press P to Unpause)";
	}
	if (GameStatus::LOSE == status)
	{
		msg = "Lose";
	}
	if (GameStatus::WIN == status)
	{
		msg = "You WON with " + std::to_string(this->m_CurrentScore) + " points!";
	}
	if (GameStatus::PLAYING == status)
	{
		msg = "Playing";
	}
	return msg;
}


