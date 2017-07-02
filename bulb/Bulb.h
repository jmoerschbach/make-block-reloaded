/*
 * Bulb.h
 *
 *  Created on: 18.06.2017
 *      Author: Jonas
 */
#include "../WallGame.h"
#ifndef BULB_BULB_H_
#define BULB_BULB_H_
#define HUE_CHANGE_SPEED 3
#define EEPROM_BULB_ADDRESS 0
class Bulb {
public:
	Bulb();
	virtual ~Bulb();

	void loop();
private:
	uint8_t hue;
	uint8_t brightness;
	uint8_t sat;
	uint8_t counter;
	byte fireArea[W][H];

	void determineHue();
	void determineBrightness();
	void determineSat();
	void writeToLeds();
	void fire();
	void igniteNewSpark();
};

#endif /* BULB_BULB_H_ */
