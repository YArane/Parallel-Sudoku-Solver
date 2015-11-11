all :
	cd puzzle_structs; make
	cd puzzle_solver; make
	cd decision_tree; make

clean:
	cd puzzle_structs; make clean
	cd puzzle_solver; make clean
	cd decision_tree; make clean
