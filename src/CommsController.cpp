/*
 * CommsController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 
#include "CommsController.h"

CommsController::CommsController(uint8_t ubrr) {
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); // Enable Rx, Tx and Rx Complete Interrupt.
	UBRR0H = (ubrr>>8);
	UBRR0L = ubrr;
	// Select 8-bit data frame, double stop bit and no parity using UCSR0C (Default values are what we want).
	UCSR0C |= (1<<USBS0); // Two stop bits.

	this->jsonComplete = false;

	// Create a new tinyJSONpp Object.
	json = new tinyjsonpp(false, 255);
}

void CommsController::transmit(uint8_t data) {
	UCSR0B &= ~(1<<RXEN0); // disable Rx.
	while (!(UCSR0A&&(1<<UDRE0))); // Wait for empty transmit buffer.
	UDR0 = data; // Sending data to TX buffer.
	while (!(UCSR0A&(1<<TXC0))); // Waits for Tx buffer to be empty.
	UCSR0B |= (1<<RXEN0); // Enable Rx.
}

void CommsController::run(){

	// Checking if Rx is complete
	if (this->jsonComplete) {
		Value val;
		val = json->getValue("3"); // Checking that Rx is meant to be for our fan.
		if (val.size > 1) { // Checking if only an update is requested.
			val = json->getValue("req", "3/spd");
			if (val.size > 0) {
				speedController->setFanSpeed(json->convertValueToInt(val));
				//this->transmit('s');
			}
			val = json->getValue("clr", "3");
			if (val.size > 0){
			// Clear errors
			}
			// Give back an update (idk how to comment the json stuff)
			json->insert("ver", "0.1.0", "3"); 
			//this->transmit('p');
			//json->insert("spd", "{}", "3");
			//char* spd = static_cast<char *>(calloc(5, sizeof(char))); // Allocating memory for conversion of int to string.
			//itoa(speedController->requestedSpeed, spd, 10); // Converting requested speed value from int to string.
			//json->insert("req", "22", "3/spd");
			////this->transmit('o');
			//itoa(speedController->currentSpeed, spd, 10); // Converting current speed value from int to string.
			//json->insert("cur", "22", "3/spd");
			// json->insert("hello", "world", "3/pwr");
		
			//free(spd);

			// uint8_t size = json->getSize();
			uint8_t size = 21;
			//this->transmit('n');
			// For loop to transmit every bit of json.
			for (uint8_t i = 0; i < size; ++i){
				this->transmit(json->getChar(i));
			}
		}
		this->jsonComplete = false; // Resetting json complete.
	}
}

void CommsController::setControllerPointers(SpeedController* speedController, PowerController* powerController, ErrorHandler* errorHandler)
{
	this->speedController = speedController;
	this->powerController = powerController;
	this->errorHandler = errorHandler;
}
