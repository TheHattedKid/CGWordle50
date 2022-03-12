#include <gint/display.h>
#include <gint/rtc.h>
#include <gint/keyboard.h>
#include "game.h"
#include "words.h"
#include <stdlib.h>
#include <gint/bfile.h>
int main(void) {
    extern bopti_image_t intro;

    dclear(C_WHITE);
    dimage(0, 0, &intro);
    dupdate();
    key_event_t k;
    int rancais = 0;
    while((k = getkey()).key != KEY_EXE) {

        if(k.key == KEY_OPTN) {
            dclear(C_WHITE);
            dprint_opt(DWIDTH / 2, 1, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_TOP,
                       "CGWordle infos:");
            dtext(1, 11, C_BLACK, "This add-in is NOT official, don't think");
            dtext(1, 22, C_BLACK, "it has been made by the guys behind Wordle");

            dtext(1, 33, C_BLACK, "Font used for welcome screen: ");
            dtext(1, 44, C_BLACK, "Curse Casual(https://www.dafont.com/fr/curse-casual-jve.font) ");
            dtext(1, 55, C_BLACK, "No modifications were done to the font.");

            dtext(1, 77, C_BLACK, "Made by L'Oieducode(A.K.A) ");
            dtext(1, 88, C_BLACK, "DevHONK, The Hatted Kid, Goose etc.");

            dtext(1, 99, C_BLACK, "Also if you seriously want ");
            dtext(1, 111, C_BLACK, "Also if you seriously want to see my bad code, ");
            dtext(1, 122, C_BLACK, "here's the GitHub repo: ");
            dtext(1, 133, C_BLACK, "https://github.com/TheHattedKid/CGWordle50");

            dtext(1, 155, C_BLACK, "Also thanks to Lephe' for");
            dtext(1, 166, C_BLACK, "his amazing work on Gint !");

            dtext(1, 188, C_BLACK, "Thanks to Gusgo for the array of english words!");
            dupdate();
            getkey();
        } else if(k.key == KEY_F1 && k.alpha && k.mod && (!k.shift)) {
            dclear(C_BLUE);
            dprint_opt(DWIDTH / 2, DHEIGHT / 2, C_BLACK, C_NONE, DTEXT_CENTER, DTEXT_MIDDLE,
                       "Mode Rancais active(sans accents)!");
            rancais = 1;
            dupdate();
            getkey();
        }
        dclear(C_WHITE);
        dimage(0, 0, &intro);
        dupdate();
    }

    rtc_time_t t;
	rtc_get_time(&t);
	srand(rtc_ticks());
	while(1) {
        setupGame(rand() % (rancais ? MOTS : WORD_COUNT), rancais);
    }
}
