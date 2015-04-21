/*****************************************************************************
*   StatusReport.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/

#include "StatusReport.h"
#include <iostream>
#include <ostream>
//#include <cstdlib>

using namespace std;

// constructor
StatusReport::StatusReport() {
}


// destructor
StatusReport::~StatusReport(){
}


//print program introduction
void StatusReport::printIntro(){
	cout<<"\n=========================================================\n"
		<<"|\tOILFIELD MONITOR SIMULATION DEMONSTRATION\t|\n"
		<<"|\tCS 307 -- Programming Assignment 01\t\t\|\n"
		<<"=========================================================\n\n";

}


// query user for xml file name
void StatusReport::queryUser(char *fileName) {
	cout<<"Please enter the name of the simulation data file then press Enter:" << endl;
	cout << "\t";
	cin >> fileName;
	//system("pause");
}


// print initialization dialogue to console
void StatusReport::printWellsEmpty() {
	cout<<"\n\nState of monitored wells.. \n"
		<<"No wells currently being monitored.\n\n";
}

void StatusReport::printSensorsEmpty(){
	cout<<"\n\nMonitored sensors. \n"
		<<"No sensors currently being monitored.\n\n";
}


// print initial key options to console for well / sensor
void StatusReport::printWellIntro() {
	cout<<"\n\n=================================================================\n"
		<<"| Press 'W' to add or remove wells from the reports.\t\t|\n"
		<<"| Press 'S' to add or remove sensors from the well report.\t|\n"
		<<"| Press 'Q' to exit the simulation.\t\t\t\t|\n"
		<<"=================================================================\n\n";
}


// print sensors
void StatusReport::printSensorStatus(Sensors *aSensor) {
	double randNum=0;
	if(aSensor->getMin()==0 && aSensor->getMax()==0) {
		aSensor->setMin(1);
		aSensor->setMax(100);
	}
	else {
		randNum = aSensor->getMin() + (rand() % (int)(aSensor->getMax() - aSensor->getMin()));
	}
	cout
		//<<"*******  TESTING SENSOR OUTPUT  *******\n"
		//<<"Sensor Well ID "<<aSensor->getWellID()<<endl
		<<"Sensor Type: "<<aSensor->getType()<<endl
		<<"Reading: "<<randNum<<" "<<aSensor->getUnitAbbr()<<endl;
		//<<"Display "<<aSensor->getDisplayName()<<endl
		//<<"Min "<<aSensor->getMin()<<endl
		//<<"Max "<<aSensor->getMax()<<endl
		//<<"Units "<<aSensor->getUnits()<<endl
		//<<"Abbrev "<<aSensor->getUnitAbbr()<<endl;
}

void StatusReport::printWellStatus(Wells *aWell) {
	cout<<"\n\nWell ID "<<aWell->getID()<<endl
		<<"Operator: "<<aWell->getOpr()<<endl;
		//<<"Sensor Number "<<aWell->getNumSensors()<<endl<<endl;
}

void StatusReport::printWellDisplay() {
	cout<<"\nDo you want to add or remove a well from the monitored list?\n"
		<<"Enter 'A' <add> or 'R' <remove> and press Enter: \n";
}

void StatusReport::printSensorDisplay() {
	cout<<"\nDo you want to add or remove a sensor from the monitored list?\n"
		<<"Enter 'A' <add> or 'R' <remove> and press Enter: \n";
}