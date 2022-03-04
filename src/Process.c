#include "Process.h"
#include "utils.h"

Process* newProcess(const char* name, float burstTime, float arrivalTime){
    Process* process = (Process*) safe_malloc(sizeof(Process));
    process->name = name;
    process->burstTime = burstTime;
    process->arrivalTime = arrivalTime;
    return process;
}

/* int compareArrivalTime(const void* p1, const void* p2){ */
/*     Process* P1 = (Process*)p1; */
/*     Process* P2 = (Process*)p2; */
/*     if( P1->arrivalTime > P2->arrivalTime ) return 1; */
/*     else if( P1->arrivalTime < P2->arrivalTime ) return -1; */
/*     return 0; */
/* } */


