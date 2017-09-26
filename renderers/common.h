#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"

void processKeys(unsigned char key, int x, int y);

void compileShader(GLuint shader);

GLuint loadVBO(geometry_t geometry);

GLuint loadIBO(geometry_t geometry);