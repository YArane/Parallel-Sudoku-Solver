#include <stdio.h>
#include <stdlib.h>
#include "../puzzle_structs/cell.h"
#include "../puzzle_structs/puzzle.h"

void lone_ranger(Puzzle *p);
void lone_ranger_row_and_col(int row, int col, Puzzle *p);
void lone_ranger_subgrid(int row, int col, Puzzle *p);

void naked_subset(Puzzle *p);
void naked_subset_row_and_col(int rowcol, Puzzle *p);
void naked_subset_subgrid(int rowcol, Puzzle *p);

int possibility_lists_equal(int *p1, int *p2);
int rank_possibility_list(int *p1);
void eliminate_naked_subset_col(Puzzle *p, int col, int *subset, int *locations);
void eliminate_naked_subset_row(Puzzle *p, int row, int *subset, int *locations);


/*
 * Function: naked_subset
 * -----------------------
 * Search for a possibility list that occurs alone as many times as it has elements
 * within a region of the puzzle
 * If found, the elements in that subset can be eliminated everywhere else in the region
 *
 * p: pointer to the puzzle
 */
void naked_subset(Puzzle *p) 
{
	int i;
	for(i=0; i<9; i++)
	{
		naked_subset_row_and_col(i, p);
		if(i==1 || i==4 || i==7)
			naked_subset_subgrid(i, p);
	}
}

/*
 * Function: naked_subset_row_and_col
 * ----------------------------------
 * Perform aforementioned naked subset searching on the row and column given
 *
 * rowcol: int from 0 to 8 - which diagonal cell to start at. I don't let you jank it up this time.
 * p: pointer to the puzzlerino
 */
void naked_subset_row_and_col(int rowcol, Puzzle *p)
{
	Cell *scan = malloc(sizeof(Cell));
	int *subset = malloc(9*sizeof(int));

	Cell *cmp = malloc(sizeof(Cell));
	int *subset2 = malloc(9*sizeof(int));

	int *locations = malloc(9*sizeof(int));

	//column junk first
	int row, row2, col, col2, i;
	int match_count;
	col = rowcol;
	for(row=0; row<9; row++)
	{
		match_count = 0;
		scan = get_cell(row, col);
		subset = scan->possibility_list;
		for(i=0; i<9; i++)
			locations[i] = 0;
		for(row2=0; row2<9; row2++) //compare this subset against all other in the column
		{
			cmp = get_cell(row2, col);
			subset2 = cmp->possibility_list;
			if(possibility_lists_equal(subset, subset2))
			{
				match_count++; //record the number of times this subset appears identically
				locations[row2] = 1;
			}
		}
		if(match_count == rank_possibility_list(subset)) //we have discovered a naked subset
		// !!!
		{
			eliminate_naked_subset_col(Puzzle *p, int col, int *subset, int *locations);
		}
	}

	//row junk next
	row = rowcol;
	for(col=0; col<9; col++)
	{
		match_count = 0;
		scan = get_cell(row, col);
		subset = scan->possibility_list;
		for(i=0; i<9; i++)
			locations[i] = 0;
		for(col2=0; col2<9; col2++) //compare this subset against all other in the row
		{
			cmp = get_cell(row, col2);
			subset2 = cmp->possibility_list;
			if(possibility_lists_equal(subset, subset2))
			{
				match_count++; //record the number of times this subset appears identically
				locations[col2] = 1;
			}
		}
		if(match_count == rank_possibility_list(subset)) //we have discovered a naked subset
		// !!!
		{
			eliminate_naked_subset_row(Puzzle *p, int row, int *subset, int *locations);
		}
	}
}

void naked_subset_subgrid(int rowcol, Puzzle *p)
{
	Cell *scan = malloc(sizeof(Cell));
	int *subset = malloc(9*sizeof(int));

	Cell *cmp = malloc(sizeof(Cell));
	int *subset2 = malloc(9*sizeof(int));

	int *locations = malloc(9*sizeof(int));

	int square_row, square_col;
	find_square_row_and_col(rowcol, rowcol, &square_row, &square_col, p);

	int row, col, row2, col2, i, match_count;
	for(row=square_row; row<square_row+3; row++)
	{
		for(col=square_col; col<square_col+3; col++)
		{
			match_count = 0;
			scan = get_cell(row, col);
			subset = scan->possibility_list;
			for(i=0; i<9; i++)
				locations[i] = 0;
			for(row2=square_row; row2<square_row+3; row2++)
			{
				for(col2=square_col; col2<square_col+3; col2++)
				{
					cmp = get_cell(row2, col2);
					subset2 = cmp->possibility_list;
					if(possibility_lists_equal(subset, subset2))
					{
						match_count++
						locations[3*row2 + col2];
					}
				}
			}
			if(match_count == rank_possibility_list(subset))
			{
				eliminate_naked_subset_subgrid(p, rowcol, subset, locations);
			}
		}	
	}
}


/*
 * Function: eliminate_naked_subset_subgrid
 * ----------------------------------------
 * A naked subset has been found in the subgrid; eliminate its elements from all cells where
 * it does not appear naked
 *
 * p: puzzle
 * rowcol: specifies the subgrid in an awkward way
 * subset: the naked subset
 * locations: where the naked subset was originally found; don't remove from these spots
 */
void eliminate_naked_subset_subgrid(Puzzle *p, int rowcol, int *subset, int *locations)
{
	int row, col, square_row, square_col, element;
	find_square_row_and_col(rowcol, rowcol, &square_row, &square_col, p);
	Cell *elim = malloc(sizeof(Cell));
	for(row=square_row; row<square_row+3; row++)
	{
		for(col=square_col; col<square_col+3; col++)
		{
			if(!locations[3*row + col])
			{
				elim = get_cell(row, col);
				for(element=0; element<9; element++)
				{
					if(subset[element])
						elim->possibility_list[element] = 0;
				}
			}
		}
	}
}

/*
 * Function: eliminate_naked_subset_col
 * ----------------------------------------
 * A naked subset has been found in the column; eliminate its elements from all cells where
 * it does not appear naked
 *
 * p: puzzle
 * col: specifies the column
 * subset: the naked subset
 * locations: where the naked subset was originally found; don't remove from these spots
 */
void eliminate_naked_subset_col(Puzzle *p, int col, int *subset, int *locations)
{
	int row, element;
	Cell *elim = malloc(sizeof(Cell));
	for(row=0; row<9; row++)
	{
		if(locations[row])
			continue;
		elim = get_cell(row, col);
		for(element=0; element<9; element++)
		{
			if(subset[element])
				elim->possibility_list[element] = 0;
		}
	}
} 

/*
 * Function: eliminate_naked_subset_row
 * ----------------------------------------
 * A naked subset has been found in the row; eliminate its elements from all cells where
 * it does not appear naked
 *
 * p: puzzle
 * row: specifies the row
 * subset: the naked subset
 * locations: where the naked subset was originally found; don't remove from these spots
 */
void eliminate_naked_subset_row(Puzzle *p, int row, int *subset, int *locations)
{
	int col, element;
	Cell *elim = malloc(sizeof(Cell));
	for(col=0; col<9; col++)
	{
		if(locations[col])
			continue;
		elim = get_cell(row, col);
		for(element=0; element<9; element++)
		{
			if(subset[element])
				elim->possibility_list[element] = 0;
		}
	}
}

/*
 * Function: possibility_lists_equal
 * ----------------------------------------
 * Return 1 if two possibility lists are equivalent, 0 else
 *
 * p1: first possibility list
 * p2: second possibility list
 */
int possibility_lists_equal(int *p1, int *p2)
{
	int i;
	int equal = 1;
	for(i=0; i<9; i++)
	{
		if(p1[i] != p2[i])
		{
			equal = 0;
			break;
		}
	}
	return equal;
}

/*
 * Function: rank_possibility_list
 * ----------------------------------------
 * Return the number of possibilities in a given possibility list
 * This is relevant for naked subset searching
 *
 * p1: the possibility list in question
 */
int rank_possibility_list(int *p1)
{
	int count = 0;
	for(i=0; i<9; i++)
	{
		if(p1[i])
			count++;
	}
	return count;
}

/*
 * Function: lone_ranger
 * ----------------------
 * Scan the whole puzzle for lone ranger solves and update accordingly
 * This is the function that should be called in practice
 *
 * p: pointer to the puzzle you want to analyze
 *
 */
void lone_ranger(Puzzle *p)
{
	int i;
	for(i=0; i<9; i++)
	{
		lone_ranger_row_and_col(i, i, p);
		if(i==1 || i==4 || i==7)
			lone_ranger_subgrid(i, i, p);
	}
}

/* 
 * Function: lone_ranger_row_and_col
 * -------------------
 * Scan the given location's row and column for lone ranger solves and update the puzzle accordingly
 * This is best called on only diagonal cells for *ahem* 'maximum efficiency' *pushes up glasses*
 *
 * row: the row index of the cell we're looking at
 * col: the column index of the cell we're looking at
 * p: pointer to the puzzle which we intend to propagate
 *
 */
void lone_ranger_row_and_col(int row, int col, Puzzle *p)
{
	//element - the number we're looking for
	//row/col_i - search indices
	//count - we're looking for exactly one of a particular element in one row or column
	//found - keep track of where we found that one
	int element, row_i, col_i, count, found;
	Cell *scan = malloc(sizeof(Cell));
	for(element=1; element<=9; element++)
	{
		count = 0;
		found = -1;
		//scan the row
		for(col_i=0; col_i<=8; col_i++)
		{
			scan = get_cell(row, col_i, p);
			if(scan->possibility_list[element])
			{
				if(count == 0)
				{
					count = 1;
					found = col_i;
				}
				else
				{
					count++;
					break;
				}
			}
		}
		if(count==1)
			set_cell_value(row, found, p, element);

		count = 0;
		found = -1;
		//scan the column
		for(row_i=0; row_i<=8; row_i++)
		{
			scan = get_cell(row_i, col, p);
			if(scan->possibility_list[element])
			{
				if(count == 0)
				{
					count = 1;
					found = row_i;
				}
				else 
				{
					count++;
					break;
				}
			}
		}
		if(count==1)
			set_cell_value(found, col, p, element);
	}
}


/* Function: lone_ranger_subgrid
 * --------------------------------
 * Scan the given location's own subgrid for lone ranger solves and update accordingly
 * 
 * row: row index of cell in question
 * col: column index of cell in question
 * p: pointer to puzzle to be solved
 *
 */
void lone_ranger_subgrid(int row, int col, Puzzle *p)
{
	int element, row_i, col_i, count, found_row, found_col, break_true, i, j;
	Cell *scan = malloc(sizeof(Cell));
	for(element=1; element<=9; element++)
	{
		count = 0;
		found_row = -1;
		found_col = -1;
		break_true = 0;
		find_square_row_and_col(row, col, &row_i, &row_i, p);
		for(i=row_i; i<row_i+3; i++)
		{
			for(j=col_i; j<col_i+3; j++)
			{
				scan = get_cell(i, j, p);
				if(scan->possibility_list[element])
				{
					if(count==0)
					{
						count = 1;
						found_row = i;
						found_col = j;
					}
					else
					{
						count++;
						break_true = 1;
						break;
					}
				}
			}
			if(break_true)
				break;

			if(count==1)
				set_cell_value(i, j, p, element);
		}
	}
}