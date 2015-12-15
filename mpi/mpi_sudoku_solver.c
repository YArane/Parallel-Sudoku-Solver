#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

static int SIZE = 81;

int get_number_possibilities(int cell[], int i);

typedef struct QueueNode{
    int *puzzle;
    struct QueueNode *next;
}QueueNode;

typedef struct Queue{
    QueueNode *head, *tail;
    int size;
}Queue;

void push(int *);
int *pop();
void enqueue(int *, int);
int* dequeue(int);

void *enqueue_listen();
void *dequeue_listen();

void print_puzzle(int *);
void *find_square_row_and_col(int row, int col, int *square_row, int *square_col, int *puzzle);
int puzzle_has_contradiction(int *puzzle);

void finalize();

MPI_Status status;

Queue queue = {};
int rank, np;
int main(int argc, char **argv){
    
        /*     ~~ init MPI ~~    */
        int provided;

        //if(MPI_SUCCESS != MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &provided)){
        if(MPI_SUCCESS != MPI_Init(&argc, &argv)){
            printf("error initializing MPI.\n");
            return -1;
        }
//        printf("%d\n", MPI_THREAD_SERIALIZED);
 //       printf("%d\n", provided);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);

        /*int puzzle[] = {
                3,1,0,2,8,0,9,7,6,
                0,0,6,0,1,7,0,0,3,
                8,7,9,4,6,3,2,5,1,
                4,0,7,0,3,0,1,0,0,
                1,3,2,0,7,4,8,9,5,
                6,9,8,5,0,1,0,4,7,
                9,8,0,3,5,6,7,2,4,
                2,6,3,7,0,8,5,1,9,
                7,0,5,1,9,2,6,3,8
        };*/
        
/*        int puzzle[] = {
                3,1,4,2,8,5,9,7,6,
                5,2,6,9,1,7,4,8,3,
                8,7,9,4,6,3,2,5,1,
                4,5,7,8,3,9,1,6,2,
                1,3,2,6,7,4,8,9,5,
                6,9,8,5,2,1,3,4,7,
                9,8,1,3,5,6,7,2,4,
                2,6,3,7,4,8,5,1,9,
                7,4,5,1,9,2,6,3,8
        };*/

        int puzzle[] = {
                3,1,4,2,8,5,9,7,6,
                5,2,6,9,1,7,4,8,3,
                8,7,9,4,6,3,2,5,1,
                4,5,7,8,3,9,1,6,2,
                1,3,2,6,7,4,8,0,5,
                6,0,8,5,2,1,3,4,7,
                9,8,1,3,5,0,7,2,4,
                2,6,3,0,4,8,5,1,9,
                7,4,0,1,9,2,6,3,8
        };
        /*     ~~ Master Process ~~    */

        if(rank == 0) push(puzzle);

        printf("rank: %d\n", rank);

        while(1){
           if(rank == 0){
                dequeue_listen();
                enqueue_listen();
           }else if(rank > 0){
                int *buffer = malloc(sizeof(int)*SIZE);     
                buffer = dequeue(1);
                 
                //printf("processor %d requested work\n", rank);               
                // find first unknown cell
                int i;
                for(i=0;i<SIZE;i++){
                    if(buffer[i] == 0)
                        break;
                }
                // puzzle is complete
                if(i == SIZE){
                    if(!puzzle_has_contradiction(buffer)){
                        print_puzzle(buffer);
                        return 0;
                    }
                }
                // make a guess, and enqueue new puzzle
                int j;
                for(j=1;j<=(int)sqrt(SIZE);j++){
                        buffer[i] = j;
                        enqueue(buffer, 1);  
                }
           }
        }

        /*
        if(rank == 0){
            push(puzzle);
            
            pthread_t dequeue_listener, enqueue_listener;

            pthread_create(&dequeue_listener, NULL, dequeue_listen, NULL);
            pthread_create(&enqueue_listener, NULL, enqueue_listen, NULL);

            while(1){
            dequeue_listen();
            enqueue_listen();
            }
        }else{
           int *buffer = malloc(sizeof(int)*SIZE);
            buffer = dequeue(1);
            enqueue(buffer, 1);

        }

        while(1){
        //    if(queue.size != 0){
                int *puzzle = malloc(sizeof(int)*SIZE);
                printf("DEQUEUING..\n");
                puzzle = dequeue(1);
                int i;
                // worker processors
//                if(rank == 1){
                    print_puzzle(puzzle); 

                    // find first unknown cell
                    for(i=0;i<SIZE;i++){
                        if(puzzle[i] == 0)
                                break;
                    }
                    // puzzle is complete
                    if(i == SIZE){
                        printf("NO EMPTY CELLS\n");
                        if(!puzzle_has_contradiction(puzzle)){
                                print_puzzle(puzzle);
                                break;
                        }
                    }
 //               }

                // make a guess, and enqueue new puzzle
                int j;
                for(j=1;j<=(int)sqrt(SIZE);j++){
                        puzzle[i] = j;

                    //    if(puzzle_has_contradiction(puzzle)){
                      //          continue;
                        //}
                    enqueue(puzzle, 1);
                }
         //   }
        }
        }*/
        /*     ~~ terminate MPI ~~    */
        
        if(MPI_SUCCESS != MPI_Finalize()){
            printf("error terminating MPI.\n");
            exit(-1);
        }
}

void finalize(){
        if(MPI_SUCCESS != MPI_Finalize()){
            printf("error terminating MPI.\n");
            exit(-1);
        }
        MPI_Abort(MPI_COMM_WORLD, 0);
        exit(0);
}

void push(int *puzzle){
    QueueNode *node = malloc(sizeof(QueueNode));
    node->puzzle = puzzle;
    node->next = NULL;

    if(queue.tail){
            queue.tail->next = node;
    }else{
            queue.head = node;
    }
    queue.tail = node;
    queue.size++;
}

int *pop(){
    int *top = malloc(sizeof(int)*SIZE); 
    top = queue.head->puzzle;
    QueueNode *second = queue.head->next;
    free(queue.head);
    queue.head = second;
    if(!queue.head){
            queue.tail = NULL;
    }
    queue.size--;
    return top;
}

int *dequeue(int process){
        MPI_Request recv_request;
        int *puzzle = malloc(sizeof(int)*SIZE);
        MPI_Irecv(puzzle, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, &status);
        return puzzle;
}

void *dequeue_listen(){
        MPI_Request send_request;
        if(queue.size > 0){
            //printf("queue.size: %d\n", queue.size);
            MPI_Isend(pop(), SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &send_request);
        }
}

void *enqueue_listen(){
        MPI_Request recv_request;
        int *buffer = malloc(sizeof(int)*SIZE);
        MPI_Irecv(buffer, SIZE, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, &status);
        push(buffer);
}

void enqueue(int *puzzle, int process){
     MPI_Request send_request;
     MPI_Isend(puzzle, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &send_request);
    
}

/*void print_puzzle(int *puzzle){
            if(!puzzle) return;
    int i;
    for(i=0;i<SIZE;i++){
        printf("%d ", puzzle[i]);
        if(i%(int)sqrt(SIZE) == 0 && i != 0)
                printf("\n");
    }
    printf("\n------------------------------------\n\n");
}*/

int puzzle_has_contradiction(int *puzzle) {
    if(!puzzle) return 0;
  int q;
  for(q=0;q<SIZE;q++){

    int row = q/(int)sqrt(SIZE);
    int col = q%(int)sqrt(SIZE);
  int newest_element_value = puzzle[q];

  // used to find the 3x3 square that will be checked for validity
  //TODO: probably not the best way to check square validity, but it works!
  int square_row, square_col;
  find_square_row_and_col(row, col, &square_row, &square_col, puzzle);

  int i, j, k, traverse_count, row_val, col_val, grid_val;
  j = square_row;
  k = square_col;
  traverse_count = 0;
  for (i = 0; i < (int)sqrt(SIZE); i++) {
    // check all elements on row
    row_val = puzzle[row*(int)sqrt(SIZE) + i];
    if (row_val != -1 && row_val == newest_element_value && i != col) {
      return 1;
    }

    //check all elements on col
    col_val = puzzle[i*(int)sqrt(SIZE) + col];
    if (col_val != -1 && !col_val && col_val == newest_element_value && i != row) {
      return 1;
    }

    //check 3x3 square
    grid_val = puzzle[j*(int)sqrt(SIZE) + k];
    if (grid_val != -1 && !grid_val && grid_val == newest_element_value && j != row && k != col) {
      return 1;
    }

    traverse_count++;
    if (traverse_count > 2) {
      j = j + 1;
      k = square_col;
      traverse_count = 0;
    } else {
      k++;
    }
  }
    }
  return 0;
}

void *find_square_row_and_col(int row, int col, int *square_row, int *square_col, int *puzzle) {

  if (row < 3) {
    *square_row = 0;
    //top 3 blocks
  } else if (row >= 3 && row < 6) {
    *square_row = 3;
  } else {
    *square_row = 6;
  }

  if (col < 3) {
    *square_col = 0;
    //top 3 blocks
  } else if (col >= 3 && col < 6) {
    *square_col = 3;
  } else {
    *square_col = 6;
  }
}

void print_puzzle(int *puzzle){
 int i, counter = 0;
 int row_counter = 0;
 for (i = 0; i < SIZE; i++) {
   if(puzzle[i] == 0){
       printf("[] ");
   }else{
       printf(" %d ", puzzle[i]);
   }
   counter++;
   if(counter%3 == 0){
       printf("|");
   }
   if (counter == 9) {
     row_counter++;
     if(row_counter%3 == 0){
           printf("\n");
           printf("------------------------------");
     }
     printf("\n");
     counter = 0;
   }
 }
 printf("\n");
}
