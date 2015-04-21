#include "satellite.h"

Satellite::Satellite() {
}

void Satellite::setData(int x, int y, double scale, QString information, int ID, double range) {
    xPos = x;
    yPos = y;
    scaleFactor = scale;
    commonName = information;
    NORADid = ID;
    satRange = range;
    satInfo = "ID:";
    satInfo.append(QString::number(ID));
    satInfo.append("\nName: ");
    satInfo.append(information);
    satInfo.append("\nRange(km): ");
    satInfo.append(QString::number(range/1000));
    satInfo.append("\n");

}

void Satellite::getData(int &x, int &y, double &scale, QString &information, int &ID) {
    x = xPos;
    y = yPos;
    scale = scaleFactor;
    information = satInfo;
    ID = NORADid;
}

QString Satellite::getSatInfo() {
    return satInfo;
}
