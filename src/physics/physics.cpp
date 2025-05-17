#include "physics.h"
#include <cmath>
#include <cstddef>
#include <raylib/raylib.h>
#include <vector>

void BouncyBall::Draw() { DrawCircle(position.x, position.y, radius, color); }
void BouncyBall::Move() {

  velocity.y += GRAVITY_CONSTANT * GetFrameTime();

  position.x += velocity.x * GetFrameTime();
  position.y += velocity.y * GetFrameTime();

  if (position.x + radius > GetScreenWidth()) {
    velocity.x = -velocity.x * bounciness;
    position.x = GetScreenWidth() - radius;
  } else if (position.x - radius < 0) {
    velocity.x = -velocity.x * bounciness;
    position.x = radius;
  }

  if (position.y + radius > GetScreenHeight()) {
    velocity.y = -velocity.y * bounciness;
    position.y = GetScreenHeight() - radius;
  } else if (position.y - radius < 0) {
    velocity.y = -velocity.y * bounciness;
    position.y = radius;
  }

  // Velocity cap (because of the buggy collision handling)
  float maxSpeed = 500.f;
  float currentSpeed =
      std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
  if (currentSpeed > maxSpeed) {
    float scale = maxSpeed / currentSpeed;
    velocity.x *= scale;
    velocity.y *= scale;
  }

  // Some damping
  float damping = (bounciness < 0.1f) ? 0.95f : 0.999f;
  velocity.x *= damping;
  velocity.y *= damping;

  // // Terminal velocity for gravity
  // float maxFallSpeed = 500.0f;
  // if (velocity.y > maxFallSpeed)
  //   velocity.y = maxFallSpeed;
}

void _HandleOneCollision(BouncyBall &ball1, BouncyBall &ball2) {
  float distance = std::sqrt(std::pow(ball2.position.x - ball1.position.x, 2) +
                             std::pow(ball2.position.y - ball1.position.y, 2));

  if (distance < 0.0001f)
    return;

  Vector2 direction = {(ball1.position.x - ball2.position.x) / distance,
                       (ball1.position.y - ball2.position.y) / distance};
  // Relative velocity
  Vector2 relativeVelocity = {ball1.velocity.x - ball2.velocity.x,
                              ball1.velocity.y - ball2.velocity.y};

  float velocityAlongDirection =
      relativeVelocity.x * direction.x + relativeVelocity.y * direction.y;

  if (velocityAlongDirection > 0)
    return;

  float e = (ball1.bounciness + ball2.bounciness) / 2.0f;
  float impulse = -(1.0f + e) * velocityAlongDirection;

  ball1.velocity.x += impulse * direction.x;
  ball1.velocity.y += impulse * direction.y;
  ball2.velocity.x -= impulse * direction.x;
  ball2.velocity.y -= impulse * direction.y;

  float overlap = (ball1.radius + ball2.radius - distance) / 2.0f;
  if (overlap > 0) {
    ball1.position.x += overlap * direction.x;
    ball1.position.y += overlap * direction.y;
    ball2.position.x -= overlap * direction.x;
    ball2.position.y -= overlap * direction.y;
  }
}

void HandleBallCollisions(std::vector<BouncyBall> &balls) {

  for (size_t i = 0; i < balls.size(); ++i) {
    for (size_t j = i + 1; j < balls.size(); ++j) {
      bool collision =
          CheckCollisionCircles(balls[i].position, balls[i].radius,
                                balls[j].position, balls[j].radius);

      if (collision) {
        _HandleOneCollision(balls[i], balls[j]);
      }
    }
  }
}
