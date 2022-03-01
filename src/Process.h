#ifndef __processh__
#define __processh__

typedef struct Process{
    const char* name;

    float burstTime, arrivalTime;
}Process;

Process* newProcess(const char* name, float burstTime, float arrivalTime);

int compareArrivalTime(const void* p1, const void* p2);

#endif
