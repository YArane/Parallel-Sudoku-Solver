#include <stdio.h>
#include <stdlib.h>
#include "../puzzle_structs/puzzle_generator.h"
//#include "../tree_structs/tree_node.h"
#include "../decision_tree/decision_tree.h"
#include <mpi.h>

/*
==============================================
===== SEE THE README FOR IMPORTANT INFO ======
==============================================
*/

void insert_some_empty_cells(Puzzle *puzzle);
void *read_matrix_data(char *filename, Puzzle *puzzle);


/* MAIN METHOD */

int main(int argc, char **argv) {
  
  int rank, np;      
  Puzzle *puzzle;
  
  //initialize MPI
  if(MPI_SUCCESS != MPI_Init(&argc, &argv)){
  	printf("error initializing MPI.\n");
  	exit(-1);	
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

    if(rank == 0){

  puzzle = init_puzzle(9);
  create_puzzle(puzzle);
            

           MPI_Datatype node = create_mpi_node(1); 
    /*        node->instance = puzzle;
            node->number_of_children = 1;
            node->level = 0;
            node->children = puzzle;*/
            
           MPI_Send(&node, 1, create_mpi_node(1), 1, 0, MPI_COMM_WORLD);

  print_puzzle(puzzle);

  if(argc == 1){
      insert_some_empty_cells(puzzle);
  }

  char *filename;
  if (argc > 1){
       filename = argv[1];   
       read_matrix_data(filename, puzzle);
  }  

  fill_possibility_lists(puzzle);
    }

    if(rank == 1){
        printf("RANK 1:\n");
        MPI_Datatype node = create_mpi_node(1);
        MPI_Recv(&node, 1, create_mpi_node(1), 0, 0, MPI_COMM_WORLD,  NULL);
    }

  Node *node = build_tree(puzzle, &rank, &np); 

  //terminate MPI
  if(MPI_SUCCESS != MPI_Finalize()){
  	printf("error terminating MPI.\n");
  	exit(-1);
  }

  return 0;
}

// For testing
void insert_some_empty_cells(Puzzle *puzzle) {
  printf("UH-OH\n");
  set_cell_value(0,0,puzzle,-1);
  set_cell_value(0,1,puzzle,-1);
  set_cell_value(1,0,puzzle,-1);
  set_cell_value(7,1,puzzle,-1);
  set_cell_value(4,2,puzzle,-1);
  set_cell_value(0,8,puzzle,-1);
  set_cell_value(3,4,puzzle,-1);
  set_cell_value(1,1,puzzle,-1);
  set_cell_value(2,3,puzzle,-1);
  // set_cell_value(4,4,puzzle,-1);
  // set_cell_value(1,3,puzzle,-1);
}

// User Input for matrix
void *read_matrix_data(char *filename, Puzzle *puzzle){
    FILE *file;

    printf("attempting to read file..\n");

    if(!(file=fopen(filename, "r"))){
        printf("error reading file.\n");
    }

    int count = 0;
    char c;
    int i = 0;
    int j = 0;
    while((c=fgetc(file)) != EOF){
        if(c == '\n') continue;
        j = count%9;
        i = count/9;
        if(c == '0'){
                set_cell_value(i, j, puzzle, -1); 
        }
        count++;
    }

}

