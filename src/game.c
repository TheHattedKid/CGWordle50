#include "game.h"
#include <string.h>
#include "keys.h"
#include "words.h"
#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdint.h>
#include <stdbool.h>
#include <gint/usb.h>
#include <gint/usb-ff-bulk.h>
#include <gint/bfile.h>
#include <gint/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <gint/rtc.h>

#define SQRE_SIZE 16
#define BRDR_S 2
void setHook(void) {
    dupdate_set_hook(GINT_CALL(usb_fxlink_videocapture, 0));
}
static void drawGame(uint16_t board[6][5], int tries) {
    int offX = ((DWIDTH) - (SQRE_SIZE * 5)) / 2;
    // bad
    int offY = 13;
    dclear(C_WHITE);
    for(int y = 0; y < 6; y++) {
        for(int x = 0; x < 5; x++) {
            char c = board[y][x] >> 8;
            uint8_t state = board[y][x] & 0xFF;
            // Draw da square
            color_t colorF =
                    state == 0 ? C_WHITE :
                    state == 1 ? C_BLACK :
                    state == 2 ? C_RGB(31, 31, 16) :
                    state == 3 ? C_RGB(16, 31, 16) :
                                 C_RED;
            color_t colorB =
                    state == 0 ? C_RGB(24, 24, 24) :
                    state == 1 ? C_BLACK :
                    state == 2 ? C_RGB(16, 16, 8) :
                    state == 3 ? C_RGB(8, 16, 8) :
                    C_RED;
            drect_border(
                    x * SQRE_SIZE + offX,
                    y * SQRE_SIZE + offY,
                    x * SQRE_SIZE + SQRE_SIZE + offX,
                    y * SQRE_SIZE + SQRE_SIZE + offY,
                    colorF,
                    BRDR_S,
                    colorB
            );
            // Draw char
            char str[2];
            str[0] = c;
            str[1] = 0;
            dtext(x * SQRE_SIZE + offX + 4, y * SQRE_SIZE + offY + 4, C_BLUE, str);
        }
    }
    dprint_opt(DWIDTH / 2, 4, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE, "CGWordle!");
    dupdate();
}
void setupGame(int id, int rench) {
    const char* word = (rench ? mots : words)[id];
	if(strlen(word) != 5) return;
	// Add a 5x6 board of "states", like in the original game
	// 0 = undecided
	// 1 = Not present in word (black)
	// 2 = Present but wrong location(yellow)
	// 3 = Present and at the right location(green)
	
	// I decided to pack the letter and state into the same 
	// variable
	// Format:
	// Top 8 bits = state
	// Bottom 8 bits = char
	uint16_t board[6][5];
	for(int y = 0; y < 6; y++) for(int x = 0; x < 5; x++) 
		board[y][x] = (' ' << 8) | 0; // be sure that the ENTIRE board is undecided
	
	int uniqueLetters = 0;
	// There can only be 5 unique letters at best, and since we're
	// dealing with the English language, 8 bits is enough to store 
	// the characters, and lets add an extra 8 bits for the count
	// (overly much but that's the min we can do)

	// Format:
	// Top 8 bits = char
	// Bottom 8 bits = count
	uint16_t count[5];
	for(int i = 0; i < 5; i++) {
		uint8_t c = word[i];
		uint8_t unique = 1;
		int j = 0;
		for(; j < uniqueLetters; j++) {
			if((count[j] >> 8 & 0xFF) == c) {
				unique = 0;
				break;
			}
		}
		if/* it is */(unique) {
			// Let's add our word
			// with count 1
			count[uniqueLetters++] = (c << 8) | 1;
			continue;
		}
		/* else */
		count[j]++; 	// Just increment it; there is
				        // a 0% probability for an overflow
				        // to happen
	}
	// Now that we've got a list of letters
	// let's actually begin
	int try = 0;
    char trybuffer[6];



    memset(trybuffer, 0, 6);
    int idx = 0;
    while(try < 6) {
        drawGame(board, try + 1);
        char c = keyAlphaToChar(getkey().key, 0);
        if(c == 1) {
            usb_interface_t const *intf[] = { &usb_ff_bulk, NULL };
            usb_open(&intf, GINT_CALL(setHook));
            continue;
        }
        // Add char to buffer
        if(strlen(trybuffer) < 5 && (c != 0 && c != '\xFF' && c != '\n')) {
            trybuffer[idx++] = c;
            // Write to grid
            board[try][idx - 1] = ((uint16_t) (c) << 8) | (board[try][idx - 1] & 0xFF);
        } else if (c == '\xFF' && idx > 0) {
            trybuffer[idx--] = 0;
            board[try][idx + 1] = (' ' << 8) | (board[try][idx + 1] & 0xFF);
        }
        // If the user pressed EXE, check if it's even long enough
        if(strlen(trybuffer) == 5 && c == '\n') {
            // Check if it's the right word
	    if(!strcmp(trybuffer, word)) {
            for(int i = 0; i < 5; i++) board[try][i] = (board[try][i] & 0xFF00) | 3;
            } else {
		        // Check if the word exists
                bool exists = false;
                for(int i = 0; i < (rench ? MOTS : WORD_COUNT); i++) {
		            if(!strcmp(trybuffer, (rench ? mots : (rench ? mots : words))[i])) {
                        exists = true;
		                // It does !
                        // Check the word
                        uint16_t count1[uniqueLetters];
                        memcpy(count1, count, uniqueLetters*sizeof(uint16_t));
                        for(int j = 0; j < 5; j++) {
                            if(word[j] == trybuffer[j]) {
                                board[try][j] = (board[try][j] & 0xFF00) | 3;
                                for(int k = 0; k < uniqueLetters; k++) {
                                    if((((count1[k] >> 8) & 0xFF) == trybuffer[j]) && ((count1[k] & 0xFF) > 0)) {
                                        count1[k]--;
                                    }
                                }
                            } else {
                                bool exi = 0;
				                for(int k = 0; k < uniqueLetters; k++) {
					                if((((count1[k] >> 8) & 0xFF) == trybuffer[j]) && ((count1[k] & 0xFF) > 0)) {
						                board[try][j] = (board[try][j] & 0xFF00) | 2;
						                count1[k]--;
						                exi = 1;
                                    }

				                }
				                if(!exi) board[try][j] = (board[try][j] & 0xFF00) | 1;
                            }
                        }
                        break;
		            }
		        }
                if(!exists) {
                    idx = 0;
                    for(int i = 0; i < 5; i++) board[try][i] = (' ' << 8);
                    memset(trybuffer, 0, 6);
                    continue;
                }
                try++;idx = 0;
		        memset(trybuffer, 0, 6);
            }
        }
    }
    dclear(C_WHITE);
    dtext(1, 1, C_BLACK, rench ? "T'as perdu ! Veux-tu réessayer ?" : "You lost! Wanna retry?");
    dtext(1, 11, C_BLACK, rench ? "Le mot etait: " : "The word was: ");
    dtext(1, 22, C_BLACK, word);
    dtext(1, 33, C_BLACK, rench ? "MENU=Non;Autre=OUI" : "MENU=NO;Other=YES");
    dupdate();
    key_event_t k = getkey();
}
