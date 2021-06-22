#ifndef SATDATA_H
#define SATDATA_H

#include <QString>

class satData
{

public:
    satData();

private:
    // FORMAT
    // Indices are one less than columns on the website
    // Variable // indices // comments

    // ELSET LINE 0
    QString satName; // 0-23 // ISS (ZARYA)

    // ELSET LINE 1
    QString lineNum1; // 0 // can use to check validity of catalog
    int ln1; // parse the above
    QString satNumber1; // 2-6 // NORAD ID
    int noradID; // convert above to integer
    QString classification; // 7 // All in the catalog are unclassified (U)

    // International designation contains some useful info on the launch
    int launchYear; // 9-10 // launch year
    int launchNumber; // 11-13 // launch number of that year
    QString pieceOfLaunchVehicle; // 14-16 // not really useful for us

    // Elset Epoch is the time this observation was valid
    // Get the full epoch before paring it down - can use for sorting if required
    QString fullEpochString;
    double fullEpoch;
    // Elset Epoch is the time this observation was valid
    QString epochYear; // 18-19 // This is the year this observation was made (hopefully w/in a few days!)
    int iYear; // convert above to int, add 2000 (if < 57) else add 1900
    QString epoch; // 20-31 // time of the year for this observation
    double dEpoch; // convert above to a double
    int iDay; // integer potion of above is the day of the year
    double uTime; // (dEpoch - iDay)*24 // this is the UTC time

    // Derivatives of Mean Motion.  2nd may be in exponential form but always seems to be zero
    QString firstDerivMeanMotion; // 33-42 // contains a double, may not use for SGP4
    QString secondDerivMeanMotion; // 44-51 // always zero?  appears to be exponential form

    // B* is the drag term in exponential form
    QString bStar; // 53-60 // Will parse this string into doubles
    double d1; // parse 53-57
    double d2; // parse 59-60 (exponent)
    double dragCoefficient; // (d1 * 1e-5)* 1e^d2 // not sure how to phrase this

    QString ephemerisType; // 62 // Appears to be always 0
    QString elementNumber; // 64-67 // Not sure if used
    QString checkSum_1; // 68 / checksum for line 1
    int check1; // convert above to integer

    // ELSET LINE 2
    QString lineNum2; // 0 // can use to check validity of catalog
    int ln2; // parse the above
    QString satNumber2; // 2-6 // NORAD ID - better be same as satNumber1 above, reject if mismatch

    // Orbital elements follow
    QString inclination; // 8-15 // orbital inclination (degrees)
    double orbitInclination;// convert above to double * PI / 180 // degrees to radians
    QString raan; // 17-24 // right ascention of ascending node (degrees)
    double rightAscentionAscendingNode; // convert above to double * PI / 180 // degrees to radians
    QString eccentricity; // 26-32 // decimal point assumed
    double orbitEccentricity; // convert above to double * 1e-7
    QString perigee; // 34-41 // argument of perigee (degrees)
    double orbitPerigee; // convert above to double * PI / 180 // degrees to radians
    QString anomaly; // 43-50 // meaqn anomaly (degrees)
    double meanAnomaly; // convert above to double * PI / 180 // degrees to radians
    QString motion; // 52-62 // mean motion (revolutions per day)
    double meanMotion; // convert above to double * 2 PI / 1440 // 1440 is minutes per day
    QString revolutionNumberAtEpoch; // 63-67 // Not sure if used
    QString checksum_2; // 68 // checksum for line 2
    int check2; // convert above to integer

    // NOTE ON CHECKSUMS - sum of all characters, modulo 10.  Non-numeric '-' gets 1, all others get 0


public:

    // Setters for the variables I think we'll need, might add more later
    void setSatName(QString name);
    void setNoradID(int id);
    void setLaunchYear(int year);
    void setLaunchNumber(int number);
    void setFullEpoch(double epoch);
    void setEpochYear(int year);
    void setEpochDay(int day);
    void setEpochTime(double time);
    void setDragCoefficient(double coef);
    void setOrbitInclination(double inclination);
    void setRightAscentionAscendingNode(double ascention);
    void setEccentricity(double eccentricity);
    void setPerigee(double perigee);
    void setMeanAnomaly(double anomaly);
    void setMeanMotion(double motion);


    // Getters for the variables I think we'll need, might add more later
    QString getSatName(void);
    int getNoradID(void);
    int getLaunchYear(void);
    int getLaunchNumber(void);
    double getFullEpoch(void);
    int getEpochYear(void);
    int getEpochDay(void);
    double getEpochTime(void);
    double getDragCoefficient(void);
    double getOrbitInclination(void);
    double getRightAscentionAscendingNode(void);
    double getEccentricity(void);
    double getPerigee(void);
    double getMeanAnomaly(void);
    double getMeanMotion(void);
    QString toString(void);



};

#endif // SATDATA_H
