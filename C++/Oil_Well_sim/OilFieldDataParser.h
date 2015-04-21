//====================================================================
// OilFieldDataParser.h
// Interface file for the data parser.
// Author: Dr. Rick Coleman
//====================================================================
#ifndef OILFIELDDATAPARSER_H
#define OILFIELDDATAPARSER_H

#include <fstream>
#include <iostream>

using namespace std;

class OilFieldDataParser
{
	private:
		ifstream	*inFile;				// Oilfield data definition file
		int			m_iNumWells;			// Number of wells in the data file
		int			m_iNumSensors;			// Number of sensors on each well
		int			m_iNextWellNumber;		// Index of the next well to read
		char		**m_cpWellID;			// Pointer to array of pointers to
											//  char arrays holding well IDs
		int			*m_ipNextSensorNumber;	// Pointer to array of ints counting
											//  sensors for each well
		char        m_sFileName[64];		// Data file

	public:
		OilFieldDataParser(char *fileName);	// Constructor
		~OilFieldDataParser();				// Destructor
		int getWellCount();
		bool getWellData(char *id, char *opr, int *numSensors);
		bool getSensorData(char *wellID, char *type, char *className, 
			char *displayName, double *min, double *max, char *units, char *unitAbbrev);
		void printWellData(); // For testing only
	private:
		bool getNextLine(char *buffer, int n);

};
#endif