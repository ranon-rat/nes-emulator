#include <iostream>
#include <raylib/raylib.h>

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for(int i=0;i<10;i++){
        DrawText("It works!", 20+i*20, 20*i, 20, BLACK);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}