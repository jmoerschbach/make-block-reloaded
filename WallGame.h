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
#define NUM_LEDS ((H*W))
extern CRGB leds[NUM_LEDS];

// enable LED_SERPENTINE if the stripe wiring used alternating directions
//#define LED_SERPENTINE
#ifdef LED_SERPENTINE
#define LED(x,y)  leds[H-((x&1)?(H-(y)-1):(y))-1 + H*(x)]
#else
#define LED(x,y)  leds[H-(y)-1+H*(x)]
#endif


#define LED_DATA_PIN    12
#endif /* WALLGAME_H_ */
