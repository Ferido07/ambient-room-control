
#include "NXP/iolpc2148.h"
#include "system.h"
#include "uart.h"
#include "heater.h"
#include "fan.h"
#include "temp_adc.h"
//#include "Int_button.h"
//#include <intrinsics.h> 
//#include "interrupts.h"
#include "light_adc.h"
#include "lamp.h"
#include "lamp_dac.h"
#include "humidity_adc.h"

void main(){
   
  //sensor readings
   unsigned short current_temp;
   unsigned short current_light;
   unsigned short current_humidity;
   //user selections
   unsigned short temp;
   unsigned short light;
   unsigned short max_humidity;
   //warning flag for humidity max
   int warned=0;
   
   clock_init();         //initialize system clock
   
   //initialize all sensors and devices 
   uart_init(); 
   heater_init();
   fan_init();
   temp_adc_init();
   light_adc_init(); 
   humidity_adc_init();
   lamp_dac_init();
   
   
   //
   send_string("        Welcome to Ambient Room Controller Panel! \r\n");
   send_string("This System Controls the Temprature, Light, and Humidity in Your Room. \r\n");
  // send_string("To set up an optimum value press the button and to leave the System \r as it is with the current values press any key. \r\n");
   send_string("---------------------------------------------------------------------\r\n");
   //display current temp
   send_string("The current temprature is :" );
   print_int(read_temp());
   send_string(" degree celsius.\r\n");
   //display current light
   send_string("The current light intesity is :");
   print_int(read_light());
   send_string(" microWatt per centimeter square\r");
   //display current humidity
   send_string("The current humidity is :");
   print_int(read_humidity());//correct read
   send_string(" percent\r");
   
   send_string("---------------------------------------------------------------------\r\n");
   //user selection of temprature
   send_string("Input the room temprature you want in degree celsius and press enter.\r"); 
   temp=get_value();
   send_string("C. \r");
   //user selection of light
   send_string("Input the room light intensity you want in microWatt per centimeter square(0-200) and press enter.\r"); 
   light=get_value();
   send_string("uW/cm2\r");
   //user selection of humidity
   send_string("Input the maximum relative humidity of the room you want in percent and press enter.\r"); 
   send_string("The recommended humidity is from 30-80%\r");
   max_humidity=get_value();
   send_string("percent. \r");
   
   //wait for input
   //char dummy=get_char();
   send_string("Adjusting the Optimum Conditions of the Room.\r\n");
   send_string("To change the values press the reset button.\r");
   
 
   
   while(1){
     
          current_temp=read_temp();     //passed the test and reads temprature acurately in the range 15-35c
            
          current_light=read_light();
          
         
          
          //temp section
          if (current_temp<temp){
              heater_on();
              fan_stop();
          }
         
          else if (current_temp>temp){
            fan_start();
            heater_off();           
          }
 
          else {
            fan_stop();
            heater_off();
            
          } 
          //light section
          if (current_light<light){
            int digital_light=light - current_light;
            digital_light*=468;
            digital_light/=100;
            lamp_dac_out(digital_light);
          }
          else if (current_light>light){
            lamp_dac_out(0);//lamp turned off
          }
          //do nothing if equal that is keep it that way
          
          //humidity warning 
          current_humidity=read_humidity();
          
          if ((current_humidity>max_humidity) && warned==0){
            send_string("Maximum relative room humidity has been reached open windows for ventilation \r or increase the room temprature to reduce relative humidity.\r");
            warned=1;  
            print_int(current_humidity);
          }
          else if (current_humidity<max_humidity){
            //print_int(current_humidity);
            warned=0; 
          }
          
      int i=100;
      while(i--);
   }  
}
