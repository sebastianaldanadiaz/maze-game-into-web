#include "maze_create.h"
#include <stdlib.h>
#include <stdio.h>

bool is_valid(int n){
    return n >= 3 && n <= 50000;
}

void maze_clean(Maze* m){
    if(m == NULL){
        return; 
    }
    for(size_t row = 0; row < (size_t)m->rows; row++){
        free(m->cells[row]); 
    }
    free(m->cells); 
    free(m); 
}

int get_Unvisited_Cells(Maze* m, int row, int col, List neighbors[4]){
    int count = 0; 

    if(row-1 >= 0 && !m->cells[row-1][col].visited){
        neighbors[count].rows = row - 1; 
        neighbors[count].cells = col; 
        count = count + 1; 
    }
    if(row+1 < m->rows && !m->cells[row+1][col].visited){
        neighbors[count].rows = row + 1; 
        neighbors[count].cells = col;
        count = count + 1; 
    }
    if(col+1 < m->cols && !m->cells[row][col+1].visited){
        neighbors[count].rows = row; 
        neighbors[count].cells = col + 1; 
        count = count + 1; 
    }
    if(col-1 >= 0 && !m->cells[row][col-1].visited){
        neighbors[count].rows = row ; 
        neighbors[count].cells = col - 1; 
        count = count + 1; 
    }
    return count; 
}

void remove_wall_between(Maze* m, int r1, int c1, int r2, int c2){
    if(r2 == r1-1){
        m->cells[r1][c1].wall_north = false; 
        m->cells[r2][c2].wall_south = false; 
    }
    else if(r2 == r1+1){
        m->cells[r1][c1].wall_south = false; 
        m->cells[r2][c2].wall_north = false; 
    }
    else if(c2 == c1+1){
        m->cells[r1][c1].wall_east = false; 
        m->cells[r2][c2].wall_west = false; 
    }
    else if(c2 == c1-1){
        m->cells[r1][c1].wall_west = false; 
        m->cells[r2][c2].wall_east = false; 
    }
}

void maze_generate(Maze* m){

    int max = m->rows * m->cols;

    int* stack_row = malloc(max * sizeof(int));
    int* stack_col = malloc(max * sizeof(int));

    if (stack_row == NULL || stack_col == NULL) {

        fprintf(stderr, "error: Memory allocation failed\n");

        free(stack_row);
        free(stack_col);

        return;
    }

    int stack_top = 0;

    int start_row = m->entry_row;
    int start_col = m->entry_col;

    m->cells[start_row][start_col].visited = true;

    stack_row[stack_top] = start_row;
    stack_col[stack_top] = start_col;

    stack_top++;

    while (stack_top > 0) {

        int row = stack_row[stack_top - 1];
        int col = stack_col[stack_top - 1];

        List neighbors[4];

        int count =
        get_Unvisited_Cells(m, row, col, neighbors);

        if (count > 0) {

            int index = rand() % count;

            int next_row = neighbors[index].rows;
            int next_col = neighbors[index].cells;

            remove_wall_between(
                m,
                row,
                col,
                next_row,
                next_col
            );

            m->cells[next_row][next_col].visited = true;

            stack_row[stack_top] = next_row;
            stack_col[stack_top] = next_col;

            stack_top++;
        }
        else {

            stack_top--;
        }
    }

    free(stack_row);
    free(stack_col);
}

Maze* maze_create(int rows, int cols){
    if(!is_valid(rows) || !is_valid(cols)){
        fprintf(stderr, "error: Invalid dimensions\n"); 
        return NULL; 
    }
     Maze* m = malloc(sizeof(Maze)); 
    if(m == NULL){
        fprintf(stderr, "error: Memory allocation failed\n");
        return NULL; 
    }
    m->rows = rows; 
    m->cols = cols; 

    m->cells = malloc(rows * sizeof(Cell*));
    if(m->cells == NULL){
        free(m); 
        return NULL;    
    }
    for(size_t row = 0; row < (size_t)rows; row++){
        m->cells[row] = malloc(cols * sizeof(Cell)); 
        if(m->cells[row] == NULL){
            maze_clean(m); 
            return NULL; 
        }
        for(size_t col = 0; col < (size_t)cols; col++){
            m->cells[row][col].wall_north  = true; 
            m->cells[row][col].wall_south  = true; 
            m->cells[row][col].wall_east   = true; 
            m->cells[row][col].wall_west   = true; 
            m->cells[row][col].visited     = false; 
            m->cells[row][col].on_solution = false; 
        }
    }
    m->entry_row = 0; 
    m->entry_col = 0; 
    m->exit_row  = rows - 1; 
    m->exit_col  = cols - 1; 

    maze_generate(m); 

    return m; 
}

bool is_Wall(Maze *m, int r1, int c1, int dirR, int dirC){
    if(dirR == -1 && dirC == 0){  
        if(m->cells[r1][c1].wall_north == true){
            return true; 
        }
    }
    else if(dirR == 1 && dirC == 0){
        if(m->cells[r1][c1].wall_south == true){
            return true; 
        }
    }
    else if(dirR == 0 && dirC == 1){
        if(m->cells[r1][c1].wall_east == true){
            return true; 
        }
    }
    else if(dirR == 0 && dirC == -1){
        if(m->cells[r1][c1].wall_west == true){
            return true; 
        }
    }
    return false;
}

