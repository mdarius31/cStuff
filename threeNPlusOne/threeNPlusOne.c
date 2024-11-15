#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdio.h>

void printThreeNPlusOne(unsigned int *t) {
  printf("[START threeNPlusOne for %u]\n", t[0]);

  for (unsigned int i = 0; t[i] != '\0'; i++) {
    printf("%u\n", t[i]);
  }
  printf("[STOP threeNPlusOne for %u]\n", t[0]);
}

int main() {

  unsigned int *t = genThreeNPlusOne(11);

  printThreeNPlusOne(t);

  free(t);

  return 0;
}
