#pragma once

// used to ignore the unused parameter warnings
#define UNUSED(x) {\
 (void)(x);\
 printf("UNUSED VAR: %s ON LINE %u\n", #x, __LINE__);\
 fflush(stdout);\
}

