puzzle_generator.o : puzzle_generator.c puzzle_generator.h puzzle.o
	gcc -c puzzle_generator.c

puzzle.o : puzzle.c puzzle.h cell.o
	gcc -c puzzle.c 

cell.o : cell.c cell.h
	gcc -c cell.c

clean :
	rm -f program cell.o puzzle.o puzzle_generator.o
