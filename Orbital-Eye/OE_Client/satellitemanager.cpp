#include "satellitemanager.h"
#include <QDebug>
#include <QString>
#include <qmath.h>
#include <QTime>

SatelliteManager::SatelliteManager(QList<satData> theData) {
    satList = theData;
    // initialize variables
    lat = 0;
    lon = 0;
    elev = 0;
    boreAz = 0;
    boreEl = 0;
    fov = 0;
    radiusInPixels = 0;
    maxScreeningRange = 2000000; // Default 2000 KM
    minScreeningRange = 200000; // Default 200 KM
}

void SatelliteManager::setViewpoint(double observerLat, double observerLon, double observerElev, double observerBoreAz, double observerBoreEl, double observerFovDeg, int fovPixels) {
    lat = observerLat;
    lon = observerLon;
    elev = observerElev;
    boreAz = observerBoreAz;
    boreEl = observerBoreEl;
    fov = observerFovDeg;
    fov = 40;
    radiusInPixels = fovPixels;
}

void SatelliteManager::setMaxRange(int newRange) {
    maxScreeningRange = newRange*1000; // Convert to meters
}

void SatelliteManager::setMinRange(int newRange) {
    minScreeningRange = newRange*1000; // Convert to meters
}

QList<Satellite> *SatelliteManager::getSatellites(qint64 time) {
    // Can only be called once per second
    findValidSats(time);
    return &validSats;
}

void SatelliteManager::findValidSats(qint64 time) {
    // for each sat in satList
    // propagate to given time
    // convert to observer boresight
    // determine if sat is within observer FOV
    // Clear validSats
    validSats.clear();
    // Add to validSats
    QList<satData>::iterator i;
    for (i = satList.begin(); i != satList.end(); ++i) {
        if ((*i).getSatName().contains("deb", Qt::CaseInsensitive)) continue;
        double *bore;
        sgp4 *prop = new sgp4(*i, time);
        prop->setObserver(lat, lon, elev, boreAz, boreEl);
        bore = prop->getBore(3);
        if (isVisible(bore)) {
            // Valid sat
            Satellite s;
            // Scale AZ/EL to pixel values
            int X = getPixelX(bore[1]);
            int Y = getPixelY(bore[2]);
            double scale = getScale(bore[0]);
            s.setData(X, Y, scale, (*i).toString(), (*i).getNoradID(), bore[0]);
            validSats.push_back(s);
        }
        delete prop;
        delete bore;
    }
}

bool SatelliteManager::isVisible(double *bore) {

    // A MAX range screen here to reject
    if (bore[0] > maxScreeningRange) return false;
    // A MIN range screen here to reject
    if (bore[0] < minScreeningRange) return false;
    // Convert az and el to boresight angle
    // Compare to FOV
    double az = bore[1]*M_PI/180.;
    double el = bore[2]*M_PI/180.;
    double offBoresightAngle = (qAcos(qCos(az)*qCos(el)))*180./M_PI;
    if (offBoresightAngle <= fov) return true;
    return false;
}

int SatelliteManager::getPixelX(double AZ) {
    // Compute integer pixel value
    // Assume top-left is pixel (0,0)
    // ((radiusInPixels/fov) * AZ) + radiusInPixels
    return (int)((radiusInPixels/fov)*AZ + radiusInPixels);
}

int SatelliteManager::getPixelY(double EL) {
    // Compute integer pixel value
    // Assume top-left is pixel (0,0)
    // ((radiusInPixels/fov) * EL) + radiusInPixels
    return (int)(-1*(radiusInPixels/fov)*EL + radiusInPixels);
}

double SatelliteManager::getScale(double range) {
    // Scale is from 0 - 1
    double maxScaled = 0.1;
    double minScaled = .75;
    // TODO HARDWIRED TEST VALUES (unit is meters)
    double maxRng = MAXSCREENINGRANGE;
    double minRng = 200000;
    // Make outliers conform
    if (range > maxRng) range = maxRng;
    if (range < minRng) range = minRng;
    return ((maxScaled - minScaled) / (maxRng - minRng))*(range-minRng) + minScaled;
}
