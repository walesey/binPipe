#include "binary.h"

void transformPixel(
  unsigned char *r, 
  unsigned char *g, 
  unsigned char *b, 
  unsigned char *a, 
  int intensity
) {
  if ((*a) < (255 - intensity)) {
    (*a) = (*a) + intensity;
  }
  (*r) = 50;
  (*g) = 255;
  (*b) = 100;
}

void linearVisualisation(
  unsigned char *input,     // pointer to the input data buffer
  unsigned char *imageData, // pointer to the output rgba image buffer
  int length,               // number of bytes to process from input
  int resolution,           // pixel width for the output 3d image (max 255)
  int intensity,            // rate of pixel transformation
  int dimensions            // number of dimensions for the output image
) {
  for (int i=0; (i+2)<length; i+=3) {
    int index = 0;
    for (int j=0; j<dimensions; j++) {
      int coordinate = (input[i+j]*resolution)/256;
      index += coordinate * (pow(resolution, j));
    }
    index *= 4;

    // existing pixel value
    unsigned char r = imageData[index];
    unsigned char g = imageData[index+1];
    unsigned char b = imageData[index+2];
    unsigned char a = imageData[index+3];
    
    // transform the color
    transformPixel(&r, &g, &b, &a, intensity);

    // write new values
    imageData[index] = r;
    imageData[index+1] = g;
    imageData[index+2] = b;
    imageData[index+3] = a;
  }
}