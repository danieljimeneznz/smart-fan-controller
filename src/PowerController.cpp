/*
 * PowerController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "PowerController.h"
#include <avr/io.h>
PowerController::PowerController() {				 
	ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC11 (Vm).
				(0<<MUX4)|
				(1<<MUX3)|
				(0<<MUX2)|
				(1<<MUX1)|
				(1<<MUX0);

	ADCSRA |=	(1<<ADEN)| // ADC Enable.
				(1<<ADSC)| // Start ADC Conversion.
				//(1<<ADATE)| // ADC Auto Trigger Enable.
				(1<<ADPS2)| // ADC pre-scaler Select Bits (64)
				(1<<ADPS1)|
				(0<<ADPS0);

	//ADCSRB |=	(0<<ADTS2)| // Setting Free Running Mode as Trigger selection
				//(0<<ADTS1)|
				//(0<<ADTS0);
	return;
}

// Do not need to set ref voltage as default is Vcc.
// pb0 = voltage input, pb1 = current measurement.

void PowerController::readValue(uint8_t channel) volatile {
	//if (channel == 11){
		//ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC11 (Vm).
					//(0<<MUX4)|
					//(1<<MUX3)|
					//(0<<MUX2)|
					//(1<<MUX1)|
					//(1<<MUX0);  0.0001051064f
	//}
	if (channel == (10)){
		ADMUXA |=	(0<<MUX5)| // Set the current ADC MUX to ADC10 (Im).
					(0<<MUX4)|
					(1<<MUX3)|
					(0<<MUX2)|
					(1<<MUX1)|
					(0<<MUX0);
	}

	ADCSRA |= (1<<ADSC);
	while (!(ADCSRA & (1<<ADSC))); // Waiting for conversion to be complete.
		
	//if (channel == (11)){ // Reading voltage.
		//voltage = ADC * 0.0032583f * 3.87f; // Converting decimal to corresponding value.
	//}

	if (channel == (10)){ // Reading current.
		current = ADC * 0.0032583f; // Converting decimal to corresponding value.
	}
}


float PowerController::getPower() volatile {
		//this->readValue(ADC_V_CHANNEL);
		this->readValue(ADC_I_CHANNEL);
		return 12.0f * current * 0.5f;
}