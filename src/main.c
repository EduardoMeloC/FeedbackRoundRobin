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
        - Múltiplas filas de feedBack (no lugar de uma única fila chamada priorityQueue)

        - Como tratar os diferentes tipos de I/O e preempção

        - Como receber a entrada do usuário
*/

typedef struct Time{
    float sinceStart;
    float deltaTime;
    float quantumCountdown;
    float diskCountdown;
    float magTapeCountdown;
    float printerCountdown;
}Time_t;
Time_t Time;

typedef struct Simulation{
    ProcessQueue* arrivalQueue;
    ProcessQueue* highPriorityQueue;
    ProcessQueue* lowPriorityQueue;
    Process* currentProcess;
    Process* previousProcess;
    float quantum;

    ProcessQueue* diskQueue;
    ProcessQueue* magTapeQueue;
    ProcessQueue* printerQueue;
    Process* currentDisk;
    Process* currentMagTape;
    Process* currentPrinter;

}Simulation;

Simulation* newSimulation(Process* processes[], int n_processes, float quantum){
    Simulation* simulation = (Simulation*) malloc(sizeof(Simulation));

    // Sort received processes and add them to the arrival queue
    ProcessQueue* arrivalQueue = newProcessQueue(n_processes);

    for(int i=0; i < n_processes-1; i++){
        for(int j=0; j < n_processes-1; j++){
            if(processes[j]->arrivalTime > processes[j+1]->arrivalTime){
                Process* temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }

    for(int i=0; i < n_processes; i++){
        enqueue(arrivalQueue, processes[i]);
    }

    simulation->arrivalQueue = arrivalQueue;

    // Process Queue will keep arrived processes with remaining burst time
    simulation->highPriorityQueue = newProcessQueue(n_processes);
    simulation->currentProcess = NULL;
    simulation->quantum = quantum;

    // Count number of total IO requests
    int n_io = 0;
    for(int i=0; i<n_processes; i++){
        n_io += processes[i]->ioSize;
    }
    simulation->diskQueue = newProcessQueue(n_io);
    simulation->magTapeQueue = newProcessQueue(n_io);
    simulation->printerQueue = newProcessQueue(n_io);

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

    ProcessQueue* diskQueue = simulation->diskQueue;
    ProcessQueue* magTapeQueue = simulation->magTapeQueue;
    ProcessQueue* printerQueue = simulation->printerQueue;
    Process* currentDisk = simulation->currentDisk;
    Process* currentMagTape = simulation->currentMagTape;
    Process* currentPrinter = simulation->currentPrinter;

    // Add the process to queue if arrived
    while(!isEmpty(arrivalQueue)){
        if(Time.sinceStart >= front(arrivalQueue)->arrivalTime){
            Process* nextProcess = dequeue(arrivalQueue);
            bool wasEmpty = isEmpty(highPriorityQueue);
            enqueue(highPriorityQueue, nextProcess);

            if(wasEmpty){
                currentProcess = front(highPriorityQueue);
                printf("\rSwitched to Process %s at time %.2f\n",
                    currentProcess->name, Time.sinceStart);
            }
        } else break;
    }

    //If current process has IO request now, send request and go to next process with remaining burst time
    if(currentProcess != NULL && currentProcess->ioSize > 0) {
        if(currentProcess->origBurstTime - currentProcess->burstTime >= currentProcess->ioTimes[currentProcess->ioAtual]){
            switch(currentProcess->ioTypes[currentProcess->ioAtual]){
                case disk:
                    enqueue(diskQueue,currentProcess);
                break;

                case magTape:
                    enqueue(magTapeQueue,currentProcess);
                break;

                case printer:
                    enqueue(printerQueue,currentProcess);
                break;

                default:
                    printf("Tipo de IO inexistente.\n");
                    exit(0);
            }

            // Reset quantum countdown
            Time.quantumCountdown = quantum;

            // Go to the next process with remaining burst time
            previousProcess = currentProcess;
            dequeue(highPriorityQueue);
            if(!isEmpty(highPriorityQueue)){
                currentProcess = front(highPriorityQueue);
            }
        }
    }

    // If current quantum countDown is over, go to next process with remaining burst time
    if(currentProcess != NULL && Time.quantumCountdown <= 0.f){
        // Reset quantum countdown
        Time.quantumCountdown = quantum;

        // Go to the next process with remaining burst time
        previousProcess = currentProcess;
        dequeue(highPriorityQueue);
        if(!isEmpty(highPriorityQueue)){
            currentProcess = front(highPriorityQueue);
        }

        // If there is still burstTime in the previous process, reenqueue it
        if(previousProcess->burstTime > 0.0f){
            enqueue(highPriorityQueue, previousProcess);
        }
        //else printf("Process %s ended at time %.2f\n", currentProcess->name, Time.sinceStart);

        // If there was a process switch, print it to the console
        if(currentProcess != previousProcess){
            printf("\rSwitched to Process %s at time %.2f\n",
                    currentProcess->name, Time.sinceStart);
        }
    }

    // Print timers to console
    if(currentProcess == NULL){
        printf("                                                  ");
        printf("\rTime: %.2f\t CPU Empty",
                Time.sinceStart);
    }
    else{
        printf("                                                  ");
        printf("\rTime: %.2f\t %s Time: %.2f",
                Time.sinceStart, currentProcess->name, currentProcess->burstTime);
    }

    // Decrease timers
    Time.quantumCountdown -= Time.deltaTime;
    if(currentProcess != NULL)
        currentProcess->burstTime -= Time.deltaTime;

    //If disk is executing, decrease time
    if(currentDisk != NULL){
        Time.diskCountdown -= Time.deltaTime;
        if(Time.diskCountdown <= 0.){
            currentDisk->ioAtual++;
            currentDisk->ioSize--;
            //TODO: EH BAIXA PRIORIDADE, NAO ALTA!!!
            enqueue(highPriorityQueue,currentDisk);
            currentDisk = NULL;
        }
    }
    if(currentDisk == NULL){
        if(!isEmpty(diskQueue)){
            currentDisk = dequeue(diskQueue);
            Time.diskCountdown = TIMER_DISK;
        }
    }
    //If magnetic tape is executing, decrease time
    if(currentMagTape != NULL){
        Time.magTapeCountdown -= Time.deltaTime;
        if(Time.magTapeCountdown <= 0.){
            currentMagTape->ioAtual++;
            currentMagTape->ioSize--;
            enqueue(highPriorityQueue,currentMagTape);
            currentMagTape = NULL;
        }
    }
    if(currentMagTape == NULL){
        if(!isEmpty(magTapeQueue)){
            currentMagTape = dequeue(magTapeQueue);
            Time.magTapeCountdown = TIMER_MAGTAPE;
        }
    }
    //If printer is executing, decrease time
    if(currentPrinter != NULL){
        Time.printerCountdown -= Time.deltaTime;
        if(Time.printerCountdown <= 0.){
            currentPrinter->ioAtual++;
            currentPrinter->ioSize--;
            enqueue(highPriorityQueue,currentPrinter);
            currentPrinter = NULL;
        }
    }
    if(currentPrinter == NULL){
        if(!isEmpty(printerQueue)){
            currentPrinter = dequeue(printerQueue);
            Time.printerCountdown = TIMER_PRINTER;
        }
    }

    // Reassign pointers
    simulation->currentProcess = currentProcess;
    simulation->previousProcess = previousProcess;

    simulation->currentDisk = currentDisk;
    simulation->currentMagTape = currentMagTape;
    simulation->currentPrinter = currentPrinter;
}


int main(int argc, char* argv[]){
    int n_processes = 5;
    Process* p1 = newProcess("P1", 10, 0);
    newIO(p1, 5, disk);
    newIO(p1, 6, magTape);
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
