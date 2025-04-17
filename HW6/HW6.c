#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17

void setPin(unsigned char address, unsigned char register, unsigned char value);
unsigned char readPin(unsigned char address, unsigned char register);

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
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    while (true) {
        gpio_put(25,0);
        sleep_ms(100);
        gpio_put(25,1);
        sleep_ms(100);
    }
}

void i2c_setup(void){
    unsigned char dir = 0b0000001;
    i2c_write_blocking(i2c_default, address, dir, 1, false);// talk to i2c chip, give address

}


void setPin(unsigned char address, unsigned char register, unsigned char value){
    i2c_write_blocking(i2c_default, address, buf, 2, false);
    i2c_write_blocking(i2c_default, address, register, 2, false);
    i2c_write_blocking(i2c_default, address, value, 2, false);
    
}

unsigned char readPin(unsigned char address, unsigned char register){

}