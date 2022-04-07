#include "file_services.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> 

void initializeStationPipes(int mypipe[MAXSTAGES + 1][2]);
void pipeRecordsToStation0(struct product_record records[], int mypipe[MAXSTAGES + 1][2]);
void pipeEndRecordToAllStations(struct product_record record, int mypipe[MAXSTAGES + 1][2]);
void readFromStation4AndWrite(char* outputFile, int mypipe[MAXSTAGES + 1][2]);