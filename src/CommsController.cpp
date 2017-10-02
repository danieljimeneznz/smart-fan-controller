/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 
#define SOFT_VER "0.1.0"
#include "CommsController.h"

CommsController::CommsController(uint8_t ubrr) {
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); // Enable Rx, Tx and Rx complete interrupt
	UBRR0H = (ubrr>>8);
	UBRR0L = ubrr;
	// Select 8-bit data frame, double stop bit and no parity using UCSR0C (Default values are what we want).
	UCSR0C |= (1<<USBS0); // two stop bits

	// Create a new tinyJSONpp Object.
	json = new tinyjsonpp(false, 255);
}

void CommsController::transmit(uint8_t data) {
	while (!(UCSR0A&&(1<<UDRE0))); // Wait for empty transmit buffer.
	UDR0 = data;//sending data to TX buffer
	while (!(UCSR0A&(1<<TXC0))); // waits for Tx buffer to be empty
	
}

void CommsController::run(){

	if (this->jsonComplete == true){//checking if Rx is complete
		Value val;
		val = json->getValue("3");//checking that Rx is meant to be for our fan
		if (val.size > 2) {//checking if only an update is requested
			val = json->getValue("req", "3/spd");
			if (val.size > 0) {
				speedController->setFanSpeed(json->convertValueToInt(val));
			}
			val = json->getValue("clr", "3");
			if (val.size > 0){
				//clear errors
			}
		//give back an update
		json->insert("spd", "SOFT_VER", "3");
		}
	}
}

void CommsController::setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler)
{
	this->speedController = speedController;
	this->powerController = powerController;
	this->errorHandler = errorHandler;
}
