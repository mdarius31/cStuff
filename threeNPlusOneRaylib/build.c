#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../lib/build/build.h"

int main(void) {
 char *compiler = "cc";
 char *namingFlag = "-o";
 const mdb_Flag baseFlags[] = {
 "-g","-m32", "-Wextra", "-Wall", "-Werror", "-ansi", "--std=c99", NULL
 };

 const unsigned int baseFlagsSize = sizeof(baseFlags);
 const unsigned int mdb_FlagSize = sizeof(mdb_Flag);

 const unsigned int flagsSize = baseFlagsSize + mdb_FlagSize;

 mdb_Flag *flags = malloc(flagsSize);
 memcpy(flags, baseFlags, sizeof(baseFlags));
 flags[(baseFlagsSize / mdb_FlagSize) - 1] =  "-lm";

 const unsigned int depFlagsSize = flagsSize;

 mdb_Flag *depFlags = malloc(depFlagsSize);
 memcpy(depFlags, baseFlags, sizeof(baseFlags));

 depFlags[(baseFlagsSize / mdb_FlagSize) - 1] =  "-c";

 mdb_Target *final = mdb_newTarget("threeNPlusOneRaylib.x86_64", "threeNPlusOneRaylib.c", compiler, namingFlag, flags);
 mdb_addNewDep(final, "threeNPlusOne.o", "../lib/threeNPlusOne/threeNPlusOne.c", compiler, namingFlag, depFlags);
 mdb_addCompiledDep(final, "../lib/raylib/src/raudio.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rglfw.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rshapes.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rtextures.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rcore.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rmodels.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/rtext.o");
 mdb_addCompiledDep(final, "../lib/raylib/src/utils.o");
 int result = mdb_buildTarget(final);


 free(flags);
 free(depFlags);
 return result;
}
