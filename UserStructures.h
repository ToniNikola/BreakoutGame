#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const enum Sides { NONE, LEFT, RIGHT, TOP, BOTTOM };
const enum GameStatus {PAUSED, WIN, LOSE, PLAYING};

struct Brick
{
	sf::Sprite* sprite;
	int breakScore;
	int HitPoints;
	sf::SoundBuffer* hitSound;
	sf::SoundBuffer* breakSound;
	bool cosmetic;
};

namespace EngineConfig
{
	const int START_LEVEL = 1;
	const int MAX_LEVELS = 3;
}


namespace LevelConfig
{
	const int TOP_TEXT_OFFSET = 20;
	const int LEFT_TEXT_OFFSET = 200;
	const int CHARACTER_SIZE = 40;
}

namespace PlayerConfig
{
	const int PLAYER_SPEED = 500;

	const int PLAYER_LIVES = 3;

	const int PEDAL_HEIGHT = 10;
	const int PEDAL_WIDTH = 100;
}

namespace BallConfig
{
	const int BALL_RADIUS = 7;
	const int BALL_SPEED = 500;

	const int DEGREE_MULTIPLIER = 40;
	const int PEDAL_CHECK_LINE_OFFSET = 20;

	const int BALL_STARTING_ANGLE = 270;
}


namespace ParserConfig {
	const char * const LEVEL_PREFIX = "Level-";
	const char * const FILE_EXTENSION = ".xml";

	const char  BlandBrickPlaceholder = '_';
	const char * const CosmeticBrickDelimiter = "Infinite";

	const char * const LevelAttribute = "Level";

	const char * const SpriteWidthAttribute = "SpriteWidth";
	const char * const SpriteHeightAttribute = "SpriteHeight";

	const char * const ColumnCountAttribute = "ColumnCount";
	const char * const RowCountAttribute = "RowCount";

	const char * const ColumnSpacingAttribute = "ColumnSpacing";
	const char * const RowSpacingAttribute = "RowSpacing";
	const char * const BackgroundTextureAttribute = "BackgroundTexture";
	const char * const MenuFontAttribute = "MenuFont";
	
	const char * const BricksNode = "Bricks";
	const char * const BrickTypesNode = "BrickTypes";
	const char * const BrickTextureAttribute = "Texture";
	const char * const BrickHitSoundAttribute = "HitSound";
	const char * const BrickHitPointsAttribute = "HitPoints";
	const char * const BrickBreakSoundAttribute = "BreakSound";
	const char * const BrickBreakScoreAttribute = "BreakScore";
};
