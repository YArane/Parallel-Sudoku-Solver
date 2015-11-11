puzzle_generator.o : puzzle_generator.c puzzle_generator.h puzzle.h
	gcc -c puzzle_generator.c

puzzle.o : puzzle.c puzzle.h cell.h
	gcc -c puzzle.c 

cell.o : cell.c cell.h
	gcc -c cell.c

clean :
	rm -f program cell.o
