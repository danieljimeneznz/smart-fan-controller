/*
 * PowerController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "PowerController.h"

PowerController::PowerController() {
	/** Setup and enable ADC **/
	ADMUXB |=	(0<<REFS2)| // Reference Selection Bits
				(0<<REFS1)| // AVcc;
				(0<<REFS0);
				 
	ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC0.
				(0<<MUX4)|
				(0<<MUX3)|
				(0<<MUX2)|
				(0<<MUX1)|
				(0<<MUX0);

	ADCSRA =	(0<<ADEN) | // ADC Disable
				(0<<ADSC) | // ADC Conversion bit
				(0<<ADIE) | // ADC Interrupt Disable
				(1<<ADPS2)| // ADC Pre-scaler Select Bits (128)
				(1<<ADPS1)|
				(1<<ADPS0);
	return;
}

void PowerController::setControllerPointers(ErrorHandler* errorHandler) {
	this->errorHandler = errorHandler;
}

float PowerController::readVoltage(uint8_t channel) {
	return 0.0f;
}
