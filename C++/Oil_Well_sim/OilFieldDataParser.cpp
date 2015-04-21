//====================================================================
// OilFieldDataParser.cpp
// Implementation file for the data parser.
// Author: Dr. Rick Coleman
// Date: December 2009
//====================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "OilFieldDataParser.h"
#include <string.h>

//-----------------------------------
// Constructor
//-----------------------------------
OilFieldDataParser::OilFieldDataParser(char *fileName)
{
	// Initialize everything
	char line[128];
	m_iNumWells = 0;
	m_iNumSensors = 0;
	m_iNextWellNumber = 0;
	m_ipNextSensorNumber = NULL;
	m_cpWellID = NULL;
	strcpy(m_sFileName, fileName);

	// Open the data file for reading
	inFile = new ifstream();				// Create an ifstream object
	inFile->open(fileName, fstream::in);	// Open it
	if(inFile->is_open())					// Make sure it is open
	{
		// Get the basic information
		while(getNextLine(line, 127))
		{
			if(strcmp(line, "<WELL>") == 0)
			{
				m_iNumWells++;	// Count all wells
			}
			else if(strcmp(line, "<SENSOR>") == 0)
			{
				m_iNumSensors++;	// Count all sensors
			}
		}
		inFile->close();	// Close the file
		delete inFile;		// Destroy the ifstream object
	}
	else
	{
		cout << "Failed to open file\n";
		cout << "Program terminated.\n\n";
		exit(0);
	}

	// Create and initialize the array of ints counting sensors for each well
	m_ipNextSensorNumber = new int[m_iNumWells];
	for(int i=0; i<m_iNumWells; i++)
	{
		m_ipNextSensorNumber[i] = 0;
		// Note: the following is syntatically the same thing
		// *(m_ipNextSensorNumber + i) = 0;
	}

	// Create the array of pointers to char arrays holding the IDs
	//  of each well.  This is used to locate the well sensor counter
	//  in calls to getSensorData()
	m_cpWellID = new char*[m_iNumWells]; // create array of pointers to char

	for(int i=0; i< m_iNumWells; i++)
	{
		m_cpWellID[i] = NULL;
	}

	// Create a new inFile object and reopen the file
	inFile = new ifstream();
	inFile->open(fileName, fstream::in);
	int wellIdx = 0; // Keep up with the well index
	if(inFile->is_open())
	{
		// Get the basic information
		while(getNextLine(line, 127))
		{
			if(strcmp(line, "<WELL>") == 0)
			{ 
				getNextLine(line, 127); // Get the next line which is WELLID tag
				if(strcmp(line, "<WELLID>") == 0)
				{
					if(getNextLine(line, 127))// Get the next line with the well ID
					{
						// Create a char array and set m_cpWellID[wellIdx] pointing to it
						m_cpWellID[wellIdx] = new char[strlen(line) + 1]; 
						strcpy(m_cpWellID[wellIdx], line);// Save the well ID
						wellIdx++;
					}
				}
			}
		}
		inFile->close();	// Close the file
		delete inFile;		// Destroy the ifstream object
	}
	else
	{
		cout << "Failed to open file\n";
		cout << "Program terminated.\n\n";
		exit(0);
	}
}

//-----------------------------------
// Destructor
//-----------------------------------
OilFieldDataParser::~OilFieldDataParser()
{
	// Destroy the arrtay of ints which was dynamically created
	delete[] m_ipNextSensorNumber;

	// Destroy all the char arrays
	for(int i=0; i<m_iNumWells; i++)
	{
		delete[] m_cpWellID[i];
	}

	// Now destroy the array of pointers
	delete[] m_cpWellID;
}

//-----------------------------------
// Get the number of wells
//-----------------------------------
int OilFieldDataParser::getWellCount()
{
	return m_iNumWells;
}

//--------------------------------------------------------------------------
// Get data on the next well
// Args:	*id - pointer to a character array to hold the well ID
//			*operator - pointer to a character array to hold operator name
//			*numSensors - pointer to an int to hold the number of sensors
//							on this well.  For this sim all wells will have
//							the same number.
//--------------------------------------------------------------------------
bool OilFieldDataParser::getWellData(char *id, char *opr, int *numSensors)
{
	int wNum = 0;
	char line[128];
	// Reopen the file
	inFile = new ifstream();
	inFile->open(m_sFileName, fstream::in);
	if(inFile->is_open())
	{
		// Read to the the current well count
		while(getNextLine(line, 127))
		{
			if(strcmp(line, "<WELL>") == 0) // Got one
			{
				if(wNum == m_iNextWellNumber)
				{
					// Get data on this one
					while(getNextLine(line, 127))
					{
						// Get the ID
						if(strcmp(line, "<WELLID>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(id, line); // Set the well ID
							}
							else
								return false; // Oops!
						}
						else if(strcmp(line, "<OPERATOR>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(opr, line); // Set the operator name
							}
							else
								return false; // Oops!
						}
						else if(strcmp(line, "</WELL>") == 0)
						{
							m_iNextWellNumber++; // Increment for next call to this function
							inFile->close();
							delete inFile; // Delete the istream object not the file
							// Set the number of sensors -  all have the same for now
							*numSensors = m_iNumSensors;
							return true; // Got it
						}
					} // end while
				} // end if(wNum == nextWell)
				else
				{
					wNum++; // Check the next one
				}
			}
		}
		inFile->close();
		delete inFile; // Delete the istream object not the file
	} // end if file open
	return false; // If we get here we have got all the wells

}

//-----------------------------------
// Get data on the next sensor
//-----------------------------------
bool OilFieldDataParser::getSensorData(char *wellID, char *type, char *className, 
		char *displayName, double *min, double *max, char *units, char *unitAbbrev)
{
	// Note: In this simulation all wells have the same number of sensors
	// Find the index of this well
	int wellIdx = 0;
	for(int i=0; i<m_iNumWells; i++)
	{
		if(strcmp(m_cpWellID[i], wellID) == 0)
			wellIdx = i;
	}

	int sNum = 0;
	char line[128];

	// Reopen the file
	inFile = new ifstream();
	inFile->open(m_sFileName, fstream::in);
	if(inFile->is_open())
	{
		// Read to the the current sensor count
		while(getNextLine(line, 127))
		{
			if(strcmp(line, "<SENSOR>") == 0) // Got one
			{
				if(sNum == m_ipNextSensorNumber[wellIdx])
				{
					// Get data on this one
					while(getNextLine(line, 127))
					{
						// Get the sensor type
						if(strcmp(line, "<TYPE>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(type, line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor class name
						else if(strcmp(line, "<CLASS>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(className, line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor display name
						else if(strcmp(line, "<DISPLAYNAME>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(displayName, line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor minimum value
						else if(strcmp(line, "<MINVALUE>") == 0)
						{
							if(getNextLine(line, 127))
							{
								*min = atof(line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor minimum value
						else if(strcmp(line, "<MAXVALUE>") == 0)
						{
							if(getNextLine(line, 127))
							{
								*max = atof(line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor units
						else if(strcmp(line, "<UNITS>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(units, line); 
							}
							else
								return false; // Oops!
						}
						// Get the sensor units abbreviation
						else if(strcmp(line, "<UNITABBREVIATION>") == 0)
						{
							if(getNextLine(line, 127))
							{
								strcpy(unitAbbrev, line); 
							}
							else
								return false; // Oops!
						}
						else if(strcmp(line, "</SENSOR>") == 0)
						{
							m_ipNextSensorNumber[wellIdx]++; // Increment for next call to this function
							inFile->close();
							delete inFile; // Delete the istream object not the file
							return true; // Got one
						}
					} // end while
				} // end if(sNum == nextSensor)
				else
				{
					sNum++; // Check the next one
				}
			}
		}
		inFile->close();
		delete inFile; // Delete the istream object not the file
	} // end if file open
	return false; // If we get here we have got all the sensors or failed to open file
}

//------------------------------------------------
// Function: getNextLine()
// Purpose: Reads lines from a file and places
//   them in buffer, removing any leading white
//   space.  Skips blank lines. Ignors comment
//   lines starting with <!-- and ending with -->
//   
// Args:  buffer -- char array to read line into.
//        n -- length of buffer.
// Returns: True if a line was successfully read,
//    false if the end of file was encountered.
// Notes: Function provided by instructor.
//------------------------------------------------
bool OilFieldDataParser::getNextLine(char *buffer, int n)
{
    bool    done = false;
	char    tempBuf[128];
	char	*temp;
    while(!done)
    {
        inFile->getline(tempBuf, n); // Read a line from the file

        if(inFile->good())          // If a line was successfully read check it
        {
           if(strlen(tempBuf) == 0)     // Skip any blank lines
               continue;
		   else if(strncmp(tempBuf, "<!--", 4) == 0) // Skip comment lines
			   continue;
           else done = true;    // Got a valid data line so return with this line
        }
        else
        {
            strcpy(buffer, "");  // Clear the buffer array
            return false;        // Flag end of file
        }
    } // end while
	// Remove white space from end of string
	temp = &tempBuf[strlen(tempBuf)]; // point to closing \0
	temp--; // back up 1 space
	while(isspace(*temp))
	{
		*temp = '\0'; // Make it another NULL terminator
		temp--;  // Back up 1 char
	}
	// Remove white space from front of string
	temp = tempBuf;
	while(isspace(*temp)) temp++; // Skip leading white space
	// Copy remainder of string into the buffer
	strcpy(buffer, temp);
    return true;  // Flag a successful read
}

//------------------------------------------------
// For testing only
//------------------------------------------------
void OilFieldDataParser::printWellData()
{
	// Print information stored in m_cpWellID
	for(int i=0; i<m_iNumWells; i++)
		cout << m_cpWellID[i] << endl;
}




