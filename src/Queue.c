#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "Queue.h"

Queue* newQueue(unsigned capacity){
    Queue* queue = (Queue *) safe_malloc(sizeof(Queue));
    queue->m_length = 0;
    queue->m_front = 0;
    queue->m_back = capacity-1;
    queue->m_array = (int*) safe_malloc(capacity * sizeof(int));
    queue->m_capacity = capacity;

    return queue;
}

Queue* Queue_delete(Queue* queue){
    Queue* this = queue;
    free(this->m_array);
    free(this);
}

bool Queue_isFull(Queue* queue){
    Queue* this = queue;
    return (this->m_length == this->m_capacity);
}

bool Queue_isEmpty(Queue* queue){
    Queue* this = queue;
    return (this->m_length == 0);
}

int Queue_front(Queue* queue){
    Queue* this = queue;
    if (Queue_isEmpty(this)){
        fprintf(stderr, "Tentou obter elemento de uma fila vazia\n");
        exit(EXIT_FAILURE);
    }
    return this->m_array[this->m_front];
}

int Queue_length(Queue* queue){
    Queue* this = queue;
    return this->m_length;
}

void Queue_enqueue(Queue* queue, int element){
    Queue* this = queue;
    if (Queue_isFull(this)){
        // @TODO: realloc no array e aumentar capacidade
        fprintf(stderr, "Tentou adicionar elemento a uma fila cheia\n");
        exit(EXIT_FAILURE);
    }

    this->m_back = (this->m_back + 1) % this->m_capacity;
    this->m_array[this->m_back] = element;
    this->m_length++;
}

int Queue_dequeue(Queue* queue){
    Queue* this = queue;
    if (Queue_isEmpty(this)){
        fprintf(stderr, "Tentou remover elemento de uma fila vazia\n");
        exit(EXIT_FAILURE);
    }

    int element = this->m_array[this->m_front];
    this->m_front = (this->m_front + 1) % this->m_capacity;
    this->m_length;
    return element;
}

const char* Queue_toString(Queue* queue){
    Queue* this = queue;
    char* str = (char*) safe_malloc(3*this->m_capacity+3 * sizeof(char));
    sprintf(str, "[");
    for(int i = this->m_front; i != this->m_back; i = (i+1)%this->m_capacity){
        sprintf(str, "%s%d, ", str, this->m_array[i]);
    }
    sprintf(str, "%s%d]", str, this->m_array[this->m_back]);
    return str;
}

/* int main() */
/* { */
/*     Queue* queue = newQueue(1000); */
 
/*     Queue_enqueue(queue, 10); */
/*     Queue_enqueue(queue, 20); */
/*     Queue_enqueue(queue, 30); */
/*     Queue_enqueue(queue, 40); */

/*     printf("%s\n", Queue_toString(queue)); */
 
/*     printf("%d dequeued from queue\n\n", */
/*            Queue_dequeue(queue)); */
 
/*     printf("%s\n", Queue_toString(queue)); */

/*     printf("Front item is %d\n", Queue_front(queue)); */
 
/*     return 0; */
/* } */
