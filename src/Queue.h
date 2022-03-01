#ifndef __queueh__
#define __queueh__

typedef struct Queue{
    int m_length;
    int m_front, m_back;
    int* m_array;
    unsigned m_capacity;
}Queue;

Queue* newQueue(unsigned capacity);

Queue* Queue_delete(Queue* queue);

bool Queue_isFull(Queue* queue);

bool Queue_isEmpty(Queue* queue);

int Queue_front(Queue* queue);

int Queue_length(Queue* queue);

void Queue_enqueue(Queue* queue, int element);

int Queue_dequeue(Queue* queue);

const char* Queue_toString(Queue* queue);

#endif
