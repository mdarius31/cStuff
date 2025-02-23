#include "../lib/threeNPlusOne/threeNPlusOne.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  unsigned int input = 0;

  if (argc <= 1)
    scanf("%u", &input);
  else
    input = atoi(argv[1]);

  unsigned int *t = genThreeNPlusOne(input);

  for (unsigned int i = 0; t[i] != '\0'; i++) {
    printf("%u\n", t[i]);
  }

  free(t);

  return 0;
}
