#include "raylib/include/raylib.h"

#define inTheCaseOf if
#define otherwise else
#define ShallTheeWindowClose WindowShouldClose

// you shall not change them
const int height = 350;
const int width = 350;
const int targetFPS = 1000000000;

// Not a constat just because
float gAcc = 12;

int main() {
    InitWindow(height, width, "There shall be a window");
    SetTargetFPS(targetFPS);
    

    int startPosX = 69;
    int startPosY = 69;
    while (!ShallTheeWindowClose()){
        BeginDrawing();
        ClearBackground(YELLOW);
        DrawRectangle(startPosX, startPosY, 69, 69, RED);
        DrawFPS(10, 10);
        EndDrawing();
        if (IsKeyDown(KEY_W)) {
            startPosY++; startPosX++;
        }
    }
    return 0;
}
