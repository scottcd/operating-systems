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

struct product_record createLastProductRecord();



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

    createReadThread(tid[0], inputFile, records);
    
    for(int i = 0; i < MAXSTAGES; i++)
    {
        createStationThreads(tid[i + 1]); 
    }

    createWriteThread(tid[6]);
    pthread_exit(NULL);
	// // read records
    
    // if (accessFile(inputFile, 0, records) == -1) 
    // {
    //     printf("Failed to read text file: %s\n", argv[1]);
    //     exit(1);
    // }
   
    // // setup pipeline
    // int mypipe[MAXSTAGES + 1][2];
	// initializeStationPipes(mypipe);
    
    // // pipe records to station 0
    // pipeRecordsToStation0(records, mypipe);
    
    // // fork and run each station
    // forkAndRunEachStation(records, mypipe);
    
    // // output stats
    // sleep(1);
    // printf("Station #    Records Processed\n");
    // printf("---------    -----------------\n");
          
    // // pipe -1 to stations
    // sleep(.1);
    // struct product_record lastRecord = createLastProductRecord();
    // pipeEndRecordToAllStations(lastRecord, mypipe);

    // readFromStation4AndWrite(outputFile, mypipe);

    return;
}

struct product_record createLastProductRecord(){
    struct product_record record;
    for (int i = 0; i < MAXSTAGES; i++)
    { 
        record.stations[i] = -1;
    }

    return record;
}