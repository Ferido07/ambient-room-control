
/*----------------------------------------------------------------------------
    File name   : dac.c
    Description : implements a driver for the DAC module for performing 
	          Digital to Analog conversion. 

    Procesor    : Philips LPC2148 with ATM7TDMI-s core
    
    ECEg-4501 - Microcomputers and interfacing, lab exercise IV

    Notes:
           LPC2148 embeddes a 10-bit Digital to Analog Converter modules(DAC) using a resistor string architecture
           with a speed vs power optimization, bias, setting. the DAC has only one output at P0.25 (AOUT)
           The ADC resolution is 10-bit (i.e. digital value is from 0 - 1023 for analog output of (0 - Vref)
            
                   Analog equivalent = (digital input * vref)/(1024)

                vref = DAC reference voltage (see simulation schematic-- it's connected to 3.3V)
                
            (read more about the DAC Module-- chapter 20 on the manual (page 294)
 ----------------------------------------------------------------------------*/
#include "NXP/iolpc2148.h"

/*-------------------------------------------------------------------------------
Function:   Initializes the DAC module

Returns:    None

Arguments: None

Notes:      The DAC has only one register with two fields (VALUE and BIAS)
            a digital input to be converted is written to VALUE and 
            bias controls the power consumption Vs convertion speed option
---------------------------------------------------------------------------------*/
void lamp_dac_init()
{
   PINSEL1_bit.P0_25 = 2; //set pin P0.25 as analog output 
         
   DACR_bit.BIAS = 0;    //high speed conversion(1us).. max power consumption (700uA)
}

/*-----------------------------------------------------------------------------------------------
Function:   Converts the given digital input to analog and outputs on P0.25 (AOUT)
          
Returns:    Nothing

Arguments:  Digital input to be converted

Notes:      digital input must be in the range 0 - 1023V
            
            analog output = (digital value * vref)/1024, where vref = 3.3V -- see schematic
------------------------------------------------------------------------------------------------*/
void lamp_dac_out(unsigned short digital)
{
   int i;
   
   DACR_bit.VALUE = digital;  //write 10-bit digital value to be converted
   
   for(i=0; i<10; i++);       //let the analog output settle (1us bias setting)
}
   
   