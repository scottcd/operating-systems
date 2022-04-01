#include "product_record.h" 
#include "record_reader.h" 
#include <unistd.h>
#include <sys/wait.h>


void initializeStationPipes();
int* forkAChildStation(int i, int j, int station_pid[], struct product_record records[]);
struct product_record createLastProductRecord();
void pipeEndRecordToAllStations(struct product_record record);
void printStationStatistics(int stationNumber, int stationStats);
void printFinalStatistics(int *station_pid, int stationStats);
int station0( int stationStats);
int station1( int stationStats);
int station2( int stationStats);
int station3( int stationStats);
int station4(int recordNumber, int stationStats);

double runningTotal = 0;
int mypipe[MAXSTAGES][2];


void runAllStations(struct product_record records[], char* outputFile)
{
    initializeStationPipes();
    int status = 0;
    char path[20] = "./order-files/";    
    strcat(path, outputFile);
    FILE *fp;
    fp = fopen(path, "w");
    int station_pid [MAXSTAGES] = {0,0,0,0,0};
    
    
    write(mypipe[0][1], &records[0], sizeof(struct product_record));
        
    int *pid;
    // create 5 child processes
    for (int j = 0; j < 5; j++)
    {
        pid = forkAChildStation(0, j, station_pid, records);
        if (pid[0] == 0)
            break;
    }
    
    // read record from pipe
    if(pid[0] != 0){
        waitpid(station_pid[4], &status, 0);
        struct product_record readRecord;
        read(mypipe[4][0], &readRecord, sizeof(struct product_record));
        // write record to file
        writeRecord(fp, &readRecord);
    }
    else{
        //printf("\n\n----\nchild\n");
        //printf("----%d\n\n", pid[1]);
    }
    
    printFinalStatistics(pid, 0);
    fclose(fp);
}

int* forkAChildStation(int i, int j, int station_pid[],  struct product_record records[])
{
    static int  r[3];
    station_pid[j] = fork();
    int stationStats= 0;
    for (int i = 1; i < getFileCount() + 1; i++)
    {
        if (station_pid[j] != 0)
        {
            write(mypipe[0][1], &records[i], sizeof(struct product_record));
            r[0] = station_pid[j];
            r[1] = 0;
        }
        else if (station_pid[j] == 0)
        {   
            //printf("station %d created.\n", j);
            // wait for the previous station to finish
            waitpid(station_pid[j ? j : j - 1], &station_pid[j], 0); 
            
            switch (j)
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
        r[0] = station_pid[j];
        r[1] = stationStats;
        
        }
    }
            r[2] = j;
            return r;
}

void printFinalStatistics(int* station_pid, int stationStats){
    // create phony product and write to pipes
    if(station_pid[0] != 0){
        // print stats
        printf("----------------\n");
        printf("Final Statistics\n");
        printf("----------------\n\n");
        printf("Station #    Records Processed\n");
        printf("---------    -----------------\n");
        struct product_record endRecord = createLastProductRecord();
        pipeEndRecordToAllStations(endRecord);
       
    }
    else{
        
        printStationStatistics(station_pid[2], station_pid[1]);
    }
}

struct product_record createLastProductRecord(){
    struct product_record record;
    for (int i = 0; i < MAXSTAGES; i++)
    { 
        record.stations[i] = -1;
    }

    return record;
}
void pipeEndRecordToAllStations(struct product_record record)
{
    write(mypipe[0][1], &record, sizeof(struct product_record)); 
    write(mypipe[1][1], &record, sizeof(struct product_record)); 
    write(mypipe[2][1], &record, sizeof(struct product_record)); 
    write(mypipe[3][1], &record, sizeof(struct product_record)); 
    write(mypipe[4][1], &record, sizeof(struct product_record)); 
}

void printStationStatistics(int stationNumber, int stationStats)
{
    
    printf("Station %d:\t    %d\n", stationNumber, stationStats);
}


int station0( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[0][0], &record, sizeof(struct product_record));
    printf("0 %s\n", record.name);

    // check if we are done
    if(record.stations[0] == -1)
    {
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
        sleep(1);
        //close(mypipe[4][1]);
        write(mypipe[2][1], &record, sizeof(struct product_record)); 
    }
    else{
        write(mypipe[1][1], &record, sizeof(struct product_record)); 
    }
    
    return stationStats;
    //printf("station 0 processing..\n" );
}



int station1( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[1][0], &record, sizeof(struct product_record));
    printf("1 %s\n", record.name);    

    // check if we are done
    if(record.stations[0] == -1)
    {
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
    //printf("station 1 processing..\n" );
    return stationStats;
}

int station2( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[2][0], &record, sizeof(struct product_record));
    printf("2 %s\n", record.name);

    // check if we are done
    if(record.stations[2] == -1)
    {
        return stationStats;
    }
    
    stationStats++;
    // compute the order total
    double amountOrdered = record.price * record.number;
    record.total = amountOrdered + record.tax + record.sANDh;

    // work is done.. set station to 1 and write to our pipe
    record.stations[2] = 1;
    write(mypipe[3][1], &record, sizeof(struct product_record));
    //printf("station 2 processing..\n" );
    return stationStats;
}

int station3( int stationStats)
{
    // read our record from the pipe
    struct product_record record;
    read(mypipe[3][0], &record, sizeof(struct product_record));
    printf("3 %s\n", record.name);
    
    // check if we are done
    if(record.stations[3] == -1)
    {
        return stationStats;
    }

    stationStats++;
    // compute and display the running total of orders seen so far
    runningTotal += record.total;
    printf("Running Total: %.2f\n", runningTotal);

    // work is done.. set station to 1 and write to our pipe
    record.stations[3] = 1;
    write(mypipe[4][1], &record, sizeof(struct product_record));
    //printf("station 3 processing..\n" );
    return stationStats;
}

int station4(int recordNumber,  int stationStats)
{
    //sleep(.1);
    // read our record from the pipe
    struct product_record record;
    read(mypipe[4][0], &record, sizeof(struct product_record));
    printf("4 %s\n", record.name);

    // check if we are done
    if(record.stations[4] == -1)
    {
        return stationStats;
    }
    stationStats++;
    record.stations[4] = 1;
    write(mypipe[4][1], &record, sizeof(struct product_record));
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

    
    //printf("station 4 processing..\n" );
    return stationStats;
}

void initializeStationPipes(){
    if (pipe(mypipe[0]) < 0)
    {
            exit(1);
    }
    if (pipe(mypipe[1]) < 0)
    {
            exit(1);
    }
    if (pipe(mypipe[2]) < 0)
    {
            exit(1);
    }
    if (pipe(mypipe[3]) < 0)
    {
            exit(1);
    }
    if (pipe(mypipe[4]) < 0)
    {
            exit(1);
    }
}