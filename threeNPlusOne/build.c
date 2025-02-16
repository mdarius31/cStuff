#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

int main(void) {
		char *compiler = "cc";
		char *namingFlag = "-o";

		mdb_Flag flags[]= {"-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};
		mdb_Flag depFlags[] = {"-c", "-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};

		mdb_Target *final = mdb_newTarget("threeNPlusOne.x86_64", "threeNPlusOne.c", compiler, namingFlag, flags);
		mdb_addNewDep(final, "threeNPlusOne.o", "../lib/threeNPlusOne/threeNPlusOne.c", compiler, namingFlag, depFlags);

		mdb_buildTarget(final);

  return 0;
}
