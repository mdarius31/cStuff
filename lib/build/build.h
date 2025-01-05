#ifndef BUILD_SYSTEM
#define BUILD_SYSTEM

#ifdef __cplusplus
extern "C" {
#endif

struct Target {
  char *name;
  struct Target *dependencies;
  char **cFlags;

} Target;

int build(struct Target *t);

#ifdef __cplusplus
}
#endif

// BUILD_SYSTEM
#endif

#ifdef BUILD_SYSTEM_IMPLEMENTATION

// in case you hate the standard libraries
#ifndef BUILD_SYSTEM_IMPLEMENTATION_DONT_USE_STDLIB

#include <stdio.h>
#include <stdlib.h>

// BUILD_SYSTEM_IMPLEMENTATION_DONT_USE_STDLIB
#endif

// used to ignore the unused parameter warnings
#define UNUSED(x) (void)(x)

int build(struct Target *t) {

  printf("%s\n", t->name);

  unsigned int depsLen = sizeof(t->dependencies) / sizeof(t->dependencies[0]);

  // for (unsigned int i = 0; i < depsLen; i++) {
  //   printf("%s ", t->dependencies[i].name);
  // }
  printf("\n");
  for (unsigned int i = 0; t->cFlags[i] != NULL; i++) {
    printf("%s ", t->cFlags[i]);
  }

  system("cc -c -o threeNPlusOne.o -m32 -Wextra -Wall -Werror -ansi --std=c99 "
         "../lib/threeNPlusOne/threeNPlusOne.c");
  system("cc -o threeNPlusOne.x86 -m32 -Wextra -Wall -Werror -ansi --std=c99 "
         "threeNPlusOne.c threeNPlusOne.o");
  printf("\nbuild complete\n");
  return 0;
}

// BUILD_SYSTEM_IMPLEMENTATION
#endif
