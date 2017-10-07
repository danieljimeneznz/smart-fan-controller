/*
 * Smart-Fan-Controller.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "prototypes.h"

#include "CommsController.h"
#include "PowerController.h"
#include "PWMController.h"
#include "SpeedController.h"
#include "ErrorHandler.h"

#include "tinyjsonpp.h"

//**************************************
// EEPROM STRING LITERAL STORAGE
//**************************************
#define SOFTWARE_VERSION "0.1.0"

// The struct is defined so that the strings are stored in EEPROM in a particular order.
struct {
	// TYPE | NAME | EEPROM LOCATION
	char root[2]; // 0
	char obj[3]; // 2
	char ver[4]; // 5
	char softVer[6]; // 9
	char spd[4]; // 15
	char req[4]; // 19
	char cur[4]; // 23
	char pwr[4]; // 27
	char clr[4]; // 31
	char ew[3]; // 35

	// Error Messages
	char reqTooLow[14]; // 38
	char blockedDuct[16]; // 52
	char lockedRotor[16]; // 68

	// Combination error messages
	char reqDuct[28]; // 84
	char reqRotor[28]; // 112
	char blockedRotor[30]; // 140
	char allErrors[42]; // 170
} EEdata EEMEM = {
	.root =  "3",
	.obj = "{}",
	.ver = "ver",
	.softVer = SOFTWARE_VERSION,
	.spd = "spd",
	.req = "req",
	.cur = "cur",
	.pwr = "pwr",
	.clr = "clr",
	.ew = "ew",

	// Error Messages
	.reqTooLow = "[\"reqTooLow\"]",
	.blockedDuct = "[\"blockedDuct\"]",
	.lockedRotor = "[\"lockedRotor\"]",

	.reqDuct = "[\"reqTooLow\",\"blockedDuct\"]",
	.reqRotor = "[\"reqTooLow\",\"lockedRotor\"]",
	.blockedRotor = "[\"blockedDuct\",\"lockedRotor\"]",
	.allErrors = "[\"reqTooLow\",\"blockedDuct\",\"lockedRotor\"]"
};



//**************************************
// MAIN PROGRAM START
//**************************************

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
	//while (!(UCSR0A & (1<<RXC0)))//loop waits for completion of incoming
	
	uint8_t data = UDR0;
	if (data == 'd'){
		commsController->jsonComplete = true;
		commsController->transmit(speedController->currentSpeed);
	}

	if (data > 30) {
		commsController->json->addCharToJSONString(data);
	}
}

ISR(TIMER0_OVF_vect){
	PORTA |= (1<<PORTA1);
	PORTA |= (1<<PORTA2);
}

ISR(TIMER1_COMPA_vect){
	speedController->measureSpeed();
}

// ISR for hall sensor.
ISR(PCINT0_vect) {
	if((PINA)&(1<<PINA0)) {
		TOCPMCOE &= ~ (1<<TOCC7OE);	//disable TOCC1 at PB2
		TOCPMCOE |= (1<<TOCC2OE);	//enable TOCC0 at PA3
		PORTB &= ~(1<<PORTB2);		//disable PB2
	} else {
		TOCPMCOE &= ~(1<<TOCC2OE);	//disable TOCC0 at PA3
		TOCPMCOE |= (1<<TOCC7OE);	//enable TOCC1 at PB2
		PORTA &= ~(1<<PORTA3);		//disable PA3
	}

	// Increment the speedCounter.
	++speedController->speedCount;
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
	speedController->setFanSpeed(20);
	//const char* string = "{\"3\":{}}";
	//for (uint8_t i = 0; i < strlen(string); ++i) {
		//commsController->json->addCharToJSONString(string[i]);
	//}

    while (1)                  
    {
		//commsController->transmit('b');
		//commsController->jsonComplete = true;
		commsController->run();
	}
}
