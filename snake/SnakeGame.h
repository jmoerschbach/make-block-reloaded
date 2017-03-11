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
	void placeFood();
	void resetGameArea();
	enum PixelState {
			FREE, FOOD, SNAKE
		};
private:
	bool isPixelFree(uint8_t x, uint8_t y);
	bool isAtLeastOnePixelFree();
	PixelState gameArea[W][H];
};

#endif /* SNAKE_SNAKEGAME_H_ */
