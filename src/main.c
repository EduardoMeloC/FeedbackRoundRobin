#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"
#include "Process.h"
#include "ProcessQueue.h"

#define FRAMES_PER_SECOND 16
#define SLEEP_TIME 1000000.0f/FRAMES_PER_SECOND

typedef struct Time{
    float sinceStart;
    float deltaTime;
    float quantumCountdown;
}Time_t;
Time_t Time;

typedef struct Simulation{
    ProcessQueue* arrivalQueue;
    ProcessQueue* processQueue;
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
        ProcessQueue_enqueue(arrivalQueue, processes[i]);
    }
    simulation->arrivalQueue = arrivalQueue;
        
    // Process Queue will keep arrived processes with remaining burst time
    simulation->processQueue = newProcessQueue(n_processes); 
    simulation->quantum = quantum;
    return simulation;
}

void update(Simulation* simulation){
    // Add the process to queue if arrived
    while(!ProcessQueue_isEmpty(simulation->arrivalQueue)){
        if(Time.sinceStart == ProcessQueue_front(simulation->arrivalQueue)->arrivalTime){
            ProcessQueue_enqueue(simulation->processQueue, 
                    ProcessQueue_dequeue(simulation->arrivalQueue));
        }
    }

    // Print to console when process switched
    if(!ProcessQueue_isEmpty(simulation->processQueue)){
        if(Time.quantumCountdown == simulation->quantum){
            simulation->currentProcess = ProcessQueue_front(simulation->processQueue);

            if(simulation->currentProcess != simulation->previousProcess){
                printf("\rSwitched to Process %s at time %.2f\n", 
                        simulation->currentProcess->name, Time.sinceStart);
            }
        }
    }

    // Print timers to console
    printf("\rTime: %.2f\t %s Time: %.2f",
            Time.sinceStart, simulation->currentProcess->name, simulation->currentProcess->burstTime);
    
    // Decrease timers
    Time.quantumCountdown -= Time.deltaTime;
    simulation->currentProcess->burstTime -= Time.deltaTime;  

    // If current quantum countDown is over, go to next process with remaining burst time
    if(Time.quantumCountdown <= 0.f){
        Time.quantumCountdown = simulation->quantum;
        simulation->previousProcess = ProcessQueue_dequeue(simulation->processQueue);
        if(simulation->previousProcess->burstTime > 0.0f)
            ProcessQueue_enqueue(simulation->processQueue, simulation->previousProcess);
    }
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
    Time.quantumCountdown = quantum;

    // Run main loop while there is still a process to arrive or to be processed
    while(!(ProcessQueue_isEmpty(simulation->processQueue) && ProcessQueue_isEmpty(simulation->arrivalQueue))){
        Time.deltaTime = SLEEP_TIME/1000000;

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
