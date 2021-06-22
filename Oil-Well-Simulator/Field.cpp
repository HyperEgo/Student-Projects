/*****************************************************************************
*   Field.cpp
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/

#include "Field.h"
#include <math.h>
#include <iostream>

#include <conio.h>
#include <stdio.h>

// constructor
Field::Field() {
}

Field::~Field() {
}


void Field::initField() {
	report.printIntro();
	char *fileName = new char[64];
	report.queryUser(fileName);
	loadFile(fileName);

}

void Field::loadFile(char *fileName) {
	OilFieldDataParser *parser = new OilFieldDataParser(fileName);
	cout<<fileName<<endl<<endl;  // check input

	/*
	int wellCount = parser->getWellCount();
	cout<<"\n******* TESTING  ********\n";
	parser->printWellData();
	cout<<"Total Well count is "<<wellCount;
	cout<<"\n******* TESTING  ********\n\n";
	system("pause");
	*/

	// get sensor data
	char wellID[32];
	char type[64];
	char className[64];
	char displayName[64];
	double min;
	double max;
	char units[64];
	char unitAbbrev[64];

	while (parser->getSensorData(wellID, type, className, displayName, &min, &max, units, unitAbbrev)) {
		s = new Sensors;
		s->setWellID(wellID);
		s->setType(type);
		s->setClassName(className);
		s->setDisplayName(displayName);
		s->setMin(min);
		s->setMax(max);
		s->setUnits(units);
		s->setUnitAbbr(unitAbbrev);
		seList.push_back(s);
	}

	// get well data
	char id[64];
	char opr[64];
	int numSensors;

	while (parser->getWellData(id, opr, &numSensors)) {
		w = new Wells;
		w->setID(id);
		w->setOpr(opr);
		w->setNumSensors(numSensors);
		weList.push_back(w);
	}

}

void Field::fiveSecondUpdate() {

	int a=getWellCount();
	if(a==weList.size()) {
		report.printWellsEmpty(); 
	}
	else {
		printWellFullList();
	}
	processWellData();
	report.printWellIntro(); // notice on bottom

	/* ***********  TESTING  *******************
	// this block prints every well, every sensor
	vector<Wells*>::iterator we;
	vector<Sensors*>::iterator se;
	for(we=weList.begin(); we!=weList.end(); we++) {
		report.printWellStatus((*we));
		for(se=senList.begin(); se!=senList.end(); se++) {
			report.printSensorStatus((*se));
		}
	}
	*/
}

void Field::processWellData() {
	int hit, in1=0, in2=0;
	char ch1, ch2;
	hit = _kbhit(); //See if a key has been pressed
    if (hit)
    {
          ch1 = _getch();	// Get the key pressed
		  if(ch1=='W' || ch1=='w') {
			// add / remove wells
			//cout<<"\nW key pressed.\n";
			report.printWellDisplay();
			cin>>ch2;

			if(ch2=='A' || ch2=='a') {
				int f=getWellCount();
				if(f==0) { // check well list is full
					cout<<"\nWell list full. Remove Wells first, then add more.\n\n";
					system("pause");
				}

				else {
					printWellQuickListAdd();
					cin>>in1;
					addWell(in1);
					system("pause");
				}
			}

			else if (ch2=='R' || ch2=='r') {
				int c=getWellCount();
				if(c==weList.size()) {
					cout<<"\nCan't remove Wells from empty list.\n\n";
					system("pause");
				}

				else {
					printWellQuickListRemove();
					cin>>in2;
					removeWell(in2);
					system("pause");
				}

			}
			else {

			}

		  }
				
		  if(ch1=='S' || ch1=='s') {
			// add remove sensors
			//cout<<"\nS key pressed.\n";
			processSensorData();

		  }
		  if(ch1=='Q' || ch1=='q') {
			// quit application
			cout<<"\nExit Application.\n";
			exit(0);
		  }

		  else {

		  }
     }
}


void Field::processSensorData() {
	int b=getWellCount();
	if(b==weList.size()) {
		cout<<"\nNo Wells present. Add Wells to invoke Sensor options.\n\n";
		system("pause");
	}
	else {
		int hit, in1=0, in2=0;
		char ch2;
		report.printSensorDisplay();
		cin>>ch2;
		if(ch2=='A' || ch2=='a'){
			int g=getSensorCount();
			if (g==0) {
				cout<<"\nSensor list full.  Remove Sensors first, then add more.\n\n";
				system("pause");
			}
			else {
				printSensorQuickListRemove();
				cin>>in1;
				addSensor(in1);
				system("pause");
			}
		}
	
		else if (ch2=='R' || ch2=='r') {
			int c=getWellCount();
			int d=getSensorCount();
			if (c==weList.size() || d==seList.size()) {
				cout<<"\nCan't remove Sensors from empty list.\n\n";
				system("pause");
			}

			else {
				printSensorQuickListAdd();
				cin>>in2;
				removeSensor(in2);
				system("pause");
			}
		}
			
		else {

		}
	}
			
}



void Field::printWellQuickListAdd() {
	cout<<"\nAvailable Wells:\n";
	for(int i=0; i<weList.size(); i++) {
		if(weList[i]->getFlag()==false) {
			cout<<i<<". "<<weList[i]->getID()<<endl;
		}
	}
	cout<<"\nEnter a number from 0 to "<<weList.size()-1<<" and press Enter.\n";
}


void Field::printWellQuickListRemove() {
	cout<<"\nAvailable Wells:\n";
	for(int i=0; i<weList.size(); i++) {
		if(weList[i]->getFlag()==true) {
			cout<<i<<". "<<weList[i]->getID()<<endl;
		}
	}
	cout<<"\nEnter a number from 0 to "<<weList.size()-1<<" and press Enter.\n";
}


void Field::printWellFullList() {
	// add sensor output to this list
	int i, j;
	for(i=0; i<weList.size(); i++) {
		if(weList[i]->getFlag()==true) {
			cout<<"\n\nWell ID: "<<weList[i]->getID()<<endl
				<<"Operator: "<<weList[i]->getOpr()<<endl;

			int z=getSensorCount();
			if(z==seList.size()) {
				cout<<"\nNo Sensors present.\n\n"; // check for empty list before printing
			}
			else {
				for(j=0; j<seList.size(); j++) {
					double randNum=0;
					if(seList[j]->getMin()==0 && seList[j]->getMax()==0) {
						seList[j]->setMin(1);
						seList[j]->setMax(100);
					}
					else {
						randNum = seList[j]->getMin() + (rand() % (int)(seList[j]->getMax() - seList[j]->getMin()));
					}
					if(seList[j]->getFlag()==true) {
					cout<<"Sensor Type: "<<seList[j]->getType()<<endl
						<<"Reading: "<<randNum<<" "<<seList[j]->getUnitAbbr()<<endl;	
					}
				}	
			}
		}
	}
}



void Field::addWell(int a) {
	for(int j=0; j<weList.size(); j++) {
		if(j==a) {
			weList[j]->setFlag(true);
			break;
		}
	}
}


void Field::removeWell(int b) {
	for(int k=0; k<weList.size(); k++) {
		if(k==b) {
			weList[k]->setFlag(false);
			break;
		}
	}

}


int Field::getWellCount() {
	int cnt=0;
	vector<Wells*>::iterator wd;
	for(wd=weList.begin(); wd!=weList.end(); wd++) {
		if((*wd)->getFlag()==false) {
			cnt++; 
		} 
	}
	return cnt;
}


void Field::printSensorQuickListAdd() {
	cout<<"\nMonitored Sensors:\n";
	for(int i=0; i<seList.size(); i++) {
		if(seList[i]->getFlag()==true) {
			cout<<i<<". "<<seList[i]->getDisplayName()<<" <"<<seList[i]->getType()<<">"<<endl;
		}
	}
	cout<<"\nEnter a number from 0 to "<<seList.size()-1<<" and press Enter.\n";
}



void Field::printSensorQuickListRemove() {
	cout<<"\nMonitored Sensors:\n";
	for(int j=0; j<seList.size(); j++) {
		if(seList[j]->getFlag()==false) {
			cout<<j<<". "<<seList[j]->getDisplayName()<<"  <"<<seList[j]->getType()<<">"<<endl;
		}
	}
	cout<<"\nEnter a number from 0 to "<<seList.size()-1<<" and press Enter.\n";

}


void Field::addSensor(int c) {
	for(int j=0; j<seList.size(); j++) {
		if(j==c) {
			seList[j]->setFlag(true);
			break;
		}
	}

}


void Field::removeSensor(int d) {
	for(int k=0; k<seList.size(); k++) {
		if(k==d) {
			seList[k]->setFlag(false);
			break;
		}
	}

}


int Field::getSensorCount() {
	int cnt=0;
	for(int i=0; i<seList.size(); i++) {
		if(seList[i]->getFlag()==false) {
			cnt++; }
	}
	return cnt;
}