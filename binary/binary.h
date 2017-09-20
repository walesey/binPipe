#include <math.h>

void linear3DVisualisation(
  unsigned char *input,     // pointer to the input data buffer
  unsigned char *imageData, // pointer to the output rgba image buffer
  int length,               // number of bytes to process from input
  int resolution,           // pixel width for the output 3d image (max 255)
  int intensity             // rate of pixel transformation
);