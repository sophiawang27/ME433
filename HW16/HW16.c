#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define IN1_PIN 26
#define IN2_PIN 27
#define IN3_PIN 20
#define IN4_PIN 21

void set_dutycycle(int duty_cycle);

int main()
{
    stdio_init_all();
    gpio_init(IN1_PIN);
    gpio_set_dir(IN1_PIN, GPIO_OUT);
    gpio_init(IN2_PIN);
    gpio_set_dir(IN2_PIN, GPIO_FUNC_PWM);
    gpio_init(IN3_PIN);
    gpio_set_dir(IN3_PIN, GPIO_OUT);
    gpio_init(IN4_PIN);
    gpio_set_dir(IN4_PIN, GPIO_FUNC_PWM);

    gpio_put(IN1_PIN, 0);
    gpio_put(IN3_PIN, 0);
    uint slice_num2 = pwm_gpio_to_slice_num(IN2_PIN);
    float div = 3.0f;
    pwm_set_clkdiv(slice_num2, div);
    uint16_t wrap = 30000; // fixed PWM period
    pwm_set_wrap(slice_num2, wrap);
    pwm_set_enabled(slice_num2, true);
    pwm_set_gpio_level(IN2_PIN, wrap); // start with 0% duty
    // may need to add wrap for in4 for differential drive
    uint slice_num4 = pwm_gpio_to_slice_num(IN4_PIN);
    pwm_set_clkdiv(slice_num4, div);
    pwm_set_wrap(slice_num4, wrap);
    pwm_set_enabled(slice_num4, true);
    pwm_set_gpio_level(IN4_PIN, wrap); // start with 0% duty

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    float duty_cycle = 0.0;
    
    while (true) {
        char c;
        printf("Press '+' or '-' to edit duty cycle\n");
        scanf("%c\n", &c);
        if (c=='='){
            duty_cycle +=1;
            if (duty_cycle >100){
                duty_cycle = 100;
            }
            set_dutycycle(duty_cycle);
        }
        else if(c=='-'){
            duty_cycle -=1;
            if (duty_cycle <-100){
                duty_cycle = -100;
            }
            set_dutycycle(duty_cycle);
        }
        else{
            printf("Incorrect symbol input\n");
        }
        sleep_ms(100);
    }
}

void set_dutycycle(int duty_cycle){
    uint16_t wrap = 30000; // fixed PWM period
    if (duty_cycle>0){
        gpio_put(IN1_PIN, 1);
        pwm_set_gpio_level(IN2_PIN, wrap * ((100-duty_cycle) / 100.0));
        gpio_put(IN3_PIN, 1);
        pwm_set_gpio_level(IN4_PIN, wrap * ((100-duty_cycle) / 100.0));        
    }
    else if (duty_cycle<0){
        gpio_put(IN1_PIN, 0);
        pwm_set_gpio_level(IN2_PIN, wrap * ((-duty_cycle) / 100.0));
        gpio_put(IN3_PIN, 0);
        pwm_set_gpio_level(IN4_PIN, wrap * ((-duty_cycle) / 100.0));     
    }
    else{
        gpio_put(IN1_PIN, 1);
        pwm_set_gpio_level(IN2_PIN,wrap); //
        gpio_put(IN3_PIN, 1);        
        pwm_set_gpio_level(IN4_PIN, wrap); // 
    }

}
