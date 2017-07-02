/*
 * Bulb.cpp
 *
 *  Created on: 18.06.2017
 *      Author: Jonas
 */
#include <EEPROM.h>
#include "Bulb.h"
#include "../hmi/keys.h"
#define SPARKING 200
#define COOLING 55
Bulb::Bulb() {
	hue = 0;
	brightness = 128;
	sat=128;
	counter = HUE_CHANGE_SPEED;
	hue = EEPROM.read(EEPROM_BULB_ADDRESS);
	brightness=EEPROM.read(EEPROM_BULB_ADDRESS+1);
	sat = EEPROM.read(EEPROM_BULB_ADDRESS+2);
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

	EEPROM.put(EEPROM_BULB_ADDRESS, hue);
}

void Bulb::determineSat() {
	if (--counter != 0)
		return;
	counter = HUE_CHANGE_SPEED;

	if (getLeftHatY() > 2200)
		sat++;
	else if (getLeftHatY() < 1800)
		sat--;

	EEPROM.put(EEPROM_BULB_ADDRESS+2, sat);
}

void Bulb::determineBrightness() {
	if (getRightHatY() > 2200 && brightness < 255)
		brightness++;
	else if (getRightHatY() < 1800 && brightness > 0)
		brightness--;
	EEPROM.put(EEPROM_BULB_ADDRESS + 1, brightness);

}

void Bulb::writeToLeds() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= CHSV(hue, sat, brightness);
		}
	}
}

void Bulb::loop() {
	determineHue();
	determineBrightness();
	//determineSat();
	writeToLeds();
	//LEDS.clear();
//	fire();
}

void Bulb::igniteNewSpark() {
	uint8_t y = random8(3);
	uint8_t x = random8(4, 12);
	fireArea[x][y] = qadd8(fireArea[x][y], random8(160, 255));
}

void Bulb::fire() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < 4; y++) {
			fireArea[x][y] += 45;
		}
	}
//make sure center bottom of fire is always hot
	for (uint8_t x = 4; x < 12; x++) {
		for (uint8_t y = 0; y < 4; y++) {
			fireArea[x][y] = qadd8(fireArea[x][y], random8(50, 100));
		}
	}

//everythings cool down a bit
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			fireArea[x][y] = qsub8(fireArea[x][y],
					random8(0, ((COOLING * 10) / 20)));
		}
	}
	//heat "rises"
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = H; y >= 2; y--) {
			fireArea[x][y] = (fireArea[x][y - 1] + fireArea[x][y - 2]
					+ fireArea[x][y - 2]) / 3;
		}
	}

	//"ignite" new sparks
	if (random8() < SPARKING) {
		igniteNewSpark();
		igniteNewSpark();
		igniteNewSpark();
	}

	//map to color
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= HeatColor(fireArea[x][y]);
		}
	}
}

