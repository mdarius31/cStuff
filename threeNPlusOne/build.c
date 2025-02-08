#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

// recommended build process
// cc -o build.x86 -m32 -Wextra -Wall -Werror -ansi --std=c99 build.c
int main(void) {
		char *compiler = "cc";
		char *namingFlag = "-o";
  mdb_Flag flags[]= {"-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};
		mdb_Flag depFlags[] = {"-c", "-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};

		mdb_Target *final = mdb_newTarget("threeNPlusOne.x86_64", "threeNPlusOne.c", compiler, namingFlag, flags);
  mdb_addNewDep(final, "threeNPlusOne.o", "../lib/threeNPlusOne/threeNPlusOne.c", compiler, namingFlag, depFlags);


		char* t =  mdb_getBuildComdForTarget(final);

		mdb_buildTarget(final);
		free(t);
  return 0;
}
