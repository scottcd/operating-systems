/*
*  Chandler Scott
*  Bonus Socket Assignment - Client
*  04/28/2022
*/

#include "product_record.h" 
#include "threads_services.h" 
#include <stdio.h>
#include <stdlib.h> 
#include "queue.h"

#define MAXFILES 30

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
    pthread_t tid[2];

    initMutex();
    initializeSemsAndQueues();

    // read record from file
    createReadThread(&tid[0], inputFile, records);

    // send record to server via socket
    // receive record from server on socket

    // write record to file
    createWriteThread(&tid[1], outputFile);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL); 
}
