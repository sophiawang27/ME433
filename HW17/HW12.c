#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
// #include "ov7670.h"
#include "cam.h"

static float M=100.0; // maximum duty
static float S=1.0; // slope of drop off
static float W=10.0; // width of deadband

void LED_controller(int com);
uint8_t green_row[80];

int main()
{
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Hello, camera!\n");

    init_camera_pins();
    gpio_init(LEDL_PIN);
    gpio_init(LEDR_PIN);
    gpio_set_function(LEDL_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LEDR_PIN, GPIO_FUNC_PWM);

    uint slice_num2 = pwm_gpio_to_slice_num(LEDL_PIN);
    float div = 1.0f;
    pwm_set_clkdiv(slice_num2, div);
    uint16_t wrap = 6250; // fixed PWM period 20kHz freq
    pwm_set_wrap(slice_num2, wrap);
    pwm_set_enabled(slice_num2, true);
    pwm_set_gpio_level(LEDL_PIN, 0); // start with 0% duty
    // may need to add wrap for in4 for differential drive
    uint slice_num4 = pwm_gpio_to_slice_num(LEDR_PIN);
    pwm_set_clkdiv(slice_num4, div);
    pwm_set_wrap(slice_num4, wrap);
    pwm_set_enabled(slice_num4, true);
    pwm_set_gpio_level(LEDR_PIN, 0); // start with 0% duty
 
    while (true) {
        char m[10];
        scanf("%s",m);
        setSaveImage(1);
        while(getSaveImage()==1){}
        convertImage();
        int com = findLine(IMAGESIZEY/2); // calculate the position of the center of the ine
        setPixel(IMAGESIZEY/2,com,0,255,0); // draw the center so you can see it in python
        //printImage();
        printf("%d\r\n",com); // comment this when testing with python
        // LED controller
        // first get one row of the image
        for (int i=0; i<80; i++){
             green_row[i] = picture.g[29,i];
             //printf("%u\n", green_row[i]);
        }
        LED_controller(com);
        //printImage();
        sleep_ms(100);
    }
}


// LED PWM (green)
void LED_controller(int com){
    uint16_t wrap = 6250; // fixed PWM period (20kHz freq)

    // // calculating the threshold
    // int sum_green=0;
    // for (int i=0; i<80; i++){ // find the average value for the threshold
    //     sum_green += green_row[i];
    // }
    // int avg_green = sum_green / 80;
    // volatile float threshold = (float)(avg_green) * 1.2;

    // // loop thru to determine the location of the com
    // int bin_vals[80];
    // int sum_bin_vals;
    // int sum_vals;
    // for (int j=0; j<80; j++){
    //     if (green_row[j]<threshold){// compare each value to the threshold, store into new array of binary vals (0=no green, 1=green)
    //         bin_vals[j] = 0;
    //     }
    //     else{
    //         bin_vals[j] = 1;
    //     }
    //     sum_vals += (bin_vals[j])*j;
    //     sum_bin_vals += bin_vals[j];
    // }
    // use the binary values to find location of com
    // int com = sum_vals / sum_bin_vals; 
    
    // relationship between com of white line and the centerline of the camera
    int P = com-40;

    // establishing duty cycles depending on the relationship above, using 3 gains (M, S, W)
    // the minimum duty cycle for movement is around 67% for the motors
    float duty_left;
    float duty_right;
    if (P < 0){     // p is too low (negative), need to turn left, higher right duty, lower left duty
        if (P <= -(W/2)){ // left side of the graph
            duty_left = S*-P;
            duty_right = M;
        }
        if (P > -(W/2)){ // middle left portion of the graph
            duty_left = M;
            duty_right = M;
        }

    }
    else if (P > 0){     // p too high (positive), need to turn right, lower right duty, higher left duty
        if (P >= (W/2)){ // right side of the graph
            duty_left = M;
            duty_right = (S*-P*(W/2)) + M; // negative slope
        }
        if (P < (W/2)){  // middle right portion of graph
            duty_left = M;
            duty_right = M;
        }
    }
    else{ // P is dead center, go fast
        duty_left = M;
        duty_right = M;
        
    }
    if (duty_left >100.0){ // set range for duty cycles
        duty_left = 100.0;
    }
    else if (duty_left < 0.0){
        duty_left = 0.0;
    }
    if (duty_right>100.0){
        duty_right = 100.0;
    }
    else if (duty_right < 0.0){
        duty_right = 0.0;
    }
    pwm_set_gpio_level(LEDL_PIN, wrap*((duty_left) / 100.0)); 
    pwm_set_gpio_level(LEDR_PIN, wrap*((duty_right) / 100.0));

}
