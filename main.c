#include <math.h>
#include "raylib.h"
#include "header/headers.h"

char joinedTeam = 0;

int main() {
	// Initialization
	// --------------------------------------
	int screenWidth = 1280;
	int screenHeight = 832;
	InitWindow(screenWidth, screenHeight, "TankIT");
	InitializeMapObjects();
	SetTargetFPS(30);
	//----------------------------------
	//Main game loop
	while(!WindowShouldClose()){	//Detect window close button or ESC key
		if(joinedTeam == 0){
			DrawMenu();
		} else {
			DrawMap();
			// DrawSprite();
		}
		if (IsKeyPressed(KEY_ONE)) joinedTeam = 1;
		else if (IsKeyPressed(KEY_TWO)) joinedTeam = 2;
	}
	//De-Initialization
	//-----------------
	UnloadMap();
	CloseWindow();	//CLose window and OpenGL context
	//---------
	return 0;
}