/*
 * Snake.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Jonas
 */

#include "Snake.h"

Snake::Snake() {
	length = 1;
	head = {1, 1};
	tail[0] = {0, 1};

}

Snake::~Snake() {
	// TODO Auto-generated destructor stub
}

void Snake::moveSnake(Direction dir) {
	for (uint16_t index = length; (index > 0); index--) {
		tail[index].x = tail[index - 1].x;
		tail[index].y = tail[index - 1].y;
	}
	tail[0].x = head.x;
	tail[0].y = head.y;
	int8_t x = 0;
	int8_t y = 0;
	switch (dir) {
	case RIGHT:
		x = 1;
		break;
	case LEFT:
		x = -1;
		break;
	case UP:
		y = 1;
		break;
	case DOWN:
		y = -1;
		break;
	}
	head.x += x;
	head.y += y;

}

void Snake::appendTail() {
	length++;

}

