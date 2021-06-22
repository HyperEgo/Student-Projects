#include "sgp4.h"

sgp4::sgp4(satData tle, qint64 time) {
    // Take this TLE and propagate it
    aTLE = &tle;
    propTime = time;
    initializeVariables();
}

sgp4::~sgp4() {
    delete eci;
    delete ecr;
    delete enu;
}

void sgp4::setObserver(double lat, double lon, double alt, double azBore, double elBore) {
    observerLat = lat;
    observerLon = lon;
    observerAlt = alt;
    observerBoreAZ = azBore;
    observerBoreEl = elBore;
}

double *sgp4::getBore(size_t s) {

    // Calculate the preliminary coefficients and ECI position
    calcCoefficients();
    // Calculate the ECR position for this time for this TLE
    calcECR();
    // Calculate the ENU position for this time for this TLE
    calcENU();
    // Calculate the RAEB position for this time for this TLE
    calcRAEB();

    return raeb;
}

void sgp4::calcCoefficients() {
    double a1 = qPow(xke/aTLE->getMeanMotion(), 2./3);
    cosio = qCos(aTLE->getOrbitInclination());
    double theta2 = cosio * cosio;
    x3thm1 = 3 * theta2 - 1;
    double betao2 = 1 - (aTLE->getEccentricity() * aTLE->getEccentricity());
    double betao = qSqrt(betao2);
    CK2 = .5 * xj2 * qPow(ae, 2);
    double del1 = 1.5 * CK2 * x3thm1 / (a1 * a1 * betao * betao2);
    double ao = a1 * (1 - del1 * (1.0 / 3 + del1 * (1 + 134.0 / 81 * del1)));
    double delo = 1.5 * CK2 * x3thm1 / (ao * ao * betao * betao2);
    meanMotionAtEpoch = aTLE->getMeanMotion() / (1 + delo);
    semimajorAxis = ao / (1 - delo);
    // Check if perigee less than 220 kilometers
    perigeeLessThan220km = false;
    if ((semimajorAxis * (1 - aTLE->getEccentricity()) / EQUATORIAL_RADIUS) < (220.0 / EARTH_RADIUS + EQUATORIAL_RADIUS)) {
        perigeeLessThan220km = true;
    }
    // Check if perigee below 156 km
    S = EQUATORIAL_RADIUS + 78 / EARTH_RADIUS;
    double s4 = S;
    qoms2t = qPow((qo-so)*ae/EARTH_RADIUS, 4);
    double qoms24 = qoms2t;
    double perigee = (semimajorAxis * (1 - aTLE->getEccentricity()) - EQUATORIAL_RADIUS) * EARTH_RADIUS;
    if (perigee < 156) {
        s4 = perigee - 78;
        if (perigee <= 98) s4 = 20;
        qoms24 = qPow((120 - s4) * EQUATORIAL_RADIUS / EARTH_RADIUS, 4);
        s4 = s4 / EARTH_RADIUS + EQUATORIAL_RADIUS;
    }
    double tsi = 1 / (semimajorAxis - s4);
    eta = semimajorAxis * aTLE->getEccentricity() * tsi;
    double etasq = eta * eta;
    double eeta = aTLE->getEccentricity() * eta;
    double psisq = qAbs(1 - etasq);
    double coef = qoms24 * qPow(tsi, 4);
    double coef1 = coef / qPow(psisq, 3.5);
    double c2 = coef1 * meanMotionAtEpoch * (semimajorAxis * (1 + 1.5 * etasq + eeta * (4 + etasq))
                + 0.75 * CK2 * tsi / psisq * x3thm1 * (8 + 3 * etasq * (8 + etasq)));
    c1 = aTLE->getDragCoefficient() * c2;
    sinio = qSin(aTLE->getOrbitInclination());
    double a3ovk2 = -1*xj3 / CK2 * qPow(EQUATORIAL_RADIUS, 3);
    double c3 = coef * tsi * a3ovk2 * meanMotionAtEpoch * EQUATORIAL_RADIUS * sinio / aTLE->getEccentricity();
    x1mth2 = 1 - theta2;
    c4 = 2 * meanMotionAtEpoch * coef1 * semimajorAxis * betao2 * (eta * (2 + 0.5 * etasq)
         + aTLE->getEccentricity() * (0.5 + 2 * etasq) - 2 * CK2 * tsi / (semimajorAxis * psisq)
         * (-3 * x3thm1 * (1 - 2 * eeta + etasq * (1.5 - 0.5 * eeta)) + 0.75 * x1mth2
         * (2 * etasq - eeta * (1 + etasq)) * qCos(2 * aTLE->getPerigee())));
    c5 = 2 * coef1 * semimajorAxis * betao2 * (1 + 2.75 * (etasq + eeta) + eeta * etasq);
    double theta4 = theta2 * theta2;
    double pinvsq = 1 / (semimajorAxis * semimajorAxis * betao2 * betao2);
    temp1 = 3 * CK2 * pinvsq * meanMotionAtEpoch;
    temp2 = temp1 * CK2 * pinvsq;
    CK4 = -.375 * xj4 * qPow(ae, 4);
    temp3 = 1.25 * CK4 * pinvsq * pinvsq * meanMotionAtEpoch;
    xmdot = meanMotionAtEpoch + 0.5 * temp1 * betao * x3thm1 + 0.0625
            * temp2 * betao * (13 - 78 * theta2 + 137 * theta4);
    double x1m5th = 1 - 5 * theta2;
    omgdot = -0.5 * temp1 * x1m5th + 0.0625 * temp2 * (7 - 114 * theta2 + 395 * theta4) + temp3
            * (3 - 36 * theta2 + 49 * theta4);
    double xhdot1 = -temp1 * cosio;
    xnodot = xhdot1 + (0.5 * temp2 * (4 - 19 * theta2) + 2 * temp3 * (3 - 7 * theta2)) * cosio;
    omgcof = aTLE->getDragCoefficient() * c3 * qCos(aTLE->getPerigee());
    xmcof = -2.0 / 3 * coef * aTLE->getDragCoefficient() * EQUATORIAL_RADIUS / eeta;
    xnodcf = 3.5 * betao2 * xhdot1 * c1;
    t2cof = 1.5 * c1;
    xlcof = 0.125 * a3ovk2 * sinio * (3 + 5 * cosio) / (1 + cosio);
    aycof = 0.25 * a3ovk2 * sinio;
    delmo = qPow(1 + eta * qCos(aTLE->getMeanAnomaly()), 3);
    sinmo = qSin(aTLE->getMeanAnomaly());
    x7thm1 = 7 * theta2 - 1;
    if (!perigeeLessThan220km) {
        double c1sq = c1 * c1;
        d2 = 4 * semimajorAxis * tsi * c1sq;
        double temp = d2 * tsi * c1 / 3;
        d3 = (17 * semimajorAxis + s4) * temp;
        d4 = 0.5 * temp * semimajorAxis * tsi
                * (221 * semimajorAxis + 31 * s4) * c1;
        t3cof = d2 + 2 * c1sq;
        t4cof = 0.25 * (3 * d3 + c1 * (12 * d2 + 10 * c1sq));
        t5cof = 0.2 * (3 * d4 + 12 * c1 * d3 + 6 * d2 * d2 + 15 * c1sq * (2 * d2 + c1sq));
    }

    // Determine the propagation time
    time_t timer(propTime);
    tm *now = gmtime(&timer);
    int seconds = now->tm_sec;
    int minute = now->tm_min;
    int hour = now->tm_hour;
    int dayOfYear = now->tm_yday + 1; // Days are indexed at 0!
    int year = (now->tm_year)+1900;
    // timeGMT is seconds of the day
    double timeGMT = seconds + (minute*60) + (hour*3600);

    calcECI(year, dayOfYear, timeGMT); // Called with propogation time
}

void sgp4::calcECI(int yearView, int dayView, double stateTime) {

    // The desired GMT epoch time for the satellite (minutes)
    double timeView = stateTime /60.0;
    // The desired GMT epoch time for the satellite (hours)
    double satTime = timeView/60.0;
    // Get the GMT time of day, day, and year for this element set
    double utimeEph = aTLE->getEpochTime();
    int dayEph = aTLE->getEpochDay();
    int yearEph = aTLE->getEpochYear();

    if (yearEph < 50) {
        yearEph += 2000;
    } else {
        yearEph += 1900;
    }

    gmsta = gmst(satTime, dayView, yearView);

    // Determine the number of days in this year
    int daysInYear = getDaysInYear(yearView);

    // Compute the time since the state vector was defined (minutes)
    double elapsedTime = timeView - utimeEph*60.0 + (dayView - dayEph)*24.0*60.0 +
        (yearView - yearEph)*daysInYear*1440.;

    double xmdf = aTLE->getMeanAnomaly() + xmdot * elapsedTime;
    double omgadf = aTLE->getPerigee() + omgdot * elapsedTime;
    double xnoddf = aTLE->getRightAscentionAscendingNode() + xnodot * elapsedTime;
    double omega = omgadf;
    double xmp = xmdf;
    double tsq = elapsedTime * elapsedTime;
    double xnode = xnoddf + xnodcf * tsq;
    double tempa = 1 - c1 * elapsedTime;
    double tempe = aTLE->getDragCoefficient() * c4 * elapsedTime;
    double templ = t2cof * tsq;
    if (!perigeeLessThan220km) {
        double delomg = omgcof * elapsedTime;
        double delm = xmcof
                * (qPow(1 + eta * qCos(xmdf), 3) - delmo);
        double temp = delomg + delm;
        xmp = xmdf + temp;
        omega = omgadf - temp;
        double tcube = tsq * elapsedTime;
        double tfour = elapsedTime * tcube;
        tempa = tempa - d2 * tsq - d3 * tcube - d4 * tfour;
        tempe = tempe + aTLE->getDragCoefficient() * c5 * (qSin(xmp) - sinmo);
        templ = templ + t3cof * tcube + tfour
                * (t4cof + elapsedTime * t5cof);
    }
    double a = semimajorAxis * qPow(tempa, 2);
    double e = aTLE->getEccentricity() - tempe;
    double xl = xmp + omega + xnode + meanMotionAtEpoch * templ;
    double beta = qSqrt(1 - e * e);
    double xn = xke / qPow(a, 1.5);

    // Long period periodics
    double axn = e * qCos(omega);
    double temp = 1 / (a * beta * beta);
    double xll = temp * xlcof * axn;
    double aynl = temp * aycof;
    double xlt = xl + xll;
    double ayn = e * qSin(omega) + aynl;

    // Solve Kepler's Equation
    double capu = fmod((xlt - xnode), (2 * M_PI));
    temp2 = capu;
    double temp4 = 0;
    double temp5 = 0;
    double temp6 = 0;
    double cosepw = 0;
    double sinepw = 0;
    for (int i = 1; i <= 10; i++) {
        sinepw = qSin(temp2);
        cosepw = qCos(temp2);
        temp3 = axn * sinepw;
        temp4 = ayn * cosepw;
        temp5 = axn * cosepw;
        temp6 = ayn * sinepw;
        double epw = (capu - temp4 + temp3 - temp2) / (1 - temp5 - temp6)
                + temp2;
        if (qAbs(epw - temp2) <= 1e-6)
            break;
        temp2 = epw;
    }

    // Short period preliminary quantities

    double ecose = temp5 + temp6;
    double esine = temp3 - temp4;
    double elsq = axn * axn + ayn * ayn;
    temp = 1 - elsq;
    double pl = a * temp;
    double r = a * (1 - ecose);
    temp1 = 1. / r;
    double rdot = xke * qSqrt(a) * esine * temp1;
    double rfdot = xke * qSqrt(pl) * temp1;
    temp2 = a * temp1;
    double betal = qSqrt(temp);
    temp3 = 1 / (1 + betal);
    double cosu = temp2 * (cosepw - axn + ayn * esine * temp3);
    double sinu = temp2 * (sinepw - ayn - axn * esine * temp3);
    double u = qAtan2(sinu, cosu); // ... MAYBE
    double sin2u = 2 * sinu * cosu;
    double cos2u = 2 * cosu * cosu - 1;
    temp = 1 / pl;
    temp1 = CK2 * temp;
    temp2 = temp1 * temp;

    // Update for short periodics

    double rk = r * (1 - 1.5 * temp2 * betal * x3thm1) + 0.5 * temp1
            * x1mth2 * cos2u;
    double uk = u - 0.25 * temp2 * x7thm1 * sin2u;
    double xnodek = xnode + 1.5 * temp2 * cosio * sin2u;
    double xinck = aTLE->getOrbitInclination() + 1.5 * temp2 * cosio * sinio * cos2u;
    double rdotk = rdot - xn * temp1 * x1mth2 * sin2u;
    double rfdotk = rfdot + xn * temp1 * (x1mth2 * cos2u + 1.5 * x3thm1);
    double sinuk = qSin(uk);
    double cosuk = qCos(uk);
    double sinik = qSin(xinck);
    double cosik = qCos(xinck);
    double sinnok = qSin(xnodek);
    double cosnok = qCos(xnodek);
    double xmx = -sinnok * cosik;
    double xmy = cosnok * cosik;
    double ux = xmx * sinuk + cosnok * cosuk;
    double uy = xmy * sinuk + sinnok * cosuk;
    double uz = sinik * sinuk;
    double vx = xmx * cosuk - cosnok * sinuk;
    double vy = xmy * cosuk - sinnok * sinuk;
    double vz = sinik * cosuk;

    // Position and velocity
    double x = rk * ux;
    double y = rk * uy;
    double z = rk * uz;
    double xdot = rdotk * ux + rfdotk * vx;
    double ydot = rdotk * uy + rfdotk * vy;
    double zdot = rdotk * uz + rfdotk * vz;

    double xkm = x * EARTH_RADIUS;
    double ykm = y * EARTH_RADIUS;
    double zkm = z * EARTH_RADIUS;
    double xdotkmps = xdot * EARTH_RADIUS / 60;
    double ydotkmps = ydot * EARTH_RADIUS / 60;
    double zdotkmps = zdot * EARTH_RADIUS / 60;
    eci = new double[6];
    eci[0] = xkm*1000.0;
    eci[1] = ykm*1000.0;
    eci[2] = zkm*1000.0;
    eci[3] = xdotkmps*1000.0;
    eci[4] = ydotkmps*1000.0;
    eci[5] = zdotkmps*1000.0;

}

void sgp4::calcECR() {
    gmstaRad = gmsta*M_PI/180.;
    double sinOfAngle = qSin(gmstaRad);
    double cosOfAngle = qCos(gmstaRad);
    ecr = new double[6];
    ecr[0] =  cosOfAngle*eci[0] + sinOfAngle*eci[1];
    ecr[1] = -sinOfAngle*eci[0] + cosOfAngle*eci[1];
    ecr[2] = eci[2];
    double xd = eci[3] + WEARTH*eci[1];
    double yd = eci[4] - WEARTH*eci[0];
    double zd = eci[5];
    ecr[3] = cosOfAngle*xd + sinOfAngle*yd;
    ecr[4] = -sinOfAngle*xd + cosOfAngle*yd;
    ecr[5] = zd;
}

// Diff between observer in ECR, converted to RRC(ENU)
void sgp4::calcENU() {
    enu = new double[3];
    double xrecr [3];

    double flatcoef, term1;
    double ecrpos[3];

    flatcoef = (requat - rpolar) / requat;
    term1 = qSqrt(1. + (1.-flatcoef) * (1.-flatcoef) * qTan(observerLat*M_PI/180.) * qTan(observerLat*M_PI/180.));

    ecrpos[0] = ( (requat/term1) + observerAlt * qCos(observerLat*M_PI/180.) ) *  qCos(observerLon*M_PI/180.);
    ecrpos[1] = ( (requat/term1) + observerAlt * qCos(observerLat*M_PI/180.) ) *  qSin(observerLon*M_PI/180.);
    ecrpos[2] = (1.-flatcoef)*(1.-flatcoef) * requat * qTan(observerLat*M_PI/180.) / term1 + observerAlt * qSin(observerLat*M_PI/180.);

    for(int i=0; i < 3; i++) {
        xrecr[i] = ecr[i] - ecrpos[i];
    }

    double clat, slat, clon, slon;
    double er2rr [3][3];

    clat = qCos(observerLat*M_PI/180.);
    slat = qSin(observerLat*M_PI/180.);
    clon = qCos(observerLon*M_PI/180.);
    slon = qSin(observerLon*M_PI/180.);

    er2rr[0][0] = -1*slon;
    er2rr[0][1] =  clon;
    er2rr[0][2] =  0.0;
    er2rr[1][0] = -1*slat*clon;
    er2rr[1][1] = -1*slat*slon;
    er2rr[1][2] =  clat;
    er2rr[2][0] =  clat*clon;
    er2rr[2][1] =  clat*slon;
    er2rr[2][2] =  slat;

    for(int i=0; i < 3; i++) {
        enu[i] = 0.;
      for(int j=0; j < 3; j++) {
          enu[i] = enu[i] + er2rr[i][j] * xrecr[j];
      }
    }
}

void sgp4::calcRAEB() {
    double rr2rf [3][3];
    double rr2bor [3][3];
    double xbor [3];
    double rf2bor[3][3] = {{-1.,0.,0.}, {0.,0.,1.}, {0.,1.,0.}};


    double caz, saz, cel, sel;

    caz = qCos(observerBoreAZ*M_PI/180.);
    saz = qSin(observerBoreAZ*M_PI/180.);
    cel = qCos(observerBoreEl*M_PI/180.);
    sel = qSin(observerBoreEl*M_PI/180.);

    rr2rf[0][0] = -1.*caz;
    rr2rf[0][1] =  saz;
    rr2rf[0][2] =  0.0;
    rr2rf[1][0] = -1.*saz * sel;
    rr2rf[1][1] = -1.*caz * sel;
    rr2rf[1][2] =  cel;
    rr2rf[2][0] =  saz * cel;
    rr2rf[2][1] =  caz * cel;
    rr2rf[2][2] =  sel;

    // Clocking angle set to zero means rr2rf is good at this point

    // Matrix product
    int n = 3;
    int m = 3;
    int l = 3;
    for(int i=0; i < n; i++) {
        for(int j=0; j < l; j++) {
            rr2bor[i][j] = 0.0;
            for(int k=0; k < m; k++) {
               rr2bor[i][j] = rr2bor[i][j] + rf2bor[i][k] * rr2rf[k][j];
            }
        }
    }

    for(int i=0; i < n; i++) {
        for(int j=0; j < m; j++) {
            xbor[i] = xbor[i] + rr2bor[i][j] * enu[j];
        }

    }
    // The following is in meters, radians, radians
    raeb = new double[3];
    raeb[0] = qSqrt(xbor[0]*xbor[0] + xbor[1]*xbor[1] + xbor[2]*xbor[2]);
    raeb[1] = qAtan2(xbor[0], xbor[1]);
    raeb[2] = qAtan2(xbor[2],qSqrt(xbor[0]*xbor[0] + xbor[1]*xbor[1]));

    // Convert to degrees
    raeb[1] = raeb[1]*180./M_PI;
    raeb[2] = raeb[2]*180./M_PI;

    if(false) qDebug() << "a";
}


/*
 * Method returns number of days in the input year,
 * accounting for leap years.
 *
 * @param year - (4 digit year, i.e. 1999)
 * @return 366 if leap year, else 365
 */
int sgp4::getDaysInYear(int year) {
    if (year % 400 == 0) {
        return 366;
    } else if (year % 100 == 0) {
        return 365;
    } else if (year % 4 == 0) {
        return 366;
    } else {
        return 365;
    }
}

/*
 * This method converts the year and day of year to the modified julian day
 *
 * @param  year  the year
 * @param  dayofyear  the day of the year
 * @return  the modified julian day
 */
int sgp4::ydoy2mjd(int year, int dayofyear) {

    int mjd = 0;
    int i, month, dayofmonth;
    int cal [] = {31,59,90,120,151,181,212,243,273,304,334,365};
    int cal_leap [] = {31,60,91,121,152,182,213,244,274,305,335,366};

    if(year % 4 == 0) {
        //this is a leap year
        for(i=0; i < 12; i++) {
            if(cal_leap[i] >= dayofyear) {
                month = i + 1;
                if(month == 1) {
                    dayofmonth = dayofyear;
                } else {
                    dayofmonth = dayofyear - cal_leap[month - 2];
                }
                mjd = ymd2mjd(year, month, dayofmonth);
                break;
            }
        }
    } else {
        //this is not a leap year
        for(i=0; i < 12; i++) {
            if(cal[i] >= dayofyear) {
                month = i + 1;
                if(month == 1) {
                    dayofmonth = dayofyear;
                } else {
                    dayofmonth = dayofyear - cal[month - 2];
                }
                mjd = ymd2mjd (year, month, dayofmonth);
                break;
            }
        }
    }
    return mjd;
}

/*
 * This method converts the year, month, and day of month to the modified julian day <br>
 * Note: only valid for years after 1900
 *
 * @param  yy  the year
 * @param  mm  the month
 * @param  dd  the day of the month
 * @return  the modified julian day
 */
int sgp4::ymd2mjd (int yy, int mm, int dd) {

    int yd, mjd;
    int t1901 = 15384;  //days lapsed from Nov 17 1858  to  Dec 31 1900
    double year = 365.25;
    int cal [] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int cal_leap [] = {0,31,60,91,121,152,182,213,244,274,305,335};

    yd = yy - 1900;

    if(yd % 4 == 0) {
        mjd = t1901 + (int) ((yd-1) * year) + cal_leap[mm - 1] + dd;
    } else {
        mjd = t1901 + (int) ((yd-1) * year) + cal[mm - 1] + dd;
    }

    return mjd;
}

double sgp4::gmst(double utime, int iDay, int iYear) {
    double gmsta;
//     THIS SUBROUTINE CALCULATES THE GREENWICH MEAN SIDEREAL TIME ANGLE
//     GIVEN THE UNIVERSAL TIME, DAY OF THE YEAR, AND YEAR
//
//     UTIME = UNIVERSAL TIME 0-24 HOURS GMT
//	   IDAY  = DAY OF THE YEAR: DAY 1-366
//	   IYEAR = THE YEAR E.G., 1999
//
//	   GMSTA = GREENWICH MEAN SIDEREAL TIME ANGLE (DEGREES)
//     APPROXIMATE ANGLE (WITHIN 1/2 DEG) OF VERNAL EQUINOX AT MIDNIGHT,
//     BEGINNING YEAR 2000 (DEG)

    double RK1 = 100.4606184;
//	C     CONVERTS TO EARTH SPIN RATE IN DEG/DAY MOD 360 (DEG/CENTURY)
    double RK2 = 36000.77004;
//	C     ACCOUNT FOR PRECESSION OF EARTH SPIN AXIS (DEG/CENTURY**2)
    double RK3 = 0.000387933;
//	C     EARTH SPIN RATE (DEG/HOUR)
    double RK4 = 15.04106864;
//	C     JULIAN DAY NUMBER BEGINNING AT 12 NOON, JAN 1, 2000 (DAYS)
    double RK5 = 2451545.0;
//	C     DAYS ELAPSED IN A CENTURY, INCLUDING LEAP DAYS (DAYS/CENTURY)
    double RK6 = 36525.0;
//	C     JULIAN DAY NUMBER FOR 0 A.D., BUT PADDED WITH LEAP DAYS FOR
//	C     0 TO 1900 A.D.  (DAYS)
    double RK7 = 1721043.5;
//	C     USED IN CONJUNCTION WITH K9 TO ACCOUNT FOR DAYS PER YEAR, PLUS
//	C     A LEAP DAY EVERY FOUR YEARS (DAYS)
    double RK8 = 367.0;
//	C     SEE RK8  (DAYS)
    double RK9 = 1.75001;
//	C     USED TO CONVERT UT TIME IN HOURS TO DAYS  (HOURS)
    double RK10 = 24.0;

//     CALCULATE THE JULIAN DATE AT 0 HOURS UT OF THE CURRENT DAY OF YEAR
      double  RJD0 = RK7 + RK8*(double)iYear - (double)((int)(RK9*(double)iYear)) +
                     (double)iDay + 0.0/RK10;
//     CALCULATE TIME IN JULIAN CENTURIES SINCE 12 HOURS UT 1 JAN 2000
      double T0 = (RJD0 - RK5) / RK6;
//     CALCUALTE THE GREENWICH MEAN SIDEREAL TIME ANGLE (DEG)
      gmsta = (RK1 + RK2*T0 + RK3*qPow(T0,2) + RK4*utime);
      gmsta = fmod(gmsta,360.);
    return gmsta;
}

void sgp4::initializeVariables() {
    CK2 = 0;
    cosio = 0;
    x3thm1 = 0;
    semimajorAxis = 0;
    meanMotionAtEpoch = 0;
    perigeeLessThan220km = false;
    qoms2t = 0;
    S = 0;
    eta = 0;
    c1 = 0;
    sinio = 0;
    x1mth2 = 0;
    c4 = 0;
    c5 = 0;
    temp1 = 0;
    temp2 = 0;
    temp3 = 0;
    CK4 = 0;
    xmdot = 0;
    omgdot = 0;
    xnodot = 0;
    xnodcf = 0;
    omgcof = 0;
    xmcof = 0;
    t2cof = 0;
    xlcof = 0;
    aycof = 0;
    delmo = 0;
    sinmo = 0;
    x7thm1 = 0;
    d2 = 0;
    d3 = 0;
    d4 = 0;
    t3cof = 0;
    t4cof = 0;
    t5cof = 0;
    gmsta = 0;
    gmstaRad = 0;
}
