#ifndef INCLUDE_PHYSICS_PHYSICS_H_
#define INCLUDE_PHYSICS_PHYSICS_H_

#include <iostream>
#include <raylib/raylib.h>
#include <vector>

#define GRAVITY_CONSTANT 600.0f

class BouncyBall {
public:
  Vector2 velocity;
  float radius;
  Vector2 position;
  float bounciness;
  Color color;

public:
  void Draw();
  void Move();
  BouncyBall(float radius, Color color = BLACK, Vector2 position = {0, 0},
             float bounciness = .5f) {
    this->radius = radius;
    this->color = color;
    this->position = position;
    this->bounciness = bounciness;
  }
};

void HandleBallCollisions(std::vector<BouncyBall> &balls);

#endif // INCLUDE_PHYSICS_PHYSICS_H_
