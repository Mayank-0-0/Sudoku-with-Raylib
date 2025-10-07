#include "include/raylib.h"
#include "sudoku_genrator.h"
#include "cell.h"
#include "gameLogic.h"
#define Columns 9
#define Rows 9
const int screenHeight = 600;
const int screenWidth = 800;
const int cellwidth = 540/Columns;
const int cellheight = 540/Rows;
void DrawBoard(Cell cell){
    DrawRectangleLines(50+cell.i*cellwidth, 50+cell.j*cellheight, cellwidth, cellheight, BLACK);
}
bool IndexIsValid(int i, int j){
    if(i<0 || i>=Columns || j<0 || j>=Rows) return false;
    return true;
}

int main(){
    int boardGenerated = 0;
    Rectangle start_button = {screenWidth/2 - 100, screenHeight/2 - 50, 200, 100};
    Vector2 mousePoint={0.0f,0.0f};
    for (int i=0;i<Columns;i++){
        for (int j=0;j<Rows;j++){
            board[i][j].i = i;
            board[i][j].j = j;
        }
    }

    int ButtonState = 0;
    bool ButtonStatus = false;
    bool ButtonActivated = false;
    
    InitWindow(screenWidth, screenHeight, "Sudoku");
    while (!WindowShouldClose()){
        mousePoint = GetMousePosition();
        BeginDrawing();
        ClearBackground(RAYWHITE);
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
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&& ButtonActivated){
                Vector2 mouse = GetMousePosition();
                int indexi = (mouse.x - 50) / cellwidth;
                int indexj = (mouse.y - 50) / cellheight;
                if (IndexIsValid(indexi, indexj)&& !board[indexi][indexj].fixed){
                    for(int k=0;k<Columns;k++){
                        for(int l=0;l<Rows;l++){
                            board[k][l].temp_fixed = false;
                        }
                    }
                    board[indexi][indexj].temp_fixed = true;
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
                }
            }
            GenerateFullSudoku(board);
            checkGameComplete=false;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}