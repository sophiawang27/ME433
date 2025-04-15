#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    stdio_init_all();
    sleep_ms(2000);
    volatile float f1, f2;
    printf("Enter two floats to use:");
    scanf("%f %f", &f1, &f2);
    volatile float f_add, f_sub, f_mult, f_div;

    absolute_time_t t1 = get_absolute_time();
    uint64_t start_time = to_us_since_boot(t1);
    for(int i=0;i<1000;i++){
        f_add = f1+f2;
    }
    absolute_time_t t2 = get_absolute_time();
    uint64_t end_time = to_us_since_boot(t2);
    int comp_time = (int)(((float)(end_time - start_time) / 1000.0) / 0.00667);

    printf("\nComputation time for floating point addition: %d\n", comp_time);

    absolute_time_t t1sub = get_absolute_time();
    uint64_t start_timesub = to_us_since_boot(t1sub);
    for(int i=0;i<1000;i++){
        f_sub = f1-f2;
    }
    absolute_time_t t2sub = get_absolute_time();
    uint64_t end_timesub = to_us_since_boot(t2sub);
    int comp_timesub = (int)(((float)(end_time - start_time) / 1000.0) / 0.00667);
    printf("\nComputation time for floating point subtraction: %d\n", comp_timesub);

    absolute_time_t t1mult = get_absolute_time();
    for(int i=0;i<1000;i++){
        f_mult = f1*f2;
    }
    absolute_time_t t2mult = get_absolute_time();
    uint64_t start_timemult = to_us_since_boot(t1mult);
    uint64_t end_timemult = to_us_since_boot(t2mult);
    int comp_timemult = (int)(((float)(end_time - start_time) / 1000.0) / 0.00667);
    printf("\nComputation time for floating point multiplication: %d\n", comp_timemult);

    absolute_time_t t1div = get_absolute_time();
    uint64_t start_timediv = to_us_since_boot(t1div);
    for(int i=0;i<1000;i++){
        f_div = f1/f2;
    }
    absolute_time_t t2div = get_absolute_time();
    uint64_t end_timediv = to_us_since_boot(t2div);
    int comp_timediv = (int)(((float)(end_time - start_time) / 1000.0) / 0.00667);
    printf("\nComputation time for floating point division: %d\n", comp_timediv);
    printf("\nResults: \n%f+%f=%f \n%f-%f=%f \n%f*%f=%f \n%f/%f=%f\n", f1,f2,f_add, f1,f2,f_sub, f1,f2,f_mult, f1,f2,f_div);

}
