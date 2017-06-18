/*
 * SnakeGame.h
 *
 *  Created on: 11.03.2017
 *      Author: Jonas
 */

#ifndef SNAKE_SNAKEGAME_H_
#define SNAKE_SNAKEGAME_H_
#include "../WallGame.h"
#include "Snake.h"
#include "Enums.h"
#define FOOD_PER_LEVEL 5
class SnakeGame {
public:
	SnakeGame();
	virtual ~SnakeGame();

	void loop();

private:
	Snake snake;

	void placeFood();
	void resetGameArea();
	void drawGameArea();bool isPixelFree(uint8_t x, uint8_t y);bool isPixelFood(
			uint8_t x, uint8_t y);bool isPixelSnake(uint8_t x, uint8_t y);bool isPixelInBounds(
			uint8_t x, uint8_t y);bool isAtLeastOnePixelFree();
	CRGB determineColorOf(uint8_t x, uint8_t y);
	void eatAndMoveSnake();
	void redrawSnake();
	void eraseSnake();
	void determineNewDirection();
	void drawSnake();
	SnakeState showTitle();bool isPixelAheadGood();
	uint8_t getSpeedInFPS();
	uint8_t calculateLevel();
	SnakeState runGameEngine();
	void reset();

	uint8_t currentLevel;
	uint8_t lengthGainedInCurrentLevel;
	PixelState gameArea[W][H];
	uint32_t nextEvent;

	SnakeState gameState;
	uint8_t gameStepCounter;
};

#endif /* SNAKE_SNAKEGAME_H_ */
