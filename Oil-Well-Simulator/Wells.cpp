/*****************************************************************************
*   Wells.cpp
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/

#include "Wells.h"
#include <string.h>

Wells::Wells() {
	flag=false;
}

Wells::~Wells() {
}

char* Wells::getID() {
	return m_id;
}

char* Wells::getOpr() {
	return m_opr;
}

int Wells::getNumSensors() {
	return m_numSensors;
}


bool Wells::getFlag() {
	return flag;
}



void Wells::setID(char* ID) {
	strcpy(m_id, ID);
}

void Wells::setOpr(char* opr) {
	strcpy(m_opr, opr);
}

void Wells::setNumSensors(int numSensors) {
	m_numSensors=numSensors;
}

void Wells::setFlag(bool flg) {
	flag=flg;
}