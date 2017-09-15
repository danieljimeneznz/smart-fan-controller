/*
 * Smart-Fan-Controller.cpp
 *
 * Created: 02-Sept-17 13:02:26
 * ELECTENG 311 Smart Fan Project
 * Group 4
 */ 

#include <avr/io.h>
#include <string.h>

#include "../include/prototypes.h"

#include "../include/CommsController.h"
#include "../include/PowerController.h"
#include "../include/PWMController.h"
#include "../include/SpeedController.h"
#include "../include/ErrorHandler.h"

int main(void)
{
	// Create controllers in stack space.
	CommsController commsController(56);
	PowerController powerController;
	PWMController pwmController;
	SpeedController speedController;
	ErrorHandler errorHandler;


	commsController.setControllerPointers(&speedController, &powerController, &errorHandler);
	powerController.setControllerPointers(&errorHandler);
	speedController.setControllerPointers(&pwmController, &errorHandler);
	errorHandler.setControllerPointers(&commsController);

	//tinyjsonpp* json = new tinyjsonpp(false, 255);

	//char* string = static_cast<char*>(calloc(60, sizeof(char)));
	//string = "{\"3\":{\"spd\":{\"req\":\"120\",\"cur\":\"123\"},\"pwr\":{\"req\":\"360\"}}}";
	//string = "{\"3\":{}}";
	//for (unsigned int i = 0; i < strlen(string); i++) {
		//json->parse(string[i]);
	//}

	//memset(string, 0, strlen(string));
	//free(string);

	//Value val;
	//val = json->getValue("req");
	//val = json->getValue("req", "3/pwr");
	//json->insert("hello", "world", "3/pwr");
	//json->insert("spd", "{}", "3");
	//json->insert("req", "120", "3/spd");
	//json->insert("cur", "123", "3/spd");
	//val = json->getValue("cur", "3/spd");
    while (1) 
    {
    }
}

