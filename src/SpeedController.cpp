/*
 * SpeedController.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include "SpeedController.h"

SpeedController::SpeedController() {

}

void SpeedController::setControllerPointers(PWMController* pwmController, ErrorHandler* errorHandler) {
	this->pwmController = pwmController;
	this->errorHandler = errorHandler;
}
