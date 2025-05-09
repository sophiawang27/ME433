#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

uint16_t adc_values[100];
float volt_values[100];

int main(void)
{
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0
    stdio_init_all();

    // initialize led and button as output and input
    gpio_init(14); // led
    gpio_set_dir(14, GPIO_OUT);
    gpio_init(15); // button
    gpio_set_dir(15, GPIO_IN);

    // wait for connection
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    gpio_put(14, true); // turn on led at the start
    
    while(gpio_get(15)){
        ; // do nothing until button is pressed
    }
    gpio_put(14, false); // turn off led only when button is pressed
    

    while (1) {
        printf("Enter number of analog samples: (1-100) \n");
        int numsamps;
        scanf("%d", &numsamps);
        for (int i=0; i<numsamps; i++){ // read the values
            adc_values[i] = adc_read();// read adc value and store in adc_values
            volt_values[i] = adc_values[i]*3.3/4095.0;// convert 12-bit number to float voltage value
            printf("Analog value: %u     ", adc_values[i]); // print the analog values
            printf("Voltage value: %f\r\n", volt_values[i]);// print voltage value
            sleep_ms(10);
        }

        sleep_ms(50);
    }
    return 0;
}
