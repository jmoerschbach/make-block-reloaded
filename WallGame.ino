#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "WallGame.h"
#include "hmi/keys.h"
#include "tetri/tetris.h"
#include "snake/SnakeGame.h"
#include "bulb/Bulb.h"
#include "menu/Menu.h"
CRGB leds[NUM_LEDS];
SnakeGame snakeGame;
Bulb bulb;
uint32_t nextEvent;
Menu menu;

WallGameState wallGameState;

void setup() {
	Serial.begin(115200);
	Serial.println("WallGame");
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	initKeys();
	// the microseconds since startup are a perfect seed as
	// the user has pressed a button since boot time
	randomSeed(micros());   // init rng
	wallGameState = STATE_MENU;

	initTetris();
	DEBUG("Setup finished");
}

void loop() {
	pollKeyStatus();
	if (isTimeForNextEvent()) {
		loopGameState();

		nextEvent = millis() + GAME_CYCLE;
		LEDS.show();
	}
}

bool isTimeForNextEvent() {
	return !((long) (nextEvent - millis()) > 0);
}

void loopGameState() {
	if (wasHomePressed())
		wallGameState = STATE_MENU;

	switch (wallGameState) {
	case STATE_TETRIS: {
		loopTetris();
		break;
	}
	case STATE_SNAKE: {
		snakeGame.loop();
		break;
	}
	case STATE_LIGHT_BULB:
		bulb.loop();
		break;
	default:
		wallGameState = menu.loop();
		break;
	}
}

