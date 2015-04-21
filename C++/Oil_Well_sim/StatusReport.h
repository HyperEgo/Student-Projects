/*****************************************************************************
*   StatusReport.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#ifndef STATUSREPORT_H
#define STATUSREPORT_H

#include "Wells.h"
#include "Sensors.h"

using namespace std;

class StatusReport {
public:
	StatusReport();
	~StatusReport();
	void printIntro();
	void queryUser(char *fileName);
	void printWellsEmpty();
	void printWellIntro();

	void printWellStatus(Wells *aWell);
	void printSensorStatus(Sensors *aSensor);
	void printWellDisplay();
	void printSensorDisplay();
	void printSensorsEmpty();

};


#endif