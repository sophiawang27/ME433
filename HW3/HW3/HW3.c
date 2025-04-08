#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/resets.h"

adc_init(); // init the adc module
adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
adc_select_input(0); // select to read from ADC0
int main()
{
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    gpio_put(14, true); // turn on led at the start
 
    while (1) {
        while(gpio_get(15)){
            ; // do nothing until button is pressed
        }
        while(!gpio_get(15)){
            gpio_put(14, true); // turn on led when button is pressed
        }
        char message[100];
        scanf("%s", message);
        printf("message: %s\r\n",message);
        sleep_ms(50);
    }
}
