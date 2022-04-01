/*
*  Chandler Scott
*  Homework 1
*  02/07/2022
*/

#include "product_record.h" 
#include "record_reader.h" 
#include "station_runner.h" 




#define MAXFILES 3

// entry point for the program
void main (int argc, char *argv[])
{
	
	if (argc != 3) 
    { 
        printf("You must provide the input and output files as two arguments\n");
        exit(1);
    }     

    char *inputFile = argv[1];
	char *outputFile = argv[2];

    struct product_record records[MAXFILES];
	// read record
    if (accessFile(inputFile, 0, records) == -1) 
    {
        printf("Failed to read text file: %s\n", argv[1]);
        exit(1);
    }
   
	// it is here that we will do hw3
	runAllStations(records, outputFile);

    // write record
	// if (accessFile(outputFile, 1, records) == -1) 
    // {
    //     printf("Failed to write text file: %s\n", argv[2]);
	// exit(1);
    // }

    return;
}




