#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "display.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17

void drawMessage(int x, int y, char * m);
void drawLetter(int x, int y, char c);

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
        // get the start time
        unsigned int t1 = to_us_since_boot(get_absolute_time());  

        // clr and print the message
        ssd1306_clear();
        sprintf(message, "i <3 u");
        drawMessage(0,0, message); // keep drawing until you hit the null
        unsigned int t2 = to_us_since_boot(get_absolute_time());  
        unsigned int tdiff = t2 - t1; // process time in microseconds
        uint64_t fps = (uint64_t)(1/(tdiff*1000000));// turn time to frames per second
        sprintf(message, "%u", fps);
        drawMessage(0,20,message);// print to the bottom of the display
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