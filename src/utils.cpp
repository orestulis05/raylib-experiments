#include "utils.h"

float Util_LerpFloat(float a, float b, float w) { return a + w * (b - a); }
Vector2 Util_LerpVec2(Vector2 a, Vector2 b, float w) {
  float x = a.x + w * (b.x - a.x);
  float y = a.y + w * (b.y - a.y);
  return Vector2{x, y};
}
