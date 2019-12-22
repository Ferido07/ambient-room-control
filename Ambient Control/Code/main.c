
#include "NXP/iolpc2148.h"
#include "system.h"
#include "uart.h"
#include "heater.h"
#include "fan.h"
#include "temp_adc.h"
#include "Int_button.h"
#include <intrinsics.h> 
#include "interrupts.h"
#include "light_adc.h"
#include "lamp.h"
#include "lamp_dac.h"

void main(){
   
   unsigned short temp;
   unsigned short light;
   
   __disable_interrupt(); //disable all interrupts (to prevent int request during initialization)
   
   clock_init();         //initialize system clock
   
   VIC_init();            //initialize vectored interrupt controller
      
   button_init();         //initialize button as external interrupt source
                          //triggered on the falling-edge at I/O port bit 14
  
   
   uart_init(); 
   heater_init();
   fan_init();
   temp_adc_init();
   light_adc_init(); 
   lamp_init();
   lamp_dac_init();

   __enable_interrupt();  //Now enable interrupts
   
   
 
   
   
  // for(int i=0;i<10000;i++); //delay
   
   send_string("        Welcome to Ambient Room Controller Panel! \r\n");
   send_string("This System Controls the Temprature, Light, and Moisture in Your Room. \r\n");
   send_string("To set up an optimum value press the button and to leave the System \r as it is with the current values press any key. \r\n");
   send_string("---------------------------------------------------------------------\r\n");
   //send_string("this one is mine");
   send_string("The current temprature is :" );
   print_int(read_temp());
   send_string(" degree celsius.\r\n"); 
   send_string("---------------------------------------------------------------------\r\n");
   
   char dummy=get_char();
   send_string("Resuming Control.\r\n");
   
   lamp_on();
   
   while(1){
     
          temp=read_temp();     //passed the test and reads temprature acurately in the range 15-35c
            // send_string("\n\r current temp: "); 
            //print_int(temp);
          light=read_light();
          
     
          if (temp<get_temp()-1){
              heater_on();
              fan_stop();
              //send_string(" ");//dummy output to clear the uart buffer
              //send_string("Turning heater on\n");
          }
          
          else if (temp>get_temp()+1){
            //turn on fan
            fan_start();
            heater_off();
            //send_string(" ");//dummy output to clear the uart buffer
              //send_string("Turning fan on\n");
             // print_int(get_temp());            
          }
 
        
         /* if (light<get_light()){
            lamp_on();
          }
          else if (light<get_light()){
            lamp_off();
          }

          else if (light>get_light()){
            
          }
          */
          else {
            fan_stop();
            heater_off();
            lamp_off();
           // send_string(" ");//dummy output to clear the uart buffer
              //send_string("Temprature is within range\n"); 
          } 
          
     // int i=100;
    //  while(i--);
   }  
}
