/* 
*  Developer: 	Chandler Scott
*  Project:	Homework 2 - Part 2
*  Created:     02/24/2022
*  Last Edit:   02/24/2022
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int value = fork();
    if (value == 0)
    {
        // child
	printf("Hi, I'm the child. My pid is %d, and my parent's pid is %d\n", getpid(), getppid());
	char* argv[] = {"hw1", "order2.txt", "output2.txt", NULL};
	execv("/home/pi/operating-systems/hw/hw1", argv);

	exit(0);
    }
    else
    {
        // parent
	int p;
	wait(&p);
	
	// display the output file created by hw1
	char* args[] = {"cat", "order-files/output2.txt", NULL};
	execv("/bin/cat", args);

	exit(0);
    }
}
