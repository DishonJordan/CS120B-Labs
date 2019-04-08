/*
 * djord007_lab02_part4.c
 *
 * Created: 4/7/2019 12:31:35 AM
 * Author : Dishon
 */ 

#include <avr/io.h>


int main(void)
{
		DDRA = 0x00; PORTA = 0xFF;
		DDRB = 0x00; PORTB = 0xFF;
		DDRC = 0x00; PORTC = 0xFF;
		
		DDRD = 0xFF; PORTD = 0x00;
		
		unsigned char weight_a = 0x00;
		unsigned char weight_b = 0x00;
		unsigned char weight_c = 0x00;
		unsigned short total_weight = 0x00;
		signed short weight_difference = 0x00;
		unsigned char temp_output = 0x00;

    /* Replace with your application code */
    while (1) 
    {
		weight_c = PINC;
		weight_a = PINA;
		weight_b = PINB; 

		total_weight = weight_a + weight_b + weight_c;

		weight_difference = weight_a - weight_c;

		temp_output = total_weight > 140 ? 0x01: 0x00;
		
		if(weight_difference > 80 || weight_difference < -80){
			temp_output = temp_output | 0x02;
		}

		PORTD = temp_output;
		

    }
}

