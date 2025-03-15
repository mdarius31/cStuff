#include "../lib/raylib/src/raylib.h"
#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAXINTSTRLEN (unsigned int)snprintf(NULL, 0, "%u", UINT_MAX)
#define UNUSED(x) (void)(x)

static char* emptyStr() {
  char* str = malloc(sizeof(char));
  if (str == NULL) return str;

  str[0] = '\0';

  return str;
}

static bool isKeyInt(KeyboardKey key) {
  return key >= KEY_ZERO && key <= KEY_NINE;
}

static bool shouldKeyBeUsed(KeyboardKey key, char* input) {
 return key != KEY_ZERO || strlen(input)!= 0;
}

static char** intArrToStrArr(int* intArr, unsigned int len) {
 char** result = malloc(sizeof(char*) * len);
 for(unsigned int i = 0; i < len; i++) {
  const char* template = "%u";
  result[i] = malloc(snprintf(NULL, 0, template, (int)intArr[i]));
  sprintf(result[i], template, intArr[i]);
 }

 return result;
}

char* strArrToStr(char** chrArr, unsigned int len) {
 UNUSED(chrArr);
 UNUSED(len);

 return NULL;
}


static void addChrToStr(char* str, char chr) {
    const unsigned int len = strlen(str);
    char* tempStr = realloc(str, sizeof(char) * (len + 1));

    if(tempStr != NULL) {
      str = tempStr;
      str[len] = chr;
      str[len + 1] = '\0';
    }
}

static void rmLastChrFromStr(char* str) {
 if(str == NULL) return;

 const unsigned int len = strlen(str);

 if(len >= 1){
   char *tempStr= realloc(str, sizeof(char) * len);

   if(tempStr != NULL) {
    str = tempStr;
    str[len - 1] = '\0';
   }
 } else str = emptyStr();

}

int main(void) {
 SetTraceLogLevel(LOG_NONE);

 InitWindow(800, 600, "Three N Plus One");
 SetWindowState(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

 SetExitKey(KEY_NULL);

 bool shouldClose = false;

 char* input = emptyStr();
 ThreeNPlusOneArr* threeNPlusOneArr = NULL;
 char** threeNPlusOneStrArr = NULL;

 const int fontSize = 40;
 const int spaceBetweenNums = 15;
 const int startX = 10;
 const int startY = 10;

 while(!(WindowShouldClose() || shouldClose)) {
  int keyPressed = GetKeyPressed();
  const unsigned int inputLength = strlen(input);

  if(inputLength < MAXINTSTRLEN &&
     isKeyInt(keyPressed) &&
     shouldKeyBeUsed(keyPressed, input)) addChrToStr(input, keyPressed);



  if(keyPressed == KEY_BACKSPACE) rmLastChrFromStr(input);

  if(keyPressed == KEY_ENTER) {
     if(inputLength > 0){
       threeNPlusOneArr = genThreeNPlusOneStruct(atoi(input));
       threeNPlusOneStrArr = intArrToStrArr((int *)(threeNPlusOneArr->arr), threeNPlusOneArr->len);

       input = emptyStr();

     }
  }

  if(keyPressed == KEY_Q || keyPressed == KEY_ESCAPE) shouldClose = true;

  if(IsWindowResized()) {

  }

  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawText(input, startX, startY, 40, BLACK);

  if(threeNPlusOneStrArr != NULL){
    int x = startX;
    int y = startY + fontSize;

    for(unsigned int i = 0; i < threeNPlusOneArr->len; i++){


     DrawText(threeNPlusOneStrArr[i], x, y, fontSize, BLACK);

     const int textWidth = MeasureText(threeNPlusOneStrArr[i], fontSize);
     const int potX = x + textWidth;
     const int potXWidth = potX + textWidth;

     if(potXWidth >= GetRenderWidth()) {
       x = startX;
       y += fontSize;
     } else {
       x = potX;
       if(i < threeNPlusOneArr->len - 1) x += spaceBetweenNums;

     }
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
