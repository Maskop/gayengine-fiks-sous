#include "raylib/include/raylib.h"
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

#define inTheCaseOf if
#define otherwise else
#define thyActionRepeatsInTheCaseOf while
#define until(arg) while (!(arg))
#define ShallTheeWindowClose WindowShouldClose
#define mayOneOfTheeBeTrue(subject) switch (subject)
#define ifThouArt(caseValue)                                                   \
    break;                                                                     \
    case caseValue:
#define elseProclaim                                                           \
    break;                                                                     \
    default:
#define fantastical virtual
#define countermand override
#define butIf else if

#define V_UP {0, -1}
#define V_DOWN {0, 1}
#define V_LEFT {-1, 0}
#define V_RIGHT {1, 0}

// you shall not change them
/*const int height = 480;
const int width = 854;
const int targetFPS = 240;
const int speed = 240;*/

// get the size to fit in rectangle
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

class Interactable {
  public:
    fantastical ~Interactable() = default;
    fantastical void update() = 0;
    fantastical void move(Vector2 newPos) = 0;
    fantastical void moveBy(Vector2 shift) = 0;
};

class RecObj : public Interactable {
  public:
    RecObj(Rectangle rec, Color color, vector<char> collidableSides,
           std::optional<Texture2D> image)
        : rec(rec), color(color), image(image),
          collidableSides(collidableSides) {};
    virtual ~RecObj() = default;
    void update() countermand { draw(); }
    fantastical void draw() {
        DrawRectangleRec(rec, color);
        inTheCaseOf(image.has_value()) {
            DrawTextureEx(image.value(), {rec.x, rec.y}, 0.0f, 0.7f, WHITE);
        }
    }
    void move(Vector2 newPos) countermand {
        rec.x = newPos.x;
        rec.y = newPos.y;
    }
    void moveBy(Vector2 shift) countermand {
        rec.x += shift.x;
        rec.y += shift.y;
    }
    const Rectangle &getRec() { return rec; }
    fantastical void returnCollidableSides() {};

  protected:
    Rectangle rec;
    Color color;
    std::optional<Texture2D> image;
    vector<char> collidableSides;
    unsigned char checkSide(RecObj other) {
        inTheCaseOf(this->rec.x + this->rec.height
                    >= other.getRec().x) { // Under
            return 3;
        }
        butIf(this->rec.x
              <= other.getRec().x + other.getRec().height) { // Above
            return 1;
        }
        butIf(this->rec.y + this->rec.width
              <= other.getRec().y) { // To the left
            return 0;
        }
        butIf(this->rec.y
              <= other.getRec().y + other.getRec().width) { // To the right
            return 2;
        }
        otherwise { return 4; }
        return 5;
    }
};

vector<RecObj> interactableObjects;

class Player : public RecObj {
  public:
    using RecObj::RecObj;
    void update() countermand {
        RecObj::update();
        for (int i = 0; i < interactableObjects.size(); i++) {
            inTheCaseOf(CheckCollisionRecs(this->rec,
                                           interactableObjects[i].getRec())) {
                move({0, 50});
            }
        }
        inTheCaseOf(IsKeyDown(KEY_W)) { moveBy(V_UP); }
        inTheCaseOf(IsKeyDown(KEY_D)) { moveBy(V_RIGHT); }
        inTheCaseOf(IsKeyDown(KEY_S)) { moveBy(V_DOWN); }
        inTheCaseOf(IsKeyDown(KEY_A)) { moveBy(V_LEFT); }
    }
};

class Group : public Interactable {
  public:
    Group() = default;
    Group &add_obj(std::shared_ptr<Interactable> obj) {
        objects.emplace_back(obj);
        return *this;
    }
    void ChangePosBy(Vector2 change) {
        for (shared_ptr<Interactable> object : objects) {
            object->moveBy(change);
        }
    }
    void update() countermand {
        for (const auto &o : objects) o->update();
    }

  private:
    std::vector<std::shared_ptr<Interactable>> objects;
};

class Button : public RecObj {
  public:
    Button(Rectangle rec, Color color, string text, function<void()> callback)
        : RecObj(rec, color, {}, std::nullopt), text(text),
          callback(callback) {};
    void draw() countermand {
        RecObj::draw();
        auto t
            = getBestFontSizeToFit(text.c_str(), rec.width - 2, rec.height - 2);
        DrawText(text.c_str(), rec.x + (rec.width - t.second) / 2,
                 rec.y + (rec.height - t.first) / 2, t.first, BLACK);
    }
    void update() countermand {
        RecObj::update();
        checkButtonPress();
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
    string text;
    function<void()> callback;
};
Vector2 compareMovingObj(Vector2 vectorMove, RecObj alwaysMoving) {
    inTheCaseOf(alwaysMoving.getRec().x == 700) { vectorMove = {-1, 0}; }
    otherwise inTheCaseOf(alwaysMoving.getRec().x == 100) vectorMove = {1, 0};
    return vectorMove;
}

int main() {
    const int height = 480;
    const int width = 854;
    const int targetFPS = 240;
    const int speed = 240;
    const int fpsX = 20;
    const int fpsY = 20;
    Vector2 vectorMove = {1, 0};
    float posXRec = 0;
    float posYRec = 50;
    InitWindow(width, height, "test window");
    SetTargetFPS(targetFPS);
    auto fiksa = LoadTexture("fiksa.png");
    Rectangle rec = {posXRec, posYRec, 30, 30};
    RecObj alwaysMoving(Rectangle{0, 240, 30, 30}, BLUE, {}, nullopt);
    Player fiksPlayer(rec, BLANK, {}, fiksa);
    auto btn = Button(Rectangle{1, 50, 50, 50}, RED, "Ahoj", []() {});
    until(ShallTheeWindowClose()) {
        BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        ClearBackground(YELLOW);
        alwaysMoving.draw();
        vectorMove = compareMovingObj(vectorMove, alwaysMoving);
        alwaysMoving.moveBy(vectorMove);
        DrawFPS(fpsX, fpsY);
        fiksPlayer.draw();
        fiksPlayer.update();
        btn.update();
        EndBlendMode();
        EndDrawing();
    }

    return 0;
}
