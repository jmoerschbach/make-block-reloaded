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

void SnakeGame::moveSnake() {
	eraseSnake();
	snake.moveSnake();
}
void SnakeGame::eraseSnake() {
//	for (uint8_t x = 0; x < W; x++) {
//		for (uint8_t y = 0; y < H; y++) {
//			if (gameArea[x][y] == SNAKE_HEAD || gameArea[x][y] == SNAKE_TAIL)
//				gameArea[x][y] = FREE;
//		}
//}
	gameArea[snake.tail[snake.length].x][snake.tail[snake.length].y] = FREE;
}
void SnakeGame::drawSnake() {
	for (uint16_t i = 0; i < snake.length; i++) {
		gameArea[snake.tail[i].x][snake.tail[i].y] = SNAKE_TAIL;
	}
	gameArea[snake.head.x][snake.head.y] = SNAKE_HEAD;
}


void SnakeGame::moveSnakeBla() {

	Coordinate pixelAhead = snake.getPixelAhead();

	if (!isPixelInBounds(pixelAhead.x, pixelAhead.y))
		return;

	if (isPixelSnake(pixelAhead.x, pixelAhead.y))
		return;

	if (isPixelFood(pixelAhead.x, pixelAhead.y)) {
		snake.appendTail();
		placeFood();
	}
	moveSnake();
}

void SnakeGame::loopSnake() {
	snake.determineNewDirection();

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
