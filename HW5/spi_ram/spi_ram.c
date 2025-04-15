#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void init_ram(void);
void ram_write(uint16_t a, float v);
float ram_read(uint16_t a);

union FloatInt {
    float f;
    uint32_t i;
};


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
    
    init_ram();

    for i=0 to 1000
        calculate v = sin(t)
        ram_write(address, v)
    
    while (true) {
        // read from one address
        float v = ram_read(address)
        // send the float to the DAC (copy in form HW4)
        sleep_ms(1);
    }
}


void init_ram(void){
    uint8_t buff[2]
    buff[0] = 0b00000101 // change status register
    buff[1] = 0b01000000 // to sequential mode
    cs low\
    spi_write_blocking(spi_default,buff,2);
    cs high
}

void ram_write(uint16_t a, float v){
    uint8_t buff[7];
    buff[0] = 0b00000010// send instruction
    buff[1] = a >> 8
    buff[2] = a & 0xFF// send address

    union FloatInt num;
    num.f = v;

    buff[3] = num.i>>24//float leftmost
    buff[4] = //float
    buff[5] = //float
    buff[6] = //float rightmost

    cs low\
    spi_write_blocking(spi_default,buff,7);
    cs high
}


float ram_read(uint16_t a){
    uint8_t out_buff[7];
    uint8_t in_buff[7];

    out_buff[0] = __mul_instruction
    out_buff[1] = address high byte
    out_buff[2] = address low byte 

    cs low\
    spi_write_read_blocking(spi_default, out_buff, in_buff, 7);
    cs high

    union FloatInt num;
    num.i = 0;
    num.i = in_buff[3]<<24 | in_buff[4]<<16 // needs more here

    return num.f
}