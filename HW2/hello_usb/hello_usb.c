/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

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

volatile int counter = 0; // button presses
volatile int state = 0; // 0 =off 1=on led
static char event_str[128];
#define GPIO_WATCH_PIN 15 // set GP15 as button to watch

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

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
        // Put the GPIO event(s) that just happened into event_str
        // so we can print it
        gpio_event_string(event_str, events);
        printf("GPIO %d %s\n", gpio, event_str);
        if (state){ // if led is on: turn off
            pico_set_led(false);
            sleep_ms(LED_DELAY_MS);
            state = 0;
        }
        else if(state == 0){ // if led is off: turn on 
            pico_set_led(true);
            sleep_ms(LED_DELAY_MS);
            state = 1;
        }
        counter ++;// add one to the counter
        printf("Button pressed %d times.\n", counter);// print button presses
}

int main() {
    stdio_init_all();
    int rc = pico_led_init();
    pico_button_init();
    hard_assert(rc == PICO_OK);
    gpio_init(GPIO_WATCH_PIN);
    gpio_set_irq_enabled_with_callback(GPIO_WATCH_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    while (true) {}
}

static const char *gpio_irq_str[] = {
    "LEVEL_LOW",  // 0x1
    "LEVEL_HIGH", // 0x2
    "EDGE_FALL",  // 0x4
    "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
for (uint i = 0; i < 4; i++) {
    uint mask = (1 << i);
    if (events & mask) {
        // Copy this event string into the user string
        const char *event_str = gpio_irq_str[i];
        while (*event_str != '\0') {
            *buf++ = *event_str++;
        }
        events &= ~mask;

        // If more events add ", "
        if (events) {
            *buf++ = ',';
            *buf++ = ' ';
        }
    }
}
*buf++ = '\0';
}