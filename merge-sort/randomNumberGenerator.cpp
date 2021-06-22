// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  
	ofstream myfile;
	myfile.open ("dataSize_97280.txt");
	

	for(long int i = 97280 ; i > 0 ; i--) 
	myfile << i <<"\n";
	myfile.close();
	return 0;
}
