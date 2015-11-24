program : sudoku_solver.o
	mpicc -o program sudoku_solver.o ../puzzle_structs/puzzle_generator.o ../puzzle_structs/puzzle.o ../puzzle_structs/cell.o ../decision_tree/decision_tree.o

sudoku_solver.o : sudoku_solver.c
	mpicc -c sudoku_solver.c

clean :
	rm -f sudoku_solver.o program
