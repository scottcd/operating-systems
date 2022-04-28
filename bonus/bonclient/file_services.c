/*
*  Chandler Scott
*  Homework 3
*  03/29/2022
*/

#include "file_services.h"
#include "thread_structs.h"

int fileCount = 0;

int getRecordCount ()
{
    return fileCount;
}


// opens a file for read/write, and then calls the appropriate method
int accessFile (char* fileName, int fd, struct product_record records[]) 
{
    FILE *fp;
    char path[20] = "./order-files/";
    strcat(path, fileName);
 
    // if fd ==1, we write
    if (fd == 1)
    {
        // write data
        fp = fopen(path, "w");
        
        if (!fp)
        {
            return -1;
        }
        // loop here
        for (int i = 0; i < fileCount; i++) 
        {
            writeRecord(fp, &records[i]);
        }
    }
    else
    {
	// read data
        fp = fopen(path, "r");

        if (!fp) 
        {
            return -1;
        }

	readRecords(fp, records);	    

    }
    fclose(fp);
    return 0;
}

// writes a product_record struct to a user-specified file
void writeRecord(FILE* fp, struct product_record* record)
{
	fprintf(fp, "%d\n", (*record).idnumber);
	fprintf(fp, "%s\n", (*record).name);
	fprintf(fp, "%.2f\n", (*record).price);
	fprintf(fp, "%d\n", (*record).number);
	fprintf(fp, "%.2f\n", (*record).tax);
	fprintf(fp, "%.2f\n", (*record).sANDh);
	fprintf(fp, "%.2f\n", (*record).total);
	
	for (int i = 0; i < MAXSTAGES; i++) 
	{
	     fprintf(fp, "%d ", (*record).stations[i]);
	}
	
	fprintf(fp, "\n");
}

// reads a product_record struct from a user-specified file
void readRecords(FILE* fp, struct product_record records[])
{
	size_t len = 0;
	void* buffer = malloc(sizeof(struct product_record));
	char *line = NULL;
	int i = 0;
	while (getline(&line, &len, fp) != -1) 
	{
	    records[i].idnumber = atoi(line);

	    fgets(buffer, sizeof(records[i].name), fp); 
	    char *p;
	    if ((p = strchr(buffer, '\n')) != NULL)
	    {
	        *p = '\0';
	    }
	   
	    strcpy(records[i].name, buffer);

	   	getline(&line, &len, fp);
	    records[i].price = atof(line);
	   
	    getline(&line, &len, fp);
	    records[i].number = atoi(line);

	    fgets(buffer, sizeof(records[i].tax), fp); 
	    records[i].tax = atof(buffer);
	    
	    fgets(buffer, sizeof(records[i].sANDh), fp); 
	    records[i].sANDh = atof(buffer);
	    
	    fgets(buffer, sizeof(records[i].total), fp); 
	    records[i].total = atof(buffer);
	    	
	    fgets(buffer, sizeof(records[i].stations), fp); 
	    for (int i = 0; i < MAXSTAGES; i++)
	    { 
		records->stations[i] = 0;
	    }
	    
	    i++;
	}
	fileCount = i;
	free(buffer);
}