#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "helper.h"
#include "ssd1306.h"
#include "display.h"


// check that the address of the imu is correct
void check_address(void){
    printf("working");
    unsigned char reg = 0x75;
    unsigned char buf;
    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, &buf, 1, false);  // false - finished with bus
    printf("%c", buf);
    if (buf != 0x68){
        while(true){
            printf("Error connecting to IMU\n");
            sleep_ms(100);
        }
    }
}

// initialize the power saving, acceleration, and gyroscope configuration
void init_imu(void){
    printf("initializing,,,");
    char power = 0x00;
    i2c_write_blocking(i2c1, IMU_ADDR, &power, 1, true);
    printf("power done");
    char accel = 0x07;
    i2c_write_blocking(i2c1, IMU_ADDR, &accel, 1, true);
    printf("accel done");
    char gyro = 0x18;
    i2c_write_blocking(i2c1, IMU_ADDR, &gyro, 1, false);
    printf("finished initializing");
}


// read the value of an input on i2c (units of g)
uint16_t read_accel(int dir){
    unsigned char reg = 0x3B;
    unsigned char buf[14];

    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 14, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, buf, 14, false);  // false - finished with bus

    uint16_t x_accel = (buf[0]<<8)|(buf[1]);
    uint16_t y_accel = (buf[2]<<8)|(buf[3]);
    uint16_t z_accel = (buf[4]<<8)|(buf[5]);
    uint16_t accel[3];
    accel[0] = x_accel;
    accel[1] = y_accel;
    accel[2] = z_accel;
    uint16_t value =  0.000061*accel[dir];
    return value;
}

// use i2c_write_blocking and i2c_read_blocking
// 14 unsigned chars sequentially in one buffer
// left bitshift by 8 for high
// OR with the low
// save in signed 16-bit number
// multiply by precalculated numbers

// read the angular velocity (degrees per second)
uint16_t read_gyro(int dir){
    unsigned char reg = 0x3B;
    unsigned char buf[14];

    i2c_write_blocking(i2c1, IMU_ADDR, &reg, 14, true);  // true to keep master control of bus
    i2c_read_blocking(i2c1, IMU_ADDR, buf, 14, false);  // false - finished with bus

    uint16_t x_angle = (buf[8]<<8)|(buf[9]);
    uint16_t y_angle = (buf[10]<<8)|(buf[11]);
    uint16_t z_angle = (buf[12]<<8)|(buf[13]);
    uint16_t gyro[3];
    gyro[0] = x_angle;
    gyro[1] = y_angle;
    gyro[2] = z_angle;

    uint16_t value = 0.007630*gyro[dir];
    return value;
}


