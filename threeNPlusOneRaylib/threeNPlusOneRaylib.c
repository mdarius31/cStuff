#include "../lib/raylib/src/raylib.h"
#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "../lib/common/common.h"

#define MAXINTSTRLEN (unsigned int)snprintf(NULL, 0, "%u", UINT_MAX)

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

float getValByPerc(float perc, float whole) {
 return (perc / 100) * whole;
}

float getPercByVal(float val, float whole){
 return (val * 100) / whole;
}


float getValByPercBetweenMinNMax(float perc, float min, float max) {
 return min + getValByPerc(perc, max - min);
}

// PRIMITIVE GLOBAL CONSTANTS
const int fontSize = 40;
const int graphFontSize = 30;
const int margin = 10;
const int instructsFontSize = 20;
const char* instructsForNumsToggle = "Press N to toggle graph nums";

// PRIMITVE GLOBALS
int windowWidth = 0;
int graphHeight = 0;
int graphWidth = 0;
unsigned int threeNPlusOneMax = 0;
bool showGraphNums = true;


void calculateGlobals() {
 windowWidth = GetRenderWidth();
 graphHeight = GetRenderHeight() - margin;
 graphWidth = windowWidth  - margin;

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

 // CONSTANTS
 const int instructsForNumsToggleTextSize = MeasureText(instructsForNumsToggle, instructsFontSize);
 const int graphXPos = margin;
 const int graphYPos = fontSize + margin;
 const float graphLineWidth = 2.0;

 calculateGlobals();

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
       threeNPlusOneMax = 0;
       for(unsigned int i = 0; i < threeNPlusOneArr->len; i++) {
        if(threeNPlusOneArr->arr[i] > threeNPlusOneMax) threeNPlusOneMax = threeNPlusOneArr->arr[i];
       }
       threeNPlusOneStrArr = intArrToStrArr((int *)(threeNPlusOneArr->arr), threeNPlusOneArr->len);

       input = emptyStr();

     }
  }

  if(keyPressed == KEY_Q || keyPressed == KEY_ESCAPE) shouldClose = true;
  if(keyPressed == KEY_N) showGraphNums = !showGraphNums;

  if(IsWindowResized()) calculateGlobals();

  BeginDrawing();
  ClearBackground(GRAY);

  DrawText(input, margin, margin, fontSize, LIGHTGRAY);


  DrawLineEx((Vector2){graphXPos,graphYPos}, (Vector2){graphXPos, graphHeight}, graphLineWidth, LIGHTGRAY);
  DrawLineEx((Vector2){graphXPos, graphHeight}, (Vector2){graphWidth, graphHeight}, graphLineWidth, LIGHTGRAY);

  if(threeNPlusOneArr != NULL) {
   DrawText(threeNPlusOneStrArr[0], margin * 2,  fontSize + margin, fontSize, BLUE);
   DrawText(instructsForNumsToggle, windowWidth - instructsForNumsToggleTextSize - margin, margin, instructsFontSize, BLACK);
   float x = margin;
   float y = graphHeight;

   for(unsigned int i = 0; i < threeNPlusOneArr->len; i++) {


      float x2 = getValByPercBetweenMinNMax(getPercByVal(i + 1, threeNPlusOneArr->len), graphXPos, graphWidth);


      float percOfCurrentValueOfThreeNPlusOne = 100 - getPercByVal(threeNPlusOneArr->arr[i], threeNPlusOneMax);
      float y2 = getValByPercBetweenMinNMax(percOfCurrentValueOfThreeNPlusOne, graphYPos, graphHeight);

      DrawLineEx((Vector2){x,y}, (Vector2){x2,y2}, graphLineWidth, RED);

      x = x2;
      y = y2;
      const int textWidth = MeasureText(threeNPlusOneStrArr[i], graphFontSize);

      if(showGraphNums) DrawText(threeNPlusOneStrArr[i], x2 - textWidth / 2, y2 - graphFontSize / 2, graphFontSize, LIGHTGRAY);


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
