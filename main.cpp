#include "raylib/include/raylib.h"
#include <ctime>
#include <functional>
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
    RecObj(Rectangle rec, Color color, std::optional<Texture2D> image)
        : rec(rec), color(color), image(image) {};
    virtual ~RecObj() = default;
    void update() override { draw(); }
    fantastical void draw() {
        DrawRectangleRec(rec, color);
        if (image.has_value()) {
            DrawTexture(image.value(), rec.x, rec.y, color);
        }
    }
    void move(Vector2 newPos) override {
        rec.x = newPos.x;
        rec.y = newPos.y;
    }
    void moveBy(Vector2 shift) override {
        rec.x += shift.x;
        rec.y += shift.y;
    }
    const Rectangle &getRec() { return rec; }

  protected:
    Rectangle rec;
    Color color;
    std::optional<Texture2D> image;
};

class Player : public RecObj {
  public:
    void getCollision(RecObj other) {}

  private:
    unsigned char checkSide(RecObj other) {
        inTheCaseOf(this->rec.x + this->rec.height
                    > other.getRec().x) { // Above
            return 1;
        }
        otherwise inTheCaseOf(
            this->rec.x < other.getRec().x + other.getRec().height) { // Under
            return 3;
        }
        otherwise inTheCaseOf(this->rec.y + this->rec.width
                              < other.getRec().y) { // To the left
            return 0;
        }
        otherwise inTheCaseOf(this->rec.y
                              < other.getRec().y
                                    + other.getRec().width) { // To the right
            return 2;
        }
        otherwise { return 4; }
        return 5;
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
    void update() override {
        for (const auto &o : objects) o->update();
    }

  private:
    std::vector<std::shared_ptr<Interactable>> objects;
};

class Button : public RecObj {
  public:
    Button(Rectangle rec, Color color, string text, function<void()> callback)
        : RecObj(rec, color, std::nullopt), text(text), callback(callback) {};
    void draw() override {
        RecObj::draw();
        auto t
            = getBestFontSizeToFit(text.c_str(), rec.width - 2, rec.height - 2);
        DrawText(text.c_str(), rec.x + (rec.width - t.second) / 2,
                 rec.y + (rec.height - t.first) / 2, t.first, BLACK);
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

int main() {
    const int height = 480;
    const int width = 854;
    const int targetFPS = 240;
    const int speed = 240;
    const int fpsX = 20;
    const int fpsY = 20;
    InitWindow(width, height, "test window");
    SetTargetFPS(targetFPS);
    auto btn = Button(Rectangle{1, 2, 50, 50}, RED, "Ahoj", []() {});
    until(ShallTheeWindowClose()) {
        BeginDrawing();
        ClearBackground(YELLOW);
        DrawFPS(fpsX, fpsY);
        btn.update();
        EndDrawing();
    }

    return 0;
}
