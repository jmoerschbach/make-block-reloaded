/*
 * Bulb.cpp
 *
 *  Created on: 18.06.2017
 *      Author: Jonas
 */

#include "Bulb.h"
#include "../hmi/keys.h"
Bulb::Bulb() {
	hue = 0;
	brightness = 128;
	counter = HUE_CHANGE_SPEED;
}

Bulb::~Bulb() {
	// TODO Auto-generated destructor stub
}

void Bulb::determineHue() {
	if (--counter != 0)
		return;
	counter = HUE_CHANGE_SPEED;

	if (getLeftHatX() > 2200)
		hue++;
	else if (getLeftHatX() < 1800)
		hue--;
}

void Bulb::determineBrightness() {
	//limit brightness due to power restrictions...
	if (getRightHatY() > 2200 && brightness < 192)
		brightness++;
	else if (getRightHatY() < 1800 && brightness > 0)
		brightness--;

}

void Bulb::writeToLeds() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= CHSV(hue, 255, brightness);
		}
	}
}

void Bulb::loop() {
	determineHue();
	determineBrightness();
	writeToLeds();
}

