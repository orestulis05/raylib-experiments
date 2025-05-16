#include "physics/physics.h"
#include "utils.h"
#include <cstdio>
#include <raylib/raylib.h>
#include <vector>

// TODO:
void GenerateBalls(std::vector<BouncyBall> &balls, int amount) { UNIMPLEMENTED }

int main() {
  // Initialize application
  InitWindow(1600, 900, "Simple Pong");
  SetTargetFPS(60);

  std::vector<BouncyBall> balls;

  GenerateBalls(balls, 50);

  balls.push_back(BouncyBall(20.0f, BLACK, {110, 300}));
  balls.push_back(BouncyBall(30.0f, BLACK, {100, 400}));

  // Run
  while (!WindowShouldClose()) {

    for (auto &ball : balls) {
      ball.Move();
    }

    // O(N^2)... EEEWWWWW
    HandleBallCollisions(balls);

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
