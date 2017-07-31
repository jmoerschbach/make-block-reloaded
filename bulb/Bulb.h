/*
 * Bulb.h
 *
 *  Created on: 18.06.2017
 *      Author: Jonas
 */
#include "../WallGame.h"
#ifndef BULB_BULB_H_
#define BULB_BULB_H_
#define HUE_SAT_CHANGE_SPEED 10
#define EEPROM_BULB_ADDRESS_HUE EEPROM_ADDRESS_SPACE_BULB + 0
#define EEPROM_BULB_ADDRESS_SAT EEPROM_ADDRESS_SPACE_BULB + 1
#define EEPROM_BULB_ADDRESS_BRIGHTNESS EEPROM_ADDRESS_SPACE_BULB + 2
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
	void saveConfiguration();
	void updateLeds();
	void fire();
	void igniteNewSpark();
};

#endif /* BULB_BULB_H_ */
