#ifndef _ARGUMENT_HANDLER_H_
#define _ARGUMENT_HANDLER_H_

#include <iostream>
#include <stdexcept>
#include <random>
#include <ctime>
#include "ApplicationData.h"


/**
	Self-described, prints usage information
*/
void printUsage();

/**
	Function which parses initial command line arguments and changes
	ApplicationData structure based on those arguments.
*/
void parseArguments(int, char **);

void validateArguments(bool = false);

void generateMatrices(int);

#endif