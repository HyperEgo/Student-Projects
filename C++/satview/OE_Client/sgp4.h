#ifndef SGP4_H
#define SGP4_H

#include "satdata.h"
#include <qmath.h>
#include <time.h>
#include <vector>
#include <QDebug>
#include <QString>

class sgp4
{
public:
    sgp4(satData tle, qint64 time);
    ~sgp4();
    void setObserver(double lat, double lon, double alt, double azBore, double elBore);
    double *getBore(size_t s);

private:
    satData *aTLE;
    qint64 propTime;
    double observerLat;
    double observerLon;
    double observerAlt;
    double observerBoreAZ;
    double observerBoreEl;
    int ydoy2mjd(int year, int dayofyear);
    int ymd2mjd (int yy, int mm, int dd);
    double gmst(double utime, int iDay, int iYear);
    void calcCoefficients();
    void calcECI(int yearView, int dayView, double stateTime);
    void calcECR();
    void calcENU();
    void calcRAEB();
    int getDaysInYear(int year);
    void initializeVariables();

    static const double xke = .743669161e-1;
    static const double xj2 = .10826299890519e-2;
    static const double ae = 1.0;
    static const double EARTH_RADIUS = 6378.137; // kilometers
    static const double EQUATORIAL_RADIUS = 1;
    static const double qo = 120.0;
    static const double so = 78.0;
    static const double xj3 = -.25321530681976e-5;
    static const double xj4 = -1.61098761e-6;
    static const double WEARTH = 7.2921151E-5;
    static const double requat = 6378137.0;
    static const double rpolar = 6356752.3142;


    double CK2;
    double cosio;
    double x3thm1;
    double semimajorAxis;
    double meanMotionAtEpoch;
    bool perigeeLessThan220km;
    double qoms2t;
    double S;
    double eta;
    double c1;
    double sinio;
    double x1mth2;
    double c4;
    double c5;
    double temp1;
    double temp2;
    double temp3;
    double CK4;
    double xmdot;
    double omgdot;
    double xnodot;
    double xnodcf;
    double omgcof;
    double xmcof;
    double t2cof;
    double xlcof;
    double aycof;
    double delmo;
    double sinmo;
    double x7thm1;
    double d2;
    double d3;
    double d4;
    double t3cof;
    double t4cof;
    double t5cof;
    double gmsta;
    double gmstaRad;

    double *eci;
    double *ecr;
    double *enu;
    double *raeb;


};

#endif // SGP4_H
