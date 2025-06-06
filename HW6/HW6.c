#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17
#define ADDRESS 0x20

void setPin(unsigned char address, unsigned char register, unsigned char value);
unsigned char readPin(unsigned char address, unsigned char register);
void i2c_setup(void);

int main()
{
    stdio_init_all();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT); // init onboard led
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    i2c_setup();
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
    while (true) {

        setPin(ADDRESS, 0x0A, 0b00000000);
        while(readPin(ADDRESS, 0x09)){
            ; // do nothing
        }
        setPin(ADDRESS, 0x0A, 0b10000000);
        while(readPin(ADDRESS, 0x09) == 127){
        }

    }
}

// turn on communication w address and gpio pins on extender
void i2c_setup(void){
    unsigned char buff[2];
    buff[0] =  0x00; // sfr iodir
    buff[1] = 0b01111111; // value to turn gp0 input gp7 output
    i2c_write_blocking(I2C_PORT, ADDRESS, buff, 2, false);// talk to i2c chip

}

// write the value of an output on i2c
void setPin(unsigned char address, unsigned char register, unsigned char value){
    unsigned char buff[2];
    buff[0] = 0x0A; // olat sfr
    buff[1] = value;
    i2c_write_blocking(I2C_PORT, address, buff, 2, false);// talk to i2c chip
    
}

// read the value of an input on i2c
unsigned char readPin(unsigned char address, unsigned char register){
    unsigned char reg = 0x09;
    unsigned char buf;
    i2c_write_blocking(I2C_PORT, address, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(I2C_PORT, address, &buf, 1, false);  // false - finished with bus
    return buf;
}