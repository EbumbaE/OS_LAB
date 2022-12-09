#include "stdlib.h"
#include "stdio.h"

#if OS == APPLE
typedef short os_int;
typedef long double os_float;
#endif

#if OS == UNIX
typedef long os_int;
typedef double os_float;
#endif

#if OS == WIN32
typedef int os_int;
typedef float os_float;
#endif

os_float Pi(os_int k);
os_float Square(os_float a, os_float b);