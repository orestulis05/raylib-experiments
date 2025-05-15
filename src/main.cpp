#include "utils.h"
#include <raylib/raylib.h>
#include <vector>

typedef struct Sprite {
  Texture2D texture;
  Rectangle rect;
  Vector2 velocity;
} Sprite;

void CheckCollisionsY(Sprite *sprite, std::vector<Sprite> &tiles) {
  for (const auto &tile : tiles) {
    if (CheckCollisionRecs(sprite->rect, tile.rect)) {
      if (sprite->rect.y > tile.rect.y) { // Colliding top
        sprite->rect.y = tile.rect.y + tile.rect.height;
        sprite->velocity.y = 0;
      } else { // Colliding bottom
        sprite->rect.y = tile.rect.y - sprite->rect.height;
      }
    }
  }
}

void CheckCollisionsX(Sprite *sprite, std::vector<Sprite> &tiles) {
  for (const auto &tile : tiles) {
    if (CheckCollisionRecs(sprite->rect, tile.rect)) {
      if (sprite->rect.x > tile.rect.x) { // Colliding left
        sprite->rect.x = tile.rect.x + tile.rect.width;
      } else { // Colliding right
        sprite->rect.x = tile.rect.x - sprite->rect.width;
      }
    }
  }
}

int main() {
  // Initialize application
  InitWindow(1600, 900, "Simple Pong");
  SetTargetFPS(60);

  Texture2D texture_linux_bg = LoadTexture("assets/textures/linux.jpg");

  Sprite player = (Sprite){.texture = texture_linux_bg,
                           .rect = (Rectangle){
                               .x = 10.0,
                               .y = 10.0,
                               .width = 64,
                               .height = 64,
                           }};

  // Run
  while (!WindowShouldClose()) {

    BeginDrawing();

    // NOTE: All drawing happens here
    ClearBackground(SKYBLUE);

    DrawTexturePro(player.texture,
                   {0, 0, static_cast<float>(player.texture.width),
                    static_cast<float>(player.texture.height)},
                   player.rect, {0, 0}, 0.0, BLACK);

    EndDrawing();
  }

  UnloadTexture(texture_linux_bg);
  CloseWindow();
  return 0;
}
