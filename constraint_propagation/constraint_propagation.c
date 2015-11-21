#include <stdio.h>
#include <stdlib.h>

/* 
 * Function: lone_ranger
 * -------------------
 * Scan the given location's row and column for lone ranger solves and update the puzzle accordingly
 * This is best called on only diagonal cells for *ahem* 'maximum efficiency' *pushes up glasses*
 *
 * row: the row index of the cell we're looking at
 * col: the column index of the cell we're looking at
 * p: pointer to the puzzle which we intend to propagate
 *
 * NOTE: THE PUZZLE IS INDEXED FROM 0. Passing (0,0) returns the top-left corner.
 *
 */
void lone_ranger(int row, int col, Puzzle *p)
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
			scan = get_cell(row, col_i, p);
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