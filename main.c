// Viki Bril 207129446 && Almog Klinger 305630683

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Scheduler.h"

//Default Values
int NUMBEROFTHREADS = 3; 
int NUMBEROFCORES = 1;
int QUANTUM = 1;  


//ARGUMENT ERRORS
char ERROR1[] = "Number of cores should be lesser than 4";
char ERROR2[] = "Number of cores should be greater than 0"; 
char ERROR3[] = "Number of threads should be lesser than 10"; 
char ERROR4[] = "Number of threads should be greater than 0"; 
char ERROR5[] = "QUANTUM should be greater than 0"; 

void Inits()
{
    srand(time(NULL));
}

int main(int argc, char* argv[])
{
    Inits();
    
    //Changing params according to Command Line Args
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0]== 'p')
        {
            NUMBEROFCORES = atoi(strchr(argv[i], 'p') + 1);
            if(NUMBEROFCORES > 4)
            {
                printf("%s\n",ERROR1);
                NUMBEROFCORES = 4;
            }
            else if(NUMBEROFCORES <= 0){
                printf("%s\n",ERROR2);
                NUMBEROFCORES = 1;
            }
        }
        if(argv[i][0]== 'n'){
            NUMBEROFTHREADS = atoi(strchr(argv[i], 'n') + 1);
            if(NUMBEROFTHREADS > 10){
                printf("%s\n",ERROR3);
                NUMBEROFTHREADS = 10;
            }
            else if(NUMBEROFTHREADS <= 0){
                printf("%s\n",ERROR4);
                NUMBEROFTHREADS = 4;
            }
        }
        if(argv[i][0]== 't')
        {
            QUANTUM = atoi(strchr(argv[i], 't') + 1);
            if(QUANTUM <= 0){
                printf("%s\n",ERROR5);
                QUANTUM = 1;
            }
        }
    }
    Scheduler scheduler;
    SchedulerInit(&scheduler, NUMBEROFCORES, NUMBEROFTHREADS, QUANTUM);
    SchedulerStart(&scheduler);
    return 0;
}