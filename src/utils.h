#ifndef INCLUDE_SRC_UTILS_H_
#define INCLUDE_SRC_UTILS_H_

#include <raylib/raylib.h>
#include <stdio.h>

#define UNIMPLEMENTED                                                          \
  std::cerr << "\n_____________________________________\n\n"                   \
            << "Line " << __LINE__ << " in " << __FILE__                       \
            << ": UNIMPLEMENTED FEATURE!"                                      \
            << "\n_____________________________________\n\n";

float Util_LerpFloat(float, float, float);
Vector2 Util_LerpVec2(Vector2, Vector2, float);

#endif // INCLUDE_SRC_UTILS_H_
