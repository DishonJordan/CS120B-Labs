/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 8 Exercise 4
*	Exercise Description:
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/

#include <avr/io.h>



void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned short x = 0x00;
	unsigned short max = 0x75;
	ADC_init();
	unsigned char b_out = 0x00;

	/* Replace with your application code */
	while (1)
	{
		
		x = ADC;
		if(x < 15){ b_out == 0x01;
		}else if(x < 30){  b_out = 0x03;
		}else if(x < 45){  b_out = 0x07;
		}else if(x < 60){  b_out = 0x0F;
		}else if(x < 75){  b_out = 0x1F;
		}else if(x < 90){  b_out = 0x3F;
		}else if(x < 105){ b_out = 0x5F;
		}else{			   b_out = 0x7F;
		}

		PORTB = b_out;
	}
}

