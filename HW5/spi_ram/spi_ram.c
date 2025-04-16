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
#define RAM_CS 13

void init_ram(void); // initialize the ram (sequential operation)
void ram_write(uint16_t a, float v); // write from pico to ram
float ram_read(uint16_t a); // read from ram to DAC
void writeDAC(int channel, float voltage);

// function to turn on and off CS
static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

// function to turn off cs
static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}

// create new var type
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
    gpio_set_dir(RAM_CS, GPIO_OUT);
    gpio_put(RAM_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
    
    init_ram(); // initialize sequential operation

    volatile uint16_t address = 0b0;
    volatile float t = 0.0;
    for (int i=0; i<1000; i++){
        t += 0.01; // incrementing time by 0.1
        float v = 1.65*sin(2.0*M_PI*t) + 1.65;
        ram_write(address, v);
        address += 4;
    }
    
    while (true) {
        volatile uint16_t address_read = 0b0;
        float voltage = ram_read(address_read); // read from one address
        writeDAC(0, voltage);// send the float to the DAC (copy in form HW4)
        printf("%f", voltage);
        sleep_ms(250); // delay one ms
        address += 4;

    }
}

// initialize the ram chip with sequential mode
void init_ram(void){
    uint8_t buff[2];
    buff[0] = 0b00000101; // change status register
    buff[1] = 0b01000000; // to sequential mode
    cs_select(RAM_CS);
    spi_write_blocking(spi_default,buff,2);
    cs_deselect(RAM_CS);
}

// write to ram
void ram_write(uint16_t a, float v){
    uint8_t buff[7];
    buff[0] = 0b00000010; // send instruction to write
    buff[1] = a >> 8;
    buff[2] = a & 0xFF;// send address

    union FloatInt num;
    num.f = v;

    buff[3] = num.i>>24; //float leftmost
    buff[4] = num.i>>16; //float
    buff[5] = num.i>>8; //float
    buff[6] = num.i; //float rightmost

    cs_select(RAM_CS);
    spi_write_blocking(spi_default,buff,7);
    cs_deselect(RAM_CS);
}

// read from ram
float ram_read(uint16_t a){
    uint8_t out_buff[7];
    uint8_t in_buff[7];

    // only the instructions and address matter
    out_buff[0] = 0b00000011; // send instruction to read
    out_buff[1] = a >> 8;
    out_buff[2] = a & 0xFF;// send address

    cs_select(RAM_CS);
    spi_write_read_blocking(spi_default, out_buff, in_buff, 7);
    cs_deselect(RAM_CS);

    union FloatInt num;
    num.i = 0;
    num.i = in_buff[3]<<24 | in_buff[4]<<16 | in_buff[5]<<8 | in_buff[6]; // bitshifting to put the input float into an int32

    return num.f;
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