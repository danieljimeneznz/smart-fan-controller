/*
 * ErrorHandler.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() {

	// Set class variables.
	this->reqTooLow = false;
	this->blockedDuct = false;
	this->lockedRotor = false;

}

void ErrorHandler::setControllerPointers(CommsController* commsController) {
	this->commsController = commsController;
}
