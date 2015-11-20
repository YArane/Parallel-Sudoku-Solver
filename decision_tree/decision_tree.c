#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decision_tree.h"


int *hash_cells(Puzzle *instance);
Cell *convert_hash_to_list(int *hash, Puzzle *instance);
int get_number_of_unknowns(Puzzle *instance);
Node *construct_tree(Node *crt, Cell *sorted_list);
Node *assign_children(Puzzle *instance, Cell *cell, int *number_of_children);
Node *init_root(Puzzle *instance, Cell *cell);
void print_tree(Node *root, int level);

Node *build_tree(Puzzle *instance){
    int recheck = 1;
    while(recheck == 1){
            printf("recheck\n");
            int i;
            recheck = 0;
            for(i=0;i<instance->size*instance->size;i++){
                if(get_number_of_possibilities(&instance->cells[i]) == 1){
                    recheck = 1;
                    int value = get_possibility(&instance->cells[i], 0);
                    set_cell_value((&instance->cells[i])->row, (&instance->cells[i])->col, instance, value);
                }
            }
    }
    int j, k;
    for(j=0;j<instance->size;j++){
            for(k=0;k<instance->size;k++){
                print_possibility_list(j, k, instance);
            }
    }
    

    printf("Building Tree..\n");
     
    // sort cells in increasing order of possibility list size
    int *hash = hash_cells(instance);
    Cell *sorted_list = convert_hash_to_list(hash, instance);

    int i=0;
    while((sorted_list+i)->value != -99){
        printf("**\n");
        printf("sorted_list [%d][%d]: %d\n", (sorted_list+i)->row, (sorted_list+i)->col, get_number_of_possibilities(sorted_list + i));
        i++;
    }

    print_puzzle(instance);
    
    if(sorted_list->value != -99){
        Node *root = init_root(instance, sorted_list);
        printf("init root success\n");
        construct_tree(root, ++sorted_list);
        printf("finished building tree\n");
        print_tree(root, 0);
    }
}

void print_tree(Node *crt, int level){
    int i;
    printf("NUMBER OF CHILDREN: %d\n", crt->number_of_children);
    for(i=0;i<crt->number_of_children;i++){
        print_puzzle_by_level(crt->children[i].instance, level);
        print_tree(&crt->children[i], ++level);
    }

}


Node *construct_tree(Node *crt, Cell *sorted_list){
    /*printf("Constructing tree\nBegininning with cell: [%d][%d]\n which has possibility list: \n", sorted_list->row, sorted_list->col);
    for(i=1;i<10;i++){
        printf("%d ", sorted_list->possibility_list[i]);
    }*/
    int i;
    if(sorted_list->value == -99){
        printf("end of tree.\n");
        return;
    }    
    if(get_number_of_possibilities(sorted_list) == 0){
            construct_tree(crt, sorted_list+1);
            return;
    }
    int number_of_children = get_number_of_possibilities(sorted_list);
    for(i=0;i<number_of_children;i++){
       crt->children[i].children = assign_children((crt->children[i].instance), &sorted_list[0], &crt->children[i].number_of_children); 
       return construct_tree(&crt->children[i], sorted_list+1);
    }
}


int get_possibility(Cell *cell, int index){
   int i, j; 
   for(i=1, j=0;i<=9;i++){
       if(cell->possibility_list[i] == 1){
              if(index == j){
                      return i;
              }else{
                  j++; 
              }
        }
   }
}

Node *assign_children(Puzzle *instance, Cell *cell, int *number_of_children){
    int i;
    *number_of_children = get_number_of_possibilities(cell); 

    Puzzle *children = malloc(sizeof(Puzzle)**number_of_children);
    Node *children_node = malloc(sizeof(Node)**number_of_children);
    for(i=0;i<*number_of_children;i++){
        children[i].size = instance->size;
        int j, k;
        children[i].cells = malloc(sizeof(Cell)*instance->size*instance->size);
        for(j=0;j<instance->size*instance->size;j++){
                children[i].cells[j].possibility_list = malloc(sizeof(int)*(instance->size+1));
            for(k=0;k<=instance->size;k++){
                memcpy(&children[i].cells[j].possibility_list[k], &instance->cells[j].possibility_list[k], sizeof(int));
            }
            memcpy(&children[i].cells[j].value, &instance->cells[j].value, sizeof(int));
        }
        set_cell_value(cell->row, cell->col, &children[i], get_possibility(cell, i));
        children_node[i].instance = &children[i];
        children_node[i].children = NULL;
    }
    
    return children_node;
}

Node *init_root(Puzzle *instance, Cell *cell){
    Node *root;
    root = malloc(sizeof(Node));

    root->instance = NULL;
    int i;
    root->children = assign_children(instance, cell, &root->number_of_children);

    return root;
}

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
    Cell *sorted_list = malloc(sizeof(Cell)*number_of_unknowns+1);
    int i, j;
    for(i=0, j=0;i<instance->size*instance->size*number_of_unknowns;i++){
        if(hash[i] != -1){
                sorted_list[j++] = instance->cells[hash[i]];
                printf("[%d][%d]: %d\n", sorted_list[j-1].row, sorted_list[j-1].col, get_number_of_possibilities(&sorted_list[j-1]));
        }
    } 
    sorted_list[j].value = -99;
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
