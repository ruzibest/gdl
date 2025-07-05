/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera mode
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <chrono>
#include <cmath>

Vector3 turnToAngle(Vector3 posToTurn, Vector3 posCenter, int degrees) {
  float radians = (degrees % 360) * (PI / 180.0f);

  // Translate to origin
  float dx = posToTurn.x - posCenter.x;
  float dz = posToTurn.z - posCenter.z;

  // Rotate in XZ plane
  float rotatedX = dx * cos(radians) - dz * sin(radians);
  float rotatedZ = dx * sin(radians) + dz * cos(radians);

  // Translate back
  Vector3 result;
  result.x = rotatedX + posCenter.x;
  result.y = posToTurn.y; // Y remains unchanged
  result.z = rotatedZ + posCenter.z;
  return result;
}

float normalizeAngle(float angle) {
  while (angle > 180.0f) angle -= 360.0f;
  while (angle < -180.0f) angle += 360.0f;
  return angle;
}

void updateRotation(float& playerRotationCurrent, float lastPlayerDirection, float deltaTime) {
  float turnSpeed = 0.5f; // degrees per millisecond or your speed unit
  float diff = normalizeAngle(lastPlayerDirection - playerRotationCurrent);

  if (fabs(diff) < 0.01f) {  // close enough
    playerRotationCurrent = lastPlayerDirection;
    return;
  }

  float turnStep = turnSpeed * deltaTime;

  if (diff > 0) {
    // turn clockwise
    playerRotationCurrent += (diff < turnStep) ? diff : turnStep;
  }
  else {
    // turn counterclockwise
    playerRotationCurrent += (diff > -turnStep) ? diff : -turnStep;
  }

  // Keep current rotation normalized too (optional)
  playerRotationCurrent = normalizeAngle(playerRotationCurrent);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1280;
  const int screenHeight = 720;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

  // Define the camera to look into our 3d world
  Camera3D camera = { 0 };
  camera.position = Vector3{ 0.0f, 25.0f, 5.0f };  // Camera position
  camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                                // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

  Vector3 playerPosition = { 0.0f, 2.0f, 0.0f };

  float playerDirection = -1;
  float lastPlayerDirection = 180;
  float playerRotationCurrent = 180;
  float jump = 0.f;
  float gravity = 1.f;
  float currentAddedHeight = 0.f;

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  auto then = std::chrono::steady_clock::now();

  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    auto now = std::chrono::steady_clock::now();
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
    then = now;
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    playerDirection = -1;

    if(IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) { playerDirection = 45; }
    else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S)) { playerDirection = 135; }
    else if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) { playerDirection = 225; }
    else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W)) { playerDirection = 315; }
    else if(IsKeyDown(KEY_W)) { playerDirection = 0; }
    else if (IsKeyDown(KEY_D)) { playerDirection = 90; }
    else if(IsKeyDown(KEY_S)) { playerDirection = 180; }
    else if (IsKeyDown(KEY_A)) { playerDirection = 270; }

    if (IsKeyPressed(KEY_SPACE) && currentAddedHeight == 0.f)
    {
      jump = 3.f;
    }

    float addHeight = 0.01f * jump * milliseconds;
    float reduceHeight = 0.01f * gravity * milliseconds;
    if (jump > 0) { jump -= 0.01f * milliseconds; }
    currentAddedHeight += addHeight - reduceHeight;
    if (currentAddedHeight < 0) { currentAddedHeight = 0.f; }

    if (playerDirection >= 0)
    {
      lastPlayerDirection = playerDirection;
      float radians = (PI / 180) * (playerDirection - 90);
      playerPosition.x += 0.01f * milliseconds * cos(radians);
      playerPosition.z += 0.01f * milliseconds * sin(radians);
    }

    camera.position.x = playerPosition.x;
    camera.position.z = playerPosition.z + 15.f;
    camera.target = playerPosition;

    BeginMode3D(camera);

    DrawCapsule({ playerPosition.x, playerPosition.y + currentAddedHeight, playerPosition.z },
      { playerPosition.x, playerPosition.y + 1.5f + currentAddedHeight, playerPosition.z }, 2.f, 16, 4, BEIGE);

    DrawGrid(300, 2.0f);

    EndMode3D();

    DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

    DrawFPS(10, 10);
  
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}