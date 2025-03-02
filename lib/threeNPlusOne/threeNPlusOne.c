#include "threeNPlusOne.h"

unsigned int getNextThreeNPlusOne(unsigned int num) {
  return num % 2 == 0 ? num / 2 : (3 * num) + 1;
}

unsigned int *genThreeNPlusOne(unsigned int num) {

  unsigned int size = uintSize * 2;
  unsigned int *out = malloc(size);
  unsigned int index = (size / uintSize) - 1;

  out[0] = num;
  out[1] = '\0';

  if (num <= 1) {
    return out;
  }

  unsigned int i = getNextThreeNPlusOne(num);

  for (; i != 1; i = getNextThreeNPlusOne(i)) {
    out[index] = i;

    size = size + uintSize;
    index = (size / uintSize) - 1;

    out = realloc(out, size);
  }

  out = realloc(out, size + uintSize);
  out[index] = 1;
  out[index + 1] = '\0';

  return out;
}


ThreeNPlusOneArr *genThreeNPlusOneStruct(unsigned int num) {

  ThreeNPlusOneArr *result = malloc(sizeof(ThreeNPlusOneArr));

  unsigned int size = uintSize;


  result->arr = malloc(size);
  result->arr[0] = num;
  result->len = 1;


  if (num <= 1) {
    return result;
  }

  unsigned int i = getNextThreeNPlusOne(num);

  for (; i != 1; i = getNextThreeNPlusOne(i)) {
    size += uintSize;

    result->arr = realloc(result->arr,size);

    result->arr[result->len] = i;

    result->len += 1;

  }

  size += uintSize;
  result->arr = realloc(result->arr, size);
  result->arr[result->len] = 1;
  result->len += 1;

  return result;
}
