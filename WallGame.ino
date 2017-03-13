#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "WallGame.h"
#include "hmi/keys.h"
#include "tetri/tetris.h"
#include "snake/SnakeGame.h"
CRGB leds[NUM_LEDS];
SnakeGame snake;

// possible game states
typedef enum {
	STATE_TETRIS, STATE_SNAKE, STATE_LIGHT_BULB
} state_t;

state_t wallGameState;
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
	switch (wallGameState) {
	case STATE_TETRIS: {
		loopTetris();
		break;
	}
	case STATE_SNAKE: {
		snake.loopSnake();
		break;
	}
	default:
		break;
	}

}
