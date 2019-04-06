/*
 * djord007_lab02_part1.c
 *
 * Created: 4/3/2019 9:14:45 AM
 * Author : Dishon Jordan
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

