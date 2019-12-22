#include "NXP/iolpc2148.h"

void fan_init(void){   
   IO0DIR_bit.P0_7 = 1;
   IO0CLR_bit.P0_7 = 1;  
}
void fan_start(void)
{
    IO0SET_bit.P0_7 = 1;
}
void fan_stop(void)
{
    IO0CLR_bit.P0_7 = 1;
}
