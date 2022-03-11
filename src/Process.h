#ifndef __processh__
#define __processh__

#define TIMER_DISK 4.0
#define TIMER_MAGTAPE 6.0
#define TIMER_PRINTER 8.0

#define MAX_IO_PER_PROCESS 20

typedef enum{ disk = 0, magTape = 1, printer = 2 } IOType;

typedef struct Process{
    const char* name;

    float burstTime, arrivalTime;
    float origBurstTime;

    float* ioTimes;
    IOType* ioTypes;
    int ioSize; //IO request list size
    int ioAtual; //index of the next IO request to do
}Process;

Process* newProcess(const char* name, float burstTime, float arrivalTime);

int newIO(const void* p, const float time ,const IOType type);

#endif
