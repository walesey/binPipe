#ifndef TYPES_H
#define TYPES_H

typedef struct {
  unsigned char *data;
  int width;
  int height;
  int depth;
} image_t;

typedef struct {
	unsigned int *indicies;
	float *verticies;
  int nbIndicies;
  int nbVerticies;
} geometry_t;

#endif