/*****************************************************************************
*   SensorManager.cpp
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#include "SensorManager.h"


SensorManager::SensorManager() {
}

SensorManager::~SensorManager() {
}

SensorManager *SensorManager::getInstance() {
	static SensorManager *theInstance = NULL;
	if(theInstance==NULL) {
		theInstance = new SensorManager;
	}
	return theInstance;
}

bool SensorManager::hasMoreSensors() {
	char *fileName = new char[64];  //ridiculous tactic just to use bool funct
	OilFieldDataParser *parser = new OilFieldDataParser(fileName);
	char wellID[63];
	char type[64];
	char className[64];
	char displayName[64];
	double min;
	double max;
	char units[64];
	char unitAbbrev[64];

	if (parser->getSensorData(wellID, type, className, displayName, &min, &max, units, unitAbbrev)) {
		s = new Sensors;
		s->setWellID(wellID);
		s->setType(type);
		s->setClassName(className);
		s->setDisplayName(displayName);
		s->setMin(min);
		s->setMax(max);
		s->setUnits(units);
		s->setUnitAbbr(unitAbbrev);
		return true;
	}

	return false;

}

Sensors *SensorManager::getSensor() {
	return s;
}

