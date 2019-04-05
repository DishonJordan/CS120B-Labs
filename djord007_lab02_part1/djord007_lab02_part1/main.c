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
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	
	unsigned char garage_sensor = 0x00;
	unsigned char light_sensor = 0x00;
	
    while (1) 
    {
		garage_sensor = PINA0;
		light_sensor = PINA1;
		
		PORTB = ~garage_sensor & light_sensor; 	
		
		
    }
	return 0;
}

