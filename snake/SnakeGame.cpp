/*
 * SnakeGame.cpp
 *
 *  Created on: 11.03.2017
 *      Author: Jonas
 */

#include "SnakeGame.h"

SnakeGame::SnakeGame() {
	resetGameArea();
	headX = W / 2;
	headY = H / 2;
	tailX = (W / 2) - 1;
	tailY = (H / 2);

	gameArea[(W / 2)][(H / 2)] = SNAKE_HEAD;
	gameArea[(W / 2) - 1][(H / 2)] = SNAKE_TAIL;
	currentDirection = RIGHT;

	gameArea[(W / 2) + 3][(H / 2)] = FOOD;

}

SnakeGame::~SnakeGame() {
	// TODO Auto-generated destructor stub
}

void SnakeGame::resetGameArea() {

	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			gameArea[x][y] = FREE;
		}
	}
}

bool SnakeGame::isPixelFree(uint8_t x, uint8_t y) {
	if (x > W || y > H)
		return false;
	return gameArea[x][y] == FREE;
}

bool SnakeGame::isPixelFood(uint8_t x, uint8_t y) {
	if (x > W || y > H)
		return false;
	return gameArea[x][y] == FOOD;
}

bool SnakeGame::isPixelSnake(uint8_t x, uint8_t y) {
	if (x > W || y > H)
		return false;
	return (gameArea[x][y] == SNAKE_HEAD) || (gameArea[x][y] == SNAKE_TAIL);
}

bool SnakeGame::isPixelInBounds(uint8_t x, uint8_t y) {
	if (x < W && y < H)
		return true;
	return false;
}

bool SnakeGame::isAtLeastOnePixelFree() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			if (isPixelFree(x, y))
				return true;
		}
	}
	return false;
}

void SnakeGame::placeFood() {
	bool placed = false;
	while (!placed && isAtLeastOnePixelFree()) {
		uint8_t x = random(W);
		uint8_t y = random(H);
		if (isPixelFree(x, y)) {
			gameArea[x][y] = FOOD;
			placed = true;
		}
	}
}

void SnakeGame::draw() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= determineColorOf(x,y);
		}
	}
}

CRGB SnakeGame::determineColorOf(uint8_t x, uint8_t y) {
	switch (gameArea[x][y]) {
	case FOOD:
		return CRGB::Green;
	case SNAKE_HEAD:
		return CRGB::Red;
	case SNAKE_TAIL:
		return CRGB::Blue;
	default:
		return CRGB::Black;
	}
}

void SnakeGame::moveSnakeHead(int8_t x, int8_t y) {
	gameArea[headX][headY] = SNAKE_TAIL;
	headX += x;
	headY += y;
	gameArea[headX][headY] = SNAKE_HEAD;
}

void SnakeGame::cutSnakeTail(int8_t x, int8_t y) {
	gameArea[tailX][tailY] = FREE;
	tailX += x;
	tailY += y;
	gameArea[tailX][tailY] = SNAKE_TAIL;
}

void SnakeGame::moveSnake(int8_t x, int8_t y) {
	uint8_t pixelAheadX = headX + x;
	uint8_t pixelAheadY = headY + y;

	if (!isPixelInBounds(pixelAheadX, pixelAheadY))
		return;

	if (isPixelSnake(pixelAheadX, pixelAheadY))
		return;

	if (!isPixelFood(pixelAheadX, pixelAheadY))
		cutSnakeTail(x, y);

	moveSnakeHead(x, y);
}

void SnakeGame::moveSnakeBla() {
	int8_t x;
	int8_t y;
	switch (currentDirection) {
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

	moveSnake(x, y);
}

void SnakeGame::loopSnake() {
	if ((long) (nextEvent - millis()) > 0) {
		return;
	}
	moveSnakeBla();
	draw();
	LEDS.show();
	nextEvent += (1000 / 2);
}
