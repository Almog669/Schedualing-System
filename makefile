all : main.c Scheduler.h Task.h
	gcc -pthread main.c -o OS