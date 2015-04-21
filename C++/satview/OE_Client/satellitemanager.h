#ifndef SATELLITEMANAGER_H
#define SATELLITEMANAGER_H
#include <QList>
#include <qmath.h>
#include "satdata.h"
#include "satellite.h"
#include "sgp4.h"

class SatelliteManager
{
public:
    SatelliteManager(QList<satData> theData);
    void setViewpoint(double observerLat, double observerLon, double observerElev, double observerBoreAz, double observerBoreEl, double observerFovDeg, int fovPixels);
    void setMaxRange(int newRange);
    void setMinRange(int newRange);
    QList<Satellite>* getSatellites(qint64 time);

private:
    QList<satData> satList;
    QList<Satellite> validSats;
    double lat;
    double lon;
    double elev;
    double boreAz;
    double boreEl;
    double fov;
    int radiusInPixels;
    int maxScreeningRange;
    int minScreeningRange;
    static const int MAXSCREENINGRANGE = 2000000; // 25000 KM
    // Private functions
    void findValidSats(qint64 time);
    void getBore(void);
    double getScale(double range);
    bool isVisible(double *bore);
    int getPixelX(double AZ);
    int getPixelY(double EL);


};

#endif // SATELLITEMANAGER_H
