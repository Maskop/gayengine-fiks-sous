#include "raylib/include/raylib.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

#define inTheCaseOf if
#define otherwise else
#define thyActionRepeatsInTheCaseOf while
#define until(arg) while(!(arg))
#define ShallTheeWindowClose WindowShouldClose

// you shall not change them
const int height = 350;
const int width = 350;
const int targetFPS = 240;

// Not a constat just because
float gAcc = 12;

Rectangle moveRec(Rectangle rec, const Vector2 & vec){
    rec.x += vec.x;
    rec.y += vec.y;
    return rec;
}

Vector2 operator*(Vector2 v, float a){
    v.x *= a;
    v.y *= a;
    return v;
}

Rectangle keyMove(Rectangle ctverec, Vector2 vec, float speed, int key, const vector<Rectangle> & bounderies) {
    inTheCaseOf (IsKeyDown(key)) {
        auto newPos = moveRec(ctverec, vec * GetFrameTime() * speed);
        bool canMove = true;
        for (const auto & r : bounderies){
            canMove = canMove && !CheckCollisionRecs(newPos, r);
        }
        if (canMove)
            ctverec = newPos; 
        }
    return ctverec;
}

class Button{
public:
    Button(Rectangle rec, string text, function<void()> callback): rec(rec), text(text), callback(callback){
    };
    void draw() {
        DrawRectangleRec(rec, WHITE);
    }
    bool checkButtonPress(){
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rec)){
            callback();
            return true;
        }
        return false;
    }

private:
    Rectangle rec;
    string text;
    function<void()> callback;
};

int main() {
    InitWindow(height, width, "There shall be a window");
    SetTargetFPS(targetFPS);
    
    Color background_color = YELLOW;
    float startPosX = 69;
    float startPosY = 69;
    int speed = 64;
    Color color_green = GREEN;
    Color color_yellow = YELLOW;
    Rectangle ctverecek{69,69,69,69};
    vector bounderies{Rectangle{-10, 0, 10, height}, Rectangle{width, 0, 10, height}, Rectangle{0, -10, width, 10}, Rectangle{0, width, width, 10}
    };
    Button tlacitko(Rectangle{100, 100, 50, 20}, "Ahoj", [&](){background_color = color_green;});
    until (ShallTheeWindowClose()){
        BeginDrawing();
        ClearBackground(background_color);
        DrawRectangleRec(ctverecek, RED);
        tlacitko.draw();
        if (tlacitko.checkButtonPress()) {
            swap(color_green, color_yellow);
        }
        for (const auto & r : bounderies){
            DrawRectangleRec(r, GREEN);
        }
        DrawFPS(10, 10);
            ctverecek = keyMove(ctverecek, {0, -1}, speed, KEY_W, bounderies);
            ctverecek = keyMove(ctverecek, {0, 1}, speed, KEY_S, bounderies);
            ctverecek = keyMove(ctverecek, {1, 0}, speed, KEY_D, bounderies);
            ctverecek = keyMove(ctverecek, {-1, 0}, speed, KEY_A, bounderies);
        EndDrawing();
    }
    return 0;
}
