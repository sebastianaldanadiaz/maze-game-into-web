#include "maze_create.h"

// Getters simples: reciben coordenadas, devuelven 0 o 1 (bool como int)
// para que el lado JS no necesite entender el layout del struct.

int web_get_rows(Maze* m){
    return m->rows;
}

int web_get_cols(Maze* m){
    return m->cols;
}

int web_get_entry_row(Maze* m){
    return m->entry_row;
}

int web_get_entry_col(Maze* m){
    return m->entry_col;
}

int web_get_exit_row(Maze* m){
    return m->exit_row;
}

int web_get_exit_col(Maze* m){
    return m->exit_col;
}

int web_wall_north(Maze* m, int row, int col){
    return m->cells[row][col].wall_north ? 1 : 0;
}

int web_wall_south(Maze* m, int row, int col){
    return m->cells[row][col].wall_south ? 1 : 0;
}

int web_wall_east(Maze* m, int row, int col){
    return m->cells[row][col].wall_east ? 1 : 0;
}

int web_wall_west(Maze* m, int row, int col){
    return m->cells[row][col].wall_west ? 1 : 0;
}