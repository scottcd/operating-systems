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
	if (argc != 5) 
    { 
        printf("You must provide the input file, output file, server, and port as four arguments.\n");
        exit(1);
    }     

    char *inputFile = argv[1];
	char *outputFile = argv[2];
    char *serverName = argv[3];
    char *serverPort = argv[4];
    struct product_record records[MAXFILES];
    
    // read record from file

    // send record to server via socket
    // receive record from server on socket

    // write record to file

}
