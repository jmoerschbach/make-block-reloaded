//
// tetris.h
//

#ifndef TETRIS_H
#define TETRIS_H

#include "../WallGame.h"
#include "../hmi/keys.h"


#define TITLE_SCROLL_SPEED      2
#define GAME_SCORE_SCROLL_SPEED 5   // frames@60Hz



#define SPEAKER_PIN      9
#define SPEAKER_PIN_2   10    // second channel option

#define NO_DROP              // use drop key as second rotate

#define EEPROM_MAGIC_MARKER 0x42

#define EEPROM_HIGHSCORE_ADDRESS EEPROM_ADDRESS_SPACE_TETRIS + 1
#define EEPROM_HIGHSCORE_MAGIC_MARKER_ADDRESS EEPROM_ADDRESS_SPACE_TETRIS + 0

#define ROWS_PER_LEVEL 10

void loopTetris();
void initTetris();
//const char bla[] PROGMEM ={0xff};
#endif // TETRIS_H
