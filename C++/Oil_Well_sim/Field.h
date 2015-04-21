/*****************************************************************************
*   Field.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/

#ifndef MAP_H
#define MAP_H

#include "StatusReport.h"
#include "OilFieldDataParser.h"
#include "Sensors.h"
#include "Wells.h"
#include <vector>

using namespace std;

class Field {
private:
	StatusReport report;
	Sensors *s;
	Wells *w;
	vector<Wells*> weList;
	vector<Sensors*> seList;

public:
	Field();
	~Field();
	void initField();
	void loadFile(char *fileName);
	void fiveSecondUpdate();
	void processWellData();
	void processSensorData();

	void printWellQuickListAdd();
	void printWellQuickListRemove();
	void printWellFullList();
	void addWell(int a);
	void removeWell(int b);

	int getWellCount();
	int getSensorCount();
	void printSensorQuickListAdd();
	void printSensorQuickListRemove();
	void addSensor(int c);
	void removeSensor(int d);

};


#endif