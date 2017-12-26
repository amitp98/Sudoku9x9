#include <gtk/gtk.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
/*	Sudoku is stored in a array of (N^2)*(N^2)	
 * values of N can be 2 / 3
 * i.e the sizes of sudoku can be 4*4 / 9*9
 * */


/* gpointer
typedef void* gpointer;
An untyped pointer. gpointer looks better and is easier to use than void*.  */
typedef struct Game{
	int N, N2, N4, lev;
	GtkWidget *window;
}Game;

int **grid;
Game Sudoku;


/*
 * Generates a new window, using this window:
 * User can play a new game OR
 * resume a previous saved game
 * */
int new();

/* difficulty function is called for getting difficulty level from user.
 * it uses a GTK_dialog to take input from the user.
 * it hides the main GTK window .
 * and opens the window containnig sudoku
 * */
void difficulty(GtkWidget *widget, int *data);

/* Callback function to Destroy the window */
void destroy(GtkWidget *widget, gpointer data);


/* Takes puzzle sudoku from a file and stores it in a char array and returns that array */
char *generate(int N, int lev);

/* Generates a new window containig the Sudoku and displays it in the grid
 * grid is of 'GtkEntry'
 * */
void game(int a, char *puzzle);

/* Loads a user saved sudoku 
 * user has to enter the name of the file 
 * the file will be searched in 'savegame' folder 
 * as game was stored in 'savegame' folder
 * calls the resumeUGame() function to fetch the file   
 * */
void resume( GtkWidget *widget, gpointer data );
void resumeUGame(GtkWidget *widget, GtkWidget *entry);
	
	

/* Used to take user Sudoku,
 * User has to select his Sudoku file to load the game
 * */
void getfile(GtkWidget *fileb, gpointer data1);

/* Callback functoins for various buttons in the window */

/* Saves the partially solved Sudoku in a filr */
void on_save_clicked(GtkWidget *widget, gpointer data);

/* Verifies the input enterd by the user in the GUI of Sudoku 
 * Displays Warning message if user enters value other than 1 to 9
 * Displays Information message , when the user correctly solves a Sudoku.
 * */
void on_verify_clicked(GtkWidget *widget, char *data);

/* Solves the Sudoku and displays it. */
void on_solve_clicked(GtkWidget *widget, char *data);

/* Calls the new() function and generates a new game */
void on_new_clicked(GtkWidget *widget, gpointer data);

/* Clears all the entries filled by user in the grid 
 * original puzzle remains after clearing
 * */
void on_clear_clicked(GtkWidget *widget, char *data);

/* Used for giving hint, fills the first empty entry in the grid */
void on_hint_clicked(GtkWidget *widget, char *data);

/* Callback function to Destroy the Sudoku Game 
 * quits the game and program ends
 * */
void destroyGame(GtkWidget *widget, gpointer data);


/* checks if number a is present in a row 
 * returns 1 if found in row
 * else returns 0
 * */

int inrow(int row, int num);

/* checks if number a is present in a column 
 * returns 1 if found in column
 * else returns 0
 * */
int incol(int col, int num);

/* checks if number a is present in mini square / box 
 * returns 1 if found 
 * else returns 0
 * */
int ingrid(int r, int c, int num);

/* checks if number a is present in row, column, mini square / box 
 * returns 1 if found 
 * else returns 0
 * */
int valid(int row, int col, int num);

/* Finds a element in the grid having value 0 */
int zero(int *row, int *col);


/* sudoku solving algorithm is applied  */
int solve();

/* calls the solve() function */
char *solver(char *data);
