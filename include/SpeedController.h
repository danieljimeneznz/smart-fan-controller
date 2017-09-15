/*
 * SpeedController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef SPEEDCONTROLLER_H_
#define SPEEDCONTROLLER_H_

#include "../include/prototypes.h"

#include "../include/PWMController.h"
#include "../include/ErrorHandler.h"

class SpeedController {
public:
	SpeedController();
	void setControllerPointers(PWMController* pwmController, ErrorHandler* errorHandler);
private:
	PWMController* pwmController;
	ErrorHandler* errorHandler;
};

#endif /* SPEEDCONTROLLER_H_ */