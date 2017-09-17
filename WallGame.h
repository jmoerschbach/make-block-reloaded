/*
 * WallGame.h
 *
 *  Created on: 08.03.2017
 *      Author: Jonas
 */

#ifndef WALLGAME_H_
#define WALLGAME_H_
#include <FastLED.h>
// total display size
#define H 20
#define W 16
#define FPS 60   // 60Hz

#define GAME_CYCLE (1000/FPS)
#define ENABLE_DEBUG
#define NUM_LEDS ((H*W))
extern CRGB leds[NUM_LEDS];

// enable LED_SERPENTINE if the stripe wiring used alternating directions
//#define LED_SERPENTINE
#ifdef LED_SERPENTINE
#define LED(x,y)  leds[H-((x&1)?(H-(y)-1):(y))-1 + H*(x)]
#else
#define LED(x,y)  leds[H-(y)-1+H*(x)]
#endif
// possible game states
enum WallGameState {
	STATE_TETRIS, STATE_SNAKE, STATE_LIGHT_BULB, STATE_MENU, STATE_LED_TEST
};
#ifdef ENABLE_DEBUG
#define DEBUG(msg) Serial.println(msg);
#else
#define DEBUG(msg) ;
#endif
#define LED_DATA_PIN    21
#define EEPROM_ADDRESS_SPACE_TETRIS 100
#define EEPROM_ADDRESS_SPACE_BULB 200
#endif /* WALLGAME_H_ */
