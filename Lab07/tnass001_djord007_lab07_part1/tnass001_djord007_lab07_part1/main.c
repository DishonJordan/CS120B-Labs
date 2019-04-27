#include <avr/io.h>
#include "io.c"

enum States {init, wait, inc, dec, reset, incWait, decWait, resWait} state;
unsigned char tmpVal;

void tick() {
	unsigned char tmpA = ~PINA;
	switch(state) {
		case init:
		state = wait;
		break;
		
		case wait:
		if ((tmpA & 0x03) == 0x01) {
			state = inc;
		}
		else if ((tmpA & 0x03) == 0x02) {
			state = dec;
		}
		else if ((tmpA & 0x03) == 0x03) {
			state = reset;
		}
		else if ((tmpA & 0x03) == 0x00) {
			state = wait;
		}
		break;
		
		case inc:
		state = incWait;
		break;
		
		case dec:
		state = decWait;
		break;
		
		case reset:
		state = resWait;
		break;
		
		case incWait:
		if ((tmpA & 0x03) == 0x03) {
			state = reset;
		}
		else if ((tmpA & 0x03) == 0x00) {
			state = wait;
		}
		else if ((tmpA & 0x03) == 0x01) {
			state = incWait;
		}
		break;
		
		case decWait:
		if ((tmpA & 0x03) == 0x03) {
			state = reset;
		}
		else if ((tmpA & 0x03) == 0x00) {
			state = wait;
		}
		else if ((tmpA & 0x03) == 0x02) {
			state = decWait;
		}
		break;
		
		case resWait:
		if ((tmpA & 0x03) == 0x03) {
			state = reset;
		}
		else if ((tmpA & 0x03) == 0x00) {
			state = wait;
		}
		break;
		
		default:
		break;
	}
	
	switch(state) {
		case init:
			break;
		
		case wait:
			break;
		
		case inc:
			LCD_ClearScreen();
			LCD_WriteData(tmpVal + '0');
			if (tmpVal < 9) {
				++tmpVal;
			}
			break;
		
		case dec:
			LCD_ClearScreen();
			LCD_WriteData(tmpVal + '0');
			if (tmpVal > 0) {
				--tmpVal;
			}
			break;
		
		case reset:
			tmpVal = 0x00;
			LCD_ClearScreen();
			LCD_WriteData(tmpVal + '0');
			break;
		
		case incWait:
			break;
		
		case decWait:
			break;
		
		case resWait:
			break;
		
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	tmpVal = 0;
	
	state = init;
	LCD_init();
	
	while(1) {
		tick();	
	}
}
