#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"
#include <assert.h>
#include <printf.h>

int main(void) {
		char *compiler = "cc";
		char *namingFlag = "-o";

		mdb_Flag flags[] = {"-static", "-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};
		mdb_Flag depFlags[] = {"-c", "-static", "-m32",  "-Wextra",   "-Wall", "-Werror",
                   "-ansi", "--std=c99", NULL};

		mdb_Target *final = mdb_newTarget("threeNPlusOne.x86_64", "threeNPlusOne.c", compiler, namingFlag, flags);
		mdb_addNewDep(final, "threeNPlusOne.o", "../lib/threeNPlusOne/threeNPlusOne.c", compiler, namingFlag, depFlags);

		assert(mdb_buildTarget(final) == 0);

		char *testCmd = "./threeNPlusOne.x86_64 10";

		printf("test command: %s\n", testCmd);

  return system(testCmd);
}
