#include "NXP/iolpc2148.h"

void heater_init(void){
    IO0DIR_bit.P0_15 = 1;
    IO0CLR_bit.P0_15 = 1;
}

void heater_on(void){
  if(IO0SET_bit.P0_15==0)
    IO0SET_bit.P0_15 = 1;
}

void heater_off(void){
    IO0CLR_bit.P0_15 = 1;
}