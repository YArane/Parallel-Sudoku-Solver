#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "../puzzle_structs/puzzle.h"

typedef struct Node{
        Puzzle *instance;
        int number_of_children;
        int level;
        struct Node *children;
}Node;


Node *build_tree(Puzzle *instance, int *rank, int *np);

int get_number_of_possibilities(Cell *cell);

typedef struct QueueNode{
    Node *value;
    struct QueueNode *next;
}QueueNode;

typedef struct Queue{
    QueueNode *head, *tail;
    int size;
}Queue;

void enqueue(Queue *queue, Node *value, int level);

Node *dequeue(Queue *queue);
        
int is_empty(Queue *queue);

void print_queue(Queue *queue);

void *create_mpi_node(int num_children);

#endif
