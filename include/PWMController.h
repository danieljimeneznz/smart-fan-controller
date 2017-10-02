/*
 * PWMController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#ifndef PWMCONTROLLER_H_
#define PWMCONTROLLER_H_

#include "prototypes.h"

class PWMController {
public:
	PWMController();

	void SetDutyCycle(uint8_t Duty);

	uint8_t Duty;
};

#endif /* PWMCONTROLLER_H_ */