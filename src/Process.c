#include "Process.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

Process* newProcess(const char* name, float burstTime, float arrivalTime){
    Process* process = (Process*) safe_malloc(sizeof(Process));
    process->name = name;
    process->burstTime = burstTime;
    process->arrivalTime = arrivalTime;

    process->origBurstTime = burstTime;
    process->ioSize = 0;
    process->ioAtual = 0;

    return process;
}

int newIO(const void* p1, const float time ,const IOType type ){
    Process* p = (Process*)p1;
    if(p->ioSize==0){
        p->ioTimes = (float*) safe_malloc(sizeof(float)*MAX_IO_PER_PROCESS);
        p->ioTypes = (IOType*) safe_malloc(sizeof(IOType)*MAX_IO_PER_PROCESS);
        p->ioTimes[0] = time;
        p->ioTypes[0] = type;
        p->ioSize++;
    }
    else {
        if(p->ioSize == MAX_IO_PER_PROCESS) {
            printf("Nao pode fazer mais de %d IO requests por processo, mude \"MAX_IO_PER_PROCESS\" em Process.c\n",MAX_IO_PER_PROCESS);
            exit(0);
        }
        p->ioTimes[p->ioSize] = time;
        p->ioTypes[p->ioSize] = type;
        p->ioSize++;
        for(int i=0; i < (p->ioSize)-1; i++){
            for(int j=0; j < (p->ioSize)-1; j++){
                if(p->ioTimes[j] > p->ioTimes[j+1]){
                    float tempTime = p->ioTimes[j];
                    IOType tempType = p->ioTypes[j];
                    p->ioTimes[j] = p->ioTimes[j+1];
                    p->ioTimes[j+1] = tempTime;
                    p->ioTypes[j] = p->ioTypes[j+1];
                    p->ioTypes[j+1] = tempType;
                }
            }
        }
    }
}
