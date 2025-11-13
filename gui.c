#include "include/raylib.h"
#include "sudoku_genrator.h"
#include "cell.h"
#include "gameLogic.h"
#include <stdio.h>
#include <stdbool.h>
#define Columns 9
#define Rows 9
const int screenHeight = 600;
const int screenWidth = 800;
const int cellwidth = 540/Columns;
const int cellheight = 540/Rows;
Rectangle hint_button={615,(screenHeight/2)+50+90,165,55};
Rectangle solve_button = {615, (screenHeight/2)+50, 165, 55};
Rectangle start_button = {screenWidth/2 - 100, screenHeight/2 - 50, 200, 100};
void DrawBoard(Cell cell){
    DrawRectangleLines(50+cell.i*cellwidth, 50+cell.j*cellheight, cellwidth, cellheight, BLACK);
}
bool IndexIsValid(int i, int j){
    if(i<0 || i>=Columns || j<0 || j>=Rows) return false;
    return true;
}
bool waitForNextClick = false;
double startTime = 0.0;
bool timerStarted = false;
int main(){
    int boardGenerated = 0;
    bool solvedbuttonclicked = false;
    bool hintbuttonclicked = false;
    int hintUsed=0;


    Vector2 mousePoint={0.0f,0.0f};
    for (int i=0;i<Columns;i++){
        for (int j=0;j<Rows;j++){
            board[i][j].i = i;
            board[i][j].j = j;
        }
    }
    int recordRow = 0;
    int recordCol = 0;
    int recordValue = 0;
    int ButtonState = 0;
    bool ButtonStatus = false;
    bool ButtonActivated = false;
    
    InitWindow(screenWidth, screenHeight, "Sudoku");
    while (!WindowShouldClose()){
        mousePoint = GetMousePosition();
        if (waitForNextClick) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                waitForNextClick = false; 
            } else {
                EndDrawing();
                continue; 
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (timerStarted) {
            double elapsed = GetTime() - startTime;
            DrawText(TextFormat("Time:  %.0f  sec", elapsed), 615+25, 30+50, 24, BLACK);
        }
        if(CheckCollisionPointRec(mousePoint, start_button) && !ButtonStatus){
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                ButtonState = 2;
                ButtonStatus = true;
                ButtonActivated = false;
            } else {
                ButtonState = 1;
            }
        } else {
            ButtonState = 0;
        }
        if(ButtonStatus && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !ButtonActivated){
            ButtonActivated = true;
        }
        if(ButtonStatus){
            DrawText("SUDOKU", screenWidth/2 - MeasureText("SUDOKU", 32)/2, 0, 32, BLACK);
            DrawRectangle(50,50,540,540,LIGHTGRAY);
            //DrawRectangleLines(615,50,160,540,LIGHTGRAY);
            DrawRectangle(615,(screenHeight/2)+50,165,55,DARKGRAY);
            DrawRectangle(617.5,(screenHeight/2)+52.5,161.5,51.5,LIGHTGRAY);
            DrawText("Solve", 615 + (165/2) - (MeasureText("Solve", 24)/2), (screenHeight/2)+50 + (55/2) - 12, 24, BLACK);
            DrawRectangle(615,(screenHeight/2)+50+90,165,55,DARKGRAY);
            DrawRectangle(617.5,(screenHeight/2)+52.5+90,161.5,51.5,LIGHTGRAY);
            DrawText("Hint", 615 + (165/2) - (MeasureText("Hint", 24)/2), (screenHeight/2)+50 + (55/2)+90 - 12, 24, BLACK);
            DrawText(TextFormat("Time:"), 615+25, 30+50, 24, BLACK);
            if(boardGenerated==0){
                GenerateFullSudoku(board);
                boardGenerated = 1;
            }
            
            for(int i=0;i<Columns;i++){
                for(int j=0;j<Rows;j++){
                    if(i%3==0 && j%3==0){
                        Rectangle grid = {50+i*cellwidth, 50+j*cellheight, cellwidth*3, cellheight*3};
                        DrawRectangleLinesEx(grid, 4, BLACK);
                    }
                    DrawBoard(board[i][j]);
                }    
            }
            SetBoard(board);
            if(CheckCollisionPointRec(mousePoint,solve_button)){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && checkGameComplete==false && !solvedbuttonclicked){
                    while(front<=rear){
                        printf("%d %d %d\n",record[front].col,record[front].row,record[front].num);
                        front++;
                    }front=0;
                    solvedbuttonclicked = true;
                }
            }
            if(solvedbuttonclicked){
                timerStarted = false;
                startTime = 0.0;
                if(!board[record[front].row][record[front].col].fixed && !board[record[front].row][record[front].col].hint){
                    board[record[front].row][record[front].col].value=record[front].num;
                    DrawText(TextFormat("%d", board[record[front].row][record[front].col].value), 50+record[front].row*60+20, 50+record[front].col*60+20, 26, GREEN);
                    DrawText(TextFormat("%d", board[record[front].row][record[front].col].value), 50+record[front].row*60+21, 50+record[front].col*60+20, 27, GREEN);
                    WaitTime(0.09);
                    recordRow = record[front].row;
                    recordCol = record[front].col;  
                    recordValue = record[front].num;
                }
                front++;
                if(record[front].row<=recordRow && record[front].col<recordCol){
                    board[recordRow][recordCol].value=0;
                }
                if(front>rear){
                    board[recordRow][recordCol].value=recordValue;
                    solvedbuttonclicked = false;
                    front = 0;
                    rear = -1;
                }
            }
            if(CheckCollisionPointRec(mousePoint,hint_button)){
                if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                    hintbuttonclicked=true;
                    printf("true");
                }
            }
            if(hintbuttonclicked){
                if(board[h[hintUsed].row][h[hintUsed].col].value==0){
                    board[h[hintUsed].row][h[hintUsed].col].value=h[hintUsed].num;
                    board[h[hintUsed].row][h[hintUsed].col].hint=true;
                    DrawText(TextFormat("%d", board[h[hintUsed].row][h[hintUsed].col].value), 50+h[hintUsed].row*60+20, 50+h[hintUsed].col*60+20, 26, GREEN);
                    DrawText(TextFormat("%d", board[h[hintUsed].row][h[hintUsed].col].value), 50+h[hintUsed].row*60+21, 50+h[hintUsed].col*60+20, 27, GREEN);
                    printf("%d\nrow- %d\ncol- %d\n",h[hintUsed].num,h[hintUsed].row,h[hintUsed].col);
                    hintUsed++;
                    hintbuttonclicked=false;
                }               
            }
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&& ButtonActivated){
                int indexi = (mousePoint.x - 50) / cellwidth;
                int indexj = (mousePoint.y - 50) / cellheight;
                if (IndexIsValid(indexi, indexj)&& !board[indexi][indexj].fixed){
                    for(int k=0;k<Columns;k++){
                        for(int l=0;l<Rows;l++){
                            board[k][l].temp_fixed = false;
                        }
                    }
                    board[indexi][indexj].temp_fixed = true;
                }
                if (!timerStarted && board[indexi][indexj].value == 0) {
                    startTime = GetTime(); 
                    timerStarted = true;
                }
            }
        } 
        else {
            if(ButtonState == 0) DrawRectangleRec(start_button, LIGHTGRAY);
            else if(ButtonState == 1) DrawRectangleRec(start_button, GRAY);
            else if(ButtonState == 2) DrawRectangleRec(start_button, DARKGRAY);
            DrawText("Start", screenWidth/2 - 25, screenHeight/2 - 10, 20, BLACK);
        }
        if (showWarning && (GetTime() - warningStartTime < 2.0)) {
            DrawText(warningText, 50,30, 18, RED);
        } else if (showWarning && (GetTime() - warningStartTime >= 2.0)) {
            showWarning = false;
        }
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&& checkGameComplete){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    board[i][j].value=0;
                    board[i][j].fixed=false;
                    board[i][j].temp_fixed=false;
                    board[i][j].hint=false;
                    count=0;
                    hintUsed=0;
                    timerStarted = false;
                    startTime = 0.0;
                }
            }
            GenerateFullSudoku(board);
            checkGameComplete=false;
            waitForNextClick = true;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}