#ifndef QUEUE_H
#define QUEUE_H

#include "decision_tree.h"

typedef struct QueueNode{
    Node *value;
    struct QueueNode *next;
}QueueNode;

typedef struct Queue{
    QueueNode *head, *tail;
}Queue;

void enqueue(Queue *queue, Node *value);

Node *dequeue(Queue *queue);
        
int is_empty(Queue *queue);

#endif
