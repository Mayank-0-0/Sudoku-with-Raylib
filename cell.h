#ifndef CELL_H
#define CELL_H
typedef struct cells{
    int i;
    int j;
    bool fixed;
    bool temp_fixed;
    int value;
} Cell;
Cell board[9][9] = {0};
#endif