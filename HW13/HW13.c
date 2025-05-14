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
#define I2C_SDA 18
#define I2C_SCL 19



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

    check_address();     // check that the address is correct
    init_imu(); // turn on chip and config accel and gyro


    while (true) {
        float accel_x = read_accel(X_DIR);
        float accel_y = read_accel(Y_DIR);
        float accel_z = read_accel(Z_DIR);
        float gyro_x = read_gyro(X_DIR);
        float gyro_y = read_gyro(Y_DIR);
        float gyro_z = read_gyro(Z_DIR);
        printf("x = %f , y = %f , z = %f\n", accel_x, accel_y, accel_z); // print accels
        sleep_ms(100);
    }
}

// draw line on OLED
// depending on tilt, change line length
// can separate components
