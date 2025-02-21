#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

int main(void) {
	char *compiler = "cc";
	char *namingFlag = "-o";
	mdb_Flag flags[] = {
		"-lm", "-m32", "-Wall", "-Wextra", NULL
	};

	mdb_Target *final = mdb_newTarget("threeNPlusOneRaylib.x86_64", "threeNPlusOneRaylib.c", compiler, namingFlag, flags);
	mdb_addNewDep(final, "threeNPlusOne.o", "../lib/threeNPlusOne/threeNPlusOne.c", compiler, namingFlag, (mdb_Flag[]) {"-c", "-m32",NULL});
	mdb_addCompiledDep(final, "../lib/raylib/src/raudio.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rglfw.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rshapes.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rtextures.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rcore.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rmodels.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/rtext.o");
	mdb_addCompiledDep(final, "../lib/raylib/src/utils.o");
	mdb_buildTarget(final);

	return 0;
}
