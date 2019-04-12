/*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
*	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
*	Lab Section: 025
*	Assignment: Lab 4  Exercise 4
*	Exercise Description:
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/

#include <avr/io.h>

enum Lock_States{ Lock_Start, EnterPound, WaitForY, EnterY, Lock_Unlock, EnterPound_Lock,WaitForY_Lock, EnterY_Lock}Current_State;
unsigned char lock_status = 0x01;
unsigned char x_button = 0x00;
unsigned char y_button = 0x00;
unsigned char pound_button = 0x00;
unsigned char inside_button = 0x00;
unsigned char c_state_debug = 0x00;

void LockSM(){
	switch(Current_State){//Transition Controller
		case Lock_Start:
		Current_State = (pound_button && !x_button && !y_button)? EnterPound : Lock_Start;
		break;
		case EnterPound:
		if(x_button || y_button){
			Current_State = Lock_Start;
			}else if(pound_button){
			Current_State = EnterPound;
			}else{
			Current_State = WaitForY;
		}
		break;
		case WaitForY:
		if(pound_button || x_button){
			Current_State = Lock_Start;
			}else if( y_button){
			Current_State = EnterY;
			}else{
			Current_State = WaitForY;
		}
		break;
		case EnterY:
		Current_State = y_button? EnterY: Lock_Unlock;

		break;
		case Lock_Unlock:
		if(!inside_button && pound_button){
			Current_State = EnterPound_Lock;
		}else if( inside_button && !pound_button){
			Current_State = Lock_Start;
		}else{
			Current_State = Lock_Unlock;
		}
		break;
		case EnterPound_Lock:
		if(x_button || y_button){
			Current_State = Lock_Unlock;
		}else if(pound_button){
			Current_State = EnterPound_Lock;
		}else{
			Current_State = WaitForY_Lock;
		}
		
		break;
		case WaitForY_Lock:
		if(pound_button || x_button){
			Current_State = Lock_Unlock;
		}else if( y_button){
			Current_State = EnterY_Lock;
		}else{
			Current_State = WaitForY_Lock;
		}
		break;
		case EnterY_Lock:
			Current_State = y_button? EnterY_Lock: Lock_Start;
		break;
		default:
			break;
		}

		switch(Current_State){//State Actions Controller
			case Lock_Start:
			lock_status = 0x01;
			c_state_debug = 0x00;
			break;
			case EnterPound:
			lock_status = 0x01;
			c_state_debug = 0x01;
			break;
			case WaitForY:
			lock_status = 0x01;
			c_state_debug = 0x02;
			break;
			case EnterY:
			lock_status = 0x00;
			c_state_debug = 0x04;
			break;
			case Lock_Unlock:
			lock_status = 0x00;
			c_state_debug = 0x08;
			break;
			case EnterPound_Lock:
			lock_status = 0x00;
			c_state_debug = 0x10;
			break;
			case WaitForY_Lock:
			lock_status = 0x00;
			c_state_debug = 0x20;
			break;
			case EnterY_Lock:
			lock_status = 0x01;
			c_state_debug = 0x40;
			break;
			default:
			c_state_debug = 0xFF;
			break;
		}
	}


	int main(void)
	{
		DDRA = 0x00; PORTA = 0xFF;
		DDRB = 0xFF; PORTB = 0x00;
		DDRC = 0xFF; PORTC = 0x00;


		Current_State = Lock_Start;
		/* Replace with your application code */
		while (1)
		{
			x_button = PINA & 0x01;
			y_button = (PINA & 0x02) >> 1;
			pound_button = (PINA & 0x04) >> 2;
			inside_button = (PINA & 0x80) >> 7;
			LockSM();
			PORTB = lock_status;
			PORTC = c_state_debug;
		}
	}