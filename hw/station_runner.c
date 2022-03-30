#include "product_record.h" 
#include "record_reader.h" 
#include <unistd.h>
#include <sys/wait.h>

void runStations(struct product_record records[])
{
    // run each record
    for (int i = 0; i < getFileCount(); i++)
    {
        int status = 0;
        int station_pid [5] = {0,0,0,0,0};
        // create 5 child processes
        for (int i = 0; i < 5; i++)
        {
            station_pid[i] = fork();
            if (station_pid[i] == 0)
            {
                waitpid(station_pid[i - 1], &status, 0);    // wait for the previous station to finish
                printf("station %d\n", i);
                _exit(0);
            }
        }
        
        waitpid(station_pid[4], &status, 0);
        printf("parent\n");
    }
}