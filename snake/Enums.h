/*
 * Enums.h
 *
 *  Created on: 13.03.2017
 *      Author: Jonas
 */

#ifndef SNAKE_ENUMS_H_
#define SNAKE_ENUMS_H_

enum Direction {
	LEFT, RIGHT, UP, DOWN
};

typedef struct {
	uint8_t x;
	uint8_t y;
} Coordinate;
enum PixelState {
	FREE, FOOD, SNAKE_TAIL, SNAKE_HEAD
};
enum SnakeState {
	PLAYING, TITLE, SCORE
};
#endif /* SNAKE_ENUMS_H_ */
