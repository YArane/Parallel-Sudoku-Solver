#include <stdlib.h>
#include <stdio.h>
#include "decision_tree.h"


int get_number_of_possibilities(Cell *cell);
int *hash_cells(Puzzle *instance);
Cell *convert_hash_to_list(int *hash, Puzzle *instance);
int get_number_of_unknowns(Puzzle *instance);


Node *build_tree(Puzzle *instance){
 //   Node *root = init_root(instance);

    printf("Building Tree..\n");
     
    printf("------ pre sort:\n");
    int i;
    for(i=0;i<instance->size*instance->size;i++){
        printf("%d\n", get_number_of_possibilities(instance->cells+i));
    }
    //Cell *sorted_list = get_sorted_possibility_list(instance);
    int *hash = hash_cells(instance);
    Cell *sorted_list = convert_hash_to_list(hash, instance);
    printf("------ post sort:\n");
    for(i=0;i<get_number_of_unknowns(instance);i++){
        printf("%d: %d\n", i, get_number_of_possibilities(sorted_list+i));
    }
}

/*void init_root(Puzzle *instance){
    Node *root;
    root = malloc(sizeof Node);
    


    root->instance = NULL;
    root->children = get_children(instance);
}

Puzzle *children get_children(Puzzle *instance){
    int i  
}
*/
int get_number_of_possibilities(Cell *cell){
    int count = 0;
    int* list = cell->possibility_list;
    int i;
     for(i=1;i<=9;i++){
        if(list[i] == 1){
            count++;
        }
   }
   return count;
}

int get_number_of_unknowns(Puzzle *instance){
    int count = 0;
    Cell *cell = instance->cells;
    int i;
    for(i=0;i<instance->size*instance->size;i++){
           if(cell[i].value == -1){
                  count++;
           }
    }
    return count;
}

Cell *convert_hash_to_list(int *hash, Puzzle *instance){
    int number_of_unknowns = get_number_of_unknowns(instance);
    Cell *sorted_list = malloc(sizeof(Cell)*number_of_unknowns);
    int i, j;
    for(i=0, j=0;i<instance->size*instance->size*number_of_unknowns;i++){
        if(hash[i] != -1){
                sorted_list[j++] = instance->cells[hash[i]];
        }
    } 
    return sorted_list;
}

int get_next_available_bucket(int *hash, int key){
   int i;
   for(i=81*(key-1);i<81*(key);i++){
        if(hash[i] == -1)
                return i;
   }
   printf("no more spots in hash\n");
   return -1;
}

void hash(int *hash, int key, Puzzle *instance){
   if(get_number_of_possibilities(&instance->cells[key]) == 0)
           return;
   int bucket = get_next_available_bucket(hash, get_number_of_possibilities(&instance->cells[key]));
   hash[bucket] = key; 
}

int *hash_cells(Puzzle *instance){
    int number_of_unknowns = get_number_of_unknowns(instance);
    int *hashed_cells = malloc(sizeof(int)*instance->size*instance->size*number_of_unknowns);

    int i;
    for(i=0;i<(instance->size*instance->size*number_of_unknowns);i++){
        hashed_cells[i] = -1;
    }

    for(i=0;i<instance->size*instance->size;i++){
        hash(hashed_cells, i, instance);
    }

    return hashed_cells;
}
