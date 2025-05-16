#include "utils.h"
#include <raylib/raylib.h>

const int distFromSides = 100;

class PhysicsRect {
public:
  Rectangle rect;

private:
  Vector2 _velocity;
  bool _colliding_up = false;
  bool _colliding_down = false;
  bool _colliding_left = false;
  bool _colliding_right = false;

private:
  void _HandleCollisionX(PhysicsRect with) {
    if (CheckCollisionRecs(rect, with.rect)) {
      if (rect.x < with.rect.x) {
        // Colliding Right
        _colliding_right = true;
      } else {
        // Colliding Left
        _colliding_left = true;
      }
    }
  }
  void _HandleCollisionY(PhysicsRect with) {
    if (CheckCollisionRecs(rect, with.rect)) {
      if (rect.y < with.rect.y) {
        // Colliding Up
        _colliding_up = true;
      } else {
        // Colliding Down
        _colliding_down = true;
      }
    }
  }
  void _SetVelocityX(float force) { _velocity.x = force; }
  void _SetVelocityY(float force) { _velocity.y = force; }
  void _Draw() { DrawRectanglePro(rect, {0, 0}, 0.0f, BLACK); }
  void _UpdatePositionX() { rect.x += _velocity.x * GetFrameTime(); }
  void _UpdatePositionY() { rect.y += _velocity.y * GetFrameTime(); }
  void _ResetColls() {
    _colliding_up = false;
    _colliding_down = false;
    _colliding_left = false;
    _colliding_right = false;
  }

public:
  bool IsCollidingUp() { return _colliding_up; }
  bool IsCollidingDown() { return _colliding_down; }
  bool IsCollidingLeft() { return _colliding_left; }
  bool IsCollidingRight() { return _colliding_right; }

  void SetVelocityX(float force) { _SetVelocityX(force); }
  void SetVelocityY(float force) { _SetVelocityY(force); }
  Vector2 GetVelocity() { return _velocity; }
  void Draw() { _Draw(); }
  void MoveAndCollide(PhysicsRect with) {
    _ResetColls();

    _UpdatePositionX();
    _HandleCollisionX(with);

    _UpdatePositionY();
    _HandleCollisionY(with);
  }
  void Move() {
    _UpdatePositionX();
    _UpdatePositionY();
  }

  PhysicsRect(Rectangle rect) {
    this->rect = rect;
    _ResetColls();
    _velocity = {0, 0};
  }
};

int main() {
  // Initialize application
  InitWindow(1600, 900, "Simple Pong");
  SetTargetFPS(60);

  PhysicsRect playerPaddle = PhysicsRect((Rectangle){
      .x = distFromSides,
      .y = static_cast<float>(GetScreenHeight() / 2.0f - 50),
      .width = 20,
      .height = 100,
  });
  float playerSpeed = 200.0f;

  PhysicsRect opponentPaddle = PhysicsRect((Rectangle){
      .x = static_cast<float>(GetScreenWidth() - 20 - distFromSides),
      .y = static_cast<float>(GetScreenHeight() / 2.0f - 50),
      .width = 20,
      .height = 100,
  });
  float opponentSpeed = 150.0f;

  PhysicsRect ball = PhysicsRect((Rectangle){
      .x = 800,
      .y = 450,
      .width = 32,
      .height = 32,
  });
  ball.SetVelocityX(200);
  ball.SetVelocityY(200);

  // Run
  while (!WindowShouldClose()) {

    // Check for player input
    playerPaddle.SetVelocityY(0);
    if (IsKeyDown(KEY_W)) {
      playerPaddle.SetVelocityY(-playerSpeed);
    }
    if (IsKeyDown(KEY_S)) {
      playerPaddle.SetVelocityY(playerSpeed);
    }

    // TODO: Enemy follow the ball

    playerPaddle.MoveAndCollide(ball);
    opponentPaddle.MoveAndCollide(ball);
    ball.Move();

    // Ball knocking back from the paddles
    if (playerPaddle.IsCollidingRight()) {
      ball.SetVelocityX(200);
    }
    if (opponentPaddle.IsCollidingLeft()) {
      ball.SetVelocityX(-200);
    }

    // Ball knocking back from the floor and ceiling
    if (ball.rect.y <= 0) {
      ball.SetVelocityY(200);
    } else if (ball.rect.y >= GetScreenHeight() - ball.rect.height) {
      ball.SetVelocityY(-200);
    }

    BeginDrawing();
    ClearBackground(WHITE);

    playerPaddle.Draw();
    opponentPaddle.Draw();
    ball.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
