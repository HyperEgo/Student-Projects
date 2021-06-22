#include "parsesatdata.h"
#include <qmath.h>
#include "Constants.h"
#include "satdata.h"

parseSatData::parseSatData() {
    // Default constructor
    // Don't use this
}

parseSatData::parseSatData(QFile* aFile) {
    theCatalog = aFile;
    qDebug() << "Valid file to constructor1? " << theCatalog->exists();
}

parseSatData::parseSatData(QString fileAndPath) {
    qDebug() << fileAndPath;
    theCatalog = new QFile(fileAndPath);
    qDebug() << "Valid file to constructor2? " << theCatalog->exists();
}

parseSatData::~parseSatData() {
}

void parseSatData::parseOne() {

    // This function just gets the first elset in the catalog
    // To parse the whole catalog, just use this code and iterate until the file is done

    // Open the file for reading
    theCatalog->open(QIODevice::ReadOnly);

    // Read three lines
    QString line0 = theCatalog->readLine();
    QString line1 = theCatalog->readLine();
    QString line2 = theCatalog->readLine();

    // Parse the lines

    // LINE 0
    // Just the name
    int lineNum0 = (line0.mid(0,1).trimmed()).toInt(); // Integrity check.
    if(lineNum0 != 0) {return;} // If line number is not '0', skip remainder of code and re-check input
    QString satName = (line0.mid(2,24).trimmed());

    // LINE 1
    // Line1 contains the following useful things:
    // line number, NORAD_ID, launch year, launch number, epoch year day and time, drag coefficient, and checksum
    int lineNum1 = (line1.mid(0,1).trimmed()).toInt(); // Use this for consistency checks in testing, ignored for now
    if(lineNum1 != 1) {return;}  // If line number is not '1', skip remainder of code and re-check input
    int noradID = (line1.mid(2,5).trimmed()).toInt();

    int launchYear = (line1.mid(9,2).trimmed()).toInt();
    if (launchYear >= 57) {
        launchYear += 1900;
    } else {
        launchYear += 2000;
    }
    int launchNum = (line1.mid(11,3).trimmed()).toInt();

    double fullEpoch = (line1.mid(18,14).trimmed()).toDouble();
    int epochYear = (line1.mid(18,2).trimmed()).toInt();
    double dEpoch = (line1.mid(20,12).trimmed()).toDouble();
    int iDay = (int)dEpoch;
    double uTime = (dEpoch - iDay)*24;

    double d1 = (line1.mid(53, 6).trimmed()).toInt();
    double d2 = (line1.mid(59,2).trimmed()).toDouble();
    // Verify w/ the mikes, think I might have this one wrong
    double dragCoefficient = (d1*1e-5)* qPow(10, d2);

    // Checksum for consistency checks in testing, ignored for now
    int checksum1 = (line1.mid(68,1).trimmed()).toInt();

    // LINE 2
    // Line2 contains the following useful things:
    // line number, NORAD_ID, inclination, rightAscentionAscendingNode, eccentricity, perigee, mean anomaly, mean motion, revolution number at epoch, and a checksum
    int lineNum2 = (line2.mid(0,1).trimmed()).toInt(); // Use this for consistency checks in testing, ignored for now
    if(lineNum2 != 2) {return;} // If line number is not '2', skip remainder of code and re-check input
    int noradID2 = (line2.mid(2,5).trimmed()).toInt(); // Use this for consistency checks in testing (ID must match line1), ignored for now
    // Orbital elements
    double orbitInclination = ((line2.mid(8,8).trimmed()).toDouble())*deg2rad;
    double rightAcsentionAscendingNode = ((line2.mid(17,8).trimmed()).toDouble())*deg2rad;
    double orbitEccentricity = ((line2.mid(26,7).trimmed()).toDouble())*1e-7; // Pretty big assumption on their part
    double orbitPerigee = ((line2.mid(34,8).trimmed()).toDouble())*deg2rad;
    double meanAnomaly = ((line2.mid(43,8).trimmed()).toDouble())*deg2rad;
    double meanMotion = ((line2.mid(52,11).trimmed()).toDouble())*2*M_PI/1440;
    // Checksum for consistency checks in testing, ignored for now
    int checksum2 = (line2.mid(68,1).trimmed()).toInt();

    // DEBUG LINES TO VERIFY THE PARSING

    // Confirm open via debug
    qDebug() << "Catalog is open: " << theCatalog->isOpen();
    qDebug() << "Catalog is readable: " << theCatalog->isReadable();
    // Confirm contents via debug
    qDebug() << line0;
    qDebug() << line1;
    qDebug() << line2;
    // Now confirm the variables
    qDebug() << "Satellite name: " << satName;
    qDebug() << "Norad ID: " << noradID;
    qDebug() << "The #" << launchNum << " satellite launched in " << launchYear;
    qDebug() << "Epoch: year: " << epochYear << " day: " << iDay << " time (UTC): " << uTime;
    qDebug() << "d1: " << d1 << " d2: " << d2 << ", drag: " << dragCoefficient;
    qDebug() << "checksum for line 1: " << checksum1;
    qDebug() << "Norad ID: " << noradID2 << " better match!";
    qDebug() << "Inclination: " << orbitInclination;
    qDebug() << "Right Ascention of the Ascending Node: " << rightAcsentionAscendingNode;
    qDebug() << "Orbit Eccentricity: " << orbitEccentricity;
    qDebug() << "Orbit Perigee: " << orbitPerigee;
    qDebug() << "Mean Anomaly: " << meanAnomaly;
    qDebug() << "Mean Motion: " << meanMotion;
    qDebug() << "checksum for line 2: " << checksum2;


    // Now create and set a single DemoTLE object

    satData sdata;
    sdata.setSatName(satName);
    sdata.setNoradID(noradID);
    sdata.setLaunchYear(launchYear);
    sdata.setLaunchNumber(launchNum);
    sdata.setFullEpoch(fullEpoch);
    sdata.setEpochYear(epochYear);
    sdata.setEpochDay(iDay);
    sdata.setEpochTime(uTime);
    sdata.setDragCoefficient(dragCoefficient);
    sdata.setOrbitInclination(orbitInclination);
    sdata.setRightAscentionAscendingNode(rightAcsentionAscendingNode);
    sdata.setEccentricity(orbitEccentricity);
    sdata.setPerigee(orbitPerigee);
    sdata.setMeanAnomaly(meanAnomaly);
    sdata.setMeanMotion(meanMotion);



    // DemoTLE is complete
    // In a loop, we can add the DemoTLE objects to a list (satList - see the getter below)
    // When the list is built, we can interface with Cindy's work and populate the table

}

void parseSatData::parseAll() {

    // Open the file for reading
    theCatalog->open(QIODevice::ReadOnly);

    int count = 0;
    qDebug() << "begin";
    while (!(theCatalog->atEnd())) {

        // Read the first three lines
        QString line0 = theCatalog->readLine();
        int lineNum0 = (line0.mid(0,1).trimmed()).toInt(); // Integrity check.
        if(lineNum0 != 0) {continue;} // If line number is not '0', skip remainder of code and re-check input
        QString line1 = theCatalog->readLine();
        int lineNum1 = (line1.mid(0,1).trimmed()).toInt(); // Use this for consistency checks
        if(lineNum1 != 1) {continue;}  // If line number is not '1', skip remainder of code and re-check input
        QString line2 = theCatalog->readLine();
        int lineNum2 = (line2.mid(0,1).trimmed()).toInt(); // Use this for consistency checks
        if(lineNum2 != 2) {continue;} // If line number is not '2', skip remainder of code and re-check input

        // Parse the lines

        // LINE 0
        // Just the name
        QString satName = (line0.mid(2,24).trimmed());

        // LINE 1
        // Line1 contains the following useful things:
        // line number, NORAD_ID, launch year, launch number, epoch year day and time, drag coefficient, and checksum
        int noradID = (line1.mid(2,5).trimmed()).toInt();

        int launchYear = (line1.mid(9,2).trimmed()).toInt();
        if (launchYear >= 57) {
            launchYear += 1900;
        } else {
            launchYear += 2000;
        }
        int launchNum = (line1.mid(11,3).trimmed()).toInt();

        double fullEpoch = (line1.mid(18,14).trimmed()).toDouble();
        int epochYear = (line1.mid(18,2).trimmed()).toInt();
        double dEpoch = (line1.mid(20,11).trimmed()).toDouble();
        int iDay = (int)dEpoch;
        double uTime = (dEpoch - iDay)*24;

        double d1 = (line1.mid(53, 6).trimmed()).toInt();
        double d2 = (line1.mid(59,2).trimmed()).toDouble();
        // Apply the assumed decimal and exponent
        double dragCoefficient = (d1*1e-5)* qPow(10, d2);

        // Checksum for consistency checks in testing, ignored for now
        int checksum1 = (line1.mid(68,1).trimmed()).toInt();

        // LINE 2
        // Line2 contains the following useful things:
        // line number, NORAD_ID, inclination, rightAscentionAscendingNode, eccentricity, perigee, mean anomaly, mean motion, revolution number at epoch, and a checksum
        int noradID2 = (line2.mid(2,5).trimmed()).toInt(); // Use this for consistency checks in testing (ID must match line1), ignored for now
        // Orbital elements
        double orbitInclination = ((line2.mid(8,8).trimmed()).toDouble())*deg2rad;
        double rightAcsentionAscendingNode = ((line2.mid(17,8).trimmed()).toDouble())*deg2rad;
        double orbitEccentricity = ((line2.mid(26,7).trimmed()).toDouble())*1e-7; // Apply assumed decimal point
        double orbitPerigee = ((line2.mid(34,8).trimmed()).toDouble())*deg2rad;
        double meanAnomaly = ((line2.mid(43,8).trimmed()).toDouble())*deg2rad;
        double meanMotion = ((line2.mid(52,11).trimmed()).toDouble())*2*M_PI/1440;
        // Checksum for consistency checks in testing, ignored for now
        int checksum2 = (line2.mid(68,1).trimmed()).toInt();


        // Now create and set a single DemoTLE object to add to the list

        satData sdata;
        sdata.setSatName(satName);
        sdata.setNoradID(noradID);
        sdata.setLaunchYear(launchYear);
        sdata.setLaunchNumber(launchNum);
        sdata.setFullEpoch(fullEpoch);
        sdata.setEpochYear(epochYear);
        sdata.setEpochDay(iDay);
        sdata.setEpochTime(uTime);
        sdata.setDragCoefficient(dragCoefficient);
        sdata.setOrbitInclination(orbitInclination);
        sdata.setRightAscentionAscendingNode(rightAcsentionAscendingNode);
        sdata.setEccentricity(orbitEccentricity);
        sdata.setPerigee(orbitPerigee);
        sdata.setMeanAnomaly(meanAnomaly);
        sdata.setMeanMotion(meanMotion);

        satList.push_back(sdata);
        count++;
    }
    qDebug() << "Sats parsed: " << count;
}

QList<satData> parseSatData::returnResults() {
    emit finishedParsing();
    return satList;
}


