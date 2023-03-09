#include "./utils.h"

#include <GL/gl.h>

bool CHECK_BIT(unsigned long var, int pos){return var & (1UL << pos);}
double now,last_time, delta_time;
GLint WIDTH = 1366, HEIGHT = 768;