#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9



int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // initialize OLED display
    ssd1306_setup();
    ssd1306_clear();
    ssd1306_update();


    while (true) {
        char message[50];
        ssd1306_clear(); // clear all bits
        sprintf(message, "hello");
        drawMessage(10, 20, message); // keep drawing until you hit the null


        ssd1306_update(); // final action
        sleep_ms(1000);
    }
}

void drawMessage(int x, int y, char * m){
    int i = 0;
    while (m[i] != 0){
        drawLetter(x + i*5, y, m[i]); // can add spacing between letters
        i++;
    }

}

void drawLetter(int x, int y, char c){ // c is ascii letter to draw
    int j;
    for (j=0; j<5; j++){
        char col = ASCII[c-0x20][j]; // skipping non displayable characters
        int i = 0;
        for (i = 0; i<8; i++){
            char bit = (col >> i)& 0b1;
            ssd1306_drawPixel(x+j, y+i, bit);
        }
    }
}