/*
*  Chandler Scott
*  Homework 3
*  03/29/2022
*/
#ifndef STATION_RUNNER    
#define STATION_RUNNER

#include "product_record.h" 
#include "file_services.h" 
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include "queue.h"

void station0(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
void station1(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
void station2(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
void station3(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
void station4(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
void writeStationStatistics(int stationNumber, int stationStats);
int* getStationStatistics();

#endif 
