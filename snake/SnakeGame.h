/*
 * SnakeGame.h
 *
 *  Created on: 11.03.2017
 *      Author: Jonas
 */

#ifndef SNAKE_SNAKEGAME_H_
#define SNAKE_SNAKEGAME_H_
#include "../WallGame.h"
class SnakeGame {
public:
	SnakeGame();
	virtual ~SnakeGame();

	void loopSnake();
	enum PixelState {
		FREE, FOOD, SNAKE_TAIL, SNAKE_HEAD
	};
	enum Direction {
		LEFT, RIGHT, UP, DOWN
	};
private:
	void placeFood();
	void resetGameArea();
	void draw();
	bool isPixelFree(uint8_t x, uint8_t y);
	bool isPixelFood(uint8_t x, uint8_t y);
	bool isPixelSnake(uint8_t x, uint8_t y);
	bool isPixelInBounds(uint8_t x, uint8_t y);
	bool isAtLeastOnePixelFree();
	CRGB determineColorOf(uint8_t x, uint8_t y);
	void moveSnakeBla();
	void moveSnake(int8_t x, int8_t y);
	void moveSnakeHead(int8_t x, int8_t y);
	void cutSnakeTail(int8_t x, int8_t y);

	PixelState gameArea[W][H];
	uint8_t headX, headY;
	uint8_t tailX, tailY;
	uint32_t nextEvent;
	Direction currentDirection;
};

#endif /* SNAKE_SNAKEGAME_H_ */
