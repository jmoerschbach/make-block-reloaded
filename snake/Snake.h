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

	void moveSnake(Direction dir);
	void appendTail();
	Coordinate tail[H*W];
	Coordinate head;
	uint16_t length;
};

#endif /* SNAKE_SNAKE_H_ */
