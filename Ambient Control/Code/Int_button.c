
/*----------------------------------------------------------------------------
    File name   : Int_button.c
    Description : demonstrates on how to interface a button as an external interupt
                  source to wake up the processor from power-down modes

    Procesor    : ARM7TDMI-S on LPC2103 MCU
    
  ECEg-4501 - Microcomputers and interfacing, lab exercise II
 ----------------------------------------------------------------------------*/

#include "NXP/iolpc2138.h"
#include "interrupts.h"
#include "uart.h"
#include "temp_adc.h"
#include "light_adc.h"
#include "lamp_dac.h"

void button_ISR(void);    //function prototype - see definition below
unsigned short temp;
int light;
int digital_light;



/*-------------------------------------------------------------------------
   Function Name: button_init

   Parameters: None
 
   Return:  None
 
   Description: Initializes the button as falling edge triggered
                external interrupt source on the MCU
                (refer cha-4 "System control" - 4.5 External interrupt inputs)
 ---------------------------------------------------------------------------*/
void button_init(void)
{
   PINSEL0_bit.P0_14 = 0x2;  //select external interrupt function on pin P0.14 (EINT 1)
  
   EXTINT = 0x0F;        //clear any pending external interrupt requests
   
   EXTMODE = 0x02;       //external interrupt 1 is edge triggered
  
   EXTPOLAR = 0x00;      //and falling edge is selected
  
   EXTWAKE = 0x02;       //configure external interrupt on P0.14 to wake up the MCU from sleep
  
   install_FIQ(VIC_EINT1, button_ISR); //Install Interrupt as FIQ
}

/*-------------------------------------------------------------------------
   Function Name: button_ISR

   Parameters: None
 
   Return    :  None
 
   Description: An interrupt service routine (ISR) to handle the external interrupt
                request by the button
 ---------------------------------------------------------------------------*/
#pragma section = ".fiqisr"      //locate code below on the FIQ mode stack

void button_ISR(void) @ ".fiqisr"
{  
   /* clear the interrupt flag; future interrupts won't be recognized otherwise
      (try untill the flag is reset --- ) */
   while(EXTINT_bit.EINT1)
   {
      EXTINT_bit.EINT1 = 1;      
   }
  
   send_string("To set an optimum temprature press 't' or 'T'.\r");
   send_string("To set an optimum light press 'l' or 'L'.\r");
   send_string("To set an optimum moisture press 'm' or 'M'\r");
   send_string("Press 'd' or 'D' if done setting up the values.\r");
   send_string("---------------------------------------------------------------------\r\n");
    
   char choice= get_char();
   while(choice!='d' && choice!='D'){
      
      
      if(choice=='t' || choice=='T'){
          //temprature selection
          send_string("\r\nInput the temprature value you want in degree celsius and press enter\r\n");
          temp=get_value();  //was classic misake of declaring local variable but fixed now
          
          send_string("\n\r The chosen temprature: ");
          print_int(temp);
          send_string("C \r\n");
          send_string("Temprature is set continue to set up or press 'd' or 'D' if done.\r\n ");
          
      }
      
      else if (choice=='l' || choice=='L'){
           //light control using lamp or curtain goes here
            send_string("Input the light intensity value you want in microWatt per centimeter square(uW/cm2).\n\r");
            send_string("Note light intensity of the lamp in the range 0-200uW/cm2.\r\n");
            //note this system assumes the maximum light intensity of the lamp to be 200uW/cm2.
            // 
            light=get_value();
           
            send_string("\n\r The chosen light intensity: ");
            print_int(light);
            
            
            send_string("uW/cm2 \r\n");
            send_string("Light is set continue to set up or press 'd' or 'D' if done.\r\n ");
            
            digital_light=light*468;
            digital_light/=100;
            
            print_int(digital_light); 
            
            lamp_dac_out(digital_light);
                
      }
      
      else if (choice=='m'||choice=='M'){
          //moisture control goes here
          send_string("To do moisture control\n");
      } 
      
      else if (choice=='d' || choice=='D')break;      
      
      else send_string(" Invalid key! \r\n");
      
      choice = get_char();
      send_char(' ');
      send_char(choice);
   } 
   send_string("\r\nThe system is set press any key to resume control.\r\n");                  
}
/*------------------------------------------------------------------------
 Function Name: get_temp

   Parameters: None
 
   Return    :  The temprature set by user.

 Description: A simple get method.
--------------------------------------------------------------------------*/
unsigned short get_temp(void){
  return temp;
}

int get_light(void){
  return light;
}  
