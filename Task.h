// Viki Bril 207129446 && Almog Klinger 305630683

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define MAX_PROCESSNAME_LEN  30


typedef struct TABLECONTROLBLOCK{						
	int PROCESSID;                   			
	int PRIORITY;                  			
	char PROCESSNAME[MAX_PROCESSNAME_LEN];         
} Task;



void TaskInit(Task* t, int PROCESSID, int PRIORITY, char* PROCESSNAME){
	t->PROCESSID = PROCESSID;
	t->PRIORITY = PRIORITY;
	strcpy(t->PROCESSNAME, PROCESSNAME);
};	

char* taskGetPROCESSNAME(Task* task){
	return task->PROCESSNAME;
};

int taskGetId(Task* task){
	return task->PROCESSID;
};

void taskWait(int t_sleep){
	sleep(t_sleep);
};


void taskGetMem(pthread_mutex_t * m){
	pthread_mutex_lock(m);
};

void taskSetMem(char* memory, char* new_memory){
	strcpy(memory, new_memory);
};


void taskReleaseMem(pthread_mutex_t * m){
	pthread_mutex_unlock(m);
};

int taskpriority(Task* task){
	return task->PRIORITY;
};


typedef struct ScheduleInfo_t
{
	Task* task;
	char* memory;
	pthread_mutex_t * mutex;
	int QUANTUM;
} ScheduleInfo;

void* taskWake(void* args){

	ScheduleInfo* sc_info = (ScheduleInfo*)args;
	
	taskGetMem(sc_info->mutex);
	printf("Current Task is %s and Last Task is %s\n", taskGetPROCESSNAME(sc_info->task), sc_info->memory);
	taskSetMem(sc_info->memory, taskGetPROCESSNAME(sc_info->task));
	taskReleaseMem(sc_info->mutex);
	taskWait(sc_info->QUANTUM);
	return NULL;
};