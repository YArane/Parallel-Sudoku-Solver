# Parallel-Sudoku-Solver
A parallel approach to solve NxN sudokus on multiple hosts

## SUPER IMPORTANT NOTES

- Grids are indexed from 0. So the top left element of a puzzle object is at row = 0 col = 0.
- The bottom right element is at row = 8 col = 8
- Undefined cells on the grid have value -1. Let's use this convention while coding!
- Possibility lists in the cell struct act as boolean tables. For example, see the list below 

 - 1 2 3 4 5 6 7 8 9 -> Sudoku puzzle values
[0,1,0,0,1,0,0,0,1,1]-> Boolean Table

- It contains 10 elements, but it corresponds to a 9x9 sudoku grid. The first element of the array is always zero, it doesn't mean anything. Every other corresponds to a possible value in the sudoku puzzle, from 1 to 9.
- The cell that contains the list shown above has possible value 1, 4, 8, and 9.