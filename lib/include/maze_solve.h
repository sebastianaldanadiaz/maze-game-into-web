#include "maze_create.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Resets all the visited cells to avoid confusion with the visited cells marked
 * during maze generation
 * 
 * @param m The maze whose cells will be reset
 */
void reset_visited(Maze* m); 

/**
 * Finds the accessible neighbor cells of a given cell
 * a cell is accessible if there is no wall between them and it has
 * not been visited yet
 * 
 * @param m The maze to search for accessible neighbors
 * @param row The row to be reviewed
 * @param col The col to be reviewed
 * @param neighbors A list of the neighbors cells to be reviewed
 * 
 * @return a count of accessible neighbor cells found
 */
int get_accessible_cells(Maze* m, int row, int col, List neighbors[4]); 


/**
 * Solves the maze using a recursive DFS algorithm
 * marks each cell that is part of the solution path with on_solution = true
 * 
 * @param m The matrix of the maze to be solved
 * @param row The row to be reviewed if there is solution or not
 * @param col The col to be reviewed if there is solution or not
 * 
 * @return if the exit was found from this cell, false if not
 */
bool maze_solve_dfs(Maze* m, int row, int col); 

/**
 * 
 * Main resolution function, resets visited cells and calls the recursive DFS 
 * solver
 * 
 * @param m The matrix of the maze to be solved
 * 
 * @return true if the maze has a solution, false if not
 */
bool maze_solve(Maze* m); 
   