#define MDB_BUILD_SYSTEM_IMPLEMENTATION
#include "../../build/build.h"

int main(void) {
	char *compiler = "cc";
	char *namingFlag = "-o";

	mdb_Flag flags[] = {
		"-c", "-m32", "-Wall", "-D_GNU_SOURCE", "-DPLATFORM_DESKTOP_GLFW",
		"-DGRAPHICS_API_OPENGL_33", "-Wno-missing-braces",
		"-Werror=pointer-arith", "-fno-strict-aliasing", "-std=c99",
		"-fPIC", "-O1", "-Werror=implicit-function-declaration",
		"-D_GLFW_X11",  "-I.",  "-Iexternal/glfw/include", NULL
	};

	mdb_buildTarget(mdb_newTarget("rcore.o", "rcore.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("rshapes.o", "rshapes.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("rtextures.o", "rtextures.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("rtext.o", "rtext.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("utils.o", "utils.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("rglfw.o", "rglfw.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("rmodels.o", "rmodels.c", compiler, namingFlag, flags));
	mdb_buildTarget(mdb_newTarget("raudio.o", "raudio.c", compiler, namingFlag, flags));

	return 0;
}
