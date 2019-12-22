#include "NXP/iolpc2148.h"

void lamp_init(void){
    IO0DIR_bit.P0_31 = 1;
    IO0CLR_bit.P0_31 = 1;
}

void lamp_on(void){
  if(IO0SET_bit.P0_31==0)
    IO0SET_bit.P0_31 = 1;
}

void lamp_off(void){
    IO0CLR_bit.P0_31 = 1;
}