#include "../lib/raylib/src/raylib.h"
#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXINTSTRLEN (unsigned int)snprintf(NULL, 0, "%u", UINT_MAX)

char* defaultInput() {
 char* input = malloc(sizeof(char*));
 input[0] = '\0';

 return input;
}

bool isKeyInt(KeyboardKey key) {
  return key >= KEY_ZERO && key <= KEY_NINE;
}

int main(void) {
 SetTraceLogLevel(LOG_NONE);

 InitWindow(800, 600, "Three N Plus One");
 // SetExitKey(KEY_NULL);

 char* input = defaultInput();
 unsigned int* threeNPlusOneArr = NULL;

 while(!WindowShouldClose()) {
  int keyPressed = GetKeyPressed();
  const unsigned int inputLength = strlen(input);

  if(inputLength < MAXINTSTRLEN &&
   isKeyInt(keyPressed) &&
   (keyPressed != KEY_ZERO || inputLength != 0))  {

    char* tempInput = realloc(input, sizeof(char*) * (inputLength + 1));
    if(tempInput != NULL) {
      input = tempInput;
      input[inputLength] = keyPressed;
      input[inputLength + 1] = '\0';
    }
  }

  if(keyPressed == KEY_BACKSPACE) {

   if(inputLength > 1){
     char *tempInput = realloc(input, sizeof(char*) * (inputLength - 1));

     if(tempInput != NULL) {
      input = tempInput;
      input[inputLength - 1] = '\0';
     }
   } else input = defaultInput();

  }

  if(keyPressed == KEY_ENTER) {
   threeNPlusOneArr = genThreeNPlusOne(atoi(input));

   for(unsigned int i = 0; threeNPlusOneArr[i] != '\0'; i++) {printf("%u ", threeNPlusOneArr[i]);}
   printf("\n\n");

   input = defaultInput();
  }

  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawText(input, 40, 40, 40, BLACK);


  EndDrawing();
 }
 CloseWindow();

 free(input);
 free(threeNPlusOneArr);
 return 0;
}
