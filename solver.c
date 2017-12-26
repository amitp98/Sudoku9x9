#include "gui.h"

int inrow(int row, int num){
	int col;
	for (col = 0; col < Sudoku.N2; col++){
		if (grid[row][col] == num){
			return 1;
		}
	}
	return 0;
}

int incol(int col, int num){
	int row;
	for (row = 0; row < Sudoku.N2; row++){
		if (grid[row][col] == num){
			return 1;
		}
	}
	return 0;
}

int ingrid(int r, int c, int num){
	int row, col;
	for (row = 0; row < Sudoku.N; row++){
		for (col = 0; col < Sudoku.N; col++){
			if (grid[row + r][col + c] == num){
				return 1;
			} 
		}
	}
	return 0;
}

int valid(int row, int col, int num){
	return !inrow(row, num) && !incol(col, num) && !ingrid(row - (row % Sudoku.N), col - (col % Sudoku.N), num);
}

int zero(int *row, int *col){
	for (*row = 0; *row < Sudoku.N2; (*row)++){
		for (*col = 0; *col < Sudoku.N2; (*col)++){
			if (grid[*row][*col] == 0){
					
				return 1;
			}
		}
	}
	return 0;
}

int solve(){
	int row, col, num;
	if (!zero(&row, &col)){
		
		return 1;
	}
	
	for(num = 1; num <= Sudoku.N2; num++){
		if(valid(row, col, num)){
			grid[row][col] = num;		/* Temporary assignment */
			
			if(solve()){
				return 1;
			}
			
			grid[row][col] = 0;			/* Not correct assignment */
		}
	}
	
	return 0;
}

char *solver(char *data){
	int k = 0, i, j, s;
	grid = (int **)malloc(Sudoku.N2 * sizeof(int *));
	for(i = 0; i < Sudoku.N2; i++)
		grid[i] = (int *)malloc(Sudoku.N2 * sizeof(int));
	
	for(i = 0; i < Sudoku.N2; i++){
			for(j = 0; j < Sudoku.N2; j++){
					grid[i][j] = data[k++] - '0';
			}
	}
	
	s = solve(grid);
	k = 0;
	data = (char *)malloc(Sudoku.N4 + 1);
	if(s){
		for(i = 0; i < Sudoku.N2; i++){
			for(j = 0; j < Sudoku.N2; j++){
					data[k++] = grid[i][j] + '0';
			
			}
		}
		data[k] = '\0';
		free(grid);
		return data;
	}
	else{
		
		return NULL;		/* NO solution */
	}
	
	return NULL;
}
