// Viki Bril 207129446 && Almog Klinger 305630683

#pragma once
#define __USE_GNU
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "Task.h"


#define MEMOEY_SIZE 32
#define RUN 1

//Struct Schedulers
typedef struct Scheduler
{
    int NUMBEROFCORES;
    int NUMBEROFTHREADS;
    int QUANTUM;
    
    pthread_mutex_t mutex;
    Task* tasks;
    char memory[MEMOEY_SIZE];

} Scheduler;

//Struct Signal Handlers
typedef struct signal_struct_t{
    Task** tasks; 
    int NUMBEROFTHREADS;
} Sig_Args;


//Signal Flag => SIGUSR
int signalFlag = 0;

//Helper Members
void taskpriorityRITYSORTER(Task** p_tasks, int NUMBEROFTHREADS);
void TASKIDSORTER();
void SWAPPER(int *arr, size_t n);






void SchedulerInit(Scheduler* SCDL, int NUMBEROFCORES, int NUMBEROFTHREADS, int QUANTUM){
    SCDL->NUMBEROFCORES = NUMBEROFCORES;
    SCDL->NUMBEROFTHREADS = NUMBEROFTHREADS;
    SCDL->QUANTUM = QUANTUM;
    SCDL->tasks = (Task*)malloc(NUMBEROFTHREADS * sizeof(Task));
    int PRIORITYrity_array[NUMBEROFTHREADS];
    for (int i = 0; i < NUMBEROFTHREADS; i++)
        PRIORITYrity_array[i] = i;
    SWAPPER(PRIORITYrity_array, NUMBEROFTHREADS);
    for(int i = 0; i < NUMBEROFTHREADS; i++){
        char PROCESSNAME[8];
        snprintf(PROCESSNAME, 8, "Task_%d", i);
        TaskInit(&SCDL->tasks[i], i, PRIORITYrity_array[i], PROCESSNAME);
    }
    pthread_mutex_init(&SCDL->mutex, NULL);
    strcpy(SCDL->memory, "None");

};




//  sig_handler
Sig_Args args = {};

// Setter for task Args
void __set_sig_args(Task** tasks, int NUMBEROFTHREADS){
    args.tasks = tasks;
    args.NUMBEROFTHREADS = NUMBEROFTHREADS;
}
// sig Handler fot SIGUSR1
void sig_handler(int signum){

    if(signum == SIGUSR1)
    {
        TASKIDSORTER();
        printf("Get sig SIGUSR1\n");
    }
}
void SchedulerStart(Scheduler* SCDL){

    signal(SIGUSR1, sig_handler);
    __set_sig_args(&SCDL->tasks, SCDL->NUMBEROFTHREADS);

    pthread_t threads[SCDL->NUMBEROFCORES];
    pthread_attr_t at;
    cpu_set_t cpuset;
    printf("Program starting on %d cores and %d threads, QUANTUM = %d\n", SCDL->NUMBEROFCORES, SCDL->NUMBEROFTHREADS, SCDL->QUANTUM);
    printf("Default tasks \n");
    printf("\tTID  |  PRIORITYrity  \n");
    for(int i = 0; i < SCDL->NUMBEROFTHREADS; i++){
        printf("\t %d   |     %d   \n", taskGetId(&SCDL->tasks[i]), taskpriority(&SCDL->tasks[i]));
    }
    
    taskpriorityRITYSORTER(&SCDL->tasks, SCDL->NUMBEROFTHREADS);
    ScheduleInfo arg[SCDL->NUMBEROFCORES];
    while(RUN)
    {
        if(!signalFlag)
        {   
            int i = 0;
            for(; i < SCDL->NUMBEROFCORES && i < SCDL->NUMBEROFTHREADS; i++){
                CPU_ZERO(&cpuset);
                CPU_SET(i, &cpuset);
                pthread_attr_init(&at);
                pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                arg[i] = (ScheduleInfo){&SCDL->tasks[i],SCDL->memory, &SCDL->mutex, SCDL->QUANTUM};
                pthread_create(&threads[i], NULL, taskWake, &arg[i]);
            }
            for(int j = 0; j < SCDL->NUMBEROFCORES; j++){
                pthread_join(threads[j], NULL);
            }
            printf("\n");

            if(!signalFlag && SCDL->NUMBEROFTHREADS > SCDL->NUMBEROFCORES)
            {
                for(; i < SCDL->NUMBEROFCORES * 2; i++){
                CPU_ZERO(&cpuset);
                CPU_SET(i - SCDL->NUMBEROFCORES, &cpuset);
                pthread_attr_init(&at);
                pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                arg[i - SCDL->NUMBEROFCORES] = (ScheduleInfo){&SCDL->tasks[i % SCDL->NUMBEROFTHREADS],SCDL->memory, &SCDL->mutex, SCDL->QUANTUM};
                pthread_create(&threads[i - SCDL->NUMBEROFCORES], NULL, taskWake, &arg[i - SCDL->NUMBEROFCORES]);
                }
                for(int j = 0; j < SCDL->NUMBEROFCORES; j++){
                    pthread_join(threads[j], NULL);
                }
            }
        }
        else{
            for(int i = 0; i < SCDL-> NUMBEROFTHREADS; i += SCDL->NUMBEROFCORES)
            {
                for(int j = 0; j < SCDL->NUMBEROFCORES; j++){
                    CPU_ZERO(&cpuset);
                    CPU_SET(j, &cpuset);
                    pthread_attr_init(&at);
                    pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                    arg[j] = (ScheduleInfo){&SCDL->tasks[(i+j) % SCDL->NUMBEROFTHREADS],SCDL->memory, &SCDL->mutex, SCDL->QUANTUM};
                    pthread_create(&threads[j], NULL, taskWake, &arg[j]);
                }
                for(int j = 0; j < SCDL->NUMBEROFCORES; j++){
                    pthread_join(threads[j], NULL);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}



void TASKIDSORTER(){
    Task* temp_array = (Task*)malloc(args.NUMBEROFTHREADS * sizeof(Task));
    Task* tasks = *args.tasks;
    for (int i = 0; i < args.NUMBEROFTHREADS; i++){
        temp_array[taskGetId(&tasks[i])] = tasks[i];
    }
    *args.tasks = temp_array;
    signalFlag = 1;
}

void taskpriorityRITYSORTER(Task** p_tasks, int NUMBEROFTHREADS){
    
    Task* temp_array = (Task*)malloc(NUMBEROFTHREADS * sizeof(Task));
    Task* tasks = *p_tasks;
    for (int i = 0; i < NUMBEROFTHREADS; i++){
        temp_array[NUMBEROFTHREADS - taskpriority(&tasks[i])- 1] = tasks[i];
    }
    *p_tasks = temp_array;
}
void SWAPPER(int *arr, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          int j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = arr[j];
          arr[j] = arr[i];
          arr[i] = t;
        }
    }
}
