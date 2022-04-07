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
#include <stdlib.h>

void forkAndRunEachStation(struct product_record records[], int mypipe[MAXSTAGES + 1][2];);
void forkAChildStation(int station_number,  struct product_record records[], int mypipe[MAXSTAGES + 1][2]);
int station0(int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station1(int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station2(int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station3(double runningTotal, int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
int station4(int recordNumber, int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record);
void printStationStatistics(int stationNumber, int stationStats);

#endif 
