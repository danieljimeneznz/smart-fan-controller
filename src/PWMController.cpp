/*
 * Smart-Fan.cpp
 *
 * Author : fjov962
 */ 

#include <avr/io.h>

int PWMcontrol (void){
//set PA1 and PA2 as output (to drive the switches) DDRA
DDRA |= (1<<DDA1)|(1<<DDA2);
//set PA3 as input (for hall-effect sensor)
DDRA &= ~(1<<DDA3);

//PORTA |= (1<<PORTA1);
//PORTA |= (1<<PORTA2);
while(1){
	if ((PINA)&(1<<PINA3)) {
		PORTA |= (1<<PORTA1);
		PORTA &= ~(1<<PORTA2);
	} else { 
		PORTA &= ~(1<<PORTA1);
		PORTA |= (1<<PORTA2);
	}
}

}