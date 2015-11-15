#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "../puzzle_structs/puzzle.h"

typedef struct Node{
        Puzzle *instance;
        Puzzle *children;
}Node;


Node *build_tree(Puzzle *instance);

#endif
