#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

adc_init(void); // init the adc module
adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
adc_select_input(0); // select to read from ADC0


uint16_t adc_values[100];
volatile float voltage;

int main(void)
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
        for (int i=0; i<numsamps; i++){
        uint16_t adc_val = adc_read();// read adc value and store in adc_val
        printf("12-bit value: %u", adc_val); // print the value 
        }
        // convert 12-bit number to float voltage value
        // print voltage value

        sleep_ms(50);
    }
    return 0;
}
