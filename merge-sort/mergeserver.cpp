/***************************************************************/
/*
Student: Sunny Patel, Fleance Collins
Professor: Dr. Kulick
Course: CPE 435 - Lab Project
Date: 12/02/2013
Assignment:  MergeSort on 20 machines

Description of a file: This is a mergeserver file. Here are the steps it follows.
                     (1) Read data from file
                     (2) Wait for 19 clients to connect
                     (3) Create threads per client to handle send and receive
                     (4) Wait for all threads to finish execution
                     (5) Run the final mergeSort

Compile Information: g++ mergeserver.cpp -o mergeserver -lpthread

Files Generated:  server.txt ===> contains the final sorted array
                  inputfile.txt ===> contains copy of what is being sent to client
                  initialData.txt ===> contains what is being read from file 
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

ofstream myfile;
ofstream inputfile;
ofstream initialdata;
string Result; 


#define FILE_SIZE 19*256*10*2  // total file size 
#define MAX_CONN 19            // Maximum connection
#define SEND_SIZE 256


#define FILENAME "dataSize_97280.txt"   // File to open

#define MY_PORT       5656
#define SIZE         1024
#define MY_UNSERVED  20
#define RECV_NUM FILE_SIZE/MAX_CONN

#include <sys/time.h>

/* copied from mpbench */
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   ((tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec)*1000000))
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

pthread_mutex_t the_mutex;
pthread_barrier_t barr;

// extra variables
int num=FILE_SIZE;
int b[FILE_SIZE];
int num_connection = 0;
int a[FILE_SIZE];	

// Thread structure to hold information about socket and id
struct threadInfo{
  int id;
  int second_socket; 
  int num_connection;   
};


/*
function readFile: reads data from file and stores in array
    input: 
        a: store data in a array
        fileSize: total number of elements to read
*/
void readFile(int *a, int fileSize)
{
	ifstream dataFile(FILENAME);
	for(int i=0;i<fileSize;i++)
		dataFile >> a[i];
	dataFile.close();
}


void merge(int*,int*,int,int,int);

/*
function mergeSort: recursive mergesort calls
    input: 
        a: store data in a array
        b: temperory array
        low: stores lower boundary
        high: stores upper boundary
*/
void mergesort(int *a, int*b, int low, int high)
{
    int pivot;
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
void merge(int *a, int *b, int low, int pivot, int high)
{
    int h,i,j,k;
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
function calcualtion: thread function
    input: 
         arg: convert it to threadInfo structure         
*/
void *calculation(void *arg)
{
    threadInfo threadData = *((threadInfo *) arg);
      
    int flags = 0;
    int points[2];
    int send_status; 
    int rcv_count;
    int rc;
        
    points[0] = num/MAX_CONN*threadData.num_connection;
    points[1] = (num/MAX_CONN*(threadData.num_connection+1)-1);
	
    int index = points[0]; 
    int len = 1024;
	
    pthread_mutex_lock(&the_mutex);	
    send_status = send(threadData.second_socket,points,sizeof(points),flags);
    pthread_mutex_unlock(&the_mutex);

    int k;

    pthread_mutex_lock(&the_mutex);	
    int addr;

    for(int i = 0; i < (RECV_NUM*sizeof(int)/1024) ; i++)
    {
        addr = index + ((i*1024)/4);

        send_status = send(threadData.second_socket,&a[addr],len,flags);


        for(int k = SEND_SIZE*i + index; k < ((i+1)* SEND_SIZE+ index) ; k++)
        inputfile << a[k] << endl;


        if (send_status<0) {
            cout << "from thread id " << threadData.id << endl;
            perror("error on send after sending data\n");
        }

    }
	 
    cout << "sent from thread id " << threadData.id << endl;
    pthread_mutex_unlock(&the_mutex);	

    /* Barrier, which actually reduced accuracy */
    rc = pthread_barrier_wait(&barr);

    pthread_mutex_lock(&the_mutex);		 
    int add ;//= index;
    for(int i = 0; i < (RECV_NUM*sizeof(int)/1024); i++)
    {
        add = index + ((i*1024)/4);
	
        while((rcv_count=recv(threadData.second_socket,&a[add],len,flags))<=0);
		        if(rcv_count < 0)
        {
        cout << "from thread id " << threadData.id << endl;
         perror("error on receiving from thread\n");
         }

     
    }
	 cout << "receiving from thread id " << threadData.id << endl;
	pthread_mutex_unlock(&the_mutex);		
	pthread_exit(0);
}


/*
function main: reads data from file, listens for client connections,
               creates a thread per client to handle send and receive,
               waits until all threads finish execution and run final mergeSort    
*/
int main(int argc, char * argv[]) {
   
    struct sockaddr_in first_addr,client_addr;
    int listen_status;
    int send_status;
    int bind_status;
    int rcv_count;
    int buffer_len;
    int flags;
    int i;
    socklen_t client_addr_len;
    int max_count;
    int iteration;
    int my_pid;
    int first_socket;
    int second_socket[20];
    int status;
	
    threadInfo con_id_array[20];
    pthread_t con_thread[20];   

    pthread_mutex_init(&the_mutex, 0);
    if(pthread_barrier_init(&barr,
    NULL, MAX_CONN))
    {
        printf("Could not create a barrier\n");
        return -1;
    }

    Result = "server.txt";
    inputfile.open("inputfile.txt");
    initialdata.open("intialData.txt");

    readFile(a,num);					// fills buf with data from file
	
	for(int k = 0 ; k < num ; k++)
		    initialdata << a[k] << endl;
	     
    initialdata.close();
		
   
    // create a socket
    first_socket = socket(PF_INET,SOCK_STREAM,0);

    if (first_socket<0) {
      perror("error opening the first socket:");
      exit(1);
    }
 
    // bind it to a port and advertise it
    // type is inet, port is my personal number,
    // INADDR_ANY means use my current ip number
    first_addr.sin_family = AF_INET;
    first_addr.sin_port = htons(MY_PORT);
    first_addr.sin_addr.s_addr = INADDR_ANY;
    bind_status = bind(first_socket,(struct sockaddr *) & first_addr,
      sizeof(struct sockaddr_in));
    if (bind_status<0) {
      perror("error on binding first_socket:");
      exit(1);
    }

    // inform the system how many unserved connections are allowed
    listen_status = listen(first_socket,MY_UNSERVED);
    if (listen_status<0) {
      perror("error on listen for fist socket:");
      exit(1);
    }

    // wait and accept connections
    while (num_connection < MAX_CONN) {
	
      second_socket[num_connection] = accept(first_socket, (struct sockaddr *)&client_addr,
      &client_addr_len);
      if (second_socket<0) {
         perror("error on accept on first socket:");
         exit(1);
      }
	
    cout << "I accepted a connection from " <<
        inet_ntoa(client_addr.sin_addr) <<
        " Port " << ntohs(client_addr.sin_port)  << endl;

    cout << "second socket is " << second_socket[num_connection]<< endl;
    num_connection++;
    }

    // Start the timer
    TIMER_CLEAR;
    TIMER_START;

    cout << "creating threads " << endl;	
    for(int i = 0; i < num_connection; i++)
    {
        con_id_array[i].id = i;
        con_id_array[i].second_socket = second_socket[i];
        con_id_array[i].num_connection = i;

        status = pthread_create(&con_thread[i], NULL, calculation, (void *) &con_id_array[i]);
        if (status != 0)
	        cout << "error on pthread_create i " << i <<" " << status << ":" <<
	           strerror(status) << endl;
        }

    // Wait for all threads to finish their job    
    for(int i = 0; i < num_connection; i++)
    {
	    status = pthread_join(con_thread[i], NULL);
	    if (status != 0)
		    cout << "error on pthread_join " << status << ":" << strerror(status)
	      << endl;
    }      

    cout << "server's mergersort " << endl;
    mergesort(a,b,0,num-1);
    TIMER_STOP;  

    // write the data to server.txt 
    myfile.open (Result.c_str());	
    cout << "final sorted array " << endl;
    
    for(int i=0; i<FILE_SIZE; i++)
	    myfile<<a[i]<<endl;	 

    cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0 
       << " seconds" << endl;

    inputfile.close();
} 
