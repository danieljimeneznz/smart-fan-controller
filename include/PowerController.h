/*
 * PowerController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#define VREF 5.0 // Reference Voltage
#define SHUNT 1.0 // Shunt resistor value.
#define ADC_RESOLUTION 10 // ADC Resolution.
#define STEP VREF/pow(2, ADC_RESOLUTION) // Step Size.
#define SOURCE_FREQUENCY 500

#define ADC_I_CHANNEL 10 // Current Measurement Channel.
#define ADC_V_CHANNEL 11 // Voltage Measurement Channel.

#include "prototypes.h"

#include "ErrorHandler.h"

class PowerController {
public:
	PowerController();
	void setControllerPointers(ErrorHandler* errorHandler);
	float getPower();
private:
	ErrorHandler* errorHandler;
	float power;
	
	float readVoltage(uint8_t channel);
	//float
};

#endif /* POWERCONTROLLER_H_ */