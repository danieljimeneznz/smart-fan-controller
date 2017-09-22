/*
 * Smart-Fan.cpp
 *
 * Author : fjov962
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
//set PA1 and PA2 as output (to drive the switches) DDRA
DDRA |= (1<<DDA1)|(1<<DDA2);
//set PA3 as input (for hall-effect sensor)
DDRA &= ~(1<<DDA3);

//Set it to fast PWM
TCCR0A |= (1<<WGM01)|(1<<WGM00);

//Pre-scaling
TCCR0B |= (1<<CS01)|(1<<CS00);

// clear OC0A on compare match with OCR0A
TCCR0A |= (1 << COM0A1);
TCCR0A |= (1 << COM0B1);

TCNT0 = 0;	//reset counter register

OCR0A = 100; //the Duty cycle n/255
OCR0B = 100; //the Duty cycle n/255

TOCPMSA0 &= ~((1<<TOCC1S1)|(1<<TOCC1S0));// route OC0A to PA2
TOCPMSA0 &= ~((1<<TOCC0S1)|(1<<TOCC0S0));// route OC0B to PA1

TIMSK0 |= (1<<TOIE0);	//Enable timer overflow interrupt

sei();	//enable interrupt

//TOCPMCOE |= (1<<TOCC1OE);	//enable TOCC1 at PA2
//TOCPMCOE |= (1<<TOCC0OE);	//enable TOCC0 at PA1

while(1){
	if((PINA)&(1<<PINA3)) {	
		TOCPMCOE &= ~(1<<TOCC1OE);	//disable TOCC1 at PA2
		TOCPMCOE |= (1<<TOCC0OE);	//enable TOCC0 at PA1
		PORTA &= ~(1<<PORTA2);		//disable PA2
	} else {
		TOCPMCOE &= ~(1<<TOCC0OE);	//disable TOCC0 at PA1
		TOCPMCOE |= (1<<TOCC1OE);	//enable TOCC1 at PA2
		PORTA &= ~(1<<PORTA1);		//disable PA1
	}
}

}

ISR(TIMER0_OVF_vect){
	PORTA |= (1<<PORTA1);
	PORTA |= (1<<PORTA2);
}

/*
while(1){
if ((PINA)&(1<<PINA3)) {
	PORTA |= (1<<PORTA1);
	PORTA &= ~(1<<PORTA2);
} else {
	PORTA &= ~(1<<PORTA1);
	PORTA |= (1<<PORTA2);
}

}
*/
