puzzle_generator.o : puzzle_generator.c puzzle_generator.h puzzle.o
	mpicc -c puzzle_generator.c

puzzle.o : puzzle.c puzzle.h cell.o
	mpicc -c puzzle.c 

cell.o : cell.c cell.h
	mpicc -c cell.c

clean :
	rm -f program cell.o puzzle.o puzzle_generator.o
