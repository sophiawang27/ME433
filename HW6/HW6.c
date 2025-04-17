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
// init onboard led
int pico_led_init(void) {
    #if defined(PICO_LED_GP14)
        // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
        // so we can use normal GPIO functionality to turn the led on and off
        gpio_init(PICO_LED_GP14);
        gpio_set_dir(PICO_LED_GP14, GPIO_OUT);
        return PICO_OK;
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        // For Pico W devices we need to initialise the driver etc
        return cyw43_arch_init();
    #endif
    }
    
// Turn the led on or off
void pico_set_led(bool led_on) {
    #if defined(PICO_LED_GP14)
        // Just set the GPIO on or off
        gpio_put(PICO_LED_GP14, led_on);
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        // Ask the wifi "driver" to set the GPIO on or off
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}

int main()
{
    stdio_init_all();
    pico_led_init();
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
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

void setPin(unsigned char address, unsigned char register, unsigned char value){}
unsigned char readPin(unsigned char address, unsigned char register){}