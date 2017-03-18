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

#define SNAKE_SPEED 40 //the lower the value, the higher the speed
#define GAME_CYCLE (1000/60) //60 FPS
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
	void redrawSnake();
	void eraseSnake();
	void determineNewDirection();
	void drawSnake();
	void showTitle();
	PixelState gameArea[W][H];
	uint32_t nextEvent;

	SnakeState gameState;
	uint8_t gameStepCounter;
};

#endif /* SNAKE_SNAKEGAME_H_ */
