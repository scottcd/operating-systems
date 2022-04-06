#include "product_record.h" 
#include "record_reader.h" 
#include <unistd.h>
#include <sys/wait.h>


void initializeStationPipes();
void forkAChildStation(int station_number,  struct product_record records[]);
struct product_record createLastProductRecord();
void printStationStatistics(int stationNumber, int stationStats);
void pipeEndRecordToAllStations(struct product_record record);
void printFinalStatistics();
int station0( int stationStats);
int station1( int stationStats);
int station2( int stationStats);
int station3( int stationStats);
int station4(int recordNumber, int stationStats);

double runningTotal = 0;
int mypipe[6][2];

void runAllStations(struct product_record records[], char* outputFile)
{
    initializeStationPipes();
    int status = 0;
    char path[20] = "./order-files/";    
    strcat(path, outputFile);
    FILE *fp;
    fp = fopen(path, "w");
    
    // write each read record to the pipeline
    int record_count = getRecordCount();   
    for(int i = 0; i < record_count; i++)
    {
        write(mypipe[0][1], &records[i], sizeof(struct product_record));  
    }
         
    // create 5 child processes
    for (int station_number = 0; station_number < 5; station_number++)
    {
        forkAChildStation(station_number, records);
    }
       
    sleep(1);
    // output stats
    printFinalStatistics();
    sleep(.1);
    // pipe -1 to stations
    struct product_record lastRecord = createLastProductRecord();
    pipeEndRecordToAllStations(lastRecord);
    
    // read each record from pipe and write to the output file
    struct product_record readRecord;
    for(int i = 0; i < record_count; i++)
    {
        read(mypipe[5][0], &readRecord, sizeof(struct product_record));
        writeRecord(fp, &readRecord);
    }
    
    fclose(fp);
}

void forkAChildStation(int station, struct product_record records[])
{
    int pid = fork();
    int stationStats= 0;
    int record_count = getRecordCount();
    
    // pipe each file
    for (int i = 0; i < record_count + 1; i++)
    {
        if (pid == 0) 
        {  
            switch (station)
            {                
            case 0:            
                stationStats = station0(stationStats);
                break;
            case 1:
                stationStats = station1(stationStats);
                break;
            case 2:
                stationStats = station2(stationStats);
                break;
            case 3:               
                stationStats = station3(stationStats);
                break;
            default:
                stationStats = station4(i, stationStats);
                break;
            }
        }
    }
}

void printFinalStatistics(){
    // print stats
    printf("----------------\n");
    printf("Final Statistics\n");
    printf("----------------\n\n");
    printf("Station #    Records Processed\n");
    printf("---------    -----------------\n");
}

struct product_record createLastProductRecord(){
    struct product_record record;
    for (int i = 0; i < MAXSTAGES; i++)
    { 
        record.stations[i] = -1;
    }

    return record;
}

void printStationStatistics(int stationNumber, int stationStats)
{
    
    printf("Station %d:\t    %d\n", stationNumber, stationStats);
}

void pipeEndRecordToAllStations(struct product_record record)
{
    write(mypipe[0][1], &record, sizeof(struct product_record)); 
    write(mypipe[1][1], &record, sizeof(struct product_record)); 
    write(mypipe[2][1], &record, sizeof(struct product_record)); 
    write(mypipe[3][1], &record, sizeof(struct product_record)); 
    write(mypipe[4][1], &record, sizeof(struct product_record)); 
}

int station0( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[0][0], &record, sizeof(struct product_record));
    // printf("0 %s\n", record.name);

    // check if we are done
    if(record.stations[0] == -1)
    {
        printStationStatistics(0, stationStats);
        _exit(0);
        return stationStats;
    }
    stationStats++;
    // compute tax amount
    record.tax = (record.price * record.number) * .05;
    
    // HERE skip station 2??
    record.stations[0] = 1;
    if (record.number >= 1000)
    {
        write(mypipe[1][1], NULL, sizeof(struct product_record)); 
        write(mypipe[2][1], &record, sizeof(struct product_record)); 
    }
    else{
        write(mypipe[1][1], &record, sizeof(struct product_record)); 
    }
    
    return stationStats;
}

int station1( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[1][0], &record, sizeof(struct product_record));
    
    // check if we are done
    if(record.stations[0] == -1)
    {
        printStationStatistics(1, stationStats);
        _exit(0);
        return stationStats;
    }

    if (&record == NULL)
    {
        return stationStats;
    }
    
    stationStats++;
    // compute the shipping and handling, $10 plus 1% of the amount ordered
    double amountOrdered = record.price * record.number;
    record.sANDh = 10 + (amountOrdered * .01);

    // work is done.. set station to 1 and write to our pipe
    record.stations[1] = 1;
    write(mypipe[2][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station2( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[2][0], &record, sizeof(struct product_record));

    // check if we are done
    if(record.stations[2] == -1)
    {
        printStationStatistics(2, stationStats);
        _exit(0);
        return stationStats;
    }
    
    stationStats++;
    // compute the order total
    double amountOrdered = record.price * record.number;
    record.total = amountOrdered + record.tax + record.sANDh;

    // work is done.. set station to 1 and write to our pipe
    record.stations[2] = 1;
    write(mypipe[3][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station3( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[3][0], &record, sizeof(struct product_record));
    
    // check if we are done
    if(record.stations[3] == -1)
    {
        printStationStatistics(3, stationStats);
        _exit(0);
        return stationStats;
    }

    stationStats++;
    // compute and display the running total of orders seen so far
    runningTotal += record.total;
    printf("Running Total: %.2f\n", runningTotal);

    // work is done.. set station to 1 and write to our pipe
    record.stations[3] = 1;
    write(mypipe[4][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station4(int recordNumber,  int stationStats)
{
    //sleep(.1);
    // read our record from the pipe
    struct product_record record;
    read(mypipe[4][0], &record, sizeof(struct product_record));
  
    // check if we are done
    if(record.stations[4] == -1)
    {
        printStationStatistics(4, stationStats);
        _exit(0);
        return stationStats;
    }
    stationStats++;
    record.stations[4] = 1;
    write(mypipe[5][1], &record, sizeof(struct product_record));
    // display the record to the screen, with fields labeled
    printf("--------------------\n");
    printf("Product Record %d\n", recordNumber);
    printf("ID: %d\n", record.idnumber);
    printf("Name: %s\n", record.name);
    printf("Price: %.2f\n", record.price);
    printf("Number: %d\n", record.number);
    printf("Tax: %.2f\n", record.tax);
    printf("sANDh: %.2f\n", record.sANDh);
    printf("Total: %.2f\n", record.total);
    printf("Stations: ");
    for(int j = 0; j < 5; j++) {
        printf("%d ", record.stations[j]);
    }
    printf("\n--------------------\n\n");

    return stationStats;
}

void initializeStationPipes(){
    for(int i = 0; i < MAXSTAGES + 1; i++)
    {
        if (pipe(mypipe[i]) < 0)
            {
                    exit(1);
            }
    }
    
}