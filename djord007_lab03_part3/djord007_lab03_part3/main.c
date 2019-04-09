/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
 *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab 3  Exercise 3 
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

	unsigned char fuel_level = 0x00;
	unsigned char temp_output = 0x00;
	unsigned char seatbelt_light = 0x00;

	while (1)
	{
		fuel_level = PINA & 0x0F;

		switch(fuel_level){
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

		seatbelt_light = (PINA & 0x70) == 0x30 ? 0x80 : 0x00;

		PORTC = temp_output + seatbelt_light;

	}
}

