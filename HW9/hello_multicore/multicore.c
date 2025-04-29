/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"

#define FLAG_VALUE 123
#define LED_PIN 15

int data_01; // global var from core 0 to 1
int data_10; // global var from core 1 to 0

void core1_entry() {
/*
    multicore_fifo_push_blocking(FLAG_VALUE);

    uint32_t g = multicore_fifo_pop_blocking();

    if (g != FLAG_VALUE)
        printf("Hmm, that's not right on core 1!\n");
    else
        printf("Its all gone well on core 1!\n");
*/
    gpio_init(LED_PIN); // led
    gpio_set_dir(LED_PIN, GPIO_OUT);// init led output pin
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0// init the adc
    while (1){
        //tight_loop_contents();
        uint32_t g = multicore_fifo_pop_blocking(); // pop from core 0
        if (g==0){
            int adc_val = adc_read();// read the adc
            data_10 = adc_val;// put the adc value into data01
            multicore_fifo_push_blocking(0); // push a command to core 0
        }
        if (g==1){
            gpio_put(LED_PIN, 1);//turn on led
            multicore_fifo_push_blocking(1); // push a command to core 0
        }
        if (g==2){
            gpio_put(LED_PIN, 0);//turn off led
            multicore_fifo_push_blocking(2); // push a command to core 0
        }
    }
}

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Hello, multicore!\n");

    multicore_launch_core1(core1_entry);
/*
    multicore_fifo_push_blocking(FLAG_VALUE); // write
    // Wait for it to start up

    uint32_t g = multicore_fifo_pop_blocking(); // read

    if (g != FLAG_VALUE)
        printf("Hmm, that's not right on core 0!\n");
    else {
        multicore_fifo_push_blocking(FLAG_VALUE);
        printf("It's all gone well on core 0!\n");
    }
*/

    while (1){
        printf("0 - ADC Value, 1 - Turn on LED, 2 - Turn off LED\n");// print user instructions
        int message;
        scanf("%d", &message);// scanf to get the instruction
        printf("%d\n", message);//  printf the command back
        multicore_fifo_push_blocking(message);// push a command to core 1
        uint32_t g = multicore_fifo_pop_blocking(); // pop from core 1
        if (g==0){
            float voltage = data_10 *3.3/ 4095.0;
            printf("ADC Voltage: %f\n", voltage);// print the value depending on what you got from pop
        }
        if (g==1){
            printf("LED Turned On\n");
        }
        if (g==2){
            printf("LED Turned Off\n");
        }

    }

}
