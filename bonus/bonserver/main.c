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
	if (argc != 2) 
    { 
        printf("You must provide the port number as an argument.\n");
        exit(1);
    }     

    char *portNumber = argv[1];
	struct product_record records[MAXFILES];
    
    // set up socket

    // read record from socket

    // process records

    // write to client

}
