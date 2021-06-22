/***************************************************************/
/*
Student: Sunny Patel, Fleance Collins
Professor: Dr. Kulick
Course: CPE 435 - Lab Project
Date: 12/02/2013
Assignment:  MergeSort on 20 machines

Description of a file: This is a mergeclient file. Here are the steps it follows.
                     (1) Connect to server
                     (2) Receive data from server
                     (3) Create 8 threads to do mergeSort
                     (4) Wait for all threads to finish execution
                     (5) Run the final mergeSort on received data
                     (6) Send data back to server

Compile Information: g++ mergeclient.cpp -o mergeclient -lpthread

Files Generated:  "[BegginningNumberOfClientDataSize]".txt ===> contains the received data from server 
                    for example: 0.txt, "2560.txt"
                 
*/
/***************************************************************/

// secondb.cpp -- client
using namespace std;
#include <iostream>
#include <sys/socket.h> // socket header files
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fstream>
#include <sstream>

#define FILE_SIZE 19*256*10*2
#define MAX_CONN 19
#define RECV_NUM FILE_SIZE/MAX_CONN

#define MY_PORT      5656 // port number
#define SIZE         1024
#define MY_UNSERVED  5
#define SLEEP_TIME   4000000
#define MAX_THREADS 8

pthread_mutex_t the_mutex;

int a[FILE_SIZE/MAX_CONN] ;

int num=FILE_SIZE/MAX_CONN;

struct threadInfo{
  int id;
  int second_socket; 
  int num_connection; 
  int start;
  int end;
};

// ip address of the EB246 machines where server resides
#define EB24606  "172.21.246.6"
#define EB24607  "172.21.246.7"
#define EB24608  "172.21.246.8"
#define EB24609  "172.21.246.9"
#define EB24610 "172.21.246.10"
#define EB24611 "172.21.246.11"
#define EB24612 "172.21.246.12"
#define EB21605 "172.21.216.5"          // this is used
#define BLACKHAWK "172.21.0.47"

int b[FILE_SIZE/MAX_CONN];

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
void * calculation(void *arg)
{
    threadInfo threadData = *((threadInfo *) arg);

    int b[threadData.num_connection]; 

    pthread_mutex_lock(&the_mutex);
    mergesort(a,b,threadData.start,threadData.end); // mergeSort
    pthread_mutex_unlock(&the_mutex);    

    pthread_exit(0);
}

/*
function main: connects to server, receives data from server, creates 8 threads,
               threads sort chunk, main process waits for all threads to be completed, 
               and run the final merge sort on received data.    
*/
int main(int argc, char * argv[]) {
    
    struct sockaddr_in first_addr,client_addr;
    char buf[SIZE];
    int data[SIZE];
    int points[2];
    int child_pid;
    int listen_status;
    int accept_status;
    int bind_status;
    int rcv_count;
    int buffer_len;
    int flags;
    int i;
    socklen_t client_addr_len;
    int max_count;
    int send_status;
    int connect_status;
    int my_pid;
    int iteration;
    int first_socket;

    threadInfo con_id_array[8];
    pthread_t con_thread[8];   
    pthread_mutex_init(&the_mutex, 0);

    int base_sz;
    int extra;

    ofstream myfile;
    string Result;          // string which will contain the result


   /* args are socket(address family, type, protocol)
      where family is always PF_INET, type is SOCK_STREAM for connected link,
      0 lets the system choose the underlying ipc protocol
   */

   // create a socket
   first_socket = socket(PF_INET,SOCK_STREAM,0);

   if (first_socket<0) {
      perror("error opening the first socket:");
      exit(1);
   }

   // connect to it
   // Note: function getaddrinfo(); can now be used instead of directly
   //       accessing this structure  
   first_addr.sin_family = AF_INET;
   first_addr.sin_port =  htons(MY_PORT);
   first_addr.sin_addr.s_addr = inet_addr(EB21605);
    int connect_tries = 0;
    do	{
	    connect_status = connect(first_socket,(struct sockaddr *) & first_addr,sizeof(struct sockaddr_in));
       if (connect_status<0) {
          perror("error on connecting first_socket: retrying... ");			
		    connect_tries++;
		    sleep(1);
		
       }
        cout << "second socket is " << connect_status<< endl;
    }while(connect_status < 0 && connect_tries < 10);

    if(connect_tries >= 10)
    {
	    perror("Failed to connect after 10 attempts.");
	    exit(-1);
    }
    int status;
    flags = 0;
    my_pid = getpid();
    iteration = 0;

    pthread_mutex_lock(&the_mutex);
    while((rcv_count=recv(first_socket,points,sizeof(points),flags))<=0);
         pthread_mutex_unlock(&the_mutex);

    pthread_mutex_lock(&the_mutex);
    int recv_add, len = 1024/4;

    for(int i = 0; i < (RECV_NUM*sizeof(int)/1024)*4; i++)
    {
        recv_add = i*1024/16;
        while((rcv_count=recv(first_socket,&a[recv_add],len,flags))<=0);
        if(rcv_count < 0 || rcv_count != 1024/4)
        {
             cout << "Recevied only " << rcv_count << endl;
             perror("error on receiving from thread\n");
        }
    }
	
    pthread_mutex_unlock(&the_mutex);

    ostringstream convert;   // stream used for the conversion

    convert << points[0];      // insert the textual representation of 'Number' in the characters in the stream

    Result = convert.str(); // set 'Result' to the contents of the stream
    Result += ".txt";

    myfile.open (Result.c_str());
    myfile << "received data" << endl;
    for(int i=0; i<  (points[1] - points[0])+1; i++)
    myfile << a[i] << endl;
    myfile << endl;

    cout << "Client Process receiving data "<< endl;	

    base_sz = ((points[1] - points[0])+1)/MAX_THREADS;
    extra =  ((points[1] - points[0])+1) % MAX_THREADS;
    con_id_array[0].start = 0;

    for(int i = 0; i < MAX_THREADS; i++)
    {
        con_id_array[i].id = i;
        con_id_array[i].end=con_id_array[i].start+base_sz + (extra>i)-1;
        con_id_array[i].num_connection = num;
        status = pthread_create(&con_thread[i], NULL, calculation, (void *) &con_id_array[i]);
        if (status != 0)
	        cout << "error on pthread_create i " << i <<" " << status << ":" <<
		        strerror(status) << endl;

        if (i+1<MAX_THREADS)
	        con_id_array[i+1].start=con_id_array[i].end+1;
    }

    for(int i = 0; i < MAX_THREADS; i++)
    {
        status = pthread_join(con_thread[i], NULL);
        if (status != 0)
        cout << "error on pthread_join " << status << ":" << strerror(status)
          << endl;
    }

    mergesort(a,b,0,points[1] - points[0]);

	cout << "complete client sorting" << endl;
	int recv_addr, len_send=1024 ;
	
    pthread_mutex_lock(&the_mutex);
    for(int i = 0; i < (RECV_NUM*sizeof(int)/1024); i++)
    {
         recv_addr = i*1024/4;
         send_status = send(first_socket,&a[recv_addr], len_send,flags);
         if (send_status<0) {
         perror("error on send\n");
         exit(1);
        }
    }
	pthread_mutex_unlock(&the_mutex);
		 	
	return 0;   
}
