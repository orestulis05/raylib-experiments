#include "utils.h"
#include <raylib/raylib.h>
#include <vector>

const float cameraTransitionWeight = 0.4;
const float cameraLookupDist = 100.0f;

// clang-format off
const int level[] = {
  0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 0, 0, 0, 0, 1,
  1, 1, 1, 0, 0, 1, 1, 1,
  1, 1, 1, 1, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
};
// clang-format on

typedef struct Sprite {
  Texture2D texture;
  Rectangle rect;
  Vector2 velocity;
  bool should_jump;
} Sprite;

void MovePlayer(Sprite *player) {
  player->velocity.x = 0;

  // Horizontal movement
  if (IsKeyDown(KEY_D)) {
    player->velocity.x = 250.0;
  } else if (IsKeyDown(KEY_A)) {
    player->velocity.x = -250.0;
  }

  // Jumping
  if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) {
    player->should_jump = true;
  }
}

void ApplyGravity(Sprite *sprite) {
  sprite->velocity.y += 15;
  if (sprite->velocity.y > 500.0) {
    sprite->velocity.y = 500.0;
  }
}

void ApplyVelocityX(Sprite *sprite) {
  sprite->rect.x += sprite->velocity.x * GetFrameTime();
}

void ApplyVelocityY(Sprite *sprite) {
  sprite->rect.y += sprite->velocity.y * GetFrameTime();
}

void CheckCollisionsY(Sprite *sprite, std::vector<Sprite> &tiles) {
  for (const auto &tile : tiles) {
    if (CheckCollisionRecs(sprite->rect, tile.rect)) {
      if (sprite->rect.y > tile.rect.y) { // Colliding top
        sprite->rect.y = tile.rect.y + tile.rect.height;
        sprite->velocity.y = 0;
      } else { // Colliding bottom
        sprite->rect.y = tile.rect.y - sprite->rect.height;
        sprite->velocity.y = 0;

        if (sprite->should_jump) { // Jump
          sprite->velocity.y = -450.0f;
          sprite->should_jump = false;
        }
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

std::vector<Sprite> LoadLevel(Texture2D temp_tex) {
  std::vector<Sprite> sprites;

  const int w = 8;
  const int h = 6;

  for (size_t i = 0; i < w * h; i++) {
    size_t x = i % w;
    size_t y = i / w;

    if (level[i] > 0) {
      sprites.push_back((Sprite){.texture = temp_tex,
                                 .rect = (Rectangle){
                                     .x = x * 64.0f,
                                     .y = y * 64.0f,
                                     .width = 64.0f,
                                     .height = 64.0f,
                                 }});
    }
  }
  return sprites;
}

void CameraLookahead(Camera2D *camera, Sprite &player,
                     Vector2 &cameraLookaheadOffset) {

  if (IsKeyPressed(KEY_RIGHT)) {
    cameraLookaheadOffset.x = -cameraLookupDist;
  } else if (IsKeyPressed(KEY_LEFT)) {
    cameraLookaheadOffset.x = cameraLookupDist;
  }

  if (IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT)) {
    cameraLookaheadOffset.x = 0;
  }

  camera->offset.x = Util_LerpFloat(camera->offset.x, cameraLookaheadOffset.x,
                                    cameraTransitionWeight);
}

void CameraZoom(Camera2D *camera, float &cameraZoom) {
  if (IsKeyPressed(KEY_UP)) {
    cameraZoom += 0.1;
  } else if (IsKeyPressed(KEY_DOWN)) {
    cameraZoom -= 0.1;
  }

  camera->zoom = Util_LerpFloat(camera->zoom, cameraZoom, 0.4);
}

int main() {
  // Initialize application
  InitWindow(1600, 900, "Linux Kills Windows");
  SetTargetFPS(60);

  Texture2D texture_linux_bg = LoadTexture("assets/textures/linux.jpg");

  Sprite player = (Sprite){.texture = texture_linux_bg,
                           .rect = (Rectangle){
                               .x = 10.0,
                               .y = 10.0,
                               .width = 64,
                               .height = 64,
                           }};

  std::vector<Sprite> level_tiles = LoadLevel(texture_linux_bg);

  Camera2D camera = (Camera2D){
      .offset = {0},
      .rotation = 0,
      .zoom = 1.0f,
  };
  Vector2 cameraLookaheadOffset = {0, 0};
  float cameraZoom = 1.0f;

  // Run
  while (!WindowShouldClose()) {

    // NOTE: Update logic happens here
    MovePlayer(&player);
    ApplyGravity(&player);

    ApplyVelocityY(&player);
    CheckCollisionsY(&player, level_tiles);

    ApplyVelocityX(&player);
    CheckCollisionsX(&player, level_tiles);

    // make the camera lock onto player
    camera.target = (Vector2){
        .x = player.rect.x - GetScreenWidth() / 2,
        .y = player.rect.y - GetScreenHeight() / 2,
    };

    CameraLookahead(&camera, player, cameraLookaheadOffset);
    CameraZoom(&camera, cameraZoom);

    // FIX: temporary constraint on player so it would not fall off the
    // screen
    if (player.rect.y > GetScreenHeight() - player.rect.height) {
      player.rect.y = GetScreenHeight() - player.rect.height;
    }

    BeginDrawing();

    // NOTE: All drawing happens here
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);

    for (const auto &tile : level_tiles) {
      DrawTexturePro(tile.texture,
                     {0, 0, static_cast<float>(tile.texture.width),
                      static_cast<float>(tile.texture.height)},
                     tile.rect, {0, 0}, 0.0, BLACK);
    }

    DrawTexturePro(player.texture,
                   {0, 0, static_cast<float>(player.texture.width),
                    static_cast<float>(player.texture.height)},
                   player.rect, {0, 0}, 0.0, WHITE);

    EndDrawing();
  }

  UnloadTexture(texture_linux_bg);
  CloseWindow();
  return 0;
}
