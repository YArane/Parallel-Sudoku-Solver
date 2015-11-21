#include <stdio.h>
#include <stdlib.h>

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
	int element, row_i, col_i, count, found_row, found_col, break_true;
	Cell *scan = malloc(sizeof(Cell));
	for(element=1; element<=9; element++)
	{
		count = 0;
		found_row = -1;
		found_col = -1;
		break_true = 0;
		find_square_row_and_col(row, col, &row_i, &row_i, p);
		for(row_i=row_i; row_i<row_i+3; row_i++)
		{
			for(col_i=col_i; col_i<col_i+3; col_i++)
			{
				scan = get_cell(row_i, col_i, p);
				if(scan->possibility_list[element])
				{
					if(count==0)
					{
						count = 1;
						found_row = row_i;
						found_col = col_i;
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
				set_cell_value(row_i, col_i, p, element);
		}
	}
}