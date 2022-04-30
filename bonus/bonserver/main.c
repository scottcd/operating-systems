/*
*  Chandler Scott
*  Homework 1
*  02/07/2022
*/

#include "product_record.h" 
#include "station_services.h" 
#include "threads_services.h" 
#include <stdio.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "queue.h"



#define MAXFILES 30

// entry point for the program
void main (int argc, char *argv[])
{
	if (argc != 2) 
    { 
        printf("You must provide the port number as an argument.\n");
        exit(1);
    }     

    pthread_t tid[2];
    initMutex();
    initializeSemsAndQueues();

    char *portNumber = argv[1];
	struct product_record records[MAXFILES];
    struct addrinfo* myinfo;
    

    // set up socket
    int sockdesc = socket(AF_INET, SOCK_STREAM, 0);
    int x = -1;
    while (x == -1)
    {
        getaddrinfo("0.0.0.0", portNumber, NULL, &myinfo);
        x = bind(sockdesc, myinfo->ai_addr, myinfo->ai_addrlen);  
    }
    printf("Listening to port %s.\n", portNumber);

    int y = listen(sockdesc, 1); 

    // read record from socket
    while (1)
    {   
        printf("Waiting for a connection...\n");
        
        int connection = accept(sockdesc, NULL, NULL);
        handleRecord(&tid[0],connection);
    }
    
}
