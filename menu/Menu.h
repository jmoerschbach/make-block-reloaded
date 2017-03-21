/*
 * Menu.h
 *
 *  Created on: 21.03.2017
 *      Author: Jonas
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_
#include "../WallGame.h"

class Menu {
	uint32_t nextEvent;

public:
	Menu();
	virtual ~Menu();

	WallGameState loopMenu();
};

#endif /* MENU_MENU_H_ */
