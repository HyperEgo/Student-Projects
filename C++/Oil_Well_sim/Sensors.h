/*****************************************************************************
*   Sensors.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#ifndef SENSORS_H
#define SENSORS_H

#include "OilFieldDataParser.h"

using namespace std;

class Sensors {
private:
	char m_wellID[32];
	char m_type[64];
	char m_className[64];
	char m_displayName[64];
	double m_min;
	double m_max;
	char m_units[64];
	char m_unitAbbr[64];
	bool flag;

public:
	Sensors();
	~Sensors();

	//getters
	char* getWellID();
	char* getType();
	char* getClassName();
	char* getDisplayName();
	double getMin();
	double getMax();
	char* getUnits();
	char* getUnitAbbr();
	bool getFlag();

	//setters
	void setWellID(char *wellIDz);
	void setType (char *typeZ);
	void setClassName(char *classNameZ);
	void setDisplayName(char *displayNameZ);
	void setMin(double a);
	void setMax(double b);
	void setUnits(char *unitsZ);
	void setUnitAbbr(char *unitAbbrZ);
	void setFlag(bool flg);

};


#endif