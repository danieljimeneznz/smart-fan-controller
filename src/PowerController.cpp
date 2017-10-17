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
	ADMUXB |=	(0<<REFS2)| // Reference Selection Bits.
				(0<<REFS1)| // AVcc;
				(0<<REFS0);
				 
	ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC11 (Vm).
				(0<<MUX4)|
				(1<<MUX3)|
				(0<<MUX2)|
				(1<<MUX1)|
				(1<<MUX0);

	ADCSRA |=	(1<<ADEN)| // ADC Enable.
				(1<<ADSC)| // Start ADC Conversion.
				(1<<ADATE)| // ADC Auto Trigger Enable.
				(1<<ADPS2)| // ADC pre-scaler Select Bits (64)
				(1<<ADPS1)|
				(0<<ADPS0);

	ADCSRB |=	(0<<ADTS2)| // Setting Free Running Mode as Trigger selection
				(0<<ADTS1)|
				(0<<ADTS0);
	return;
}

// Do not need to set ref voltage as default is Vcc.
// pb0 = voltage input, pb1 = current measurement.

void PowerController::readValue(uint8_t channel) volatile {
	if (channel == (10)){
		ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC11 (Vm).
					(0<<MUX4)|
					(1<<MUX3)|
					(0<<MUX2)|
					(1<<MUX1)|
					(1<<MUX0);
	}
	if (channel == (11)){
		ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC10 (Im).
					(0<<MUX4)|
					(1<<MUX3)|
					(0<<MUX2)|
					(1<<MUX1)|
					(0<<MUX0);
	}
	
	while (!(1<<ADIF)); // Waiting for conversion to be complete.
		
	if (channel == (10)){ // Reading voltage.
		voltage = ADC;
		voltage = voltage * 0.004887f; // Converting decimal to corresponding value.
	}

	if (channel == (11)){ // Reading current.
		current = ADC;
		current = current * 0.004887f; // Converting decimal to corresponding value.
	}
			
	ADCSRA |= (1<<ADSC);
}


float PowerController::getPower() volatile {
		PowerController::readValue(ADC_V_CHANNEL);
		PowerController::readValue(ADC_I_CHANNEL);
		return voltage * current;
}