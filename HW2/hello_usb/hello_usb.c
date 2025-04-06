/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#ifndef PICO_LED_GP14
#define PICO_LED_GP14 14
#endif

#ifndef PICO_BUTTON_GP15
#define PICO_BUTTON_GP15 15
#endif

volatile int counter = 0;

// Perform initialisation
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

// initialize the button to count button presses
void pico_button_init(void){
#if defined(PICO_BUTTON_GP15)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
        gpio_init(PICO_BUTTON_GP15);
        gpio_set_dir(PICO_BUTTON_GP15, GPIO_IN);
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

// get the value from the input button
int pico_get_button(void) {
    #if defined(PICO_BUTTON_GP15)
        // Just set the GPIO on or off
        int button_val = gpio_get(PICO_BUTTON_GP15);
    #endif
    return button_val;
    }


int main() {
    stdio_init_all();
    int rc = pico_led_init();
    pico_button_init();
    hard_assert(rc == PICO_OK);
    while (true) {
        printf("Button pressed %d times.\n", counter);
        while (!pico_get_button()){
            ; // do nothing
        }
        
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
        sleep_ms(50);
    }
}
