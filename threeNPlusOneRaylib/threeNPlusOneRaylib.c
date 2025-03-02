#include "../lib/raylib/src/raylib.h"
#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXINTSTRLEN (unsigned int)snprintf(NULL, 0, "%u", UINT_MAX)

char* defaultInput() {
 char* input = malloc(sizeof(char *));
 if (input == NULL) return NULL;

 input[0] = '\0';

 return input;
}

bool isKeyInt(KeyboardKey key) {
  return key >= KEY_ZERO && key <= KEY_NINE;
}

bool shouldKeyBeUsed(KeyboardKey key, char* input) {
 return key != KEY_ZERO || strlen(input)!= 0;
}

char** intArrToStrArr(int* intArr, unsigned int len) {
 char** result = malloc(sizeof(char*) * len);
 for(unsigned int i = 0; i < len; i++) {
  result[i] = malloc(snprintf(NULL, 0, "%u", intArr[i]));
  sprintf(result[i], "%u", intArr[i]);
 }

 return result;
}

int main(void) {
 SetTraceLogLevel(LOG_NONE);

 InitWindow(800, 600, "Three N Plus One");
 // SetExitKey(KEY_NULL);

 char* input = defaultInput();
 ThreeNPlusOneArr* threeNPlusOneArr = NULL;
 char** threeNPlusOneStrArr = NULL;

 while(!WindowShouldClose()) {
  int keyPressed = GetKeyPressed();
  const unsigned int inputLength = strlen(input);

  if(inputLength < MAXINTSTRLEN &&
   isKeyInt(keyPressed) &&
   shouldKeyBeUsed(keyPressed, input))  {

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
     if(inputLength > 0){
       threeNPlusOneArr = genThreeNPlusOneStruct(atoi(input));
       threeNPlusOneStrArr = intArrToStrArr((int *)(threeNPlusOneArr->arr), threeNPlusOneArr->len);
       printf("len: %u for num: %u\n", threeNPlusOneArr->len, threeNPlusOneArr->arr[0]);


       input = defaultInput();

     }
  }

  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawText(input, 40, 40, 40, BLACK);
  if(threeNPlusOneStrArr != NULL){
    for(unsigned int i = 0; i < threeNPlusOneArr->len; i++){
     DrawText(threeNPlusOneStrArr[i], 40 + (40 * i), 80, 40, BLACK);
    }
  }
  EndDrawing();
 }
 CloseWindow();

 free(input);
 free(threeNPlusOneArr);
 free(threeNPlusOneStrArr);
 return 0;
}
