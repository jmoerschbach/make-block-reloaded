/*
 * SnakeGame.cpp
 *
 *  Created on: 11.03.2017
 *      Author: Jonas
 */

#include "SnakeGame.h"
#include "../hmi/keys.h"
#include "../hmi/text.h"

SnakeGame::SnakeGame() {
	reset();
}

SnakeGame::~SnakeGame() {
	// TODO Auto-generated destructor stub
}

void SnakeGame::reset() {
	resetGameArea();
	placeFood();
	snake.reset();
	gameState = TITLE;
	currentLevel = 0;
	lengthGainedInCurrentLevel = 0;
	gameStepCounter = snake.getSpeedInFPS(currentLevel);
}
void SnakeGame::resetGameArea() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			gameArea[x][y] = FREE;
		}
	}
}

bool SnakeGame::isPixelFree(uint8_t x, uint8_t y) {
	if (isPixelInBounds(x, y))
		return gameArea[x][y] == FREE;
	return false;
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
	return (x < W) && (y < H);
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

void SnakeGame::drawGameArea() {
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
		return CRGB(0x101010); //dark gray
	}
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

void SnakeGame::redrawSnake() {
	eraseSnake();
	drawSnake();
}

bool SnakeGame::isPixelAheadGood() {
	Coordinate pixelAhead = snake.getPixelAhead();
	if (!isPixelInBounds(pixelAhead.x, pixelAhead.y)) {
		Serial.println("not in bounds");
		return false;
	}

	if (isPixelSnake(pixelAhead.x, pixelAhead.y)) {
		Serial.println("snake ahead");
		return false;
	}

	return true;
}

void SnakeGame::eatAndMoveSnake() {

	Coordinate pixelAhead = snake.getPixelAhead();

	if (isPixelFood(pixelAhead.x, pixelAhead.y)) {
		snake.appendTail();
		lengthGainedInCurrentLevel++;
		placeFood();
	}
	snake.moveSnake();
}

SnakeState SnakeGame::showTitle() {
	LEDS.clear();
	uint8_t offset = 0;
	offset = drawChar('S', 0, 15, 0, text_char_width('S'), CRGB::Red);
	drawChar('N', offset, 15, 0, text_char_width('N'), CRGB::Blue);
	drawChar('A', offset, 10, 0, text_char_width('A'), CRGB::Blue);
	offset += drawChar('K', offset, 4, 0, text_char_width('K'),
			CRGB::Blue);
	drawChar('E', offset, 4, 0, text_char_width('E'), CRGB::Blue);
	if (wasAnyKeyPressed())
		return PLAYING;
	return TITLE;
}

uint8_t SnakeGame::calculateLevel() {
	if (lengthGainedInCurrentLevel == FOOD_PER_LEVEL) {
		currentLevel++;
		lengthGainedInCurrentLevel = 0;
	}
	return currentLevel;
}

SnakeState SnakeGame::runGameEngine() {
	if (!isPixelAheadGood()) {
		reset();
		return TITLE;
	}
	eatAndMoveSnake();
	redrawSnake();
	drawGameArea();
	return PLAYING;
}

void SnakeGame::loopSnake() {

	if ((long) (nextEvent - millis()) > 0) {
		return;
	}
	switch (gameState) {
	case TITLE:
		gameState = showTitle();
		break;
	case PLAYING:
		snake.determineNewDirection();
		if (--gameStepCounter == 0) {
			gameState = runGameEngine();
			gameStepCounter = snake.getSpeedInFPS(calculateLevel());
		}
		break;
	case SCORE:
		break;
	}

	LEDS.show();
	nextEvent += GAME_CYCLE;
}
