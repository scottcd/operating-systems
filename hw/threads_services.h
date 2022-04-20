/*
*  Chandler Scott
*  Homework 4
*  04/11/2022
*/


#include "product_record.h" 
#include "file_services.h" 
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void initializeSemsAndQueues();
void createReadThread(pthread_t *tid, char* fileName,struct product_record records[]);
void createStationThreads(pthread_t *tid);
void createWriteThread(pthread_t *tid, char* fileName);
struct product_record createLastProductRecord();