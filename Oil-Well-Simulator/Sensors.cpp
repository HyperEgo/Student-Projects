/*****************************************************************************
*   Sensors.cpp
*   Programming Assignment 01 - Oil Field Data Simulator
*   Author:  Fleance Collins II
*   Date: June 2013
*   This program is my own work.
*****************************************************************************/

#include "Sensors.h"
#include <string.h>


// getters
Sensors::Sensors() {
	m_min=0;
	m_max=0;
	flag=true;
}

Sensors::~Sensors() {
}

char* Sensors::getWellID() {
	return m_wellID;
}

char* Sensors::getType() {
	return m_type;
}

char* Sensors::getClassName() {
	return m_className;
}

char* Sensors::getDisplayName() {
	return m_displayName;
}

double Sensors::getMin() {
	return m_min;
}

double Sensors::getMax() {
	return m_max;
}

char* Sensors::getUnits() {
	return m_units;
}

char* Sensors::getUnitAbbr() {
	return m_unitAbbr;
}

bool Sensors::getFlag() {
	return flag;
}


//setters
void Sensors::setWellID(char *wellIDz) {
	strcpy(m_wellID, wellIDz);
}


void Sensors::setType(char *typeZ) {
	strcpy(m_type, typeZ);
}

void Sensors::setClassName(char *classNameZ) {
	strcpy(m_className, classNameZ);
}


void Sensors::setDisplayName(char *displayNameZ) {
	strcpy(m_displayName, displayNameZ);
}


void Sensors::setMin(double a) {
	m_min=a;
}

void Sensors::setMax(double b) {
	m_max=b;
}

void Sensors::setUnits(char *unitsZ) {
	strcpy(m_units, unitsZ);
}

void Sensors::setUnitAbbr(char *unitAbbr) {
	strcpy(m_unitAbbr, unitAbbr);
}

void Sensors::setFlag(bool flg) {
	flag=flg;
}