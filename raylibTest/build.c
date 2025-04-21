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
