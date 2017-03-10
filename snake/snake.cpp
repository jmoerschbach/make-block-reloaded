#include "snake.h"


void loopSnake() {
	for (uint8_t x = 0; x < W; x++) {
		for (uint8_t y = 0; y < H; y++) {
			LED(x,y)= CRGB::Blue;
			delay(100);
			LEDS.show();
		}
	}
}
