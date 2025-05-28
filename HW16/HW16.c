#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define IN1_PIN 26
#define IN2_PIN 27
#define IN3_PIN 20
#define IN4_PIN 21
// left is in1 and in2
// right is in3 and in4

void set_dutycycle(float duty_cycle);

int main()
{
    stdio_init_all();
    gpio_init(IN1_PIN);
    gpio_set_function(IN1_PIN, GPIO_FUNC_PWM);
    gpio_init(IN2_PIN);
    gpio_set_function(IN2_PIN, GPIO_FUNC_PWM);
    gpio_init(IN3_PIN);
    gpio_set_function(IN3_PIN, GPIO_FUNC_PWM);
    gpio_init(IN4_PIN);
    gpio_set_function(IN4_PIN, GPIO_FUNC_PWM);


    uint slice_num2 = pwm_gpio_to_slice_num(IN2_PIN);
    float div = 1.0f;
    pwm_set_clkdiv(slice_num2, div);
    uint16_t wrap = 6250; // fixed PWM period 20kHz freq
    pwm_set_wrap(slice_num2, wrap);
    pwm_set_enabled(slice_num2, true);
    pwm_set_gpio_level(IN2_PIN, 0); // start with 0% duty
    // may need to add wrap for in4 for differential drive
    uint slice_num4 = pwm_gpio_to_slice_num(IN4_PIN);
    pwm_set_clkdiv(slice_num4, div);
    pwm_set_wrap(slice_num4, wrap);
    pwm_set_enabled(slice_num4, true);
    pwm_set_gpio_level(IN4_PIN, 0); // start with 0% duty
    uint slice_num1 = pwm_gpio_to_slice_num(IN1_PIN);
    pwm_set_clkdiv(slice_num1, div);
    pwm_set_wrap(slice_num1, wrap);
    pwm_set_enabled(slice_num1, true);
    pwm_set_gpio_level(IN1_PIN, 0); // start with 0% duty
    uint slice_num3 = pwm_gpio_to_slice_num(IN3_PIN);
    pwm_set_clkdiv(slice_num3, div);
    pwm_set_wrap(slice_num3, wrap);
    pwm_set_enabled(slice_num3, true);
    pwm_set_gpio_level(IN3_PIN, 0); // start with 0% duty

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    float duty_cycle = 0.0;

    while (true) {
        char c;
        set_dutycycle(90);
        // printf("Press '+' or '-' to edit duty cycle\n");
        // scanf("%c\n", &c);
        // if (c=='='){ // using = instead of + because of the shift button
        //     duty_cycle +=1.0;
        //     if (duty_cycle >100){
        //         duty_cycle = 100;
        //     }
        //     set_dutycycle(duty_cycle);
        //     printf("Duty cycle: %f\n", duty_cycle);
        // }
        // else if(c=='-'){
        //     duty_cycle -=1.0;
        //     if (duty_cycle <-100){
        //         duty_cycle = -100;
        //     }
        //     set_dutycycle(duty_cycle);
        //     printf("Duty cycle: %f\n", duty_cycle);
        // }
        // else{
        //     printf("Incorrect symbol input\n");
        // }
        sleep_ms(100);
    }
}

void set_dutycycle(float duty_cycle){
    uint16_t wrap = 6250; // fixed PWM period (20kHz freq)
    if (duty_cycle>0.0){
        pwm_set_gpio_level(IN1_PIN, 0);
        pwm_set_gpio_level(IN2_PIN, wrap*((duty_cycle) / 100.0)); //requires 67% to start moving
        pwm_set_gpio_level(IN3_PIN, wrap*((duty_cycle) / 100.0));
        pwm_set_gpio_level(IN4_PIN, 0);
      
    }
    else if (duty_cycle<0.0){

        pwm_set_gpio_level(IN1_PIN, wrap*((-duty_cycle) / 100.0)); // requires -66% to start moving
        pwm_set_gpio_level(IN2_PIN, 0);
        pwm_set_gpio_level(IN3_PIN, 0);
        pwm_set_gpio_level(IN4_PIN, wrap*((-duty_cycle) / 100.0)); 
    }
    else{
        pwm_set_gpio_level(IN1_PIN, 0);
        pwm_set_gpio_level(IN2_PIN, 0);
        pwm_set_gpio_level(IN3_PIN, 0);
        pwm_set_gpio_level(IN2_PIN, 0);
    }

}
