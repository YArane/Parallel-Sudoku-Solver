#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

static int size;

int main(int argc, char **argv){
    
        /*     ~~ init MPI ~~    */
        
        int rank, np;
        if(MPI_SUCCESS != MPI_Init(&argc, &argv)){
            printf("error initializing MPI.\n");
            return -1;
        }
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);
     
                
        /*     ~~ Master Process ~~    */

        if(rank == 0){
            /*     ~~ Read Matrix ~~    */


            FILE *file;
            if(!(file = fopen(argv[1], "r")) || argc != 3){
                printf("usage: %s [matrix.txt], [size]\n", argv[0]);
                return -1;
            }
            
            size = atoi(argv[2]);
            int cell[size*size];
            char c;

            int i;
            while((c=fgetc(file)) != EOF){
               if(c == '\n') continue;
               cell[i++] = atoi(&c);
            }

            fclose(file);
                
        }


        /*     ~~ terminate MPI ~~    */
        
        if(MPI_SUCCESS != MPI_Finalize()){
            printf("error terminating MPI.\n");
            exit(-1);
        }
}
