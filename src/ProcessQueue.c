#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "ProcessQueue.h"

ProcessQueue* newProcessQueue(unsigned capacity){
    ProcessQueue* queue = (ProcessQueue *) safe_malloc(sizeof(ProcessQueue));
    queue->m_length = 0;
    queue->m_front = 0;
    queue->m_back = capacity-1;
    queue->m_array = (Process**) safe_malloc(capacity * sizeof(Process*));
    queue->m_capacity = capacity;

    return queue;
}

ProcessQueue* ProcessQueue_delete(ProcessQueue* queue){
    ProcessQueue* this = queue;
    free(this->m_array);
    free(this);
}

bool ProcessQueue_isFull(ProcessQueue* queue){
    ProcessQueue* this = queue;
    return (this->m_length == this->m_capacity);
}

bool ProcessQueue_isEmpty(ProcessQueue* queue){
    ProcessQueue* this = queue;
    return (this->m_length == 0);
}

Process* ProcessQueue_front(ProcessQueue* queue){
    ProcessQueue* this = queue;
    if (ProcessQueue_isEmpty(this)){
        fprintf(stderr, "Tentou obter elemento de uma fila vazia\n");
        exit(EXIT_FAILURE);
    }
    return this->m_array[this->m_front];
}

int ProcessQueue_length(ProcessQueue* queue){
    ProcessQueue* this = queue;
    return this->m_length;
}

void ProcessQueue_enqueue(ProcessQueue* queue, Process* element){
    ProcessQueue* this = queue;
    if (ProcessQueue_isFull(this)){
        // @TODO: realloc no array e aumentar capacidade
        fprintf(stderr, "Tentou adicionar elemento a uma fila cheia\n");
        exit(EXIT_FAILURE);
    }

    this->m_back = (this->m_back + 1) % this->m_capacity;
    this->m_array[this->m_back] = element;
    this->m_length++;
}

Process* ProcessQueue_dequeue(ProcessQueue* queue){
    ProcessQueue* this = queue;
    if (ProcessQueue_isEmpty(this)){
        fprintf(stderr, "Tentou remover elemento de uma fila vazia\n");
        exit(EXIT_FAILURE);
    }

    Process* element = this->m_array[this->m_front];
    this->m_front = (this->m_front + 1) % this->m_capacity;
    this->m_length--;
    return element;
}

const char* ProcessQueue_toString(ProcessQueue* queue){
    ProcessQueue* this = queue;
    char* str = (char*) safe_malloc(16*this->m_capacity+3 * sizeof(char));
    sprintf(str, "[");
    for(int i = this->m_front; i != this->m_back; i = (i+1)%this->m_capacity){
        sprintf(str, "%s%s, ", str, this->m_array[i]->name);
    }
    sprintf(str, "%s%s]", str, this->m_array[this->m_back]->name);
    return str;
}

/*
int main()
{
    ProcessQueue* queue = newProcessQueue(10);

    Process* p1 = newProcess("P1", 10, 0);
    Process* p2 = newProcess("P2",  1, 0);
    Process* p3 = newProcess("P3",  2, 0);
    Process* p4 = newProcess("P4",  1, 0);
    Process* p5 = newProcess("P5",  5, 0);
 
    ProcessQueue_enqueue(queue, p1);
    ProcessQueue_enqueue(queue, p2);
    ProcessQueue_enqueue(queue, p3);
    ProcessQueue_enqueue(queue, p4);
    ProcessQueue_enqueue(queue, p5);

    printf("%s\n", ProcessQueue_toString(queue));
 
    printf("%s dequeued from queue\n\n",
           ProcessQueue_dequeue(queue)->name);
 
    printf("%s\n", ProcessQueue_toString(queue));

    printf("Front item is %s\n", ProcessQueue_front(queue)->name);
 
    return 0;
}
*/
