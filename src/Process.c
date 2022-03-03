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
    }
}

/*void update_IO_queue(IOType type, void* queue void* argCurrent, void* targetQueue, float countDown, float deltaTime ){
    Process* current = (Process*) argCurrent;

    //If printer is executing, decrease time
    if(current != NULL){
        countdown -= deltaTime;
        if(countdown <= 0.){
            current->ioAtual++;
            current->ioSize--;
            enqueue(targetQueue,current);
            current = NULL;
        }
    }
    if(current == NULL){
        if(!isEmpty(queue)){
            current = dequeue(queue);
            countdown = TIMER_PRINTER;
        }
    }
}/*/

/* int compareArrivalTime(const void* p1, const void* p2){ */
/*     Process* P1 = (Process*)p1; */
/*     Process* P2 = (Process*)p2; */
/*     if( P1->arrivalTime > P2->arrivalTime ) return 1; */
/*     else if( P1->arrivalTime < P2->arrivalTime ) return -1; */
/*     return 0; */
/* } */
