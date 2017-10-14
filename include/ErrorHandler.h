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
	void setControllerPointers(volatile CommsController* commsController, volatile SpeedController* speedController) volatile;
	void run() volatile;
	bool checkForErrors() volatile;
	void clearErrors() volatile;

	volatile bool reqTooLow;
	volatile bool blockedDuct;
	volatile bool lockedRotor;

	volatile uint16_t errorEEPROMStart;
	volatile uint8_t errorEEPROMLength;
	volatile uint8_t timerCount;
	volatile uint8_t lockedRotorCount;
private:
	volatile CommsController* commsController;
	volatile SpeedController* speedController;
};

#endif /* ERRORHANDLER_H_ */