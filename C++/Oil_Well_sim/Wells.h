/*****************************************************************************
*   Wells.h
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/
#ifndef WELLS_H
#define WELLS_H

#include "Sensors.h"


class Wells {
private:
	char m_id[64];
	char m_opr[64];
	int m_numSensors;
	bool flag;

public:
	Wells();
	~Wells();

	char* getID();
	char* getOpr();
	int getNumSensors();
	bool getFlag();

	void setID(char *ID);
	void setOpr(char *opr);
	void setNumSensors(int numSensors);
	void setFlag(bool flg);	

};


#endif