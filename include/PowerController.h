

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
	uint16_t getPower();
	uint16_t voltage, current;
	
private:
	ErrorHandler* errorHandler;
	uint16_t power;
	
	uint16_t readVoltage(uint8_t channel);

};

#endif /* POWERCONTROLLER_H_ */
