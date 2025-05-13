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