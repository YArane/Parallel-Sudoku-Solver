solver : mpi_sudoku_solver.o
	mpicc -o solver mpi_sudoku_solver.o

mpi_sudoku_solver.o : mpi_sudoku_solver.c
	mpicc -c mpi_sudoku_solver.c

clean :
	rm -f mpi_sudoku_solver.o solver
