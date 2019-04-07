
/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab 2  Exercise 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0x00;
	unsigned char input = 0x00;

    while (1) 
    {
		input = PINA;
		PORTB = (input == 0x01)? 0x01: 0x00;
		
    }
	return 0;
}

