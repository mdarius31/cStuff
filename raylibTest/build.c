#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

int main(void) {
		char *compiler = "cc";
		char *namingFlag = "-o";

		mdb_Flag flags[] = {
			"-lm",
			NULL
		};

	mdb_Target *final = mdb_newTarget("raylibTest.x86_64", "raylibTest.c", compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/raudio.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rglfw.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rshapes.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rtextures.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rcore.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rmodels.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/rtext.o", NULL, compiler, namingFlag, flags);
	mdb_addNewDep(final, "../lib/raylib/src/utils.o", NULL, compiler, namingFlag, flags);
	mdb_buildTarget(final);

	return 0;
}
