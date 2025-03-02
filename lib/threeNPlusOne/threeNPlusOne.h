#pragma once

#include <stdlib.h>

#define uintSize sizeof(unsigned int)

typedef struct {
 unsigned int* arr;
 unsigned int len;
} ThreeNPlusOneArr;

unsigned int getNextThreeNPlusOne(unsigned int num);
unsigned int *genThreeNPlusOne(unsigned int num);
ThreeNPlusOneArr *genThreeNPlusOneStruct(unsigned int num);
