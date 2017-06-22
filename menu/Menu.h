/*
 * Menu.h
 *
 *  Created on: 21.03.2017
 *      Author: Jonas
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_
#include "../WallGame.h"
#define NUM_AVAILABLE_OPTIONS 3
#define OFFSET 8
class Menu {

public:
	Menu();
	virtual ~Menu();

	WallGameState loop();
private:
	int8_t index;
	WallGameState availableOptions[NUM_AVAILABLE_OPTIONS] = { STATE_TETRIS,
			STATE_SNAKE, STATE_LIGHT_BULB };

	void drawSelectionIndicator();
	void drawSnakeOptionAt(uint8_t startY);
	void drawTetrisOptionAt(uint8_t startY);
	void drawBulbOptionAt(uint8_t startY);
};

#endif /* MENU_MENU_H_ */
