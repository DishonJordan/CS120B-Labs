/*
 * tnass001_djord007_lab04_part2.c
 *
 * Created: 4/11/2019 10:38:27 PM
 * Author : nasse
 */ 

#include <avr/io.h>

enum States {Init, Start, Increment, Decrement, Reset} State;

void tick() {
	switch(State) {
		case Init:
			State = Start;
			break;
			
		case Start:
			PORTC = 0x07;
			if ((PINA & 0x01) == 0x01) {
				State = Decrement;
			}
			else if ((PINA & 0x00) == 0x00) {
				State = Increment;
			}
			break;
			
		case Increment:
			if (PORTC > 9) {
				break;
			}
			PORTC += 0x01;
			if ((PINA & 0x01) == 0x01) {
				State = Decrement;
			}
			else if ((PINA & 0x00) == 0x00) {
				State = Increment;
			}
			break;
			
		case Decrement:
			if (PORTC < 0) {
				break;
			}
			PORTC -= 0x01;
			if ((PINA & 0x01) == 0x01) {
				State = Decrement;
			}
			else if ((PINA & 0x00) == 0x00) {
				State = Increment;
			}
			break;
			
		case Reset:
			if ((PINA & 0x03) == 0x03) {
				State = Reset;
			}
			else if ((PINA & 0x01) == 0x01) {
				State = Decrement;
			}
			else if ((PINA & 0x00) == 0x00) {
				State = Increment;
			}
			break;
	}
}

int main(void)
{
   DDRA = 0x00; PORTA = 0xFF;
   DDRC = 0xFF; PORTC = 0x07;
   unsigned char tmpC = 0x00;
   
   while(1) {
	   tick();
   }
}

