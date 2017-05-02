/* 
 tetris.ino

 Real tetris for the 15*20 make:block
 */

#include "tetris.h"

#include <EEPROM.h>
#include <FastLED.h>

#include "Arduino.h"

#include "config.h"
#include "initials.h"
#include "score.h"
#include "../hmi/text.h"
#include "title.h"

#define INIT_LEVEL 0

#define GAME_X     0
#define GAME_Y     1

#define PREVIEW_X  12
#define PREVIEW_Y  (H-2)

#define SCORE_X 11
#define SCORE_Y  1
#define SCORE_W  4
#define SCORE_H  5

#define LEVEL_X  11
#define LEVEL_Y   8

#define GAME_IS_FINISHED 1
#define GAME_IS_RUNNING 0

#define EMPTY_BLOCK 0

// possible game states
typedef enum {
	STATE_CONFIG, STATE_TITLE, STATE_GAME, STATE_SCORE, STATE_INITIALS,
} tetris_state_t;

tetris_state_t gameState;

// caution: this does not check for boundaries
void drawFilledRectangle(int8_t x, int8_t y, uint8_t w, uint8_t h, CRGB c) {
	for (int8_t i = x; i < x + w; i++)
		for (int8_t j = y; j < y + h; j++)
			LED(i,j)= c;
		}

// ------- game engine

// colors according to "tetris company standard"
// up to 16 colors possible with this engine
static const uint32_t tetromino_colors[] = { 0x202020, 0x00ffff, 0xffa500,
		0xffff00,   // empty, I, L, O
		0x4040ff, 0xff0000, 0x00ff00, 0xc000c0,   // J, Z, S, T
		0xffffff, 0x000000                        // highlight, closed
		};

/* mapping of tetrominos under all four angles */
//#Tetrominos, #Angles, #Blocks per Tetromino, #Coordinates
static const int8_t tetrominos[][4][4][2] PROGMEM = { //
		{ //
				{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } }, /* cyan    */
				{ { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } }, /*  #*##   */
				{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } }, /*         */
				{ { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 } } /*         */
				}, //
				{ { { -1, 1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } }, /* orange  */
				{ { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } }, /*  #*#    */
				{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, -1 } }, /*  #      */
				{ { 0, -1 }, { 0, 0 }, { 0, 1 }, { 1, 1 } } /*         */
				}, //
				{ { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, /* yellow  */
				{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, /*   *#    */
				{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, /*   ##    */
				{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } } /*         */
				}, //
				{ { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 } }, /* blue    */
				{ { 0, -1 }, { 0, 0 }, { 0, 1 }, { -1, 1 } }, /*  #*#    */
				{ { -1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } }, /*    #    */
				{ { 1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } } /*         */
				}, //
				{ { { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } }, /* red     */
				{ { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } }, /*  #*     */
				{ { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } }, /*   ##    */
				{ { 0, -1 }, { 0, 0 }, { -1, 0 }, { -1, 1 } } /*         */
				}, //
				{ { { 1, 0 }, { 0, 0 }, { 0, 1 }, { -1, 1 } }, /* green   */
				{ { -1, -1 }, { -1, 0 }, { 0, 0 }, { 0, 1 } }, /*   *#    */
				{ { 1, 0 }, { 0, 0 }, { 0, 1 }, { -1, 1 } }, /*  ##     */
				{ { -1, -1 }, { -1, 0 }, { 0, 0 }, { 0, 1 } } /*         */
				}, //
				{ { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 } }, /* purple  */
				{ { 0, -1 }, { 0, 0 }, { -1, 0 }, { 0, 1 } }, /*  #*#    */
				{ { -1, 0 }, { 0, 0 }, { 0, -1 }, { 1, 0 } }, /*   #     */
				{ { 0, -1 }, { 0, 0 }, { 1, 0 }, { 0, 1 } } /*         */
				} //
		};

// standard tetris size as seen on gameboy
#define GAME_W  10
#define GAME_H  18

// ------------------- the entire in-game state ------------

// two blocks are saved per byte -> 90 bytes
uint8_t game_area[GAME_W / 2][GAME_H];

struct tetromino_S {
	uint8_t x, y;
	uint8_t rot, type, next;
} tetromino;

uint8_t gravityStepCounter;
uint8_t currentLevel;
uint32_t currentScore;
uint8_t numberOfRemovedRowsInCurrentLevel;
/** is increased when player moves tetromino down manually -> added to score*/
uint8_t continousDropCounter;
/** the highscore is saved in EEPROM*/
uint32_t highScore;

/** each bit encodes one row: Bit 0 set: row 0 is completed -> up to 32 rows are supported, we need only 18*/
uint32_t tableOfFullRows;
uint8_t row_remove_timer;
uint32_t next_event;

uint8_t getLevelRate() {
	// speed table. taken from gameboy version. values in 60Hz steps
	static const uint8_t step_cnt_table[] PROGMEM =
	// 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
			{ 53, 49, 45, 41, 37, 33, 28, 22, 17, 11, 10, 9, 8, 7, 6, 6, 5, 5,
					4, 4, 3 };

	return pgm_read_byte(
			step_cnt_table + ((currentLevel < 20) ? currentLevel : 20));
}

// show level
// level: 0-9 = green digit 0-9, 10-19 = yellow digit 0-9, >= 20 = red X
void drawLevel() {
	drawFilledRectangle(LEVEL_X, LEVEL_Y, 3, 5, CRGB::Black);
	if (currentLevel < 10)
		drawChar('0' + currentLevel, LEVEL_X, LEVEL_Y, 0, 3, CRGB(0x00ff00));
	else if (currentLevel < 20)
		drawChar('0' + currentLevel - 10, LEVEL_X, LEVEL_Y, 0, 3,
				CRGB(0xffff00));
	else
		drawChar('X', LEVEL_X, LEVEL_Y, 0, 3, CRGB(0xff0000));
}

void setTetrominoBlock(uint8_t x, uint8_t y, uint8_t colorIndex) {
	// we should never draw outside the game area
	if ((x >= GAME_W) || (y >= GAME_H))
		return;

	//we store 2 tetrominos per byte (one tetromino needs a nibble)
	if (x & 1)
		game_area[x / 2][y] = (game_area[x / 2][y] & 0x0f) | (colorIndex << 4);
	else
		game_area[x / 2][y] = (game_area[x / 2][y] & 0xf0)
				| (colorIndex & 0x0f);
}

uint8_t getTetrominoBlock(uint8_t x, uint8_t y) {
	// return 1 for all blocks outside game area so these seem to be
	// "occupied" to the game engine. Return 0 for one row above
	// game area to ease implementation of scrolling down
	if ((x >= GAME_W) || (y > GAME_H))
		return 1;
	if (y == GAME_H)
		return 0;

	if (x & 1)
		return game_area[x / 2][y] >> 4;
	return game_area[x / 2][y] & 0x0f;
}

void drawTetromino(bool show) {
	// get pointer to current tetromino shape at current angle
	int8_t const (*p)[2] = tetrominos[tetromino.type][tetromino.rot];
	// set all four blocks a tetromino consists of
	for (uint8_t i = 0; i < 4; i++) {

		setTetrominoBlock(tetromino.x + pgm_read_byte(&p[i][0]),
				tetromino.y - pgm_read_byte(&p[i][1]),
				show ? tetromino.type + 1 : 0); //+ 1 because colorArray[0] == Background color, colorArray[1] == color of first tetromino type
	}
}

char isCurrentTetrominoDrawable() {
	// get pointer to current tetromino shape at current angle
	int8_t const (*p)[2] = tetrominos[tetromino.type][tetromino.rot];

	// check all four blocks a tetromino consists of
	for (uint8_t i = 0; i < 4; i++)
		if (getTetrominoBlock(tetromino.x + pgm_read_byte(&p[i][0]),
				tetromino.y - pgm_read_byte(&p[i][1])) != EMPTY_BLOCK)
			return false;

	return true;
}

void erasePreviewArea() {
	for (uint8_t y = 0; y < 2; y++)
		for (uint8_t x = 0; x < 4; x++)
			LED(PREVIEW_X+x-1, PREVIEW_Y-y)= CRGB::Black;
		}

void showPreviewOfNextTetromino() {
	const int8_t (*p)[2] = tetrominos[tetromino.next][0];
	for (uint8_t i = 0; i < 4; i++)
		LED(PREVIEW_X+(int8_t)pgm_read_byte(&p[i][0]),
				(PREVIEW_Y-(int8_t)pgm_read_byte(&p[i][1])))= CRGB(tetromino_colors[tetromino.next + 1]);
	}

void createNewTetromino() {
	tetromino.rot = 0;
	tetromino.type = tetromino.next;
	tetromino.next = random(0, 7);
	tetromino.x = 4;
	// on gameboy the tetrominos spawn one row below top
	// game area ...
	tetromino.y = GAME_H - 2;
}

void finishGame() {
	// set current tetromino to 0xff indicating that
	// no game is in progress anymore
	tetromino.type = 0xff;
	row_remove_timer = 0;
	// remove level indicator
	drawFilledRectangle(LEVEL_X, LEVEL_Y, 3, 5, CRGB::Black);
	// remove score
	drawFilledRectangle(SCORE_X, SCORE_Y, SCORE_W, SCORE_H, CRGB::Black);
}

void createAndShowNewTetromino() {
	createNewTetromino();
	erasePreviewArea();

	if (isCurrentTetrominoDrawable()) {
		drawTetromino(true);
		showPreviewOfNextTetromino();
	} else {
		finishGame();
	}
}

// move tetromino in the given direction or return false
// if impossible
bool moveTetrominoIfPossible(int8_t x, int8_t y, int8_t rot) {
	bool couldBeMoved = true;

	// remove current tetromino
	drawTetromino(false);

	// advance tetromino
	tetromino.x += x;
	tetromino.y += y;
	tetromino.rot = (tetromino.rot + rot) & 3;

	// and check if it could be drawn
	if (!isCurrentTetrominoDrawable()) {
		// restore old position
		tetromino.x -= x;
		tetromino.y -= y;
		tetromino.rot = (tetromino.rot - rot) & 3;
		couldBeMoved = false;
	}

	drawTetromino(true);
	return couldBeMoved;
}

void checkForCompletedRows() {
	for (uint8_t y = 0; y < GAME_H; y++) {
		// a row is full if no block is empty
		bool isRowComplete = true;
		for (uint8_t x = 0; x < GAME_W; x++)
			if (!getTetrominoBlock(x, y))
				isRowComplete = false;
		if (isRowComplete) {
			// trigger removal of that row
			tableOfFullRows |= (1 << y);
			// line clear take 90 frames according to
			// http://tetrisconcept.net/wiki/Tetris_%28Game_Boy%29
			row_remove_timer = 90;
		}
	}
}
bool isAtLeastOneRowCompleted() {
	return tableOfFullRows > 0;
}
void lockTetromino() {
	// lock keys so the have to be released before auto repeat kicks in again
	lockKeys();

	// any manual drop before placement gives one extra point
	currentScore += continousDropCounter;
	continousDropCounter = 0;

	checkForCompletedRows();
	// no row removed: spawn new tetromino immediately
	if (!isAtLeastOneRowCompleted())
		createAndShowNewTetromino();
}

void loadHighScore() {

	if (EEPROM.read(
	EEPROM_HIGHSCORE_MAGIC_MARKER_ADDRESS) == EEPROM_MAGIC_MARKER)
		EEPROM.get(EEPROM_HIGHSCORE_ADDRESS, highScore);
	else {
		highScore = 0; // no high score yet
		EEPROM.write(EEPROM_HIGHSCORE_MAGIC_MARKER_ADDRESS,
		EEPROM_MAGIC_MARKER);
		EEPROM.put(EEPROM_HIGHSCORE_ADDRESS, highScore);
	}
}

void initializeGameArea() {
	for (uint8_t x = 0; x < GAME_W / 2; x++)
		for (uint8_t y = 0; y < GAME_H; y++)
			game_area[x][y] = EMPTY_BLOCK;
}

void initGame() {
	LEDS.clear();
	LEDS.setBrightness(config_get_brightness());

	initializeGameArea();

	tableOfFullRows = 0;  // no row being removed
	currentLevel = INIT_LEVEL;
	numberOfRemovedRowsInCurrentLevel = 0;
	currentScore = 0;
	continousDropCounter = 0;
	gravityStepCounter = getLevelRate();

	loadHighScore();

	drawLevel();

	// for some reason the first call to random always returns 0 ...
	tetromino.next = random(0, 7);
	tetromino.next = random(0, 7);
	createAndShowNewTetromino();
}

#define PULSE_STEPS  60

void drawScore() {

	static uint8_t pulse_cnt;
	static uint32_t cur_score = 0;
	static char score_str[7] = "0";
	static uint8_t score_len = 3;
	CRGB color = CRGB::White;

	// let score "pulse" if hi score was exceeded
	if (currentScore <= highScore) {
		pulse_cnt = 0;
		color = CRGB::White;
	} else {
		uint8_t shade;
		pulse_cnt++;
		if (pulse_cnt < PULSE_STEPS)
			shade = 255 * pulse_cnt / PULSE_STEPS;
		else
			shade = 255 * (2 * PULSE_STEPS - pulse_cnt) / PULSE_STEPS;

		color = CRGB(shade, 255 - shade, 255);

		if (pulse_cnt == 2 * PULSE_STEPS - 1)
			pulse_cnt = 0;
	}

	// update score string if necessary
	if (currentScore != cur_score) {
		ltoa(currentScore, score_str, 10);
		cur_score = currentScore;
		score_len = text_str_len(score_str);
	}

	// scroll score
	static int8_t score_scroll = 0, sub_score_scroll =
	GAME_SCORE_SCROLL_SPEED;
	if (sub_score_scroll == 0) {
		drawFilledRectangle(SCORE_X, SCORE_Y, SCORE_W, SCORE_H, CRGB::Black);
		// if only one digit: don't scroll at all
		// otherwise only scroll up to the last digit and stay
		// there for a moment
		text_scroll(score_str,
				(score_len == 3) ? 0 :
				(score_scroll > score_len - 3) ? score_len - 3 : score_scroll,
				SCORE_X, SCORE_W, SCORE_Y, color);
		score_scroll++;
		if (score_scroll == score_len + 20)
			score_scroll = -5;

		sub_score_scroll = GAME_SCORE_SCROLL_SPEED;
	} else
		sub_score_scroll--;
}

bool isGameFinished() {
	return tetromino.type == 0xff;
}

void saveHighScore() {
	if (currentScore > highScore)
		EEPROM.put(EEPROM_HIGHSCORE_ADDRESS, currentScore);
}

void calculateNewScore(uint8_t numberOfRowsRemoved) {
	static const uint8_t score_step[] = { 4, 10, 30, 120 };
	currentScore += 10l * score_step[numberOfRowsRemoved - 1]
			* (currentLevel + 1);
	if (currentScore > 999999)
		currentScore = 999999;
}

bool isRowComplete(uint8_t y) {
	return tableOfFullRows & (1 << y);
}

void shiftAboveLinesDown(uint8_t y) {
	uint8_t k = y;
	// shift all lines above down one line
	while (k < GAME_H) {
		for (uint8_t x = 0; x < GAME_W; x++)
			setTetrominoBlock(x, k, getTetrominoBlock(x, k + 1));
		k++;
	}
	// also shift table of full rows down
	tableOfFullRows = (tableOfFullRows & ~(1 << y)) >> 1;

}

void increaseLevel() {
	numberOfRemovedRowsInCurrentLevel++;
	if (numberOfRemovedRowsInCurrentLevel == ROWS_PER_LEVEL) {
		numberOfRemovedRowsInCurrentLevel = 0;
		currentLevel++;
		drawLevel();
	}
}

uint8_t removeRows() {
	uint8_t numberOfRowsRemoved = 0;
	// finally remove the full rows
	for (uint8_t y = 0; y < GAME_H; y++) {
		if (isRowComplete(y)) {
			shiftAboveLinesDown(y);
			numberOfRowsRemoved++;
			increaseLevel();

			y--; // check same row again
		}
	}

	return numberOfRowsRemoved;
}

void advanceTetrominoByGravity() {
	// advance tetromino by gravity
	if ((!isGameFinished()) && (--gravityStepCounter == 0)) {
		if (!moveTetrominoIfPossible(0, -1, 0)) {
			lockTetromino();
		} else {
			// clear "continous drop counter" if the tetromino drops by gravity
			continousDropCounter = 0;
		}
		gravityStepCounter = getLevelRate();
	}
}

void advanceTetrominoManually() {
	// advance tetromino manually
	int8_t x = 0, y = 0, rot = 0;
	if (wasLeftPressed())
		x--;

	if (wasRightPressed())
		x++;

	if (wasRotatePressed())
		rot = -1;

	if (wasDownPressed())
		y = -1;

	// drop acts like rotate
	if (wasDropPressed())
		rot = -1;

	// do manual movement
	if (x || rot)
		moveTetrominoIfPossible(x, 0, rot);

	// y movement needs to be handles seperately since only
	// this will cause the tetromino to lock
	if (y) {
		if (moveTetrominoIfPossible(0, y, 0)) {
			gravityStepCounter = getLevelRate();
			continousDropCounter++;
		} else
			lockTetromino();
	}
}

void drawGameArea() {
	// blit game_area to screen
	for (uint8_t y = 0; y < GAME_H; y++) {
		if (isRowComplete(y) && (row_remove_timer & 16)) {
			for (uint8_t x = 0; x < GAME_W; x++) {
				LED(x+GAME_X,GAME_Y+y)= CRGB::White;
			}
		} else {
			for (uint8_t x = 0;x < GAME_W;x++) {
				LED(x+GAME_X,GAME_Y+y) = CRGB(tetromino_colors[getTetrominoBlock(x, y)]);
			}
		}
	}
}

uint8_t runTetris() {

	if (isGameFinished()) {
		if (row_remove_timer <= GAME_H) {
			for (uint8_t x = 0; x < GAME_W; x++) {

				setTetrominoBlock(x, row_remove_timer - 1, 9);
				setTetrominoBlock(x, row_remove_timer, 8);
			}
			row_remove_timer++;
		} else {
			saveHighScore();
			return GAME_IS_FINISHED;
		}
	} else if (isAtLeastOneRowCompleted()) {
		// row removal is in progress
		continousDropCounter = 0;
		row_remove_timer--;

		if (row_remove_timer == 0) {
			uint8_t numberOfRowsRemoved = removeRows();
			calculateNewScore(numberOfRowsRemoved);
			tableOfFullRows = 0;
			createAndShowNewTetromino();
		}
	} else {

		advanceTetrominoManually();
		advanceTetrominoByGravity();
	}

	drawGameArea();

	drawScore();

	return GAME_IS_RUNNING;
}

void pauseGame() {
}

void loopTetris() {
	// frame time hasn't elapsed yet?
	// the following will also work when millis() wraps (after 49 days :-)
//	if ((long) (next_event - millis()) > 0) {
//		return;
//	}

	switch (gameState) {
	case STATE_CONFIG:
		if (config_process()) {
			title_init();
			gameState = STATE_TITLE;
		}
		break;

	case STATE_TITLE:
		if (title_process()) {
			initGame();
			gameState = STATE_GAME;
		}
		break;

	case STATE_GAME:
		//song_process(currentLevel + 1);
		if (runTetris() == GAME_IS_FINISHED) {
			if (currentScore > highScore) {
				initials_init(currentScore);
				gameState = STATE_INITIALS;
			} else {
				score_init(currentScore, currentScore > highScore);
				gameState = STATE_SCORE;
			}
			//song_process(0);
		}

		break;

	case STATE_SCORE:
		switch (score_process()) {
		case 1:
			// user pressed a key -> jump directly into
			// next game
			initGame();
			gameState = STATE_GAME;
			break;

		case 2:
			// timeout, jump to title screen
			title_init();
			gameState = STATE_TITLE;
			break;
		}
		break;

	case STATE_INITIALS:
		if (initials_process()) {
			title_init();
			gameState = STATE_TITLE;
		}
		break;
	default:
		break;
	}

	//LEDS.show();

	//next_event = millis() + GAME_CYCLE;
}

void initTetris() {
	pinMode(SPEAKER_PIN, OUTPUT);

// init game cycle counter
	next_event = millis() + GAME_CYCLE;

	loadConfiguration();

//key press at startup -> configuration setup
	if (wasAnyKeyPressed()) {
		config_init();
		gameState = STATE_CONFIG;
	} else {
		// normal startup
		title_init();
		gameState = STATE_TITLE;

	}

	//song_init();
}
