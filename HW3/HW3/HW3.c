#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/resets.h"

adc_init(); // init the adc module
adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
adc_select_input(0); // select to read from ADC0

volatile int analog_val;
volatile float volt_val;

int main()
{
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    gpio_put(14, true); // turn on led at the start
 
    while(gpio_get(15)){
        ; // do nothing until button is pressed
    }
    while(!gpio_get(15)){
        gpio_put(14, true); // turn on led only when button is pressed
    }
    

    while (1) {
        printf("Enter number of analog samples: (1-100) \n");
        int numsamps;
        scanf("%d", &numsamps);


        sleep_ms(50);
    }
}
