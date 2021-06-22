#ifndef LLFROMXY_H
#define LLFROMXY_H

class LLfromXY
{
public:
    // Default Constructor
    LLfromXY();
    // Getter Functions
    double getLatitude(int mouseY);
    double getLongitude(int mouseX);
    // Setter function to override map defaults if desired
    void setMapValues(double mapHeightInPixels, double mapWidthInPixels,
                      double topLatitude, double bottomLatitude,
                      double leftLongitude, double rightLongitude);
    // Inputs for map
    // Map Size in pixels
    double mapSizeX;
    double mapSizeY;

    // Map latitude for top and bottom pixels (Y axis)
    double mapLatTop;
    double mapLatBottom;
    // Map longitude for leftmost and rightmost pixels (X axis)
    double mapLongLeft;
    double mapLongRight;

    // Calculated lat and long to return
    double calculatedLat;
    double calcluatedLong;

private:
    void calculateScaleFactors(void);
    double latSlope;
    double longSlope;

};

#endif // LLFROMXY_H
