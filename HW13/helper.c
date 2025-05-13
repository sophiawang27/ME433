#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "helper.h"

char check_address(void){
    char buf;
    i2c_read_blocking(i2c1, WHO_AM_I, &buf, 1, false);  // false - finished with bus
    return buf;
}

void init_imu(void){
    char power = 0x00;
    i2c_write_blocking(i2c1, PWR_MGMT_1, &power, 1, false);
    char accel = 0x07;
    i2c_write_blocking(i2c1, ACCEL_CONFIG, &accel, 1, false);
    char gyro = 0x18;
    i2c_write_blocking(i2c1, GYRO_CONFIG, &gyro, 1, false);
}


// read the value of an input on i2c
uint16_t read_accel(unsigned char address){
    unsigned char reg = 0x09;
    unsigned char buf[2];


    i2c_write_blocking(i2c1, address, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, address, &buf, 1, false);  // false - finished with bus
    uint16_t gyro = (buf[0]<<8)|(buf[1]);
    return gyro;
}
// use i2c_write_blocking and i2c_read_blocking
// 14 unsigned chars sequentially in one buffer
// left bitshift by 8 for high
// OR with the low
// save in signed 16-bit number
// multiply by precalculated numbers


uint16_t read_gyro(unsigned char address){
    unsigned char buf[2];


    uint16_t gyro = (buf[0]<<8)|(buf[1]);
    return gyro;
}


// draw line on OLED
// depending on tilt, change line length
// can separate components
