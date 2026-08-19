#ifndef MAZE_CREATE_H
#define MAZE_CREATE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct{
    bool wall_north; 
    bool wall_south; 
    bool wall_east;   
    bool wall_west;
    bool visited; 
    bool on_solution; 
} Cell; 

typedef struct{
    int rows; 
    int cols;
    Cell** cells; 
    int entry_row; 
    int entry_col; 
    int exit_row; 
    int exit_col; 
} Maze; 

typedef struct{
    int rows; 
    int cells; 
} List; 



/**
 * Check if the number is valid for be part of the size of the matrix
 * 
 * @param n the number to be verified
 * 
 * @return true if the number is valid or false if not
 * 
 * @remark the number is valid only if the number is more than 3 or less than 50000
 */
bool is_valid(int n); 

/**
 * Clean the memory of the matrix
 * 
 * @param m The matrix of the maze to be cleaned
 */
void maze_clean(Maze* m); 

/**
 * Creates a blank maze (with all walls set)
 * 
 * @param row The number of rows of the new maze
 * @param col The number of cols of the new maze
 * 
 * @return a maze
 * 
 * @remark this function only works if the dimensions of the input are valid,
 * the function called "is_valid()" is necessary to verify that, the dimensions
 * must be between 2 and 100
 * 
 */
Maze* maze_create(int rows, int cols); 

/**
 * Creates a list of cells next to a certain cell that have not been visited yet
 * 
 * @param m The matrix of the maze to verify if its cells are accessible or not
 * @param row The row to be reviewed
 * @param col The col to be reviewed
 * @param neighbors A list of the neighbors cells to be reviewed
 * 
 * @return a count of accessible neighbor cells found
 */
int get_Unvisited_Cells(Maze* m, int row, int col, List neighbors[4]);

/**
 * Removes the wall between two adjacent cells to create a path
 * 
 * @param m The matrix of the maze 
 * @param r1 coordinate of the first cell
 * @param c1 coordinate of the first cell
 * @param r2 coordinate of the second cell
 * @param c2 coordinate of the second cell
 * 
 */
void remove_wall_between(Maze* m, int r1, int c1, int r2, int c2); 

/**
 * generates a maze using an iterative DFS algorithm it guarantees that there 
 * is a path from entry to exit
 * 
 * @param m The matrix of the maze to be generated
 */
void maze_generate(Maze* m); 

bool is_Wall(Maze *m, int r1, int c1, int dirR, int dirC); 


#endif