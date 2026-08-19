#include "maze_create.h"
#include "maze_solve.h"
#include <stdio.h>


/**
 * Writes the full maze into a txt file, including dimensions,
 * entry, exit and all cell data
 * 
 * @param m The matrix of the maze that is going to be written on the file
 * @param f The txt file where the maze is going to be written
 * 
 */
void maze_write_txt(Maze* m, FILE* f);

/**
 * Reads a maze from a txt file, including dimensions, entry, exit and all cell 
 * data
 * 
 * @param f The txt file that is going go be read
 * 
 * @return The maze read on the  file
 * 
 * @remark The function only works if there is content on the file, and in the
 * expected format
 */
Maze* maze_read_txt(FILE* f); 

/**
 * Writes the full maze into a binary file, including dimensions,
 * entry, exit and all cell data
 * 
 * @param m The matrix of the maze that is going to be written on the file
 * @param f The binary file where the maze is going to be written

 */
void maze_write_binary(Maze* m, FILE* f);


/**
 * Reads a maze from a binary file, including dimensions, entry, exit and all cell 
 * data
 * 
 * @param f The binary file that is going go be read
 * 
 * @return The maze read on the file
 * 
 * @remark The function only works if there is content on the file, and in the 
 * expected format
 */
Maze* maze_read_binary(FILE* f); 
  