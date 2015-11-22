#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


void enquque(Queue *queue, Node *value){
    QueueNode *node = malloc(sizeof(QueueNode));
    node->value = value;
    node->next = NULL;

    if(queue->tail){
        queue->tail->next = node;
    }else{
        queue->head = node;
    }
    queue->tail = node;
}

Node *dequeue(Queue *queue){
    Node *top = queue->head->value;
    QueueNode *second = queue->head->next;
    free(queue->head);
    queue->head = second;
    if(!queue->head){
            queue->tail = NULL;
    }
    return top;
}

int is_empty(Queue *queue){
    return !queue->head;
}

