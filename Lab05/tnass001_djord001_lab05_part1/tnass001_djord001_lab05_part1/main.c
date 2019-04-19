/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 025
 *	Assignment: Lab 5   Exercise 1
 *	Exercise Description: 
 *	
 *	(From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). 
 *	A series of LEDs connected to PB5..PB0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PB5 lights. 
 *	If the level is 3 or 4, PB5 and PB4 light. 5-6 lights PB5..PB3. 7-9 lights PB5..PB2. 10-12 lights PB5..PB1. 13-15 lights PB5..PB0. 
 *	Also, PB6 connects to a "Low fuel" icon, which should light if the level is 4 or less. 
 *	Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses.

 */
#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned char fuel_level = 0x00;
	unsigned char temp_output = 0x00;

	while (1)
	{
		fuel_level = PINA & 0x0F;

		switch(fuel_level){
			case 0x00:
			temp_output = 0x40;
			break;
			case 0x01:
			case 0x02:
			temp_output = 0x60;
			break;
			case 0x03:
			case 0x04:
			temp_output = 0x70;
			break;
			case 0x05:
			case 0x06:
			temp_output = 0x38;
			break;
			case 0x07:
			case 0x08:
			case 0x09:
			temp_output = 0x3C;
			break;
			case 0x0A:
			case 0x0B:
			case 0x0C:
			temp_output = 0x3E;
			break;
			case 0x0D:
			case 0x0E:
			case 0x0F:
			temp_output = 0x3F;
			break;
			default:
			temp_output = 0x00;
		}


		PORTB = temp_output;


	}
}



