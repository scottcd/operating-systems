/*
*  Chandler Scott
*  Homework 3
*  03/29/2022
*/
#ifndef RECORD_READER    
#define RECORD_READER

#include <stdio.h>
#include <stdlib.h>
#include "product_record.h" 
#include <string.h>   


int getRecordCount();
int accessFile(char* fileName, int fd, struct product_record records[]);
void writeRecord(FILE* fp, struct product_record* record);
void readRecords(FILE* fp, struct product_record records[]);

#endif 