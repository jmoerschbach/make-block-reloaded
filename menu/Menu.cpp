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

	LEDS.clear();

	if (wasUpPressed())
		return STATE_TETRIS;
	else if (wasDownPressed())
		return STATE_SNAKE;
	else
		return STATE_MENU;
}

