#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "WallGame.h"
#include "tetris.h"
#include "Snake.h"
CRGB leds[NUM_LEDS];

// possible game states
typedef enum {
	STATE_TETRIS, STATE_SNAKE, STATE_LIGHT_BULB
} state_t;

state_t wallGameState;
void setup() {
	Serial.begin(115200);
	Serial.println("Tetris");
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);

	wallGameState = STATE_TETRIS;

	initTetris();
}

void loop() {
	pollKeyStatus();
	switch (wallGameState) {
	case STATE_TETRIS: {
		loopTetris();
		break;
	}
	case STATE_SNAKE: {
		loopSnake();
		break;
	}
	default:
		break;
	}

}
