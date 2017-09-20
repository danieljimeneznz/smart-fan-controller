/*
 * ErrorHandler.h
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 


#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include "prototypes.h"

#include "CommsController.h"

class ErrorHandler {
public:
	ErrorHandler();
	void setControllerPointers(CommsController* commsController);
private:
	CommsController* commsController;
};

#endif /* ERRORHANDLER_H_ */