/*****************************************************************************
*   OilFieldSim.cpp
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "Field.h"
#include "StatusReport.h"

using namespace std;

int main()
{
	Field field;

	// local vars and random seed, testing stuff
	const bool SEED_RANDOM = true;  // seed random flag controller
	//const int randMin = 1;  // testing
	//const int randMax = 20; // testing
	int randCurve = 0;

	struct _timeb   tStruct;
    double thisTime = 0;
    double outputTime;
	double startTime;

	// seed random function
	if (SEED_RANDOM) {
		srand((unsigned int)thisTime);
	} 
	else {
		srand(90210);
	}

	_ftime_s(&tStruct);	// get start time
    thisTime = tStruct.time + (((double)(tStruct.millitm)) / 1000.0); // Convert to double
    outputTime = thisTime + 5.0; // Set next 5 second interval time
	startTime = thisTime;
	//randCurve = randMin + (rand() % (int)(randMax - randMin)); // testing

	field.initField();
	
	while(true) 
	{
        _ftime_s(&tStruct);
        thisTime = tStruct.time + (((double)(tStruct.millitm)) / 1000.0);
        // check 5 second interval
        if(thisTime >= outputTime) 
		{
			field.fiveSecondUpdate();
			outputTime += 5;
        }
	}


	/* 
	*********  Further Notes  **************
	error detection for monitored wells, beginning
	no sensors to add
	no sensor to remove
	(same with wells)
	checking for keyboard input

	*/

	return 0;
}