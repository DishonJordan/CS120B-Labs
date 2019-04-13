    /*	Partner 1 Name & E-mail: Dishon Jordan djord007@ucr.edu
    *	Partner 2 Name & E-mail: Travis Nasser tnass001@ucr.edu
    *	Lab Section: 025
    *	Assignment: Lab 4  Exercise 5
    *	Exercise Description:
    *
    *	I acknowledge all content contained herein, excluding template or example
    *	code, is my own original work.
    */

    #include <avr/io.h>

    enum Lock_States{ Lock_Start, CheckButton, WaitButtonRelease, Lock_Unlock}Current_State;
    unsigned char lock_status = 0x01;
    unsigned char x_button = 0x00;
    unsigned char y_button = 0x00;
    unsigned char pound_button = 0x00;
	unsigned char code[4] = {'#','x','y','x'};
	unsigned char count = 0x00;
    unsigned char inside_button = 0x00;
    unsigned char c_state_debug = 0x00;

    void LockSM(){
	    switch(Current_State){//Transition Controller
			case Lock_Start:
				Current_State = (!pound_button && !x_button && !y_button)? Current_State : CheckButton;
			break;
			case CheckButton:
				switch(count){
					case 0:
						Current_State = (pound_button && !x_button && !y_button)? WaitButtonRelease : Lock_Start;
					break;
					case 1:
						Current_State = (!pound_button && x_button && !y_button)? WaitButtonRelease : Lock_Start;
					break;
					case 2:
						Current_State = (!pound_button && !x_button && y_button)? WaitButtonRelease : Lock_Start;
					break;
					case 3:
						Current_State = (!pound_button && x_button && !y_button)? WaitButtonRelease : Lock_Start;
					break;
					default:
					break;
				}
			break;
			case WaitButtonRelease:
				count++;
				if(count == 4){
					Current_State = Lock_Unlock;
				}else{
				Current_State = (!pound_button && !x_button && !y_button)? CheckButton : Lock_Start;
				}
			break;
			case Lock_Unlock:
				Current_State = y_button? Lock_Unlock : Lock_Start;
			break;
			default:
			break;

	    }

	    switch(Current_State){//State Actions Controller
			case Lock_Start:
			count = 0;
			lock_status = 0x01;
			c_state_debug = 0x00;
			break;
			case CheckButton:	
			lock_status = 0x01;
			c_state_debug = 0x01;		
			break;
			case WaitButtonRelease:
			count++;
			lock_status = 0x01;
			c_state_debug = 0x02;
			break;
			case Lock_Unlock:
			lock_status = 0x00;
			c_state_debug = 0x03;
			break;
			default:
			break;
	    }
    }


    int main(void)
    {
	    DDRA = 0x00; PORTA = 0xFF;
	    DDRB = 0xFF; PORTB = 0x00;
	    DDRC = 0xFF; PORTC = 0x00;
		count = 0x00;

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