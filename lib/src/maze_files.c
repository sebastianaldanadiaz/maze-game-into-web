#include "maze_files.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

Maze* maze_read_txt(FILE* f){

    if (f == NULL) {
        fprintf(stderr, "error: File is null\n");
        return NULL;
    }

    int capacity = 128;
    char** lines = malloc(capacity * sizeof(char*));

    if (lines == NULL) {
        return NULL;
    }

    int total_lines = 0;
    char buffer[100000];

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (total_lines >= capacity) {
            capacity *= 2;

            char** temp =
            realloc(lines, capacity * sizeof(char*));

            if (temp == NULL) {
                for (int i = 0; i < total_lines; i++) {
                    free(lines[i]);
                }
                free(lines);

                return NULL;
            }
            lines = temp;
        }

        int len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        lines[total_lines] =
        malloc((strlen(buffer) + 1) * sizeof(char));

        if (lines[total_lines] == NULL) {
            for (int i = 0; i < total_lines; i++) {
                free(lines[i]);
            }
            free(lines);
            return NULL;
        }

        strcpy(lines[total_lines], buffer);

        total_lines++;
    }

    int rows = (total_lines - 1) / 2;
    int cols = (strlen(lines[0]) - 1) / 2;

    if (!is_valid(rows) || !is_valid(cols)) {
        fprintf(stderr, "error: Invalid dimensions\n");
        for (int i = 0; i < total_lines; i++) {
            free(lines[i]);
        }

        free(lines);
        return NULL;
    }

    Maze* m = malloc(sizeof(Maze));

    if (m == NULL) {
        for (int i = 0; i < total_lines; i++) {
            free(lines[i]);
        }
        free(lines);

        return NULL;
    }

    m->rows = rows;
    m->cols = cols;

    m->cells = malloc(rows * sizeof(Cell*));

    if (m->cells == NULL) {

        free(m);

        for (int i = 0; i < total_lines; i++) {
            free(lines[i]);
        }

        free(lines);
        return NULL;
    }

    for (int row = 0; row < rows; row++) {
        m->cells[row] =
        malloc(cols * sizeof(Cell));
        if (m->cells[row] == NULL) {
            maze_clean(m);
            for (int i = 0; i < total_lines; i++) {
                free(lines[i]);
            }
            free(lines);
            return NULL;
        }

        for (int col = 0; col < cols; col++) {
            m->cells[row][col].wall_north = true;
            m->cells[row][col].wall_south = true;
            m->cells[row][col].wall_east  = true;
            m->cells[row][col].wall_west  = true;
            m->cells[row][col].visited = false;
            m->cells[row][col].on_solution = false;
        }
    }
    int visual_rows = total_lines;
    int visual_cols = strlen(lines[0]);
    for (int r = 0; r < visual_rows; r++) {
        for (int c = 0; c < visual_cols; c++) {
            char ch = lines[r][c];
            if (ch != 'S' && ch != 'F') {
                continue;
            }
            bool is_entry = (ch == 'S');
            if (r == 0) {
                int cell_col = (c - 1) / 2;
                if (is_entry) {
                    m->entry_row = 0;
                    m->entry_col = cell_col;
                }
                else {
                    m->exit_row = 0;
                    m->exit_col = cell_col;
                }
                m->cells[0][cell_col].wall_north = false;
            }
            else if (r == visual_rows - 1) {
                int cell_col = (c - 1) / 2;
                if (is_entry) {
                    m->entry_row = rows - 1;
                    m->entry_col = cell_col;
                }
                else {
                    m->exit_row = rows - 1;
                    m->exit_col = cell_col;
                }
                m->cells[rows - 1][cell_col].wall_south = false;
            }
            else if (c == 0) {
                int cell_row = (r - 1) / 2;
                if (is_entry) {
                    m->entry_row = cell_row;
                    m->entry_col = 0;
                }
                else {
                    m->exit_row = cell_row;
                    m->exit_col = 0;
                }
                m->cells[cell_row][0].wall_west = false;
            }
            else if (c == visual_cols - 1) {
                int cell_row = (r - 1) / 2;
                if (is_entry) {
                    m->entry_row = cell_row;
                    m->entry_col = cols - 1;
                }
                else {
                    m->exit_row = cell_row;
                    m->exit_col = cols - 1;
                }
                m->cells[cell_row][cols - 1].wall_east = false;
            }
        }
    }

    for (int row = 0; row < rows; row++) {
        int vrow = row * 2 + 1;
        for (int col = 0; col < cols; col++) {
            int vcol = col * 2 + 1;
            char ch = lines[vrow][vcol];
            if (ch == '.') {
                m->cells[row][col].on_solution = true;
            }

            m->cells[row][col].wall_north =
            (lines[vrow - 1][vcol] == '#');

            m->cells[row][col].wall_south =
            (lines[vrow + 1][vcol] == '#');

            m->cells[row][col].wall_east =
            (lines[vrow][vcol + 1] == '#');

            m->cells[row][col].wall_west =
            (lines[vrow][vcol - 1] == '#');
        }
    }

    for (int i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    return m;
}
void maze_write_txt(Maze* m, FILE* f){
    if (f == NULL) {
        fprintf(stderr, "error: File is null\n");
        return;
    }

    int visual_rows = m->rows * 2 + 1;
    int visual_cols = m->cols * 2 + 1;

    char** grid = malloc(visual_rows * sizeof(char*));

    if (grid == NULL) {
        return;
    }

    for (int r = 0; r < visual_rows; r++) {
        grid[r] = malloc((visual_cols + 1) * sizeof(char));
        if (grid[r] == NULL) {
            for (int i = 0; i < r; i++) {
                free(grid[i]);
            }
            free(grid);

            return;
        }

        for (int c = 0; c < visual_cols; c++) {
            grid[r][c] = '#';
        }

        grid[r][visual_cols] = '\0';
    }

    // Open the path

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            int vr = row * 2 + 1;
            int vc = col * 2 + 1;

            grid[vr][vc] = ' ';

            if (!m->cells[row][col].wall_north) {
                grid[vr - 1][vc] = ' ';
            }

            if (!m->cells[row][col].wall_south) {
                grid[vr + 1][vc] = ' ';
            }

            if (!m->cells[row][col].wall_east) {
                grid[vr][vc + 1] = ' ';
            }

            if (!m->cells[row][col].wall_west) {
                grid[vr][vc - 1] = ' ';
            }
        }
    }

    // Open Solution

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            int vr = row * 2 + 1;
            int vc = col * 2 + 1;
            if (m->cells[row][col].on_solution) {
                grid[vr][vc] = '.';
            }
        }
    }

    // Entry
    if (m->entry_row == 0) {
        int vc = m->entry_col * 2 + 1;
        grid[0][vc] = 'S';
    }
    else if (m->entry_row == m->rows - 1) {
        int vc = m->entry_col * 2 + 1;
        grid[visual_rows - 1][vc] = 'S';
    }

    else if (m->entry_col == 0) {
        int vr = m->entry_row * 2 + 1;
        grid[vr][0] = 'S';
    }

    else if (m->entry_col == m->cols - 1) {
        int vr = m->entry_row * 2 + 1;
        grid[vr][visual_cols - 1] = 'S';
    }

    // Exit
    if (m->exit_row == 0) {
        int vc = m->exit_col * 2 + 1;
        grid[0][vc] = 'F';
    }

    else if (m->exit_row == m->rows - 1) {
        int vc = m->exit_col * 2 + 1;
        grid[visual_rows - 1][vc] = 'F';
    }

    else if (m->exit_col == 0) {
        int vr = m->exit_row * 2 + 1;
        grid[vr][0] = 'F';
    }

    else if (m->exit_col == m->cols - 1) {
        int vr = m->exit_row * 2 + 1;
        grid[vr][visual_cols - 1] = 'F';
    }

    for (int r = 0; r < visual_rows; r++) {
        fprintf(f, "%s\n", grid[r]);
        free(grid[r]);
    }

    free(grid);
}

void maze_write_binary(Maze* m, FILE* f) {
    if (f == NULL) {
        fprintf(stderr, "error: File is null\n");
        return;
    }

    uint8_t rows    = (uint8_t)m->rows;
    uint8_t cols    = (uint8_t)m->cols;
    uint8_t entry_r = (uint8_t)m->entry_row;
    uint8_t entry_c = (uint8_t)m->entry_col;
    uint8_t exit_r  = (uint8_t)m->exit_row;
    uint8_t exit_c  = (uint8_t)m->exit_col;

    fwrite(&rows,    sizeof(uint8_t), 1, f);
    fwrite(&cols,    sizeof(uint8_t), 1, f);
    fwrite(&entry_r, sizeof(uint8_t), 1, f);
    fwrite(&entry_c, sizeof(uint8_t), 1, f);
    fwrite(&exit_r,  sizeof(uint8_t), 1, f);
    fwrite(&exit_c,  sizeof(uint8_t), 1, f);

    for (size_t row = 0; row < (size_t)m->rows; row++) {
        for (size_t col = 0; col < (size_t)m->cols; col++) {
            uint8_t byte = 0;
            byte |= (m->cells[row][col].wall_north  ? 1 : 0) << 0;
            byte |= (m->cells[row][col].wall_south  ? 1 : 0) << 1;
            byte |= (m->cells[row][col].wall_east   ? 1 : 0) << 2;
            byte |= (m->cells[row][col].wall_west   ? 1 : 0) << 3;
            byte |= (m->cells[row][col].on_solution ? 1 : 0) << 4;
            byte |= (m->cells[row][col].visited     ? 1 : 0) << 5;
            fwrite(&byte, sizeof(uint8_t), 1, f);
        }
    }
}

Maze* maze_read_binary(FILE* f) {
    if (f == NULL) {
        fprintf(stderr, "error: File is null\n");
        return NULL;
    }

    uint8_t rows, cols, entry_row, entry_col, exit_row, exit_col;
    fread(&rows,      sizeof(uint8_t), 1, f);
    fread(&cols,      sizeof(uint8_t), 1, f);
    fread(&entry_row, sizeof(uint8_t), 1, f);
    fread(&entry_col, sizeof(uint8_t), 1, f);
    fread(&exit_row,  sizeof(uint8_t), 1, f);
    fread(&exit_col,  sizeof(uint8_t), 1, f);

    if (!is_valid(rows) || !is_valid(cols)) {
        fprintf(stderr, "error: Invalid dimensions\n");
        return NULL;
    }

    Maze* m = maze_create(rows, cols);
    if (m == NULL){
        return NULL;   
    } 

    m->entry_row = entry_row;
    m->entry_col = entry_col;
    m->exit_row  = exit_row;
    m->exit_col  = exit_col;

    for (size_t row = 0; row < (size_t)m->rows; row++) {
        for (size_t col = 0; col < (size_t)m->cols; col++) {
            uint8_t byte;
            fread(&byte, sizeof(uint8_t), 1, f);
            m->cells[row][col].wall_north  = (byte >> 0) & 1;
            m->cells[row][col].wall_south  = (byte >> 1) & 1;
            m->cells[row][col].wall_east   = (byte >> 2) & 1;
            m->cells[row][col].wall_west   = (byte >> 3) & 1;
            m->cells[row][col].on_solution = (byte >> 4) & 1;
            m->cells[row][col].visited     = (byte >> 5) & 1;
        }
    }

    return m;
}