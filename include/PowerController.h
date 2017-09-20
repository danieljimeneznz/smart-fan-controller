/*
 * PowerController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#include "prototypes.h"

#include "ErrorHandler.h"

class PowerController {
public:
	PowerController();
	void setControllerPointers(ErrorHandler* errorHandler);
	void readVoltage();
private:
	ErrorHandler* errorHandler;
};

#endif /* POWERCONTROLLER_H_ */