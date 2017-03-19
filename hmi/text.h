/*
 * text.h
 *
 *  Created on: 10.03.2017
 *      Author: Jonas
 */

#ifndef TET_TEXT_H_
#define TET_TEXT_H_
#include <FastLED.h>
uint8_t drawChar(char chr, int8_t x, int8_t y, int8_t skip, uint8_t len,
		CRGB c);
void text_str(const char *str, int8_t x, int8_t y, int8_t skip, int8_t len,
		CRGB c);
void text_scroll(const char *str, int16_t offset, int8_t x, uint8_t len,
		int8_t y, CRGB c);
int16_t text_str_len(const char *str);
uint8_t text_char_width(char chr);

#endif /* TET_TEXT_H_ */
