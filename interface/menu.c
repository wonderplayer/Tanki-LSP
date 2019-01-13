#include "raylib.h"

// Draws game menu
void DrawMenu(){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("TANKIT", 300, 50, 50, BLACK);
        DrawText("1. Join Team One", 20, 100, 20, BLACK);
        DrawText("2. Join Team Two", 20, 120, 20, BLACK);
    EndDrawing();
    return;
}