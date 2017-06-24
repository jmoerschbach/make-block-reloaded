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
class Bulb {
public:
	Bulb();
	virtual ~Bulb();

	void loop();
private:
	uint8_t hue;
	int brightness;
	int counter;

	void determineHue();
	void determineBrightness();
	void writeToLeds();
};

#endif /* BULB_BULB_H_ */
