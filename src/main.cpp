#include "physics/physics.h"
#include <cstdio>
#include <raylib/raylib.h>
#include <vector>

int main() {
  // Initialize application
  InitWindow(1600, 900, "Simple Pong");
  SetTargetFPS(60);

  std::vector<BouncyBall> balls;
  balls.push_back(BouncyBall(20.0f, BLACK, {110, 300}));
  balls.push_back(BouncyBall(30.0f, BLACK, {100, 400}));

  // Run
  while (!WindowShouldClose()) {

    for (auto &ball : balls) {
      ball.Move();
    }

    HandleBallCollisions(balls);
    // O(N^2)... EEEWWWWW

    BeginDrawing();
    ClearBackground(WHITE);

    for (auto &ball : balls) {
      ball.Draw();
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
