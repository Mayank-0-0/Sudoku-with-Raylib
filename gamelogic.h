#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include "include/raylib.h"
#include <stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "cell.h"
char warningText[64] = "";
double warningStartTime = 0;
bool showWarning = false;
bool checkGameComplete;

void SetBoard(Cell board[9][9]);
bool IsAllowedNumber(Cell board[9][9],int row,int col,int num);
int IsValid(Cell board[9][9],int row,int col,int num);
bool IfGameCompleted(Cell board[9][9]);
#endif

void SetBoard(Cell board[9][9]){
    checkGameComplete = IfGameCompleted(board);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[i][j].value!=0 && board[i][j].fixed ){
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+20, 50+j*60+20, 26, BLACK);
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+21, 50+j*60+20, 27, BLACK);
            }if(board[i][j].value!=0 && board[i][j].fixed==false && !board[i][j].hint){
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+20, 50+j*60+20, 26, GRAY);
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+21, 50+j*60+20, 27, GRAY);
            }if(board[i][j].value!=0 && board[i][j].hint){
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+20, 50+j*60+20, 26, BLUE);
                DrawText(TextFormat("%d", board[i][j].value), 50+i*60+21, 50+j*60+20, 27, BLUE);
            }
            if(board[i][j].temp_fixed){
                DrawRectangle(50 + i*60 + 10, 50 + j*60 + 10, 50 - 10, 50 - 10, WHITE);
                DrawRectangleLines(50 + i*60 + 10, 50 + j*60 + 10, 50 - 10, 50 - 10, BLACK);
                int key = GetKeyPressed();
                if (key >= KEY_ONE && key <= KEY_NINE) {
                    int value = key - KEY_ZERO;
                    if(IsAllowedNumber(board,i,j,value)==false){
                        board[i][j].value = 0;
                        board[i][j].temp_fixed = false;
                        continue;
                    }else{
                        board[i][j].value = value;
                        board[i][j].temp_fixed = false;
                    }
                    DrawText(TextFormat("%d", board[i][j].value), 50+i*60+20, 50+j*60+20, 26, GRAY);
                }else if(key==KEY_BACKSPACE || key==KEY_DELETE){
                    board[i][j].value = 0;
                    board[i][j].temp_fixed = false;
                }
            }
        }
    }
    if(checkGameComplete){
        int text1Width = MeasureText("CONGRATULATIONS!!", 30);
        int text2Width = MeasureText("YOU HAVE COMPLETED THE GAME!", 30);
        int text3Width = MeasureText("***click anywhere to start a new game***",20);
        DrawRectangle(0, 0,800, 600, Fade (BLACK, 0.8f));
        DrawText("CONGRATULATIONS!!", 400-text1Width/2,300, 30, WHITE);
        DrawText("YOU HAVE COMPLETED THE GAME!", 400-text2Width/2,340, 30, WHITE);
        DrawText("***click anywhere to start a new game***",400-text3Width/2,380,20,WHITE);        
    }
}

bool IsAllowedNumber(Cell board[9][9],int row,int col,int num){
    int result = IsValid(board,row,col,num);
    if(result==1){
        strcpy(warningText, "Number already present in column");
        warningStartTime = GetTime();
        showWarning = true;
        return false;
    }else if(result==2){
        strcpy(warningText, "Number already present in row");
        warningStartTime = GetTime();
        showWarning = true;
        return false;

    }else if(result==3){
        strcpy(warningText, "Number already present in box");
        warningStartTime = GetTime();
        showWarning = true;
        return false;
    }
    return true;
}

int IsValid(Cell board[9][9],int row,int col,int num){
    for(int x=0;x<9;x++){
        if(board[x][col].value==num && x!=row)
            return 1;
    }
    for(int x=0;x<9;x++){
        if(board[row][x].value==num && x!=col)
            return 2;
    }
    for(int x=(row/3)*3;x<(row/3)*3+3;x++){
        for(int y=(col/3)*3;y<(col/3)*3+3;y++){
            if(board[x][y].value==num && x!=row && y!=col)
                return 3;
        }
    }
    return 0;
}

bool IfGameCompleted(Cell board[9][9]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(board[i][j].value==0)
                return false;
        }
    }
    return true;
}

