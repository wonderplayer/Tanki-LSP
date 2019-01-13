#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBJECT_HEIGHT 16
#define OBJECT_WIDTH 16
#define MAP_DIMENSION 13
#define MAP_OBJECT_SPRITE_OFFSET 256
#define ENLARGED_OBJECT_SIZE 64
#define TANK_SPEED 4

typedef struct MapObject{
    int id;
    int state;
    int x;
    int y;
} MapObject;

MapObject *mapObjects;
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
int mapObjectCount;
char *log_text;


void MoveTank(float movementX, float movementY); // Moves tank to the right direction
bool CanMoveThere(float x, float y); // Determines, if tank cam move to the desired place
void CreateMap(); // Creates map array (will have to update to create map from package)
void InitializeObjects(); // Initializes all global file variables and object states
void ParseMapObjects();

//Initializes sprites and game objects
void InitializeMapObjects(){
    log_text = malloc(1024);
    strcpy(log_text, "\0");
    sprite = LoadTexture("resources/Sprites.png");
    Vector2 origin = {(float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteOrigin = origin;
    Rectangle rec = {0.0f, 0.0f, (float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteTankRec = rec;
    Rectangle rec2 = {(float)OBJECT_WIDTH, (float)OBJECT_HEIGHT, (float)ENLARGED_OBJECT_SIZE, (float)ENLARGED_OBJECT_SIZE};
    tank = rec2;
    Rectangle rec3 = {10.0f, 0.0f, (float)ENLARGED_OBJECT_SIZE, (float)ENLARGED_OBJECT_SIZE};
    mapRec = rec3;
    Rectangle rec4 = {(float)MAP_OBJECT_SPRITE_OFFSET, 0.0f, (float)OBJECT_WIDTH, (float)OBJECT_HEIGHT};
    spriteMapRec = rec4;
    mapSize = MAP_DIMENSION * MAP_DIMENSION;
    CreateMap();
    ParseMapObjects();
    return;
}

// Draws game menu
void DrawMap(){
    if(IsKeyPressed(KEY_D)){
        spriteOffset = 6;
        currentFrame = spriteOffset;
    }
    else if(IsKeyPressed(KEY_A)){
        spriteOffset = 2;
        currentFrame = spriteOffset;
    }
    else if(IsKeyPressed(KEY_W)){
        spriteOffset = 0;
        currentFrame = spriteOffset;
    }
    else if(IsKeyPressed(KEY_S)){
        spriteOffset = 4;
        currentFrame = spriteOffset;
    }

    if(IsKeyDown(KEY_D)) MoveTank((float)TANK_SPEED, 0.0f); 
    else if(IsKeyDown(KEY_A)) MoveTank(-(float)TANK_SPEED, 0.0f);
    else if(IsKeyDown(KEY_W)) MoveTank(0.0f, -(float)TANK_SPEED);
    else if(IsKeyDown(KEY_S)) MoveTank(0.0f, (float)TANK_SPEED);

    // Draw
    BeginDrawing();
        ClearBackground(BLACK);
/*
        DrawTexture(sprite, 10, 10, WHITE);
        DrawRectangleLines(10, 10, OBJECT_WIDTH, OBJECT_HEIGHT, LIME);
        DrawRectangleLines(10 + spriteTankRec.x, 10 + spriteTankRec.y, spriteTankRec.width, spriteTankRec.height, RED);
*/
        DrawTexturePro(sprite, spriteTankRec, tank, spriteOrigin, 0.0f, WHITE);
        if(IsKeyDown(KEY_D)) DrawText("D", 1000, 20, 30, BLUE);
        else if(IsKeyDown(KEY_A)) DrawText("A", 1000, 20, 30, BLUE);
        else if(IsKeyDown(KEY_W)) DrawText("W", 1000, 20, 30, BLUE);
        else if(IsKeyDown(KEY_S)) DrawText("S", 1000, 20, 30, BLUE);
        DrawText(FormatText("x: '%.0f' y: '%.0f'", tank.x, tank.y), 1000, 50, 30, BLUE);
        DrawText(log_text, 1000, 100, 30, BLUE);
        strcpy(log_text, "\0");

        for(int i = 0; i < mapObjectCount; i++)
        {
            mapRec.x = (float)(mapObjects[i].x * ENLARGED_OBJECT_SIZE + OBJECT_WIDTH);
            mapRec.y = (float)(mapObjects[i].y * ENLARGED_OBJECT_SIZE + OBJECT_HEIGHT);
            switch (mapObjects[i].id)
            {
                case 0: // Space - should never happen
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
                default: // Space - should never happen
                    spriteMapRec.x = (float)(MAP_OBJECT_SPRITE_OFFSET + 6 * OBJECT_WIDTH);
                    spriteMapRec.y = 0.0f;
                    break;
            }
            DrawTexturePro(sprite, spriteMapRec, mapRec, spriteOrigin, 0.0f, WHITE);
            DrawText(FormatText("%d", i),mapRec.x, mapRec.y, 10, BLUE);
        }        
    EndDrawing();
    return;
}

void DrawSprite(){
    BeginDrawing();
        DrawTexture(sprite, 10, 40, WHITE);
    EndDrawing();
}

bool IsTankGoingIntoObject(float x, float y, float tankX, float tankY, int mapObjectX, int mapObjectY){
    if((tankX + x + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH > mapObjectX*ENLARGED_OBJECT_SIZE 
        && tankX + x + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH < mapObjectX*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE 
        || tankX + x - OBJECT_WIDTH < mapObjectX*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE
        && tankX + x - OBJECT_WIDTH > mapObjectX*ENLARGED_OBJECT_SIZE
        )
        &&( 
            mapObjectY*ENLARGED_OBJECT_SIZE < tankY + y - OBJECT_HEIGHT
            && mapObjectY*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tankY + y - OBJECT_HEIGHT
            || mapObjectY*ENLARGED_OBJECT_SIZE < tankY + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
            && mapObjectY*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tankY + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
            || mapObjectY*ENLARGED_OBJECT_SIZE == tankY + y - OBJECT_HEIGHT
            && mapObjectY*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE == tankY + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        ))
    {
        return true;
    }
    return false;
}

bool CanMoveThere(float x, float y){
    // Screen borders
    if(tank.x + x + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH > MAP_DIMENSION*ENLARGED_OBJECT_SIZE || tank.x + x - OBJECT_WIDTH < 0.0f) return false;
    if(tank.y + y + ENLARGED_OBJECT_SIZE - OBJECT_HEIGHT > MAP_DIMENSION*ENLARGED_OBJECT_SIZE || tank.y + y - OBJECT_HEIGHT < 0.0f) return false;
    //----
    // Unpassable objects
    for(int i = 0; i < mapObjectCount; i++){
        
        if(mapObjects[i].id != 6 && IsTankGoingIntoObject(x, y, tank.x, tank.y, mapObjects[i].x, mapObjects[i].y)){
            strcat(log_text, FormatText("1: T  obj: '%d'", mapObjects[i].id));
            return false; // Right, left
        }
        if(mapObjects[i].id != 6 && IsTankGoingIntoObject(y, x, tank.y, tank.x, mapObjects[i].y, mapObjects[i].x)){
            strcat(log_text, FormatText("2: T  obj: '%d'", mapObjects[i].id));
            return false; // Top, down
        }
        // if((tank.x + x + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH > mapObjects[i].x*ENLARGED_OBJECT_SIZE 
        //     && tank.x + x + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH < mapObjects[i].x*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE 
        //     || tank.x + x - OBJECT_WIDTH < mapObjects[i].x*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE
        //     && tank.x + x - OBJECT_WIDTH > mapObjects[i].x*ENLARGED_OBJECT_SIZE
        //     )
        //     &&( 
        //         mapObjects[i].y*ENLARGED_OBJECT_SIZE < tank.y + y - OBJECT_HEIGHT
        //         && mapObjects[i].y*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tank.y + y - OBJECT_HEIGHT
        //         || mapObjects[i].y*ENLARGED_OBJECT_SIZE < tank.y + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //         && mapObjects[i].y*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tank.y + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //         || mapObjects[i].y*ENLARGED_OBJECT_SIZE == tank.y + y - OBJECT_HEIGHT
        //         && mapObjects[i].y*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE == tank.y + y - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //     ))
        // {
        //     strcat(log_text, "1: T");
        //     return false; // Right, left
        // }
        // if((tank.y + y + ENLARGED_OBJECT_SIZE - OBJECT_HEIGHT > mapObjects[i].y*ENLARGED_OBJECT_SIZE 
        //     && tank.y + y + ENLARGED_OBJECT_SIZE - OBJECT_WIDTH < mapObjects[i].y*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE 
        //     || tank.y + y - OBJECT_WIDTH < mapObjects[i].y*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE
        //     && tank.y + y - OBJECT_WIDTH > mapObjects[i].y*ENLARGED_OBJECT_SIZE
        //     )
        //     &&( 
        //         mapObjects[i].x*ENLARGED_OBJECT_SIZE < tank.x + x - OBJECT_HEIGHT
        //         && mapObjects[i].x*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tank.x + x - OBJECT_HEIGHT
        //         || mapObjects[i].x*ENLARGED_OBJECT_SIZE < tank.x + x - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //         && mapObjects[i].x*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE > tank.x + x - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //         || mapObjects[i].x*ENLARGED_OBJECT_SIZE == tank.x + x - OBJECT_HEIGHT
        //         && mapObjects[i].x*ENLARGED_OBJECT_SIZE + ENLARGED_OBJECT_SIZE == tank.x + x - OBJECT_HEIGHT + ENLARGED_OBJECT_SIZE
        //     ))
        // {
        //     strcat(log_text, "2: T");
        //     return false; // Up, down
        // }
    }
    return true;
}

void MoveTank(float movementX, float movementY){
    if(!CanMoveThere(movementX, movementY)){
        return;
    };
    currentFrame++;
    if(currentFrame > spriteOffset + 1) currentFrame = spriteOffset;
    spriteTankRec.x = (float)currentFrame*(float)OBJECT_WIDTH;
    tank.x += movementX;
    tank.y += movementY;
    return;
}

void CreateMap(){
    map = malloc(sizeof(int)*mapSize);
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
    
    for(int i = 0; i < mapSize; i++) map[i] = tempMap[i];
    return;
}

void ParseMapObjects(){
    mapObjects = malloc(sizeof(MapObject) * mapSize);
    int j=0;
    for(int i = 0; i < mapSize; i++)
    {
        if(map[i] == 0) continue;
        mapObjects[j].id = map[i]/10;
        mapObjects[j].state = map[i]%10;
        mapObjects[j].x = i%13;
        mapObjects[j].y = i/13;
        j++;
    }
    mapObjectCount = j;
    return;
}

void UnloadMap(){
    free(map);
    free(mapObjects);
    UnloadTexture(sprite);
    return;
}