#include "product_record.h" 
#include "record_reader.h" 
#include <unistd.h>
#include <sys/wait.h>


void initializeStationPipes();
void station0();
void station1();
void station2();
void station3();
void station4();

double runningTotal = 0;
int s0[2], s1[2], s2[2], s3[2], s4[2]; 

void runStations(struct product_record records[], char* outputFile)
{
    initializeStationPipes();
    int status = 0;
    char path[20] = "./order-files/";    
    strcat(path, outputFile);
    FILE *fp;
    fp = fopen(path, "w");
    
    // run each record
    for (int i = 0; i < getFileCount(); i++)
    {
        
        write(s0[1], &records[i], sizeof(struct product_record));
        int station_pid [5] = {0,0,0,0,0};
        // create 5 child processes
        for (int j = 0; j < 5; j++)
        {
            station_pid[j] = fork();
            if (station_pid[j] == 0)
            {   
                //printf("station %d created.\n", j);
                // wait for the previous station to finish
                waitpid(station_pid[j ? j : j - 1], &status, 0); 
                
                switch (j)
                {
                case 0:
                    station0();
                    break;
                case 1:
                    station1();
                    break;
                case 2:
                    station2();
                    break;
                case 3:
                    // running total
                    station3();
                    break;
                default:
                    station4(i + 1);
                    break;
                }

                _exit(0);
            }
        }
        // wait for station 4 to finish
        waitpid(station_pid[4], &status, 0);
        // read record from pipe
        struct product_record readRecord;
        read(s4[0], &readRecord, sizeof(struct product_record));
        // write record to file
        writeRecord(fp, &readRecord);
        
        //printf("Parent here.. children are done!\n" );
    }
    fclose(fp);
}

void station0()
{
    // read our record from the pipe
    struct product_record record;
    read(s0[0], &record, sizeof(struct product_record));
    
    // compute tax amount
    record.tax = (record.price * record.number) * .05;
    
    
    // HERE skip station 2??
    record.stations[0] = 1;
    if (record.number >= 1000)
    {
        write(s0[1], NULL, sizeof(struct product_record)); 
        write(s1[1], &record, sizeof(struct product_record)); 
    }
    else{
        write(s0[1], &record, sizeof(struct product_record)); 
    }


    
    
    //printf("station 0 processing..\n" );
}

void station1()
{
    // read our record from the pipe
    struct product_record record;
    read(s0[0], &record, sizeof(struct product_record));
    
    if (&record == NULL)
    {
        return;
    }

    // compute the shipping and handling, $10 plus 1% of the amount ordered
    double amountOrdered = record.price * record.number;
    record.sANDh = 10 + (amountOrdered * .01);

    // work is done.. set station to 1 and write to our pipe
    record.stations[1] = 1;
    write(s1[1], &record, sizeof(struct product_record));
    //printf("station 1 processing..\n" );
}

void station2()
{
    // read our record from the pipe
    struct product_record record;
    read(s1[0], &record, sizeof(struct product_record));
    
    // compute the order total
    double amountOrdered = record.price * record.number;
    record.total = amountOrdered + record.tax + record.sANDh;

    // work is done.. set station to 1 and write to our pipe
    record.stations[2] = 1;
    write(s2[1], &record, sizeof(struct product_record));
    //printf("station 2 processing..\n" );
}

void station3()
{
    // read our record from the pipe
    struct product_record record;
    read(s2[0], &record, sizeof(struct product_record));
    
    // compute and display the running total of orders seen so far
    runningTotal += record.total;
    printf("Running Total: %.2f\n", runningTotal);

    // work is done.. set station to 1 and write to our pipe
    record.stations[3] = 1;
    write(s3[1], &record, sizeof(struct product_record));
    //printf("station 3 processing..\n" );
}

void station4(int recordNumber)
{
    // read our record from the pipe
    struct product_record record;
    read(s3[0], &record, sizeof(struct product_record));
    record.stations[4] = 1;

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

    write(s4[1], &record, sizeof(struct product_record));
    //printf("station 4 processing..\n" );
}

void initializeStationPipes(){
    if (pipe(s0) < 0)
    {
            exit(1);
    }
    if (pipe(s1) < 0)
    {
            exit(1);
    }
    if (pipe(s2) < 0)
    {
            exit(1);
    }
    if (pipe(s3) < 0)
    {
            exit(1);
    }
    if (pipe(s4) < 0)
    {
            exit(1);
    }
}