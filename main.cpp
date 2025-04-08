#include "raylib/include/raylib.h"
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <memory>

using namespace std;

#define inTheCaseOf if
#define otherwise else
#define thyActionRepeatsInTheCaseOf while
#define until(arg) while (!(arg))
#define ShallTheeWindowClose WindowShouldClose

// you shall not change them
const int height = 480;
const int width = 854;
const int targetFPS = 240;
const int speed = 240;
// Not a constat just because
float gAcc = 12;

std::pair<int, int> getBestFontSizeToFit(const char *text, int maxWidth,
                                         int maxHeight) {
    int fontSize = 1;
    int textWidth;
    while (true) {
        textWidth = MeasureText(text, fontSize);
        int textHeight = fontSize; // Roughly the font height
        if (textWidth > maxWidth || textHeight > maxHeight)
            break;
        fontSize++;
    }
    return {fontSize - 1, textWidth}; // Last fitting size
}

Rectangle moveRec(Rectangle rec, const Vector2 &vec) {
    rec.x += vec.x;
    rec.y += vec.y;
    return rec;
}

Vector2 operator*(Vector2 v, float a) {
    v.x *= a;
    v.y *= a;
    return v;
}

Rectangle keyMove(Rectangle ctverec, Vector2 vec, float speed, int key,
                  const vector<Rectangle> &bounderies) {
    inTheCaseOf(IsKeyDown(key)) {
        auto newPos = moveRec(ctverec, vec * GetFrameTime() * speed);
        bool canMove = true;
        for (const auto &r : bounderies) {
            canMove = canMove && !CheckCollisionRecs(newPos, r);
        }
        if (canMove)
            ctverec = newPos;
    }
    return ctverec;
}

class RecObj {
  public:
    RecObj(Rectangle rec, Color color, std::optional<Texture2D> image)
        : rec(rec), color(color), image(image) {};
    void draw() {
        DrawRectangleRec(rec, color);
        if (image.has_value()) {
            DrawTexture(image.value(), rec.x, rec.y, color);
        }
    }
    void move(Rectangle newPos) { rec = newPos; }
    const Rectangle &getRec() { return rec; }

  private:
    Rectangle rec;
    Color color;
    std::optional<Texture2D> image;
};

class Group {
  public:
    Group() = default;
    Group & add_obj(std::shared_ptr<RecObj> obj){
        objects.emplace_back(obj);
        return *this;
    }

  private:
    std::vector<std::shared_ptr<RecObj>> objects;

};

class Button {
  public:
    Button(Rectangle rec, string text, function<void()> callback)
        : rec(rec), text(text), callback(callback) {};
    void draw() {
        DrawRectangleRec(rec, WHITE);
        auto t
            = getBestFontSizeToFit(text.c_str(), rec.width - 2, rec.height - 2);
        DrawText(text.c_str(), rec.x + (rec.width - t.second) / 2, rec.y + 1,
                 t.first, BLACK);
    }
    bool checkButtonPress() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
            && CheckCollisionPointRec(GetMousePosition(), rec)) {
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
    InitWindow(width, height, "Hybajici se kocicka");
    InitAudioDevice();
    SetTargetFPS(targetFPS);

    Sound mnoukani = LoadSound("mnoukani.mp3");
    Color background_color = YELLOW;
    float startPosX = 69;
    float startPosY = 69;
    Color color_green = GREEN;
    Color color_yellow = YELLOW;
    auto kosiska = LoadTexture("kocicka.jpg");
    RecObj ctverecek(Rectangle{69, 69, kosiska.width, kosiska.height}, RED,
                     kosiska);
    vector bounderies{
        Rectangle{-10, 0, 10, height}, Rectangle{width, 0, 10, height},
        Rectangle{0, -10, width, 10}, Rectangle{0, height, width, 10}};
    Button tlacitko(Rectangle{0, height - 50, 100, 50}, "Ahoj",
                    [&]() { background_color = color_green; });
    Button tlacitkoMnau(Rectangle{0, 0, 100, 50}, "Ahoj",
                    [&]() { PlaySound(mnoukani); });
    until(ShallTheeWindowClose()) {
        BeginDrawing();
        ClearBackground(background_color);
        ctverecek.draw();
        tlacitko.draw();
        tlacitkoMnau.draw();
        tlacitkoMnau.checkButtonPress();
        if (tlacitko.checkButtonPress()) {
            swap(color_green, color_yellow);
        }
        for (const auto &r : bounderies) {
            DrawRectangleRec(r, GREEN);
        }
        DrawFPS(10, 10);
        ctverecek.move(
            keyMove(ctverecek.getRec(), {0, -1}, speed, KEY_W, bounderies));
        ctverecek.move(
            keyMove(ctverecek.getRec(), {0, 1}, speed, KEY_S, bounderies));
        ctverecek.move(
            keyMove(ctverecek.getRec(), {1, 0}, speed, KEY_D, bounderies));
        ctverecek.move(
            keyMove(ctverecek.getRec(), {-1, 0}, speed, KEY_A, bounderies));
        EndDrawing();
    }
    return 0;
    }
