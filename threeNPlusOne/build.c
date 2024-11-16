#include "../lib/build/build.h"

int main(void) {
  system("rm threeNPlusOne.o threeNPlusOne.x86_64");

  system("cc -c -o threeNPlusOne.o -m32 -Wextra -Wall -Werror -ansi --std=c99 "
         "../lib/threeNPlusOne/threeNPlusOne.c");
  system("cc -o threeNPlusOne.x86_64 -m32 -Wextra -Wall -Werror -ansi "
         "--std=c99 threeNPlusOne.c threeNPlusOne.o");
  system("rm threeNPlusOne.o");
  return 0;
}
