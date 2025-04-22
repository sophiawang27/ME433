#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "hardware/adc.h"

void drawMessage(int x, int y, char * m);
void drawLetter(int x, int y, char c);