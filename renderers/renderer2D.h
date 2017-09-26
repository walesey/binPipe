#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "textfile.h"
#include "math.h"
#include "types.h"

void renderer2D_initRenderer();

void renderer2D_setImage(image_t img);

void renderer2D_onUpdate(void (*fn)());
