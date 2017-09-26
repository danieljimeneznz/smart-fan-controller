/*
 * Smart-Fan-Controller.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "prototypes.h"

#include "CommsController.h"
#include "PowerController.h"
#include "PWMController.h"
#include "SpeedController.h"
#include "ErrorHandler.h"

// Pointers used by ISR.
CommsController* commsController;
PowerController* powerController;
PWMController* pwmController;
SpeedController* speedController;
ErrorHandler* errorHandler;

ISR(ADC_vect) {
	//powerController->readVoltage(11);
}

ISR(USART0_RX_vect) {// There are two interrupts available,
	//one is USART0_RXC_vect (waits for completion) and USART0_RXs_vect (waits for start of incoming)
	//but atmel in only accepting USART0_RX_vect which i cant find in the data sheet
	while (!(UCSR0A & (1<<RXC0)))//loop waits for completion of incoming
	
	//commsController->json->parse(UDR0);

	if (UDR0 == 'd'){
		commsController->jsonComplete = true;
		PORTA |= (1 << PORTA7);
	}
}

ISR(TIMER0_OVF_vect){
	PORTA |= (1<<PORTA1);
	PORTA |= (1<<PORTA2);
}

int main(void)
{
	// Instantiate objects in stack.
	CommsController commsC(MYBRR);
	PowerController powerC;
	PWMController pwmC;
	SpeedController speedC;
	ErrorHandler errorH;

	// Set relationship between objects
	commsC.setControllerPointers(&speedC, &powerC, &errorH);
	powerC.setControllerPointers(&errorH);
	speedC.setControllerPointers(&pwmC, &errorH);
	errorH.setControllerPointers(&commsC);
	
	// Set ISR pointers
	commsController = &commsC;
	powerController = &powerC;
	pwmController = &pwmC;
	speedController = &speedC;
	errorHandler = &errorH;

	// Enable Interrupts
	sei(); // Set global interrupt enable.

	//tinyjsonpp* json = new tinyjsonpp(false, 255);

	//char* string = static_cast<char*>(calloc(60, sizeof(char)));
	//string = "{\"3\":{\"spd\":{\"req\":\"120\",\"cur\":\"123\"},\"pwr\":{\"req\":\"360\"}}}";
	//string = "{\"3\":{}}";
	//for (unsigned int i = 0; i < strlen(string); i++) {
		//json->parse(string[i]);
	//}

	//memset(string, 0, strlen(string));
	//free(string);

	//Value val;
	//val = json->getValue("req");
	//val = json->getValue("req", "3/pwr");
	//json->insert("hello", "world", "3/pwr");
	//json->insert("spd", "{}", "3");
	//json->insert("req", "120", "3/spd");
	//json->insert("cur", "123", "3/spd");
	//val = json->getValue("cur", "3/spd");
    while (1) 
    {
		if((PINA)&(1<<PINA0)) {
			TOCPMCOE &= ~(1<<TOCC1OE);	//disable TOCC1 at PA7
			TOCPMCOE |= (1<<TOCC0OE);	//enable TOCC0 at PA3
			PORTA &= ~(1<<PORTA7);		//disable PA7
		} else {
			TOCPMCOE &= ~(1<<TOCC0OE);	//disable TOCC0 at PA3
			TOCPMCOE |= (1<<TOCC1OE);	//enable TOCC1 at PA7
			PORTA &= ~(1<<PORTA3);		//disable PA3
		}
    }
}
