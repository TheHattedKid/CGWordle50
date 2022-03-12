//
// Created by hatkid on 05/02/2022.
//

#include "keys.h"
#include <gint/keycodes.h>
#include <gint/display.h>
char keyAlphaToChar(int key, int caps) {
    switch (key) {
        case 0x61:
            return caps ? 'A' : 'a';
        case 0x62:
            return caps ? 'B' : 'b';
        case 0x63:
            return caps ? 'C' : 'c';
        case 0x64:
            return caps ? 'D' : 'd';
        case 0x65:
            return caps ? 'E' : 'e';
        case 0x66:
            return caps ? 'F' : 'f';

        case 0x51:
            return caps ? 'G' : 'g';
        case 0x52:
            return caps ? 'H' : 'h';
        case 0x53:
            return caps ? 'I' : 'i';
        case 0x54:
            return caps ? 'J' : 'j';
        case 0x55:
            return caps ? 'K' : 'k';
        case 0x56:
            return caps ? 'L' : 'l';

        case 0x41:
            return caps ? 'M' : 'm';
        case 0x42:
            return caps ? 'N' : 'n';
        case 0x43:
            return caps ? 'O' : 'o';

        case 0x31:
            return caps ? 'P' : 'p';
        case 0x32:
            return caps ? 'Q' : 'q';
        case 0x33:
            return caps ? 'R' : 'r';
        case 0x34:
            return caps ? 'S' : 's';
        case 0x35:
            return caps ? 'T' : 't';

        case 0x21:
            return caps ? 'U' : 'u';
        case 0x22:
            return caps ? 'V' : 'v';
        case 0x23:
            return caps ? 'W' : 'w';
        case 0x24:
            return caps ? 'X' : 'x';
        case 0x25:
            return caps ? 'Y' : 'y';

        case 0x11:
            return caps ? 'Z' : 'z';
        case 0x15:
            return '\n';
        case KEY_F6:
            return 1;
    }
    return 0;
}