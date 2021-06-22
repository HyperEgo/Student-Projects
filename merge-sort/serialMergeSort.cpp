/***************************************************************/
/*
Student: Sunny Patel, Fleance Collins
Professor: Dr. Kulick
Course: CPE 435 - Lab Project
Date: 12/02/2013
Assignment:  MergeSort on 1 machine

Description of a file: This is a serialMergeSort file. Here are the steps it follows.
                     (1) Read data from file
                     (2) Start Timer
                     (3) Run mergeSort
                     (4) Stop Timer

Compile Information: g++ serialMergeSort.cpp -o serialMergeSort

                
*/
/***************************************************************/

using namespace std;

#include <iostream>
#include <sys/socket.h> // socket header files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <iomanip>

#include <cmath>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>

#include <sys/time.h>


/* copied from mpbench */
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   ((tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec)*1000000))
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;


long  FILE_SIZE= 19*256;
#define MAX_CONN 19
#define FILENAME "data.txt"

int a[9728000];	
int b[9728000];
long num=FILE_SIZE;



void merge(int*, int*,long int, long int, long int);

/*
function mergeSort: recursive mergesort calls
    input: 
        a: store data in a array
        b: temperory array
        low: stores lower boundary
        high: stores upper boundary
*/
void mergesort(int *a, int*b, long low, long high)
{
    long pivot;
    if(low<high)
    {
        pivot=(low+high)/2;
        mergesort(a,b,low,pivot);
        mergesort(a,b,pivot+1,high);
        merge(a,b,low,pivot,high);
    }
}

/*
function merge: actual merging
    input: 
        a: store data in a array
        b: temperory array
        low: stores lower boundary
        high: stores upper boundary
        pivot: for index
*/
void merge(int *a, int *b, long low, long pivot, long high)
{
   long  h,i,j,k;
    h=low;
    i=low;
    j=pivot+1;

    while((h<=pivot)&&(j<=high))
    {
        if(a[h]<=a[j])
        {
            b[i]=a[h];
            h++;
        }
        else
        {
            b[i]=a[j];
            j++;
        }
        i++;
    }
    if(h>pivot)
    {
        for(k=j; k<=high; k++)
        {
            b[i]=a[k];
            i++;
        }
    }
    else
    {
        for(k=h; k<=pivot; k++)
        {
            b[i]=a[k];
            i++;
        }
    }

 
    for(k=low; k<=high; k++) a[k]=b[k];
}

/*
function readFile: reads data from file and stores in array
    input: 
        a: store data in a array
        fileSize: total number of elements to read
*/
void readFile(int *a, long fileSize)
{
    ifstream dataFile(FILENAME);
    cout << "in readFile "<< endl;
    for(long i=0;i<fileSize;i++)
	    dataFile >> a[i];
    cout << "in readFile "<< endl;	
    dataFile.close();
}

/*
function main: reads data from file, start timer, run mergeSort, stop timer    
*/
main()
{
    readFile(a,num);	

 	cout << "reading the file is successful" << endl;
	TIMER_CLEAR;
	TIMER_START;    
	
	mergesort(a,b,0,num-1);
	
	TIMER_STOP;   
	
	cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0 
       << " seconds" << endl;
 
}


