#include <stdio.h>
#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"



int main(void) {
	mdb_Flag flags[] = {"-ansi", "--std=c99", "-Wall", "-Wextra", "-lm", "-g", "-ggdb" , NULL};
	mdb_Target* final = mdb_newTarget("tikTakToe.x86_64","tikTakToe.c","cc", "-o", flags);
 	mdb_addCompiledDep(final, "../lib/raylib/src/raudio.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rglfw.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rshapes.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rtextures.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rcore.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rmodels.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/rtext.o");
 	mdb_addCompiledDep(final, "../lib/raylib/src/utils.o");

	
	return mdb_buildTarget(final);
}
