#ifndef __queueh__
#define __queueh__

#include "Process.h"

typedef struct ProcessQueue{
    int m_length;
    int m_front, m_back;
    Process** m_array;
    unsigned m_capacity;
}ProcessQueue;

ProcessQueue* newProcessQueue(unsigned capacity);

ProcessQueue* delete(ProcessQueue* queue);

bool isFull(ProcessQueue* queue);

bool isEmpty(ProcessQueue* queue);

Process* front(ProcessQueue* queue);

int length(ProcessQueue* queue);

void enqueue(ProcessQueue* queue, Process* element);

Process* dequeue(ProcessQueue* queue);

const char* toString(ProcessQueue* queue);

#endif
