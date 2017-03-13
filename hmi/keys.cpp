#include <Arduino.h>
#include <Wii.h>
#include <usbhub.h>
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
//WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wiimote - you only have to do this once
WII Wii(&Btd); // After that you can simply create the instance like so and then press any button on the Wiimote

void initKeys() {

	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1)
			; //halt
	}
	Serial.print(F("\r\nWiimote Bluetooth Library Started"));
}

// lock the auto repeat
void lockKeys() {

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



bool wasUpPressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(UP);
	return false;
}
bool wasRotatePressed() {
	return wasUpPressed();
}
bool wasPausePressed() {
	if (Wii.wiiUProControllerConnected)
		return Wii.getButtonClick(HOME);
	return false;
}

bool wasAnyKeyPressed() {
	return wasLeftPressed() || wasRightPressed() || wasDropPressed()
			|| wasRotatePressed();
}

void pollKeyStatus() {
	Usb.Task();
}

