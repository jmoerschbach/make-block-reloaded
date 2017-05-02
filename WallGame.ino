#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "WallGame.h"
#include "hmi/keys.h"
#include "tetri/tetris.h"
#include "snake/SnakeGame.h"
#include "menu/Menu.h"
CRGB leds[NUM_LEDS];
SnakeGame snakeGame;
uint32_t nextEvent;
Menu menu;

WallGameState wallGameState;

void setup() {
	Serial.begin(115200);
	Serial.println("Tetris");
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	initKeys();
	// the microseconds since startup are a perfect seed as
	// the user has pressed a button since boot time
	randomSeed(micros());   // init rng
	wallGameState = STATE_SNAKE;

	pollKeyStatus();
	initTetris();
}

void loop() {
	pollKeyStatus();
	if ((long) (nextEvent - millis()) > 0) {
		return;
	}
	if (wasHomePressed())
		wallGameState = STATE_MENU;

	switch (wallGameState) {
	case STATE_TETRIS: {
		loopTetris();
		break;
	}
	case STATE_SNAKE: {
		snakeGame.loopSnake();
		break;
	}
	case STATE_MENU: {
		wallGameState = menu.loopMenu();
		break;
	}
	default:
		break;
	}
	LEDS.show();
	nextEvent = millis() + GAME_CYCLE;
}
