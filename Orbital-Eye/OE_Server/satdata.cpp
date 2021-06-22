#include "satdata.h"

satData::satData() {
}

void satData::setSatName(QString name) {
    satName = name;
}

void satData::setNoradID(int id) {
    noradID = id;
}

void satData::setLaunchYear(int year) {
    launchYear = year;
}

void satData::setLaunchNumber(int number) {
    launchNumber = number;
}

void satData::setFullEpoch(double epoch) {
    fullEpoch = epoch;
}

void satData::setEpochYear(int year) {
    iYear = year;
}

void satData::setEpochDay(int day) {
    iDay = day;
}

void satData::setEpochTime(double time) {
    uTime = time;
}

void satData::setDragCoefficient(double coef) {
    dragCoefficient = coef;
}

void satData::setOrbitInclination(double inclination) {
    orbitInclination = inclination;
}

void satData::setRightAscentionAscendingNode(double ascention) {
    rightAscentionAscendingNode = ascention;
}

void satData::setEccentricity(double eccentricity) {
    orbitEccentricity = eccentricity;
}

void satData::setPerigee(double perigee) {
    orbitPerigee = perigee;
}

void satData::setMeanAnomaly(double anomaly) {
    meanAnomaly = anomaly;
}

void satData::setMeanMotion(double motion) {
    meanMotion = motion;
}

QString satData::getSatName(void) {
    return satName;
}

int satData::getNoradID(void) {
    return noradID;
}

int satData::getLaunchYear(void) {
    return launchYear;
}

int satData::getLaunchNumber(void) {
    return launchNumber;
}

double satData::getFullEpoch(void) {
    return fullEpoch;
}

int satData::getEpochYear(void) {
    return iYear;
}

int satData::getEpochDay(void) {
    return iDay;
}

double satData::getEpochTime(void) {
    return uTime;
}

double satData::getDragCoefficient(void) {
    return dragCoefficient;
}

double satData::getOrbitInclination(void) {
    return orbitInclination;
}

double satData::getRightAscentionAscendingNode() {
    return rightAscentionAscendingNode;
}

double satData::getEccentricity(void) {
    return orbitEccentricity;
}

double satData::getPerigee(void) {
    return orbitPerigee;
}

double satData::getMeanAnomaly(void) {
    return meanAnomaly;
}

double satData::getMeanMotion(void) {
    return meanMotion;
}

QString satData::toString(void) {
    // Left todo
    return "todo";
}
