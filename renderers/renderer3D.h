#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "textfile.h"
#include "math.h"

#define FILE_BUFFER_SIZE 1024

// Camera config
#define CAMERA_FOV_Y 45.0
#define CAMERA_NEAR 0.1
#define CAMERA_FAR 1000.0

#define CAMERA_SPEED 0.01
#define CAMERA_SCROLL_SPEED 0.01

#define RAY_RESOLUTION 100.0

#ifndef RENDERER3D_H
#define RENDERER3D_H

typedef struct {
  unsigned char *data;
  int width;
  int height;
  int depth;
} Image;

typedef struct {
	unsigned int *indicies;
	float *verticies;
  int nbIndicies;
  int nbVerticies;
} Geometry;

#endif

void initRenderer();

void setImage(Image img);

void onUpdate(void (*fn)());
