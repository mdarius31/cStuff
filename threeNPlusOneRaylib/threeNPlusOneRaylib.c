#include "../lib/raylib/src/raylib.h"
#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdbool.h>


int main(void) {
 InitWindow(800, 600, "darius test");
 SetExitKey(KEY_NULL);

 while(!WindowShouldClose()) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  EndDrawing();
 }
 CloseWindow();
 return 0;
}
