#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>
#include "cell.h"


/* 
 * Function: init_cell
 * -------------------
 * initiates a cell
 *
 * value: the value of the cell
 * n: size of the puzzle
 *
 * returns: the pointer to the cell
 *
 */
Cell *init_cell(int value, int n){
  Cell *cell;
  // check malloc of struct
  if(!(cell = malloc(sizeof(Cell)))){
    printf("error allocating cell.\n");
    return NULL;
  }
  // check malloc of possibility pointer
  int *list;
  if(!(cell->possibility_list = malloc((n + 1) * sizeof(int)))){
    printf("error allocating possibility list.\n");
  }

  cell->value = value;

return cell;

}

void *create_mpi_cell(){
        // MPI struct
         int num_items = 4;
         int block_length[4] = {1, 1, 1, 1};
         MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
         MPI_Datatype mpi_cell;                                      
         MPI_Aint offsets[4];                                     
                                                                 
         offsets[0] = (MPI_Aint)offsetof(Cell, value);          
         offsets[1] = (MPI_Aint)offsetof(Cell, possibility_list);
         offsets[2] = (MPI_Aint)offsetof(Cell, row);            
         offsets[3] = (MPI_Aint)offsetof(Cell, col);
                                                              
         MPI_Type_create_struct(num_items, block_length, offsets, types, &mpi_cell); 
         MPI_Type_commit(&mpi_cell);                                                         
         return mpi_cell;       
}

void fill_possibilities_with_zeros(Cell *cell){
    int i;
    int *list = cell->possibility_list;
    for(i=0;i<9;i++){
            list[i] = 0;
    }
}
