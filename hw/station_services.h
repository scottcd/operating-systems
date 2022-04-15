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

int station0(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2]);
int station1(int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station2(int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station3(double runningTotal, int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station4(int recordNumber, int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
void printStationStatistics(int stationNumber, int stationStats);

#endif 
