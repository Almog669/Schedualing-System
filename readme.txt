Member 1: Name: ABC
Member 2: Name: DEF

API EXPLAINED
    taskGetName - Returns a unique ascii name of the thread (It is less than 19 bytes long).
    taskGetId - Returns a unique ID.
    taskWait - Suspends a task for n seconds.
    taskGetMem - Get the buffer.
    taskSetMem - Set the buffer.
    taskReleaseMem - Release the buffer, not owner of buffer.
    taskPrio - Return the priority of the task.
    taskWake - Wake a task by its task id.

Compile:
    Method 1:   make all 
    Method 2:   gcc main.c -o OS -lpthread

Run:
    ./OS (DEFAULT PARAMS)
    ./OS p1 n3 (PARAMS)
    p => Number Of Processes
    n => Number Of Threads

Signal:
    ps -fC OS
    kill -s USR1 <PID>