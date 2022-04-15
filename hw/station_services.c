#include "station_services.h" 
#include <semaphore.h>


int station0(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{
    int stationStats = 0;
    struct product_record record;
    sem_wait(&mysem[0]);
    sem_wait(&mysem[1]);
    while(1)
    {
        dequeue(&product_queue[0], &record);
        printf("%d %d\n\n", record.idnumber, record.stations[0]);

        if(record.stations[0] == -1)
            break;

        // compute tax amount
        record.tax = (record.price * record.number) * .05;
        record.stations[0] = 1;
        stationStats++;

        if (record.number >= 1000)
        {
            //write(mypipe[1][1], NULL, sizeof(struct product_record)); 
            //write(mypipe[2][1], &record, sizeof(struct product_record)); 
        }
        else{
            //write(mypipe[1][1], &record, sizeof(struct product_record)); 
        }
        enqueue(&product_queue[1], &record);
        
    }
    //enqueue(&product_queue[1], &record);
    sem_post(&mysem[0]);
    sem_post(&mysem[1]);
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