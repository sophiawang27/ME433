#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "display.h"
#include "hardware/adc.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17

int main()
{
    stdio_init_all();

    // initialize ADC
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // initialize OLED display
    ssd1306_setup();
    ssd1306_clear();
    ssd1306_update();


    while (true) {
        char message[50];
        // get the start time
        unsigned int t1 = to_us_since_boot(get_absolute_time());  

        // get the adc vals and convert to voltage
        uint16_t adc_value = adc_read();// read adc value and store in adc_values
        float volt_value = adc_value*3.3/4095.0;// convert 12-bit number to float voltage value

        // clr and print the message
        ssd1306_clear();
        sprintf(message, "%f", volt_value);
        drawMessage(0,0, message); // keep drawing until you hit the null
        unsigned int t2 = to_us_since_boot(get_absolute_time());  
        unsigned int tdiff = t2 - t1; // process time in microseconds
        uint64_t fps = (uint64_t)(1/((float)(tdiff/1000000.0)));// turn time to frames per second
        sprintf(message, "%u fps", fps);
        drawMessage(0,20,message);// print to the bottom of the display
        ssd1306_update(); // final action
        sleep_ms(500);
    }
}
