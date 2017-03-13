/*
 * SnakeGame.cpp
 *
 *  Created on: 11.03.2017
 *      Author: Jonas
 */

#include "SnakeGame.h"
#include "../hmi/keys.h"
SnakeGame::SnakeGame() {
	resetGameArea();

	gameArea[snake.head.x][snake.head.y] = SNAKE_HEAD;
	gameArea[snake.tail[0].x][snake.tail[0].y] = SNAKE_TAIL;
	currentDirection = RIGHT;
	gameState = TITLE;

	placeFood();

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

void SnakeGame::moveSnake(int8_t x, int8_t y) {
	uint8_t pixelAheadX = snake.head.x + x;
	uint8_t pixelAheadY = snake.head.y + y;

	if (!isPixelInBounds(pixelAheadX, pixelAheadY))
		return;

	if (isPixelSnake(pixelAheadX, pixelAheadY))
		return;

	if (isPixelFood(pixelAheadX, pixelAheadY)) {
		snake.appendTail();
		placeFood();
	}
	eraseSnake();
	snake.moveSnake(currentDirection);
}
void SnakeGame::eraseSnake() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			if (gameArea[x][y] == SNAKE_HEAD || gameArea[x][y] == SNAKE_TAIL)
				gameArea[x][y] = FREE;
		}
	}
}
void SnakeGame::drawSnake() {
	for (uint16_t i = 0; i < snake.length; i++) {
		gameArea[snake.tail[i].x][snake.tail[i].y] = SNAKE_TAIL;
	}
	gameArea[snake.head.x][snake.head.y] = SNAKE_HEAD;
}

void SnakeGame::moveSnakeBla() {
	int8_t x = 0;
	int8_t y = 0;
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
void SnakeGame::determineNewDirection() {
	if (wasDownPressed() && currentDirection != UP)
		currentDirection = DOWN;
	else if (wasLeftPressed() && currentDirection != RIGHT)
		currentDirection = LEFT;
	else if (wasRightPressed() && currentDirection != LEFT)
		currentDirection = RIGHT;
	else if (wasUpPressed() && currentDirection != DOWN)
		currentDirection = UP;
}
void SnakeGame::loopSnake() {
	determineNewDirection();

	if ((long) (nextEvent - millis()) > 0) {
		return;
	}
	switch (gameState) {
	case TITLE:
		if (wasAnyKeyPressed())
			gameState = PLAYING;
		break;
	case PLAYING:
		moveSnakeBla();
		drawSnake();
		break;
	case SCORE:
		break;
	}

	draw();
	LEDS.show();
	nextEvent += (1000 / 2);
}
