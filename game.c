#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

enum PosibleState{
    Die,
    Live,
    NextLive,
    NextDie,
};

enum PosibleState toggleCell(enum PosibleState s) {
    switch (s) {
        case Die:
            return Live;
        case Live:
            return Die;
        default:
            return Die;
    }
}

const Color BG_COLOR = {26, 27, 38, 255};
const Color FG_COLOR = {169, 177, 214, 255};
const int ScreenWidth = 600;
const int ScreenHeight = 600;
const int L_CELL = 25;


int GetMinWidthHeight() {
    return (GetScreenWidth() > GetScreenHeight()) ? GetScreenHeight() : GetScreenWidth();
}

int GetStartWidth() {
    return (GetScreenWidth() - GetMinWidthHeight()) / 2;
}

int GetStartHeight() {
    return (GetScreenHeight() - GetMinWidthHeight()) / 2;
}

void DrawGridCells() {
    int startWidht = GetStartWidth();
    int startHeight = GetStartHeight();
    int cellSize = GetMinWidthHeight() / L_CELL;

    for (int i=1; i < L_CELL; i++) {
        DrawLine(startWidht + (i * cellSize), startHeight, startWidht + (i * cellSize), GetScreenHeight(), FG_COLOR);
        DrawLine(startWidht, startHeight + (i * cellSize), GetScreenWidth(), startHeight + (i * cellSize), FG_COLOR);
    }
}

void DrawCells(enum PosibleState **game) {
    int startWidht = GetStartWidth();
    int startHeight = GetStartHeight();
    int cellSize = GetMinWidthHeight() / L_CELL;

    for (int i = 0; i < L_CELL; i++) {
        for (int j = 0; j < L_CELL; j++) {
            if (game[i][j] == 1) {
                DrawRectangle(startWidht + (j * cellSize), startHeight + (i * cellSize), cellSize, cellSize, FG_COLOR);
            }
        } }
}

void CheckMouseClick(enum PosibleState **game) {
    int startWidht = GetStartWidth();
    int startHeight = GetStartHeight();
    int cellSize = GetMinWidthHeight() / L_CELL;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int positionX = GetMouseX();
        int positionY = GetMouseY();
        int cellY = (positionX - startWidht) / cellSize;
        int cellX = (positionY - startHeight) / cellSize;
        game[cellX][cellY] = toggleCell(game[cellX][cellY]);
    }
}

void MustTogglePlay(bool *p) {
    if (IsKeyPressed(KEY_SPACE))
        *p = !(*p);
}

int CountLiveNeighbors(enum PosibleState **game, int i, int j) {
    int LiveNeighbors = 0;
    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++ ) {
            int XPos = (i + k + L_CELL) % (L_CELL);
            int YPos = (j + l + L_CELL) % (L_CELL);
            if ((k != 0 || l != 0) && game[XPos][YPos] == Live || game[XPos][YPos] == NextDie)
                LiveNeighbors++;
        }
    }

    return LiveNeighbors;
}

void UpdateGame(enum PosibleState **game) {
    for (int i = 0; i < L_CELL; i++) {
        for (int j = 0; j < L_CELL; j++) {
            int numberLiveNeighbors = CountLiveNeighbors(game, i, j);
            if (numberLiveNeighbors == 3 && game[i][j]==Die) game[i][j] = NextLive;
            if ((numberLiveNeighbors == 3 || numberLiveNeighbors == 3) && game[i][j] == Live) game[i][j] = Live;
            if (numberLiveNeighbors <= 1 && game[i][j]==Live) game[i][j] = NextDie;
            if (numberLiveNeighbors >= 4 && game[i][j]==Live) game[i][j] = NextDie;
        }
    }

    for (int i = 0; i < L_CELL; i++) {
        for (int j = 0; j < L_CELL; j++) {
            int numberLiveNeighbors = CountLiveNeighbors(game, i, j);
            switch (game[i][j]) {
                case NextLive:
                    game[i][j] = Live;
                    break;
                case NextDie:
                    game[i][j] = Die;
                default:
                    game[i][j] = game[i][j];
                    break;
            }
        }
    }
}

int main(void) {

    bool gameBegan = false;

    InitWindow(ScreenWidth, ScreenHeight, "Game Of Life");
    SetTargetFPS(60);
    
    enum PosibleState **game= (enum PosibleState **)calloc(L_CELL, sizeof(enum PosibleState *));

    for (int i = 0; i < L_CELL; i++) {
        game[i] = (enum PosibleState *)calloc(L_CELL, sizeof(enum PosibleState));
    }

    float timeElapsed = 0.0;

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(BG_COLOR);

            DrawGridCells();
            DrawCells(game);
            if (!gameBegan) {
                CheckMouseClick(game);
            }
            if (gameBegan && timeElapsed >= 0.5) {
                UpdateGame(game);
                timeElapsed = 0.0;
            }
            timeElapsed = timeElapsed + GetFrameTime();
            MustTogglePlay(&gameBegan);

        EndDrawing();
    }

    for (int i = 0; i < L_CELL; i++) {
        free(game[i]);
    }
    free(game);
    CloseWindow();
    return 0;
}
