#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define OBJECT_HEIGHT 16
#define OBJECT_WIDTH 16
#define MAP_DIMENSION 13
#define MAP_OBJECT_SPRITE_OFFSET 256
#define ENLARGED_OBJECT_SIZE 40

int *map;
Texture2D sprite;
Vector2 spriteOrigin;
Rectangle spriteTankRec;
Rectangle mapRec;
Rectangle tank;
Rectangle spriteMapRec;
int spriteOffset = 0;
int currentFrame = 0;
int mapSize;

void MoveTank(float movementX, float movementY){
    currentFrame++;
    if(currentFrame > spriteOffset + 1) currentFrame = spriteOffset;
    spriteTankRec.x = (float)currentFrame*(float)OBJECT_WIDTH;
    tank.x += movementX;
    tank.y += movementY;
    return;
}
// Draws game menu
void DrawMap(){
    if(IsKeyPressed(KEY_D)){
        spriteOffset = 6;
        currentFrame = spriteOffset;
    }
    if(IsKeyPressed(KEY_A)){
        spriteOffset = 2;
        currentFrame = spriteOffset;
    }
    if(IsKeyPressed(KEY_W)){
        spriteOffset = 0;
        currentFrame = spriteOffset;
    }
    if(IsKeyPressed(KEY_S)){
        spriteOffset = 4;
        currentFrame = spriteOffset;
    }

    if(IsKeyDown(KEY_D)) MoveTank(2.0f, 0.0f);
    if(IsKeyDown(KEY_A)) MoveTank(-2.0f, 0.0f);
    if(IsKeyDown(KEY_W)) MoveTank(0.0f, -2.0f);
    if(IsKeyDown(KEY_S)) MoveTank(0.0f, 2.0f);

    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
/*
        DrawTexture(sprite, 10, 10, WHITE);
        DrawRectangleLines(10, 10, OBJECT_WIDTH, OBJECT_HEIGHT, LIME);
        DrawRectangleLines(10 + spriteTankRec.x, 10 + spriteTankRec.y, spriteTankRec.width, spriteTankRec.height, RED);
*/
        mapRec.x = (float)ENLARGED_OBJECT_SIZE;
        mapRec.y = 0.0f;
        for(int i = 0; i < mapSize; i++)
        {
            if(i % 13 == 0){
                mapRec.y += (float)ENLARGED_OBJECT_SIZE;
                mapRec.x = (float)ENLARGED_OBJECT_SIZE;
            }
            switch (map[i] / 10)
            {
                case 0: // Space
                    spriteMapRec.x = (float)(MAP_OBJECT_SPRITE_OFFSET + 6 * OBJECT_WIDTH);
                    spriteMapRec.y = 0.0f;
                    break;
                case 1: // Eagle
                case 2:
                    spriteMapRec.x = (float)(MAP_OBJECT_SPRITE_OFFSET + 3 * OBJECT_WIDTH);
                    spriteMapRec.y = OBJECT_HEIGHT * 2;
                    break;
                case 3: // Weak wall
                    spriteMapRec.x = (float)MAP_OBJECT_SPRITE_OFFSET;
                    spriteMapRec.y = 0.0f;
                    break;
                case 4: // Strong wall
                    spriteMapRec.x = (float)MAP_OBJECT_SPRITE_OFFSET;
                    spriteMapRec.y = OBJECT_HEIGHT;
                    break;
                case 5: // River
                    spriteMapRec.x = (float)MAP_OBJECT_SPRITE_OFFSET;
                    spriteMapRec.y = OBJECT_HEIGHT * 2;
                    break;
                case 6: // Bush
                    spriteMapRec.x = (float)MAP_OBJECT_SPRITE_OFFSET + OBJECT_WIDTH;
                    spriteMapRec.y = OBJECT_HEIGHT * 2;
                    break;
                default: // Space
                    spriteMapRec.x = (float)(MAP_OBJECT_SPRITE_OFFSET + 6 * OBJECT_WIDTH);
                    spriteMapRec.y = 0.0f;
                    break;
            }
            DrawTexturePro(sprite, spriteMapRec, mapRec, spriteOrigin, 0.0f, WHITE);
            DrawText(FormatText("%d", i),mapRec.x, mapRec.y, 10, BLUE);
            mapRec.x = (float)(mapRec.x + ENLARGED_OBJECT_SIZE);
            
        }
        DrawTexturePro(sprite, spriteTankRec, tank, spriteOrigin, 0.0f, WHITE);
        
    EndDrawing();
    return;
}

void DrawSprite(){
    BeginDrawing();
        DrawTexture(sprite, 10, 40, WHITE);
    EndDrawing();
}

void CreateMap(int x, int y){
    map = malloc(sizeof(int)*x*y);
    int tempMap[MAP_DIMENSION * MAP_DIMENSION] = {
        [0 ... 2] = 0, 40, [4 ... 6] = 0, 40, 10, [8 ... 12] = 0,
        0, 30, 0, 40, [17 ... 19] = 0, 31, 0, 32, 0, 33, 0,
        0, 30, [28 ... 31] = 0, 30, 30, 0, 30, 41, 31, 0,
        [39 ... 41] = 0, 31, [43 ... 47] = 0, 40, [49 ... 51] = 0,
        60, 0, 0, 30, 0, 0, 40, 0, 0, 30, 60, 30, 40,
        60, 60, [67 ... 69] = 0, 30, 0, 0, 40, 0, 60, 0, 0,
        0, [79 ... 81] = 30, [82 ... 84] = 60, 40, 0, 0, 60, 30, 0,
        [91 ... 93] = 0, 40, 60, 30, 0, 30, 0, 30, 0, 30, 0,
        40, 30, 0, 40, 0, 30, 0, 30, [112 ... 114] = 0, 30, 0,
        0, 30, 0, 30, 0, [122 ... 124] = 30, 0, 30, 40, 30, 0,
        0, 30, 0, 30, 0, [135 ... 137] = 30, [138 ... 142] = 0,
        0, 30, [145 ... 151] = 0, 30, 0, 30, 0,
        0, 30, 0, 30, 0, 0, 20, 0, 0, 30, 30, 30, 0
    };
    
    for(int i = 0; i < x*y; i++) map[i] = tempMap[i];
    return;
}

//Initializes sprites and game objects
void InitializeMapObjects(){
    sprite = LoadTexture("resources/Sprites.png");
    Vector2 origin = {(float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteOrigin = origin;
    Rectangle rec = {0.0f, 0.0f, (float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteTankRec = rec;
    Rectangle rec2 = {600.0f, 100.0f, (float)ENLARGED_OBJECT_SIZE, (float)ENLARGED_OBJECT_SIZE};
    tank = rec2;
    Rectangle rec3 = {10.0f, 0.0f, (float)ENLARGED_OBJECT_SIZE, (float)ENLARGED_OBJECT_SIZE};
    mapRec = rec3;
    Rectangle rec4 = {(float)MAP_OBJECT_SPRITE_OFFSET, 0.0f, (float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteMapRec = rec4;
    mapSize = MAP_DIMENSION * MAP_DIMENSION;
    CreateMap(16, 16);
    return;
}

void UnloadMap(){
    free(map);
    UnloadTexture(sprite);
    return;
}

