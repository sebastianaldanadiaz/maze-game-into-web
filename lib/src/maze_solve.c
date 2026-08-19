#include "maze_create.h"
#include <stdlib.h>
#include <stdio.h>


void reset_visited(Maze* m){

    for(size_t row = 0; row < (size_t)m->rows; row++){
        for(size_t col = 0; col < (size_t)m->cols; col++){
            m->cells[row][col].visited = false;
            m->cells[row][col].on_solution = false;
        }
    }
}

int get_accessible_cells(Maze* m, int row, int col,List neighbors[4]){
    int count = 0;
    if (!m->cells[row][col].wall_north && row - 1 >= 0) {
        neighbors[count].rows = row - 1;
        neighbors[count].cells = col;
        count++;
    }

    if (!m->cells[row][col].wall_south && row + 1 < m->rows) {
        neighbors[count].rows = row + 1;
        neighbors[count].cells = col;
        count++;
    }

    if (!m->cells[row][col].wall_east && col + 1 < m->cols) {
        neighbors[count].rows = row;
        neighbors[count].cells = col + 1;
        count++;
    }

    if (!m->cells[row][col].wall_west && col - 1 >= 0) {
        neighbors[count].rows = row;
        neighbors[count].cells = col - 1;
        count++;
    }

    return count;
}
bool maze_solve_dfs(Maze* m, int row, int col){
    m->cells[row][col].visited = true; 

    if(row == m->exit_row && col == m->exit_col){
        m->cells[row][col].on_solution = true; 
        return true; 
    }
    int count = 0; 
    List neighbors[4]; 
    count = get_accessible_cells(m, row, col, neighbors); 

    for(size_t i = 0; i < (size_t)count; i++){
        if(maze_solve_dfs(m, neighbors[i].rows, neighbors[i].cells)){
            m->cells[row][col].on_solution = true; 
            return true; 
        }
    }
    return false; 
}

bool maze_solve(Maze* m){

    if (m == NULL) {
        fprintf(stderr, "error: the Maze is null\n");
        return false;
    }

    reset_visited(m);

    int max = m->rows * m->cols;

    int* queue_row = malloc(max * sizeof(int));
    int* queue_col = malloc(max * sizeof(int));

    int* parent_row = malloc(max * sizeof(int));
    int* parent_col = malloc(max * sizeof(int));

    if (queue_row == NULL || queue_col == NULL || parent_row == NULL ||
        parent_col == NULL) {

        fprintf(stderr, "error: Memory allocation failed\n");

        free(queue_row);
        free(queue_col);
        free(parent_row);
        free(parent_col);

        return false;
    }

    for (int i = 0; i < max; i++) {
        parent_row[i] = -1;
        parent_col[i] = -1;
    }

    int front = 0;
    int back = 0;

    queue_row[back] = m->entry_row;
    queue_col[back] = m->entry_col;

    back++;

    m->cells[m->entry_row][m->entry_col].visited = true;

    bool found = false;

    while (front < back) {
        int row = queue_row[front];
        int col = queue_col[front];

        front++;

        if (row == m->exit_row && col == m->exit_col) {
            found = true;
            break;
        }

        List neighbors[4];

        int count =
        get_accessible_cells(m, row, col, neighbors);

        for (int i = 0; i < count; i++) {

            int nr = neighbors[i].rows;
            int nc = neighbors[i].cells;

            if (!m->cells[nr][nc].visited) {
                m->cells[nr][nc].visited = true;
                queue_row[back] = nr;
                queue_col[back] = nc;

                back++;

                int idx = nr * m->cols + nc;

                parent_row[idx] = row;
                parent_col[idx] = col;
            }
        }
    }

    if (!found) {

        fprintf(
            stderr,
            "error: Solving failed: No path from entrance to exit\n"
        );

        free(queue_row);
        free(queue_col);
        free(parent_row);
        free(parent_col);

        return false;
    }

    int row = m->exit_row;
    int col = m->exit_col;

    while (
        !(row == m->entry_row &&
          col == m->entry_col)
    ) {

        m->cells[row][col].on_solution = true;

        int idx = row * m->cols + col;

        int pr = parent_row[idx];
        int pc = parent_col[idx];

        row = pr;
        col = pc;
    }

    m->cells[m->entry_row][m->entry_col].on_solution = true;

    free(queue_row);
    free(queue_col);
    free(parent_row);
    free(parent_col);

    return true;
}