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
class SnakeGame {
public:
	SnakeGame();
	virtual ~SnakeGame();

	void loopSnake();

private:
	Snake snake;

	void placeFood();
	void resetGameArea();
	void draw();bool isPixelFree(uint8_t x, uint8_t y);bool isPixelFood(
			uint8_t x, uint8_t y);bool isPixelSnake(uint8_t x, uint8_t y);bool isPixelInBounds(
			uint8_t x, uint8_t y);bool isAtLeastOnePixelFree();
	CRGB determineColorOf(uint8_t x, uint8_t y);
	void moveSnakeBla();
	Coordinate getPixelAhead();
	void moveSnake();
	void drawSnake();
	void eraseSnake();
	void determineNewDirection();
	PixelState gameArea[W][H];
	uint32_t nextEvent;

	SnakeState gameState;
};

#endif /* SNAKE_SNAKEGAME_H_ */
