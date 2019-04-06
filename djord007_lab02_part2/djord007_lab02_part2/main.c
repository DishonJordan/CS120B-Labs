/*
 * djord007_lab02_part2.c
 *
 * Created: 4/6/2019 1:22:37 PM
 * Author : Dishon
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

