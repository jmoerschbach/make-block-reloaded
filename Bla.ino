#include <EEPROM.h>
#include <FastLED.h>
#include "Arduino.h"
#include "tetris.h"


void setup() {
	Serial.begin(115200);
	Serial.println("Tetris");
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);

	initTetris();
}


void loop() {
	pollKeyStatus();
	loopTetris();
}
