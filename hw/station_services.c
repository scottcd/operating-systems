#include "station_services.h" 


void forkAndRunEachStation(struct product_record records[], int mypipe[MAXSTAGES + 1][2])
{
    int status = 0;
    // write each read record to the pipeline
    int record_count = getRecordCount();   
         
    // create 5 child processes
    for (int station_number = 0; station_number < 5; station_number++)
    {
        forkAChildStation(station_number, records, mypipe);
    }
}   
      
void forkAChildStation(int station, struct product_record records[], int mypipe[MAXSTAGES + 1][2])
{
    int pid = fork();
    int stationStats= 0;
    int record_count = getRecordCount();
    double runningTotal = 0;

    // pipe each file
    for (int i = 0; i < record_count + 1; i++)
    {
        if (pid == 0) 
        {  
            struct product_record record;
            read(mypipe[station][0], &record, sizeof(struct product_record));

            if(record.stations[station] == -1)
                {
                    printStationStatistics(station, stationStats);
                    _exit(0);
                }

            switch (station)
            {                
            case 0:            
                stationStats = station0(stationStats, mypipe, record);
                break;
            case 1:
                stationStats = station1(stationStats, mypipe, record);
                break;
            case 2:
                stationStats = station2(stationStats, mypipe, record);
                break;
            case 3:               
                stationStats = station3(runningTotal, stationStats, mypipe, record);
                break;
            default:
                stationStats = station4(i, stationStats, mypipe, record);
                break;
            }
        }
    }
}

int station0( int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record)
{
    // compute tax amount
    record.tax = (record.price * record.number) * .05;
    record.stations[0] = 1;
    stationStats++;

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

int station1( int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record)
{
    if (&record == NULL)
    {
        return stationStats;
    }
    
    // compute the shipping and handling, $10 plus 1% of the amount ordered
    double amountOrdered = record.price * record.number;
    record.sANDh = 10 + (amountOrdered * .01);
    stationStats++;

    // work is done.. set station to 1 and write to our pipe
    record.stations[1] = 1;
    write(mypipe[2][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station2( int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record)
{ 
    // compute the order total
    double amountOrdered = record.price * record.number;
    record.total = amountOrdered + record.tax + record.sANDh;
    stationStats++;

    // work is done.. set station to 1 and write to our pipe
    record.stations[2] = 1;
    write(mypipe[3][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station3(double runningTotal, int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record)
{
    // compute and display the running total of orders seen so far
    runningTotal += record.total;
    printf("Running Total: %.2f\n", runningTotal);
    stationStats++;

    // work is done.. set station to 1 and write to our pipe
    record.stations[3] = 1;
    write(mypipe[4][1], &record, sizeof(struct product_record));
    
    return stationStats;
}

int station4(int recordNumber,  int stationStats, int mypipe[MAXSTAGES + 1][2], struct product_record record)
{
    record.stations[4] = 1;
    stationStats++;
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

void printStationStatistics(int stationNumber, int stationStats)
{   
    printf("Station %d:\t    %d\n", stationNumber, stationStats);
}