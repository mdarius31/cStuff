#define BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

// recommended build process
// cc -o build.x86 -m32 -Wextra -Wall -Werror -ansi --std=c99 build.c
int main(void) {

  char *cFlags[] = {"-m32",  "-Wextra",   "-Wall", "-Weerror",
                    "-ansi", "--std=c99", NULL};

  struct Target dependencies[2] = {
      {.name = "threeNPlusOne.c", .dependencies = NULL, .cFlags = cFlags},
      {.name = "threeNPlusOne.o", .dependencies = NULL, .cFlags = cFlags},
  };

  struct Target t = {.name = "threeNPlusOne.x86",
                     .dependencies = dependencies,
                     .cFlags = cFlags};

  int result = build(&t);

  return result;
}
