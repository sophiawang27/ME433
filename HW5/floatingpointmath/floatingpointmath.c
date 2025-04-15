#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    stdio_init_all();
    volatile float f1, f2;
    printf("Enter two floats to use:");
    scanf("%f %f", &f1, &f2);
    volatile float f_add, f_sub, f_mult, f_div;

    absolute_time_t t1 = get_absolute_time();
    for(int i=0;i<1000;i++){
        f_add = f1+f2;
    }
    absolute_time_t t2 = get_absolute_time();
    uint64_t start_time = to_us_since_boot(t1);
    uint64_t end_time = to_us_since_boot(t2);
    uint64_t comp_time = ((end_time - start_time)/1000)/0.00667;

    printf("\nComputation time for floating point addition: %llu\n", comp_time);

    absolute_time_t t1sub = get_absolute_time();
    for(int i=0;i<1000;i++){
        f_sub = f1-f2;
    }
    absolute_time_t t2sub = get_absolute_time();
    uint64_t start_timesub = to_us_since_boot(t1);
    uint64_t end_timesub = to_us_since_boot(t2);
    uint64_t comp_timesub = ((end_timesub - start_timesub)/1000)/0.00667;
    printf("\nComputation time for floating point subtraction: %llu\n", comp_timesub);

    absolute_time_t t1mult = get_absolute_time();
    for(int i=0;i<1000;i++){
        f_mult = f1*f2;
    }
    absolute_time_t t2mult = get_absolute_time();
    uint64_t start_timemult = to_us_since_boot(t1);
    uint64_t end_timemult = to_us_since_boot(t2);
    uint64_t comp_timemult = ((end_timemult - start_timemult)/1000)/0.00667;
    printf("\nComputation time for floating point multiplication: %llu\n", comp_timemult);

    absolute_time_t t1div = get_absolute_time();
    for(int i=0;i<1000;i++){
        f_div = f1/f2;
    }
    absolute_time_t t2div = get_absolute_time();
    uint64_t start_timediv = to_us_since_boot(t1);
    uint64_t end_timediv = to_us_since_boot(t2);
    uint64_t comp_timediv = ((end_timediv - start_timediv)/1000)/0.00667;
    printf("\nComputation time for floating point division: %llu\n", comp_timediv);
    printf("\nResults: \n%f+%f=%f \n%f-%f=%f \n%f*%f=%f \n%f/%f=%f\n", f1,f2,f_add, f1,f2,f_sub, f1,f2,f_mult, f1,f2,f_div);
    while (true) {

    }
}
