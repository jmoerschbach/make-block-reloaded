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
	sat = 128;
	state = RAINBOW;
	counter = HUE_SAT_CHANGE_SPEED;
	EEPROM.get(EEPROM_BULB_ADDRESS_HUE, hue);
	EEPROM.get(EEPROM_BULB_ADDRESS_BRIGHTNESS, brightness);
	EEPROM.get(EEPROM_BULB_ADDRESS_SAT, sat);
}

Bulb::~Bulb() {
	// TODO Auto-generated destructor stub
}

void Bulb::determineHue() {
	if (getLeftHatX() > 2200) {
		hue++;
	} else if (getLeftHatX() < 1800) {
		hue--;
	}
}

void Bulb::determineSat() {

	if (getLeftHatY() > 2200) {
		sat++;
	} else if (getLeftHatY() < 1800) {
		sat--;
	}

}

void Bulb::determineBrightness() {
	if (getRightHatY() > 2200 && brightness < 255) {
		brightness++;
	} else if (getRightHatY() < 1800 && brightness > 0) {
		brightness--;
	}

}

void Bulb::updateLeds() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= CHSV(hue, sat, brightness);
		}
	}
}

void Bulb::saveConfiguration() {
	if (wasKeyAPressed()) {
		EEPROM.update(EEPROM_BULB_ADDRESS_BRIGHTNESS, brightness);
		EEPROM.update(EEPROM_BULB_ADDRESS_HUE, hue);
		EEPROM.update(EEPROM_BULB_ADDRESS_SAT, sat);
	}
}

void Bulb::sinelon() {
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, 20, 20);
	//fadeToBlackBy(&leds[20], 20, 20);
	int pos = beatsin16(30, 0, 20 - 1);
	leds[pos] += CHSV(hue, 255, 192);
	for (uint8_t i = 0; i < 20; i++) {
		leds[i + 20] = leds[i];
	}
	//leds[pos+20] += CHSV(hue, 255, 192);
}

void Bulb::copyColumn(uint8_t source, uint8_t target) {
	for (uint8_t row = 0; row < H; row++) {
		leds[target * H + row] = leds[source * H + row];
	}

}
void Bulb::spreadFirstColumn() {
	for (uint8_t target = 1; target < 15; target++) {
		copyColumn(0, target);
	}

}
void Bulb::confetti() {
	// random colored speckles that blink in and fade smoothly
//  fadeToBlackBy( leds, NUM_LEDS, 3);
	brighten8_lin(5);
	fadeLightBy(leds, NUM_LEDS, 5);
	//fadeUsingColor(leds, NUM_LEDS, CRGB::Red);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void Bulb::rainbow() {

	fill_rainbow(leds, H, hue++, 10);
	spreadFirstColumn(); //copyColumn(0, 1);

}

void Bulb::loop() {
	if (--counter == 0) {
		counter = HUE_SAT_CHANGE_SPEED;

		switch (state) {
		case RAINBOW:
			rainbow();
			break;
		case BULB:
			bulb();
			break;
		default:
			break;
		}
	}
}

void Bulb::bulb() {
	determineSat();
	determineHue();
	determineBrightness();
	updateLeds();
	saveConfiguration();
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

