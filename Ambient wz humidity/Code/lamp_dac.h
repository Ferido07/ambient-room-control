
/*--------------------------------------------------------------
 File:      dac.h
 Purpose:   Header file for DAC driver
 Compiler:  IAR EW 5.5
 System:    Philips LPC2148 MCU with ARM7TDMI-s core
----------------------------------------------------------------*/

//function prototypes
void lamp_dac_init();
void lamp_dac_out(unsigned short digital);