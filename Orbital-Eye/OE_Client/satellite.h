#ifndef SATELLITE_H
#define SATELLITE_H

#include <QString>

class Satellite
{
public:
    Satellite();
    void setData(int x, int y, double scale, QString information, int ID, double range);
    void getData(int &x, int &y, double &scale, QString &information, int &ID);
    QString getSatInfo();

private:
    int xPos;
    int yPos;
    double scaleFactor;
    QString satInfo;
    QString commonName;
    int NORADid;
    double satRange;
};

#endif // SATELLITE_H
