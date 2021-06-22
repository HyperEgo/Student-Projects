#include "llfromxy.h"

LLfromXY::LLfromXY() {
    // Hardwired for the map image I chose
    // Change these if another map image is selected
    mapSizeX = 469; // image is 469 pixels wide 430, 230
    mapSizeY = 247; // image is 247 pixels tall
    // Map latitude for top and bottom edges (Y axis)
    mapLatTop = 50; // 50 degrees latitude is top of image
    mapLatBottom = 25; // 25 degrees latitude is bottom of image
    // Map longitude for leftmost and rightmost edges (X axis)
    mapLongLeft = -130; // -130 degrees is left edge of image
    mapLongRight = -65; // -65 degrees is right edge of image

    // Calculate the scale factors
    calculateScaleFactors();
}

void LLfromXY::setMapValues(double mapHeightInPixels,
                                     double mapWidthInPixels,
                                     double topLatitude,
                                     double bottomLatitude,
                                     double leftLongitude,
                                     double rightLongitude) {
    mapSizeX = mapHeightInPixels;
    mapSizeY = mapWidthInPixels;
    mapLatTop = topLatitude;
    mapLatBottom = bottomLatitude;
    mapLongLeft = leftLongitude;
    mapLongRight = rightLongitude;

    // Calculate the scale factors
    calculateScaleFactors();
}

void LLfromXY::calculateScaleFactors(void) {
    // Simple linear equations are close enough
    // Determine the slope for Latitude
    latSlope = (mapLatTop - mapLatBottom)/ (0. - mapSizeX);
    // Determine the slope for Longitude
    longSlope = (mapLongLeft - mapLongRight) / (0. - mapSizeY);
}

double LLfromXY::getLatitude(int mouseY) {
    return latSlope*mouseY + mapLatTop;
}

double LLfromXY::getLongitude(int mouseX) {
    return longSlope*mouseX + mapLongLeft;
}
