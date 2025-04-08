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

// you shall not change them
const int height = 480;
const int width = 854;
const int targetFPS = 240;
const int speed = 240;


// get the size to fit in rectangle
std::pair<int, int> getBestFontSizeToFit(const char *text, int maxWidth, int maxHeight) {
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
  void moveBy(Vector2 shift) {
    rec.x += shift.x;
    rec.y += shift.y;
  }
  const Rectangle &getRec() { return rec; }

private:
  Rectangle rec;
  Color color;
  std::optional<Texture2D> image;
};

class Group {
public:
  Group() = default;
  Group &add_obj(std::shared_ptr<RecObj> obj) {
    objects.emplace_back(obj);
    return *this;
  }
  void ChangePosBy(Vector2 change) {
    for (shared_ptr<RecObj> object : objects) {
      object->moveBy(change);
    }
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
    auto t = getBestFontSizeToFit(text.c_str(), rec.width - 2, rec.height - 2);
    DrawText(text.c_str(), rec.x + (rec.width - t.second) / 2, rec.y + 1,
             t.first, BLACK);
  }
  bool checkButtonPress() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rec)) {
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

    return 0;
}
