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
std::air<int, int> getBestFontSizeToFit(const char *text, int maxWidth, int maxHeight) {
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
