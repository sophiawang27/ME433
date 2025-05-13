#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "helper.h"
#include "ssd1306.h"
#include "display.h"


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 20
#define I2C_SCL 21



int main()
{
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    check_address();
    init_imu(); // turn on chip and config accel and gyro
    // check that the address is correct


    while (true) {
        uint16_t accel_x = read_accel(X_DIR);
        uint16_t accel_y = read_accel(Y_DIR);
        uint16_t accel_z = read_accel(Z_DIR);
        uint16_t gyro_x = read_gyro(X_DIR);
        uint16_t gyro_y = read_gyro(Y_DIR);
        uint16_t gyro_z = read_gyro(Z_DIR);
        printf("x = %u , y = %u , z = %u\n", accel_x, accel_y, accel_z);
        sleep_ms(100);
    }
}

// use i2c_write_blocking and i2c_read_blocking
// 14 unsigned chars sequentially in one buffer
// left bitshift by 8 for high
// OR with the low
// save in signed 16-bit number
// multiply by precalculated numbers

// temp 25 C

// draw line on OLED
// depending on tilt, change line length
// can separate components
