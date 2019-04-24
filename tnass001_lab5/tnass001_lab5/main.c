/*
 * tnass001_lab5.c
 *
 * Created: 4/17/2019 9:05:34 AM
 * Author : nasse
 */ 

#include <avr/io.h>


int main(void)
{
		DDRB = 0xFF; PORTB = 0x00;
    while (1) 
    {
		PORTB = 0x0F;
    }
}

