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

ProcessQueue* ProcessQueue_delete(ProcessQueue* queue);

bool ProcessQueue_isFull(ProcessQueue* queue);

bool ProcessQueue_isEmpty(ProcessQueue* queue);

Process* ProcessQueue_front(ProcessQueue* queue);

int ProcessQueue_length(ProcessQueue* queue);

void ProcessQueue_enqueue(ProcessQueue* queue, Process* element);

Process* ProcessQueue_dequeue(ProcessQueue* queue);

const char* ProcessQueue_toString(ProcessQueue* queue);

#endif
