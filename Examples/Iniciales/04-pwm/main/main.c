#include <stdio.h>
#include "comon.h"
#include "man_fade.h"

//const static char *TAG = "Ejemplo 04 - PWM";

const uint32_t LIMIT_LEDC = (1UL << RES_LEDC) - 1;

void app_main(void)
{
    init_ledc();

    while (1)
    {
        manual_fading();
    }
}
