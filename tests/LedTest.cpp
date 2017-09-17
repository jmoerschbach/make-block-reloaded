/*
 * LedTest.cpp
 *
 *  Created on: 16.09.2017
 *      Author: Jonas
 */

#include "LedTest.h"

LedTest::LedTest() {
	currentLed = 0;
	stepCounter = 4;
	state = RED;
}

LedTest::~LedTest() {
	// TODO Auto-generated destructor stub
}

void LedTest::loop() {
	if (stepCounter-- == 0) {
		stepCounter = 4;
		leds[currentLed] = CRGB::Black;
		currentLed++;

		if (currentLed >= NUM_LEDS) {
			currentLed = 0;
			if (state == RED)
				state = GREEN;
			else if (state == GREEN)
				state = BLUE;
			else
				state = RED;
		}
		switch (state) {
		case RED:
			leds[currentLed] = CRGB::Red;
			break;
		case GREEN:
			leds[currentLed] = CRGB::Green;
			break;
		case BLUE:
			leds[currentLed] = CRGB::Blue;
			break;
		}
	}
}

