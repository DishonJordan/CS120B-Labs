/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab 2  Exercise 2 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char total = 0x00;

    while (1) 
    {
		total = (PINA & 0x01) + (PINA >> 1 & 0x01) + (PINA >> 2 & 0x01) + (PINA >> 3 & 0x01);
	  
		PORTC = total; 


    }
}

