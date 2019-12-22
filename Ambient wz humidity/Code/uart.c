
/*----------------------------------------------------------------------------
  File name   : uart.c
  Description : implements functions for the UART module for performing simple data 
                transmit/receive functions on the serial line using RS-232 protocol

  System      : Philips LPC2148 MCU with ARM7TDMI-S core
  
  ECEg-4501 - Microcomputers and interfacing, lab exercise II
 
  
  Note:   Please refer to the manual on Chapter - 10  (Universal Asynchronous Reciever & Transmitter)
          as you study this

 ----------------------------------------------------------------------------*/
 
#include "NXP/iolpc2148.h"  //for register definitions

/*-------------------------------------------------------------------------------
Function:    uart_init  
Description: Initializes the UART module
Arguments:   None
Returns:     Nothing
Notes:       Port config: 1 stop bit, No parity, packet size=8-bit, baud rate = 19200bps
----------------------------------------------------------------------------------*/
void uart_init(void)
{
  PINSEL0_bit.P0_0 = 1;     //select pin function as uart (chapter6-pin connect block) -- TXD
  PINSEL0_bit.P0_1 = 1;     //select pin function as uart (chapter6-pin connect block) -- RXD
  
  U0LCR_bit.DLAB = 1;       //Divisor latch enabled (reqired only during configuration)
  
  U0DLM = 0;                
  U0DLL = 156;              //Baud rate dividing ratio = 156 (for 19200bps - see manual on how to calculate)
  
  U0FDR_bit.MULVAL = 1;     //Fractional baud rate multiplier = 1  (optional)
  U0FDR_bit.DIVADDVAL=0;    //Fractional baud rae divider = 0+1    (optional)
  
  U0FCR_bit.FCRFE = 1;      //RX and TX FIFO enabled
  U0LCR |= 0x03;            //8bits data, no parity, 1 stop bit
  U0TER_bit.TxEn = 1;       //Enable Transmitter
  
  U0LCR_bit.DLAB = 0;       //Divisor latch disabled (no more needed)
}

/*----------------------------------------------------------
Function:   Transmits a single byte on the UART serial line
Returns:    Nothing
Arguments:  Character to be transmitted
Notes:      waits until the transmit holding register (THR) becomes empty
            (interrupts and timeout can be used for more efficient implementation)  
------------------------------------------------------------*/
void send_char(char data)
{
  while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
  U0THR = data;                   //Shift out the data serially on the TX line
}

/*----------------------------------------------------------
Function:   Transmits a string of bytes on the UART serial line
Returns:    Nothing
Arguments:  a pointer to the string
Notes:      waits until the transmit holding register (THR) becomes empty 
            (interrupts and timeout can be used for more efficient implementation)    
------------------------------------------------------------*/
void send_string(char *str)
{
 
  while(*str != 0)
  {
    while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
    U0THR = *str++;              //Shift out the data serially on the TX line
  }
}

/*----------------------------------------------------------
Function:   Receives a single byte from the UART serial line
Returns:    The character received
Arguments:  None
Notes:      waits until valid data is received from the line   
            (interrupts and timeout can be used for efficient implementation)    
------------------------------------------------------------*/
char get_char(void)
{
  char data;                     //variable to hold incoming byte 
  
  while(U0LSR_bit.DR == 0);     //wait for data to come in
  data = U0RBR;                 //copy byte from the receive register to a variable
  return data;                  //return data received to the calling program
}


  


/*----------------------------------------------------------
Function:   Receives a string of bytes on the UART serial line and fills in the given buffer
Returns:    Nothing
Arguments: a pointer to the string buffer to be filled and the length of the string in bytes
Notes:      waits until data is received
            (interrupts and timeout can be used for efficient implementation)    
------------------------------------------------------------*/
void get_string(char *buffer, unsigned short length)
{
  for(int i=0; i < length; i++)
  {
    while(U0LSR_bit.DR == 0);    //wait for data to come in
    *buffer++ = U0RBR;           //copy byte from the receive register to buffer
  }
}

//A simple function to print a four digit decimal integer on terminal
void print_int(int integer)
{
   char digit[4];
     
   digit[0] = integer/1000;   //first digit
   
   integer %= 1000;           //The remainder after dividing by 1000
   
   digit[1] = integer/100;   //the second digit
   
   integer %= 100;           //The remainder after dividing by 100
   
   digit[2] = integer/10;    //third digit
   
   digit[3] = integer%10;    //fourth digit
   
   //change digits in to ASCII and print (if not zero) (add 0x30 or 48 to change to ASCII)
   for(int i=0; i<4; i++)
   {
      send_char(digit[i]+48);
   }
}

unsigned short get_value(void){
  
         char digits[5];
         unsigned short value=0;
         int i=0;
         
         do
         {
            digits[i] = get_char();        //get the digit
            
            if(digits[i] == '\r') break;   //quit if 'Enter' is pressed 
            
            send_char(digits[i]);          //echo the character
         }
         while(i++ < 4);                   //four digits max (1023)
         
         //i=3 for max of 4 input and less depending on the number of digits
         i -= 1;
         
         //convert the character array in to a single integer
         for(int j=1; i>=0; i--, j *= 10){
           //-48 is used for ascii correction
            value += (unsigned short)((digits[i]-48) * j);
           // send_string("ok now in the for loop so the problem is in conversion\r" ); 
         }
         
         return value;
}

