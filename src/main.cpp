#include "physics/physics.h"
#include "utils.h"
#include <cstddef>
#include <cstdio>
#include <random>
#include <raylib/raylib.h>
#include <vector>

void GenerateBalls(std::vector<BouncyBall> &balls, int amount) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> rPosY(50.f, GetScreenHeight() - 50.f);
  std::uniform_real_distribution<float> rPosX(50.f, GetScreenWidth() - 50.f);
  std::uniform_real_distribution<float> rBounciness(0.2f, 0.6f);
  std::uniform_real_distribution<float> rRadius(10.f, 50.f);
  std::uniform_real_distribution<float> rVel(-100.f, 100.f);

  for (size_t i = 0; i < amount; ++i) {
    BouncyBall ball(rRadius(gen), BLACK, {rPosX(gen), rPosY(gen)},
                    rBounciness(gen));
    ball.velocity = {rVel(gen), rVel(gen)};
    balls.push_back(ball);
  }
}

int main() {
  // Initialize application
  InitWindow(1600, 900, "Bouncy Balls");
  SetTargetFPS(60);

  std::vector<BouncyBall> balls;
  GenerateBalls(balls, 100);

  // Run
  while (!WindowShouldClose()) {

    for (auto &ball : balls) {
      ball.Move();
    }

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
