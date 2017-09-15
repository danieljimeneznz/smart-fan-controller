/*
 * PowerController.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#include "../include/prototypes.h"

#include "../include/ErrorHandler.h"

class PowerController {
public:
	PowerController();
	void setControllerPointers(ErrorHandler* errorHandler);
private:
	ErrorHandler* errorHandler;
};

#endif /* POWERCONTROLLER_H_ */