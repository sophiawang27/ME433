#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "display.h"

void drawMessage(int x, int y, char * m){
    int i = 0;
    while (m[i] != 0){
        drawLetter(x + i*5, y, m[i]); // can add spacing between letters
        i++;
    }
}

// void drawLetter(int x, int y, char c){ // c is ascii letter to draw
//     int j;
//     for (j=0; j<5; j++){
//         char col = ASCII[c-0x20][j]; // skipping non displayable characters
//         int i = 0;
//         for (i = 0; i<8; i++){
//             char bit = (col >> i)& 0b1;
//             ssd1306_drawPixel(x+j, y+i, bit);
//         }
//     }
// }