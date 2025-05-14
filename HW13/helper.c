#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "helper.h"
#include "ssd1306.h"
#include "display.h"


// check that the address of the imu is correct
void check_address(void){
    unsigned char reg = 0x75;
    unsigned char buf;
    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, &buf, 1, false);  // false - finished with bus
    if (buf != 0x68){
        while(true){
            printf("Error connecting to IMU\n");
            sleep_ms(100);
        }
    }
}

// initialize the power saving, acceleration, and gyroscope configuration
void init_imu(void){
    char power[2]; 
    power[0] = PWR_MGMT_1;
    power[1] = 0x00;
    i2c_write_blocking(i2c1, IMU_ADDR, power, 2, true);
    char accel[2];
    accel[0] = ACCEL_CONFIG;
    accel[1] = 0x07;
    i2c_write_blocking(i2c1, IMU_ADDR, accel, 1, true);
    char gyro[2];
    gyro[0] = GYRO_CONFIG;
    gyro[1] = 0x18;
    i2c_write_blocking(i2c1, IMU_ADDR, gyro, 1, false);
}


// read the value of an input on i2c (units of g)
float read_accel(int dir){
    unsigned char reg = 0x3B;
    unsigned char buf[14];

    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, buf, 14, false);  // false - finished with bus

    int16_t x_accel = (buf[0] << 8) | buf[1];
    int16_t y_accel = (buf[2] << 8) | buf[3];
    int16_t z_accel = (buf[4] << 8) | buf[5];

    float accel[3];
    accel[0] = 0.000061 * x_accel;
    accel[1] = 0.000061 * y_accel;
    accel[2] = 0.000061 * z_accel;
    return accel[dir];
}

// use i2c_write_blocking and i2c_read_blocking
// 14 unsigned chars sequentially in one buffer
// left bitshift by 8 for high
// OR with the low
// save in signed 16-bit number
// multiply by precalculated numbers

// read the angular velocity (degrees per second)
float read_gyro(int dir){
    unsigned char reg = 0x3B;
    unsigned char buf[14];

    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, buf, 14, false);  // false - finished with bus
    float gyro[3];

    int16_t x_gyro = (buf[8]<<8)|(buf[9]);;
    int16_t y_gyro = (buf[10]<<8)|(buf[11]);
    int16_t z_gyro = (buf[12]<<8)|(buf[13]);
    gyro[0] = 0.007630*x_gyro;
    gyro[1] = 0.007630*y_gyro;
    gyro[2] = 0.007630*z_gyro;
    return gyro[dir];
}


// draw line on OLED
// depending on tilt, change line length
// can separate components
// draw line in x direction for x acceleration
void drawLine_x(float x_accel){

}

// draw line in y dir for y acceleration
void drawLine_y(float y_accel){

}