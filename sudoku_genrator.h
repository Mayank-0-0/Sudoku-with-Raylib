#ifndef SUDOKU_GENRATOR_H
#define SUDOKU_GENRATOR_H
#include "include/raylib.h"
#include <stdbool.h>
#include<stdlib.h>
#include "cell.h"
bool IsValidNumber(Cell board[9][9], int row, int col, int num);
void GenerateFullSudoku(Cell board[9][9]);
void RemoveNumbers(Cell board[9][9]);
int SolveSudoku(Cell board[9][9], int row, int col);
#endif
void GenerateFullSudoku(Cell board[9][9]){
    int randomNum=rand()%9+1;
    board[0][0].value = randomNum;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(i!=0 && j!=0)
                board[i][j].value = 0;
        }
    }
    SolveSudoku(board,0,0);
    RemoveNumbers(board);
}

void RemoveNumbers(Cell board[9][9]){
    //int randomNum=rand()%10+25;
    int randomNum=80;
    while(randomNum!=81){
        int i=rand()%9;
        int j=rand()%9;
        if(board[i][j].value!=0){
            board[i][j].value=0;
            randomNum++;
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[i][j].value==0){
                board[i][j].fixed = false;
            }else{
                board[i][j].fixed = true;
            }
        }
    }
} 
bool IsValidNumber(Cell board[9][9],int row,int col,int num){
    for(int x=0;x<9;x++){
        if(board[x][col].value==num && x!=row)
            return false;
    }
    for(int x=0;x<9;x++){
        if(board[row][x].value==num && x!=col)
            return false;
    }
    for(int x=(row/3)*3;x<(row/3)*3+3;x++){
        for(int y=(col/3)*3;y<(col/3)*3+3;y++){
            if(board[x][y].value==num && x!=row && y!=col)
                return false;
        }
    }
    return true;
}
int SolveSudoku(Cell board[9][9], int row, int col){
    if(row==9 && col==0) 
        return 1;
    else{
        if(board[row][col].value!=0){
            SolveSudoku(board, (col==8)?row+1:row,col==8?0:col+1);
        }else{
            for(int i=0;i<9;i++){
                if(IsValidNumber(board,row,col,i+1)){
                    board[row][col].value = i+1;
                    if(SolveSudoku(board, (col==8)?row+1:row,col==8?0:col+1))
                        return 1;
                    board[row][col].value = 0;
                }
            }
        }
    }
    return 0;
}   
