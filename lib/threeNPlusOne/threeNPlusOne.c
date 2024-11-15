#include "threeNPlusOne.h"

unsigned int getNextThreeNPlusOne(unsigned int num) {
  unsigned int out = 0;

  bool even = num % 2 == 0;

  if (even) {
    out = num / 2;
  } else {
    out = (num * 3) + 1;
  }

  return out;
}

unsigned int *genThreeNPlusOne(unsigned int num) {

  unsigned int size = uintSize * 2;
  unsigned int *out = malloc(size);
  unsigned int index = (size / uintSize) - 1;

  out[0] = num;
  out[1] = '\0';

  if (num == 1) {
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
