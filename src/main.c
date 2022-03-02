#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"
#include "Process.h"
#include "ProcessQueue.h"

#define FRAMES_PER_SECOND 16
#define SLEEP_TIME 1000000.0f/FRAMES_PER_SECOND

/*
    TODO:
        - Atualmente o primeiro processo a executar não é impresso no stdout. 
          É preciso que ele seja impresso assim que entrar na priorityQueue 
          (e não no primeiro frame)
        
        - Múltiplas filas de feedBack (no lugar de uma única fila chamada priorityQueue)

        - Como tratar os diferentes tipos de I/O e preempção

        - Como receber a entrada do usuário
*/

typedef struct Time{
    float sinceStart;
    float deltaTime;
    float quantumCountdown;
}Time_t;
Time_t Time;

typedef struct Simulation{
    ProcessQueue* arrivalQueue;
    ProcessQueue* highPriorityQueue;
    ProcessQueue* lowPriorityQueue;
    Process* currentProcess;
    Process* previousProcess;
    float quantum;
}Simulation;

Simulation* newSimulation(Process* processes[], int n_processes, float quantum){
    Simulation* simulation = (Simulation*) malloc(sizeof(Simulation));

    // Sort received processes and add them to the arrival queue
    ProcessQueue* arrivalQueue = newProcessQueue(n_processes);
    qsort(processes, n_processes, sizeof(*processes), compareArrivalTime);
    for(int i=0; i < n_processes; i++){
        enqueue(arrivalQueue, processes[i]);
    }
    simulation->arrivalQueue = arrivalQueue;
        
    // Process Queue will keep arrived processes with remaining burst time
    simulation->highPriorityQueue = newProcessQueue(n_processes); 
    simulation->quantum = quantum;
    return simulation;
}

void update(Simulation* simulation){
    // Simulation variables in function scope to aliviate verbosishness
    ProcessQueue* arrivalQueue = simulation->arrivalQueue;
    ProcessQueue* highPriorityQueue = simulation->highPriorityQueue;
    ProcessQueue* lowPriorityQueue = simulation->lowPriorityQueue;
    Process* currentProcess = simulation->currentProcess;
    Process* previousProcess = simulation->previousProcess;
    float quantum = simulation->quantum;


    // Add the process to queue if arrived
    while(!isEmpty(arrivalQueue)){
        if(Time.sinceStart >= front(arrivalQueue)->arrivalTime){
            Process* nextProcess = dequeue(arrivalQueue);
            enqueue(highPriorityQueue, nextProcess);
        }
    }

    // Update current process (currently being called every frame)
    if(!isEmpty(highPriorityQueue)){
        currentProcess = front(highPriorityQueue);
    }

    // If current quantum countDown is over, go to next process with remaining burst time
    if(Time.quantumCountdown <= 0.f){
        // Reset quantum countdown
        Time.quantumCountdown = quantum;

        // Go to the next process with remaining burst time
        previousProcess = currentProcess;
        if(!isEmpty(highPriorityQueue)){
            dequeue(highPriorityQueue);
            currentProcess = front(highPriorityQueue);
        }

        // If there is still burstTime in the previous process, reenqueue it
        if(previousProcess->burstTime > 0.0f){
            enqueue(highPriorityQueue, previousProcess);
        }

        // If there was a process switch, print it to the console
        if(currentProcess != previousProcess){
            printf("\rSwitched to Process %s at time %.2f\n", 
                    currentProcess->name, Time.sinceStart);
        }
    }

    // Print timers to console
    printf("\rTime: %.2f\t %s Time: %.2f",
            Time.sinceStart, currentProcess->name, currentProcess->burstTime);

    // Decrease timers
    Time.quantumCountdown -= Time.deltaTime;
    currentProcess->burstTime -= Time.deltaTime;  
}


int main(int argc, char* argv[]){
    int n_processes = 5;
    Process* p1 = newProcess("P1", 10, 0);
    Process* p2 = newProcess("P2",  1, 0);
    Process* p3 = newProcess("P3",  2, 0);
    Process* p4 = newProcess("P4",  1, 0);
    Process* p5 = newProcess("P5",  5, 0);

    Process* processes[5] = {p1, p2, p3, p4, p5};

    float quantum = 1;

    Simulation* simulation = newSimulation(processes, 5, quantum);

    // Initialize Simulation
    bool isSimulationRunning = true;
    Time.sinceStart = 0;
    Time.deltaTime = SLEEP_TIME/1000000;
    Time.quantumCountdown = quantum;

    // Run main loop while there is still a process to arrive or to be processed
    while(!(isEmpty(simulation->highPriorityQueue) && isEmpty(simulation->arrivalQueue))){
        // Update is the main function for the simulation
        update(simulation);

        Time.sinceStart += Time.deltaTime;
        fflush(stdout);
        usleep((int) (SLEEP_TIME));
    }
    printf("\r--------------------------------------------\n");
    printf("            Simulation Finished!\n");
    printf("--------------------------------------------\n");

}
