/*
 * Snake.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Jonas
 */

#include "Snake.h"
#include "../hmi/keys.h"
Snake::Snake() {
	length = 1;
	head = {1, 1};
	tail[0] = {0, 1};
	currentDirection = RIGHT;
}

Snake::~Snake() {
	// TODO Auto-generated destructor stub
}

void Snake::adaptHeadCoordinate() {
	head = getPixelAhead();
}

void Snake::adaptTailCoordinates() {
	for (uint16_t index = length; (index > 0); index--) {
		tail[index].x = tail[index - 1].x;
		tail[index].y = tail[index - 1].y;
	}
	tail[0].x = head.x;
	tail[0].y = head.y;
}

void Snake::moveSnake() {
	adaptTailCoordinates();
	adaptHeadCoordinate();
}

void Snake::appendTail() {
	length++;

}

Coordinate Snake::getPixelAhead() {
	Coordinate pixelAhead = { head.x, head.y };
	switch (currentDirection) {
	case RIGHT:
		pixelAhead.x += 1;
		break;
	case LEFT:
		pixelAhead.x -= 1;
		break;
	case UP:
		pixelAhead.y += 1;
		break;
	case DOWN:
		pixelAhead.y -= 1;
		break;

	}
	return pixelAhead;
}
void Snake::determineNewDirection() {
	if (wasDownPressed() && currentDirection != UP)
		currentDirection = DOWN;
	else if (wasLeftPressed() && currentDirection != RIGHT)
		currentDirection = LEFT;
	else if (wasRightPressed() && currentDirection != LEFT)
		currentDirection = RIGHT;
	else if (wasUpPressed() && currentDirection != DOWN)
		currentDirection = UP;
}
