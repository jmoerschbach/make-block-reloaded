/*
 * Menu.cpp
 *
 *  Created on: 21.03.2017
 *      Author: Jonas
 */

#include "Menu.h"
#include "../hmi/keys.h"
Menu::Menu() {
	nextEvent = millis() + GAME_CYCLE;
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

WallGameState Menu::loopMenu() {
	if ((long) (nextEvent - millis()) > 0) {
		return STATE_MENU;
	}
	LEDS.clear();
	LEDS.show();
	nextEvent = millis() + GAME_CYCLE;
	if (wasUpPressed())
		return STATE_TETRIS;
	else if (wasDownPressed())
		return STATE_SNAKE;
	else
		return STATE_MENU;
}

