#include "pipeline_services.h" 


// init threads instead
void initializeStationPipes(int mypipe[MAXSTAGES + 1][2]){
    for(int i = 0; i < MAXSTAGES + 1; i++)
    {
        if (pipe(mypipe[i]) < 0)
            {
                    exit(1);
            }
    } 
}

void pipeRecordsToStation0(struct product_record records[], int mypipe[MAXSTAGES + 1][2])
{
    int record_count = getRecordCount();   
    for(int i = 0; i < record_count; i++)
    {
        write(mypipe[0][1], &records[i], sizeof(struct product_record));  
    }
}

void pipeEndRecordToAllStations(struct product_record record, int mypipe[MAXSTAGES + 1][2])
{
    write(mypipe[0][1], &record, sizeof(struct product_record)); 
    write(mypipe[1][1], &record, sizeof(struct product_record)); 
    write(mypipe[2][1], &record, sizeof(struct product_record)); 
    write(mypipe[3][1], &record, sizeof(struct product_record)); 
    write(mypipe[4][1], &record, sizeof(struct product_record)); 
}

 void readFromStation4AndWrite(char* outputFile, int mypipe[MAXSTAGES + 1][2]){   
    // read each record from pipe and write to the output file
    struct product_record readRecord;
    char path[20] = "./order-files/";    
    strcat(path, outputFile);
    FILE *fp;
    fp = fopen(path, "w");
    int record_count = getRecordCount();   
    for(int i = 0; i < record_count; i++)
    {
        read(mypipe[5][0], &readRecord, sizeof(struct product_record));
        writeRecord(fp, &readRecord);
    }
    
    fclose(fp);
}