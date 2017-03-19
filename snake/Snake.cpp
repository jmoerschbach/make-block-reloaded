/*
 * Snake.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Jonas
 */

#include "Snake.h"
#include "../hmi/keys.h"
Snake::Snake() {
	reset();
}

Snake::~Snake() {
	// TODO Auto-generated destructor stub
}
void Snake::reset() {
	length = 1;
	head = {1, 1};
	tail[0] = {0, 1};
	currentDirection = RIGHT;
	newDirection = RIGHT;
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

uint8_t Snake::getSpeedInFPS(uint8_t level) {

	static const uint8_t speedTable[] PROGMEM =
	// 0  1  2  3  4  5  6  7  8  9 10
			{ 40, 35, 30, 25, 20, 17, 15, 13, 11, 9, 8 };

	return pgm_read_byte(speedTable + ((level < 10) ? level : 10));
}

void Snake::moveSnake() {
	//takeOver();
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

void Snake::takeOver() {
	switch (currentDirection) {
	case UP:
		if (newDirection != DOWN)
			currentDirection = newDirection;
		break;
	case DOWN:
		if (newDirection != UP)
			currentDirection = newDirection;
		break;
	case LEFT:
		if (newDirection != RIGHT)
			currentDirection = newDirection;
		break;
	case RIGHT:
		if (newDirection != LEFT)
			currentDirection = newDirection;
		break;
	default:
		break;
	}

	Serial.print("new direction: ");
	Serial.println(currentDirection);
}
