#include<avr/io.h>
#define F_CPU 12000000
#include <util/delay.h> 
#include "uart.h"

int main(void)
{
	unsigned char data;
	DDRB=0X0F;              // For drive motors
	DDRD=0XF0;				// For Lifters
	uart_init();		 	//Initialization of UART
	
	_delay_ms(100);
 while(1)
  {
	data=uart_read();		//read a charactor from UART
	switch (data)
	 {
	  case '1':				//if received 1
	   PORTB=0x05;			//Move left
	   break; 
	  case '2':				//if received 2
	   PORTB=0x0A;			//move right
	   break; 
	  case '3':				// move frwd
	   PORTB=0x06;
	   break; 					
	  case '4':             // move back
	   PORTB=0x09;
	   break;  
	  case '5':				//if received 5
	   PORTB=0x00;			//stop the bot
	   break; 
	  case '6':
	   PORTD=0x50;
	   break;
	  case '7':
	   PORTD=0xA0;
	   break;
	  case '8':  
	   PORTD=0x60;
	   break;
	  case'9':
	   PORTD=0x90;
	   break;
	  case'0':
	   PORTD=0x00;
	   break;
	 }
	
  }
  
}
