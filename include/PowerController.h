
/*
 * PowerController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#define ADC_I_CHANNEL 10 // Current Measurement Channel.
#define ADC_V_CHANNEL 11 // Voltage Measurement Channel.

#include "prototypes.h"

#include "ErrorHandler.h"

class PowerController {
public:
	PowerController();
	float getPower() volatile;
	volatile float voltage; 
	volatile float current;
	
private:
	void readValue(uint8_t channel) volatile;

};

#endif /* POWERCONTROLLER_H_ */