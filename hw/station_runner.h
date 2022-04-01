/*
*  Chandler Scott
*  Homework 3
*  03/29/2022
*/
#ifndef STATION_RUNNER    
#define STATION_RUNNER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   
#include <unistd.h>
#include <sys/wait.h>
#include "product_record.h" 
#include "record_reader.h" 


void runAllStations(struct product_record records[], char* outputFile);

#endif 