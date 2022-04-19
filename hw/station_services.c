#include "station_services.h" 
#include <semaphore.h>
#include <pthread.h>

int stationStatistics[5];

void station0(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{
    int stationStats = 0;
    struct product_record record;  
    
    while(1)
    {
        sem_wait(&mysem[1]);
        sem_wait(&mysem[0]);

        if(isEmpty(&product_queue[0]) == 1)
        {
            sem_post(&mysem[0]);
            sem_post(&mysem[1]);
            continue;
        }

        dequeue(&product_queue[0], &record);

        if(record.stations[0] == -1)
        {
            enqueue(&product_queue[1], &record);
            sem_post(&mysem[0]);
            sem_post(&mysem[1]);
            break;
        }

        // compute tax amount
        record.tax = (record.price * record.number) * .05;
        record.stations[0] = 1;
        stationStats++;

        if (record.number >= 1000)
        {
            enqueue(&product_queue[2], &record);
        }
        else{
            enqueue(&product_queue[1], &record);
        }    
        sem_post(&mysem[0]);
        sem_post(&mysem[1]);   
    }
    
    // print stats here
    writeStationStatistics(0, stationStats);
    return;
}

void station1(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{
    int stationStats = 0;
    struct product_record record;

    while(1)
    {
        sem_wait(&mysem[2]);
        sem_wait(&mysem[1]);
        if(isEmpty(&product_queue[1]) == 1)
        {
            sem_post(&mysem[1]);
            sem_post(&mysem[2]);
            continue;
        }
        dequeue(&product_queue[1], &record);

        if(record.stations[0] == -1)
        {
            // enqueue to next
            enqueue(&product_queue[2], &record);
            sem_post(&mysem[1]);
            sem_post(&mysem[2]);
            break;
        }

        // compute the shipping and handling, $10 plus 1% of the amount ordered
        double amountOrdered = record.price * record.number;
        record.sANDh = 10 + (amountOrdered * .01);
        stationStats++;
        record.stations[1] = 1;

        enqueue(&product_queue[2], &record);
        sem_post(&mysem[1]);
        sem_post(&mysem[2]); 
    }
          
    writeStationStatistics(1, stationStats);
    return;
}

void station2(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{ 
    int stationStats = 0;
    struct product_record record;

    while(1)
    {
        sem_wait(&mysem[3]);
        sem_wait(&mysem[2]);

        if(isEmpty(&product_queue[2]) == 1)
        {
            sem_post(&mysem[2]);
            sem_post(&mysem[3]);
            continue;
        }

        dequeue(&product_queue[2], &record);

        if(record.stations[0] == -1)
        {
            // enqueue to next
            enqueue(&product_queue[3], &record);
            sem_post(&mysem[2]);
            sem_post(&mysem[3]);
            break;
        }
    
        // compute the order total
        double amountOrdered = record.price * record.number;
        record.total = amountOrdered + record.tax + record.sANDh;
        stationStats++;
        record.stations[2] = 1;
        
        enqueue(&product_queue[3], &record);
        sem_post(&mysem[2]);
        sem_post(&mysem[3]);  
    }
    
    writeStationStatistics(2, stationStats);
    return;
}

void station3(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{
    int stationStats = 0;
    float runningTotal = 0;
    struct product_record record;   

    while(1)
    {
        sem_wait(&mysem[4]);
        sem_wait(&mysem[3]);

        if(isEmpty(&product_queue[3]) == 1)
        {
            sem_post(&mysem[3]);
            sem_post(&mysem[4]);
            continue;
        }

        dequeue(&product_queue[3], &record);

        if(record.stations[0] == -1)
        {
            // enqueue to next
            enqueue(&product_queue[4], &record);
            sem_post(&mysem[3]);
            sem_post(&mysem[4]);
            break;
        }
    
        // compute and display the running total of orders seen so far
        runningTotal += record.total;
        printf("Running Total: %.2f\n", runningTotal);
        stationStats++;
        record.stations[3] = 1;
        
        enqueue(&product_queue[4], &record);
        sem_post(&mysem[3]);
        sem_post(&mysem[4]); 
    }
       
    writeStationStatistics(3, stationStats);
    return;
}

void station4(sem_t mysem[MAXSTAGES + 1], queue product_queue[MAXSTAGES + 2])
{
    int stationStats = 0,
    runningCounter = 0;
    struct product_record record;

    while(1)
    {
        sem_wait(&mysem[5]);
        sem_wait(&mysem[4]);
        if(isEmpty(&product_queue[4]) == 1)
        {
            sem_post(&mysem[4]);
            sem_post(&mysem[5]);
            continue;
        }

        runningCounter++;
        dequeue(&product_queue[4], &record);

        if(record.stations[0] == -1)
        {
            // enqueue to next
            enqueue(&product_queue[5], &record);
            sem_post(&mysem[4]);
            sem_post(&mysem[5]);
            break;
        }
    
        // display the record to the screen, with fields labeled
        record.stations[4] = 1;
        stationStats++;
        printf("--------------------\n");
        printf("Product Record %d\n", runningCounter);
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

        enqueue(&product_queue[5], &record);
        sem_post(&mysem[4]);
        sem_post(&mysem[5]);
    }
        
    writeStationStatistics(4, stationStats);
    return;
}

void writeStationStatistics(int stationNumber, int stationStats)
{   
    stationStatistics[stationNumber] = stationStats;
    //printf("Station %d:\t    %d\n", stationNumber, stationStats);
}

int* getStationStatistics()
{
    return stationStatistics;
}