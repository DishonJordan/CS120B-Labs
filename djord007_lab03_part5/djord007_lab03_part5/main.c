/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab 3  Exercise 5 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;
	unsigned short temp_output = 0x00;

    while (1) 
    {
		temp_output = (PIND << 1) + (PINB & 0x01);

		if(temp_output >= 70){
			PORTB = 0x02;
		}else if( temp_output > 5 && temp_output < 70){
			PORTB = 0x04;
		}else{
			PORTB = 0x00;
		}

    }
}

