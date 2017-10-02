#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "WallGame.h"
#include "hmi/keys.h"
#include "tetri/tetris.h"
#include "snake/SnakeGame.h"
#include "bulb/Bulb.h"
#include "menu/Menu.h"
#include "tests/LedTest.h"
CRGB leds[NUM_LEDS];
SnakeGame snakeGame;
Bulb bulb;
LedTest ledTest;
uint32_t nextEvent;
Menu menu;
bool enabled = true;

WallGameState wallGameState;

void setup() {
	Serial.begin(115200);
	Serial.println("WallGame");
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	initKeys();
	// the microseconds since startup are a perfect seed as
	// the user has pressed a button since boot time
	randomSeed(micros());   // init rng
	wallGameState = STATE_LIGHT_BULB;

	initTetris();
	DEBUG("Setup finished");
}
void readSerialConsole() {
	String str = "";
	if (Serial.available() > 0) {
		str = Serial.readStringUntil('\n');
		DEBUG(str);
		if (str.compareTo("en") == 0)
			enabled = true;
		else if (str.compareTo("di") == 0)
			enabled = false;
	}
}
void loop() {
	pollKeyStatus();
	readSerialConsole();
	if (enabled) {
		if (isTimeForNextEvent()) {
			loopGameState();
			nextEvent = millis() + GAME_CYCLE;
			LEDS.show();
		}
	} else
		LEDS.clear(true);
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
	case STATE_LED_TEST:
		ledTest.loop();
		break;
	default:
		wallGameState = menu.loop();
		break;
	}
}

