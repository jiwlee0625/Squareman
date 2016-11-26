// Make a simple GBA state machine
// Name: Ji Won Lee
#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
//#include "text.h"
#include "title2.h"
#include "square.h"
#include "win.h"
#include "gameover.h"

#define speed	1
// State enum definition
enum GBAState {
	START,
	START_NODRAW,
	PLAY,
	PLAY_NODRAW,
	WIN,
	WIN_NODRAW,
	GAMEOVER,
	GAMEOVER_NODRAW
	// TODO add more states here!
};

int play() {
	SQUAREMAN hero = {0, 0, 0, 0};
	SQUAREMAN villain = {220, 140, 220, 140};
	int returnEndState = 0;
	drawRect(0, 0, 240, 160, WHITE);
	drawRect(villain.x, villain.y, SQUARE_WIDTH, SQUARE_HEIGHT, RED);
	drawImage3(hero.x, hero.y, SQUARE_WIDTH, SQUARE_HEIGHT, square);
	int villainXVelocity = -3;
	int villainYVelocity = -3;
	while (returnEndState == 0 && !KEY_DOWN_NOW(BUTTON_START)) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			returnEndState = 1;
		} 
		else {
			villain.prevX = villain.x;
			villain.prevY = villain.y;
			villain.x += villainXVelocity;
			villain.y += villainYVelocity;
			if ((villain.x <= 0) || ((villain.x + SQUARE_WIDTH) >= SCREEN_WIDTH - 1)) {
				villainXVelocity = villainXVelocity * -1;
				if (villain.x <= 0) {
					villain.x = 0;
				}
				if ((villain.x + SQUARE_WIDTH) >= SCREEN_WIDTH - 1) {
					villain.x = SCREEN_WIDTH - 1 - SQUARE_WIDTH;
				}
			}
			if ((villain.y <= 0) || ((villain.y + SQUARE_HEIGHT) >= SCREEN_HEIGHT)) {
				villainYVelocity = villainYVelocity * -1;
				if (villain.y <= 0) {
					villain.y = 0;
				}
				if ((villain.y + SQUARE_HEIGHT) >= SCREEN_HEIGHT) {
					villain.y = SCREEN_HEIGHT - SQUARE_HEIGHT;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_A)) {
				//check if it's all black/not white
				//if it is, win
				//if not, continue
				int checker = 0;
				int i = 0;
				while (checker == 0 && i < SCREEN_HEIGHT) {
					for (int j = 0; j < SCREEN_WIDTH; j++) {
						if (getPixel(j, i) == WHITE) {
							checker = 1;
						}
					}
					i++;
				}
				if (checker == 0) {
					returnEndState = 3;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_LEFT) && (hero.x > 0)) {
				hero.prevX = hero.x - 1;
				hero.x -= speed;
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT) && ((hero.x + SQUARE_WIDTH) < SCREEN_WIDTH)) {
				hero.prevX = hero.x;
				hero.x += speed;
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN) && (hero.y + SQUARE_HEIGHT) < SCREEN_HEIGHT) {
				hero.prevY = hero.y;
				hero.y += speed;
			}
			if (KEY_DOWN_NOW(BUTTON_UP) && (hero.y > 0)) {
				hero.prevY = hero.y - 1;
				hero.y -= speed;
			}
			//TODO check for collision
			drawRect(villain.prevX, villain.prevY, SQUARE_WIDTH, SQUARE_HEIGHT, BLACK);
			drawRect(villain.x, villain.y, SQUARE_WIDTH, SQUARE_HEIGHT, RED);
			drawRect(hero.prevX, hero.prevY, SQUARE_WIDTH, SQUARE_HEIGHT, BLACK);
			drawImage3(hero.x, hero.y, SQUARE_WIDTH, SQUARE_HEIGHT, square);
			if (hero.x < villain.x + SQUARE_WIDTH && hero.x + SQUARE_WIDTH > villain.x && hero.y < villain.y + SQUARE_HEIGHT && SQUARE_HEIGHT + hero.y > villain.y) {
    				// collision detected!

				returnEndState = 2;
			}
		}
		waitForVBlank();
	}
	return returnEndState;
}

int main() {

	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = START;
	// TODO add any additional variables here that you need
	// Hint: You probably need one to do accurate detection of pressing A without
	// rapidly cycling through all your states!
	int playEndState;
	while(1) {
		waitForVBlank();
		switch(state) {
		case START:
			drawImage3(0, 0, TITLE2_WIDTH, TITLE2_HEIGHT, title2);
			while(KEY_DOWN_NOW(BUTTON_START)) {}
			if (!KEY_DOWN_NOW(BUTTON_START)) {
				state = START_NODRAW;
			}
			break;
		case START_NODRAW:
			// TODO make it change to another state by pressing A
			if (KEY_DOWN_NOW(BUTTON_START)) {
				state = PLAY;
			}
			break;

		// TODO add more cases (states) to your switch statement!
		case PLAY:
			fillScreen3(BLACK);
			//drawString(4, 4, "This is the game.", WHITE);
			if (!KEY_DOWN_NOW(BUTTON_START)) {
				state = PLAY_NODRAW;
			}
			break;
		case PLAY_NODRAW:
			playEndState = play();
			if (playEndState == 1) {
				state = START;
			} else if (playEndState == 2) {
				state = GAMEOVER;
			} else if (playEndState == 3) {
				state = WIN;
			}
			break;
		case WIN:
			drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
			if (!KEY_DOWN_NOW(BUTTON_START)) {
				state = WIN_NODRAW;
			}
			break;
		case WIN_NODRAW:
			if (KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
			}
			break;
		case GAMEOVER:
			drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover);
			if (!KEY_DOWN_NOW(BUTTON_START)) {
				state = GAMEOVER_NODRAW;
			}
			break;
		case GAMEOVER_NODRAW:
			if (KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
			}
			break;
		}
		// TODO you should probably do something here with one of your variables
	}

	return 0;
}
