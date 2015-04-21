/*****************************************************************************
*   SensorManager.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "OilFieldDataParser.h"
#include "Sensors.h"

using namespace std;

class SensorManager {
private:
	Sensors *s;
	//OilFieldDataParser *parser;

public:
	SensorManager();
	~SensorManager();
	static SensorManager *getInstance();
	bool hasMoreSensors();
	Sensors *getSensor();

};

#endif