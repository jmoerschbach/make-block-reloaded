/*
 * LedTest.h
 *
 *  Created on: 16.09.2017
 *      Author: Jonas
 */

#ifndef TESTS_LEDTEST_H_
#define TESTS_LEDTEST_H_
#include "../WallGame.h"
class LedTest {
public:
	LedTest();
	virtual ~LedTest();

	void loop();
private:
	uint16_t currentLed;
	uint8_t stepCounter;

	enum LedColorState {
		RED, GREEN, BLUE
	};

	LedColorState state;
};
#endif /* TESTS_LEDTEST_H_ */
