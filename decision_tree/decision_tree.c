#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>
#include <sys/time.h>
#include "decision_tree.h"


int *hash_cells(Puzzle *instance);
Cell *convert_hash_to_list(int *hash, Puzzle *instance);
int get_number_of_unknowns(Puzzle *instance);
Node *construct_tree(Node *crt, Cell *sorted_list);
Node *assign_children(Puzzle *instance, Cell *cell, int *number_of_children);
Node *init_root(Puzzle *instance, Cell *cell);
void print_tree(Node *root, int level);
void bfs(Node *root, Cell *sorted_list);
int check_puzzle_validity(Puzzle *instance);

void *create_mpi_node(int num_children){
         // MPI struct
          int num_items = 4;
          int block_length[4] = {1, 1, 1, num_children};
          MPI_Datatype mpi_node;                                      
          MPI_Datatype types[4] = {create_mpi_puzzle(), MPI_INT, MPI_INT, mpi_node};
          MPI_Aint offsets[4];                                     
          offsets[0] = (MPI_Aint)offsetof(Node, instance);          
          offsets[1] = (MPI_Aint)offsetof(Node, number_of_children);
          offsets[2] = (MPI_Aint)offsetof(Node, level);            
          offsets[3] = (MPI_Aint)offsetof(Node, children);
                                                               
          MPI_Type_create_struct(num_items, block_length, offsets, types, &mpi_node); 
          MPI_Type_commit(&mpi_node);                                                         
          return mpi_node;       
}

Node *build_tree(Puzzle *instance, int *rank, int *np){

    struct timeval start, end;
    Node root;
    Cell *sorted_list;

    if(*rank == 0){

    int recheck = 1;
    while(recheck == 1){
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

    printf("Building Tree..\n");
     
    // sort cells in increasing order of possibility list size
    int *hash = hash_cells(instance);
    sorted_list = convert_hash_to_list(hash, instance);

    print_puzzle(instance);
    
    gettimeofday(&start, NULL);

    root = (Node){instance, 0, 0, NULL};

    }

    bfs(&root, sorted_list);

    gettimeofday(&end, NULL);
    
    printf("\n\nAlgorithm's computational part duration :%ld\n", \
                                          ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));


}

void bfs(Node *root, Cell *sorted_list){
    Queue queue = {};


    enqueue(&queue, root, 0);
    int previous_level = 0;
    while(!is_empty(&queue) && sorted_list->value != -99){
        Node *crt = dequeue(&queue);
        // request job from master
        if(check_puzzle_validity(crt->instance)){
            crt->children = assign_children(crt->instance, sorted_list, &crt->number_of_children);
            int i;
            #pragma omp parallel num_threads(4) shared(queue)
            {
            #pragma omp for
            for(i=0;i<get_number_of_possibilities(sorted_list);i++){
                if(check_puzzle_validity(crt->children[i].instance)==1 && get_number_of_unknowns(crt->children[i].instance) == 0){
                    printf("FOUND SOLUTION :D\n");
                    print_puzzle(crt->children[i].instance);
                    return;
                }
                }
               // print_puzzle_by_level(crt->children[i].instance, previous_level);
                enqueue(&queue, &crt->children[i], crt->level+1);
            }
        }
        if(crt->level != previous_level){
            printf("level: %d\n", crt->level);
            previous_level = crt->level;
            sorted_list++;
        }
    }
    
}

void get_job(){
  // slaves request job from master  
}

void print_tree(Node *crt, int level){
    int i;
    printf("NUMBER OF CHILDREN: %d\n", crt->number_of_children);
    for(i=0;i<crt->number_of_children;i++){
        print_puzzle_by_level(crt->children[i].instance, level);
        print_tree(&crt->children[i], ++level);
    }

}


int check_puzzle_validity(Puzzle *instance){
    int m, n;
    int correct_puzzle = 1;
    for(m = 0;m<instance->size;m++){
        for(n = 0;n<instance->size;n++){
           // if(get_cell(m, n, instance)->value != -1){
                if(puzzle_has_contradiction(m, n, instance)){
                        correct_puzzle = 0;
                }
            //}
        }
    }
    return correct_puzzle;
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
        }
    } 
    sorted_list[j].value = -99;
    return sorted_list;
}

int get_next_available_bucket(int *hash, int key){
   int i;
        if(hash[i] == -1)
                return i;
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

void enqueue(Queue *queue, Node *value, int level){
     QueueNode *node = malloc(sizeof(QueueNode));
     value->level = level;
     node->value = value; 
     node->next = NULL;
 
     if(queue->tail){
         queue->tail->next = node;
     }else{
         queue->head = node;
     }
     queue->tail = node;
     queue->size++;
}
 
 Node *dequeue(Queue *queue){
     Node *top = queue->head->value;
     QueueNode *second = queue->head->next;
     free(queue->head);
     queue->head = second;
     if(!queue->head){
             queue->tail = NULL;
     }
     queue->size--;
     return top;
 }
 
 int is_empty(Queue *queue){
     return (queue->head == NULL);
 }
