/*
 * keys.h
 *
 *  Created on: 10.03.2017
 *      Author: Jonas
 */

#ifndef TET_KEYS_H_
#define TET_KEYS_H_

void initKeys();
void lockKeys();
bool wasDropPressed();
bool wasDownPressed();
bool wasLeftPressed();
bool wasRightPressed();
bool wasRotatePressed();
bool wasPausePressed();
bool wasAnyKeyPressed();
void pollKeyStatus();

#endif /* TET_KEYS_H_ */
