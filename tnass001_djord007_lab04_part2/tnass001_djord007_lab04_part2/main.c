/*
 * tnass001_djord007_lab04_part2.c
 *
 * Created: 4/11/2019 10:38:27 PM
 * Author : nasse
 */ 

#include <avr/io.h>

enum States {Init, Increment, Decrement, Reset} State;

void tick() {
	switch(State) {			
		case Init:
			if ((PINA & 0x03) == 0x02) {
				State = Decrement;
			}
			else if ((PINA & 0x03) == 0x01) {
				State = Increment;
			}
			else if ((PINA & 0x03) == 0x03) {
				State = Reset;
			}
			break;
			
		case Increment:
			if ((PINA & 0x03) == 0x02) {
				State = Decrement;
			}
			else if ((PINA & 0x03) == 0x01) {
				State = Increment;
			}
			else if((PINA & 0x03) == 0x03) {
				State = Reset;
			}
			break;
			
		case Decrement:
			if ((PINA & 0x03) == 0x02) {
				State = Decrement;
			}
			else if ((PINA & 0x03) == 0x01) {
				State = Increment;
			}
			else if((PINA & 0x03) == 0x03) {
				State = Reset;
			}
			break;
			
		case Reset:
			if ((PINA & 0x03) == 0x03) {
				State = Reset;
			}
			else if ((PINA & 0x03) == 0x02) {
				State = Decrement;
			}
			else if ((PINA & 0x03) == 0x01) {
				State = Increment;
			}
			break;
			
		default:
			break;
	}
	
	switch(State) {
		case Init:
			PORTC = 0x07;
			break;
			
		case Increment: 
			if (PORTC > 9) {
				break;
			}
			PORTC += 1;
			break;
		
		case Decrement:
			if (PORTC <= 0) {
				break;
			}
			PORTC -= 1;
			break;
			
		case Reset:
			PORTC = 0x00;
			
		default:
			break;
	}
}

int main(void)
{
   DDRA = 0x00; PORTA = 0xFF;
   DDRC = 0xFF; PORTC = 0x07;
   State = Init;

   while(1) {
	   tick();
   }
}

