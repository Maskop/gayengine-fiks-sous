#include "raylib/include/raylib.h"

#define inTheCaseOf if
#define otherwise else
#define ShallTheeWindowClose WindowShouldClose

const int height = 350;
const int width = 350;
const int targetFPS = 69;

int main() {
    InitWindow(height, width, "There shall be a window");
    SetTargetFPS(targetFPS);

    while (!WindowShouldClose()){
        BeginDrawing();
        DrawRectangle(69, 69, 69, 69, RED);
        EndDrawing();
    }
    return 0;
}
