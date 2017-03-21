/*
 * Snake.h
 *
 *  Created on: 13.03.2017
 *      Author: Jonas
 */

#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_
#include <stdint.h>
#include "Enums.h"
#include "../WallGame.h"
class Snake {
public:
	Snake();
	virtual ~Snake();
	Coordinate getPixelAhead();
	void determineNewDirection();
	void moveSnake();
	void appendTail();
	void reset();
	Coordinate tail[H * W];
	Coordinate head;
	uint16_t length;

private:
	void adaptHeadCoordinate();
	void adaptTailCoordinates();
	void takeOver();
	Direction currentDirection;
	Direction newDirection;
};

#endif /* SNAKE_SNAKE_H_ */
