#include <Arduino.h>
#include "tetris.h"
#include <Wii.h>
#include <usbhub.h>
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
		/* You can create the instance of the class in two ways */
//WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote
// bit 0 = last state
// bit 1 = repeat state
// bit 2-7 = repeat counter

uint8_t key_state[5];
uint8_t ret;
static const uint8_t key_pins[] = {
KEY_LEFT_PIN, KEY_RIGHT_PIN, KEY_ROTATE_PIN, KEY_DOWN_PIN,
KEY_DROP_PIN };

void keys_init() {
	for (uint8_t i = 0; i < 5; i++) {
		// enable internal pullups
		pinMode(key_pins[i], INPUT_PULLUP);
		key_state[i] = 0;
	}

	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1)
			; //halt
	}
	Serial.print(F("\r\nWiimote Bluetooth Library Started"));
}

// lock the auto repeat
void lockKeys() {
	for (uint8_t i = 0; i < 5; i++)
		key_state[i] |= 0xfe;
}

bool isAnyKeyCurrentlyPressed() {
	for (uint8_t i = 0; i < 5; i++) {
		if (isKeyCurrentlyPressed(key_pins[i]))
			return true;
	}
	return false;
}

bool isKeyCurrentlyPressed(uint8_t key) {
	return digitalRead(key) == LOW;
}

bool wasKeyAlreadyPressed(uint8_t indexOfKey) {
	return (key_state[indexOfKey] & 1) == 1;
}

bool wasDropPressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(A);
	return false;
	//return (ret & KEY_DROP);
}
bool wasDownPressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(DOWN);
	return false;
	//return (ret & KEY_DOWN);
}

bool wasLeftPressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(LEFT);
	return false;
	//return (ret & KEY_LEFT);
}

bool wasRightPressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(RIGHT);
	return false;
	//return (ret & KEY_RIGHT);
}

bool wasRotatePressed() {
	if (Wii.wiiUProControllerConnected)
			return Wii.getButtonClick(UP);
		return false;
	//return (ret & KEY_ROTATE);
}

bool wasPausePressed() {
	return (ret & KEY_PAUSE);
}

bool wasAnyKeyPressed() {
	return wasLeftPressed() || wasRightPressed() || wasDropPressed()
			|| wasRotatePressed();
}

// mode 0 = game, 1 = config, 2 = initials
uint8_t pollKeyStatus(uint8_t mode) {
	Usb.Task();
	ret = 0;

	// rotate key does not repeat
	for (uint8_t i = 0; i < 5; i++) {
		// has key state changed?
		if (isKeyCurrentlyPressed(key_pins[i]) == (!wasKeyAlreadyPressed(i))) {
			key_state[i] &= 1;     // clear all counter bits
			key_state[i] ^= 1;     // toggle state bit
			if (wasKeyAlreadyPressed(i)) { /* key has just been pressed */
				ret |= 1 << i;

				// check if down is pressed while up (drop) was already pressed
				// or vice versa
				if ((((1 << i) == KEY_DROP)
						&& isKeyCurrentlyPressed(KEY_DOWN_PIN))
						|| (((1 << i) == KEY_DOWN)
								&& isKeyCurrentlyPressed(KEY_DROP_PIN)))
					ret |= KEY_PAUSE;
			}
		} else if (wasKeyAlreadyPressed(i)) {
			// key is kept pressed. This will cause some repeat on some keys

			// increase counter value in bits 2..7, saturate counter
			if ((key_state[i] >> 2) < 63)
				key_state[i] = key_state[i] + 4;

			uint8_t counter = key_state[i] >> 2;

			// repeat for horizontal movement
			if (((1 << i) == KEY_LEFT) || ((1 << i) == KEY_RIGHT)) {
				// "DAS" delay of 24 Frames according to tetris concept
				// afterwards 1/9G
				if ((!(key_state[i] & 2) && (counter == 24))
						|| ((key_state[i] & 2) && (counter == 9) && (mode != 1))
						|| ((key_state[i] & 2) && (counter == 1) && (mode == 1))) {
					key_state[i] = 3;   // restart counter for continous repeat
					ret |= 1 << i;        // report key
				}
			}

			// mode = 0/1 = normal game mode
			if (mode != 2) {
				// "soft drop"
				if ((1 << i) == KEY_DOWN) {
					if (counter == 3) {    // 1/3G
						key_state[i] = 3; // restart counter for continous repeat
						ret |= 1 << i;        // report key
					}
				}
			} else {
				// key repear for "initials" enter dialog
				if (((1 << i) == KEY_DOWN) || ((1 << i) == KEY_DROP)) {
					if ((!(key_state[i] & 2) && (counter == 24))
							|| ((key_state[i] & 2) && (counter == 9))) {
						key_state[i] = 3; // restart counter for continous repeat
						ret |= 1 << i;        // report key
					}
				}
			}
		}
	}
	return ret;
}

