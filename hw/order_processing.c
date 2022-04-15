/*
*  Chandler Scott
*  Homework 1
*  02/07/2022
*/

#include "product_record.h" 
#include "file_services.h" 
#include "station_services.h" 
#include "pipeline_services.h" 
#include "threads_services.h"
#include "thread_structs.h"
#include <pthread.h>


// entry point for the program
void main (int argc, char *argv[])
{
	if (argc != 3) 
    { 
        printf("You must provide the input and output files as two arguments\n");
        exit(1);
    }     

    char *inputFile = argv[1];
	char *outputFile = argv[2];
    struct product_record records[MAXFILES];
    
    pthread_t tid[7];

    initializeSems();
    createReadThread(tid[0], inputFile, records);
    
    for(int i = 0; i < MAXSTAGES; i++)
    {
        createStationThreads(tid[i + 1]); 
    }

    createWriteThread(tid[6], outputFile);
    pthread_exit(NULL);
	
    return;
}
