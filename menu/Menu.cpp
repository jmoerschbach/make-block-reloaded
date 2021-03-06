/*
 * Menu.cpp
 *
 *  Created on: 21.03.2017
 *      Author: Jonas
 */

#include "Menu.h"
#include "../hmi/keys.h"
Menu::Menu() {
	index = 0;
}

Menu::~Menu() {
}

void Menu::drawSelectionIndicator() {
	LED(0,(index*OFFSET))= CRGB::Red;
	LED(1,(index*OFFSET)) = CRGB::Red;
	LED(0,(index*OFFSET)+1) = CRGB::Red;
	LED(1,(index*OFFSET)+1) = CRGB::Red;
}

void Menu::drawSnakeOptionAt(uint8_t startY) {
	for (uint8_t x = 3; x < 6; x++) {
		LED(x,(startY*OFFSET))= CRGB::Blue;
	}
	for(uint8_t x = 5; x < 9; x++) {
		LED(x,(startY*OFFSET)+1) = CRGB::Blue;
	}
	LED(9, (startY*OFFSET)+1) = CRGB::Red;
	LED(11, (startY*OFFSET)+1) = CRGB::Green;
}

void Menu::drawTetrisOptionAt(uint8_t startY) {
	LED(3, (startY*OFFSET))= CRGB(0xc000c0);
	LED(4, (startY*OFFSET)) = CRGB(0xc000c0);
	LED(5, (startY*OFFSET)) = CRGB(0xc000c0);
	LED(4, (startY*OFFSET)+1) = CRGB(0xc000c0);

	LED(6, (startY*OFFSET))= CRGB(0x00ffff);
	LED(7, (startY*OFFSET))= CRGB(0x00ffff);
	LED(8, (startY*OFFSET))= CRGB(0x00ffff);
	LED(8, (startY*OFFSET)+1)= CRGB(0x00ffff);

	LED(6, (startY*OFFSET)+1) = CRGB(0xff0000);
	LED(7, (startY*OFFSET)+1) = CRGB(0xff0000);
	LED(7, (startY*OFFSET)+2) = CRGB(0xff0000);
	LED(8, (startY*OFFSET)+2) = CRGB(0xff0000);

	LED(9, (startY*OFFSET)) = CRGB(0xffff00);
	LED(10, (startY*OFFSET)) = CRGB(0xffff00);
	LED(9, (startY*OFFSET)+1) = CRGB(0xffff00);
	LED(10, (startY*OFFSET)+1) = CRGB(0xffff00);
}

void Menu::drawBulbOptionAt(uint8_t startY) {
	LED(3, (startY*OFFSET))= CRGB(0xc000c0);
	LED(4, (startY*OFFSET)) = CRGB(0xc000c0);
	LED(5, (startY*OFFSET)) = CRGB(0xc000c0);
	LED(4, (startY*OFFSET)+1) = CRGB::Yellow;
}

WallGameState Menu::loop() {
	LEDS.clear();
	if (wasUpPressed()) {
		index++;
	}
	if (wasDownPressed()) {
		index--;
	}
	if (index > NUM_AVAILABLE_OPTIONS - 1)
		index = 0;
	if (index < 0)
		index = NUM_AVAILABLE_OPTIONS - 1;

	drawSelectionIndicator();
	drawBulbOptionAt(2);
	drawSnakeOptionAt(1);
	drawTetrisOptionAt(0);
	if (wasKeyPlusPressed())
		return STATE_LED_TEST;
	if (wasKeyAPressed())
		return availableOptions[index];
	else
		return STATE_MENU;
}

