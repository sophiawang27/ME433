#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <math.h>

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void writeDAC(int channel, float volt);

// function to turn on and off CS
static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

// 
static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}


int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    while (true) {
        int i = 0;
        float t = 0.0;
        int up = 1; // increasing triangle wave = 1, decreasing = 0
        float max_val = 3.3; // max voltage
        float tri_wave=0;
        float inc = 1.65*2.0/50.0; // increment for triangle wave
        for(i=0; i<10000; i++){
            t = t+0.01;
            float v = 1.65*sin(4.0*M_PI*t) + 1.65; // 2Hz sine wave
            writeDAC(0,v);

            // make the triangle waveform
            if (up){ // the triangle wave is increasing
                tri_wave = tri_wave + inc;
                if (tri_wave > max_val){
                    tri_wave = max_val;
                    up = 0;
                }
            }

            if (!up){ // the triangle wave is decreasing
                tri_wave = tri_wave - inc;
                if (tri_wave < 0){
                    tri_wave = 0;
                    up = 1;
                }
            }

            writeDAC(1, tri_wave); // triangle wave
            sleep_ms(10); 
        }
    }
}

void writeDAC(int channel, float voltage){
    uint8_t data[2];
    int len = 2;
    // change the first four (leftmost) bits to represent channel and tie values high
    uint16_t d = 0;
    d = d | channel << 15;
    d = d | 0b111 << 12;
    // convert the float voltage to a 10-bit analog number
    uint16_t v = (uint16_t)(voltage * 1023/ 3.3);
    d = d | v << 2; // add this value to the data

    data[0] = d >> 8; // leftmost 8 bits
    data[1] = d & 0xFF;  // rightmost 8 bits
    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, len); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);
}